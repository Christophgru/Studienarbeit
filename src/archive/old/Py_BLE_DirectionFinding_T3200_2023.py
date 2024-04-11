#### Notes ####
# Open terminal and install dependencies using the following command
# pip install numpy matplotlib


#### includes ####
import queue
import serial
import re
import math
import time
import datetime
import numpy as np
from queue import Queue
from typing import Any
from matplotlib import pyplot as plt
from math import pi
from math import tan

#### Defines ####
# Constants for deviation calculation

DEVIATION_COEFFICIENTS = [
    2.21 * 10 ** (-10), -3.65 * 10 ** (-9), -1.73 * 10 ** (-6),
    4.51 * 10 ** (-5), 0.00343, -0.068, 3.6
]
ANCHOR_MARKER_SIZE = 10
TAG_MARKER_SIZE = 10
TAG_LABEL_FONT_SIZE = 28

#### Graphical output settings ####
plt.rcParams.update({'font.size': 12})

#### Global Variables ####

# baudrate = 115200, ready to send/ clear to send = 1, If a timeout is set it may return less characters as requested.
# With no timeout it will block until the requested number of bytes is read.
serialdata_anchor1 = serial.Serial('COM11', 115200, timeout=0.05, rtscts=1)
serialdata_anchor2 = serial.Serial('COM12', 115200, timeout=0.05, rtscts=1)
# define a FIFO Queue for each anchor to exchange records safely between multiple threads
fifo_anchor1 = queue.Queue(200)
# Constructor for FIFO queue (defined size is infinite)
fifo_anchor2 = queue.Queue(200)


#### Functions ####
# Function to calculate deviation from true angle
# 6th-order polynomial
def getDeviation(angle):
    dev = (
            DEVIATION_COEFFICIENTS[0] * angle ** 6 +
            DEVIATION_COEFFICIENTS[1] * angle ** 5 +
            DEVIATION_COEFFICIENTS[2] * angle ** 4 +
            DEVIATION_COEFFICIENTS[3] * angle ** 3 +
            DEVIATION_COEFFICIENTS[4] * angle ** 2 +
            DEVIATION_COEFFICIENTS[5] * angle +
            DEVIATION_COEFFICIENTS[6]
    )
    return dev


# Function to calculate x, y coordinates for given angles theta2 & theta3
def calcXY(theta2, theta3, distance):
    theta2_rad = np.deg2rad(theta2)
    theta3_rad = np.deg2rad(theta3)
    if theta2_rad == 0:
        y = tan(pi / 2 - theta3_rad) * distance
        x = 0
    elif theta3_rad == 0:
        y = tan(pi / 2 + theta2_rad) * distance
        x = distance
    else:
        y = distance / (tan(theta3_rad) - tan(theta2_rad))
        x = -y * tan(theta2_rad)
    return x, y


# Function for angle range validation
def is_within_angle_range(theta, lower, upper):
    return lower <= theta <= upper


# Function to calculate position inaccuracy
def get_inaccuracy(theta2, theta3, distance, theta2offset, theta3offset):
    try:
        inaccuracy = 991
        # Fehlerhafte Positionsdaten
        if theta2 == theta3 or theta2 == -90 or theta3 == -90 or theta2 == 90 or theta3 == 90 or theta2 > 180 or theta2 < -90 or theta3 < -90 or theta3 > 180:
            inaccuracy = np.NaN

        # Sender liegt rechtsseitig des hinteren Empfängers
        elif is_within_angle_range(theta2, 90, 180) and is_within_angle_range(theta3, 90, 180) and theta3 < theta2:
            [xwahr, ywahr] = calcXY(theta2, theta3, distance)
            # Prüfung, ob ein Schnittpunkt der Signaleinfallswinkel vorliegt
            if theta2 - getDeviation(theta2 + theta2offset) > theta3 + getDeviation(theta3 + theta3offset):
                [xmax, ymax] = calcXY(theta2 - getDeviation(theta2 + theta2offset),
                                      theta3 + getDeviation(theta3 + theta3offset), distance)
                inaccuracy = abs(ymax - ywahr)
            else:
                inaccuracy = np.NaN

                # Sender befindet sich zwischen den beiden Empfängern
        elif is_within_angle_range(theta2, 0, 90) and is_within_angle_range(theta3, 0, 90) and theta2 < theta3:
            [xwahr, ywahr] = calcXY(theta2, theta3, distance)
            # Prüfung, ob ein Schnittpunkt der Signaleinfallswinkel vorliegt
            if theta3 - getDeviation(theta3 + theta3offset) > theta2 + getDeviation(theta2 + theta2offset):
                [xmax, ymax] = calcXY(theta2 + getDeviation(theta2 + theta2offset),
                                      theta3 - getDeviation(theta3 + theta3offset), distance)
                inaccuracy = abs(ymax - ywahr)
            else:
                inaccuracy = np.NaN

        # Sender liegt linksseitig des vorderen Empfängers
        elif -90 < theta2 <= 0 and is_within_angle_range(theta3, 0, 90) and theta2 < theta3:
            [xwahr, ywahr] = calcXY(theta2, theta3, distance)
            # Prüfung, ob ein Schnittpunkt der Signaleinfallswinkel vorliegt 
            # Fallunterscheidung, zur Visualisierung des größeren Fehlers
            if theta2 + getDeviation(theta2 + theta2offset) < theta3 - getDeviation(theta3 + theta3offset):
                [unused, ymax_x] = calcXY(theta2 + getDeviation(theta2 + theta2offset),
                                          theta3 - getDeviation(theta3 + theta3offset), distance)
                yinaccuracyx = abs(ymax_x - ywahr)
                [unused, ymax_y] = calcXY(theta2 - getDeviation(theta2 + theta2offset),
                                          theta3 + getDeviation(theta3 + theta3offset), distance)
                yinaccuracyy = abs(ymax_y - ywahr)
                inaccuracy = max([yinaccuracyx, yinaccuracyy])
            else:
                inaccuracy = np.NaN

                # Sender liegt frontal vor dem LKW
        elif is_within_angle_range(theta2, -90, 0) and -90 < theta3 <= 0 and theta2 < theta3:
            [xwahr, ywahr] = calcXY(theta2, theta3, distance)
            # Prüfung, ob ein Schnittpunkt der Signaleinfallswinkel vorliegt
            if theta2 + getDeviation(theta2 + theta2offset) < theta3 - getDeviation(theta3 + theta3offset):
                [xmax, ymax] = calcXY(theta2 + getDeviation(theta2 + theta2offset),
                                      theta3 - getDeviation(theta3 + theta3offset), distance)
                inaccuracy = abs(ymax - ywahr)
            else:
                inaccuracy = np.NaN

        return inaccuracy

    except ZeroDivisionError:
        # Fehlerbehandlung, falls eine Division durch Null auftritt
        pass
    except Exception as e:
        # allgemeine Fehlerbehandlung 
        print("Fehler:", e)
        return np.NaN


# Function for serial port initialization
def initialize_serial_ports():
    global fifo_anchor1
    global fifo_anchor2
    global serialdata_anchor1
    global serialdata_anchor2

    try:
        serialdata_anchor1.isOpen()
        print("COM3 (first anchor) is opened!")

    except IOError:
        serialdata_anchor1.close()
        serialdata_anchor1.open()
        print("port was already open, was closed and opened again!")

    try:
        serialdata_anchor2.isOpen()
        print("COM4 (second anchor) is opened!")

    except IOError:
        serialdata_anchor2.close()
        serialdata_anchor2.open()
        print("port was already open, was closed and opened again!")


def calculate_border_coordinates(angle_offset, fail_safe_angle_range, overall_area):
    angle = (90 - fail_safe_angle_range + angle_offset) * np.pi / 180
    length = np.sqrt(2 * overall_area ** 2)
    x_length = np.sqrt(length ** 2 / (1 + np.tan(angle) ** 2))
    x_vector = np.arange(0, x_length, 0.1)
    y_vector = np.tan(angle) * x_vector
    return x_vector, y_vector


def plot_border_lines(x_coords, y_coords, offset, color):
    plt.plot(x_coords + offset[0], y_coords + offset[1], linestyle='--', linewidth=1, color=color, alpha=0.5)


def on_close(event):
    print("Grafische Ausgabe wurde geschlossen. Programm wird beendet.")
    plt.ioff()
    plt.close()
    serialdata_anchor1.close()
    serialdata_anchor2.close()
    exit()


def plotsketch(distance, theta2_offset, theta3_offset, x, y, color):
    overall_area = 3 * distance
    fail_safe_angle_range = 60  # system parameter
    x_left_1, y_left_1 = calculate_border_coordinates(theta2_offset, fail_safe_angle_range, overall_area)
    x_right_1, y_right_1 = calculate_border_coordinates(-theta2_offset, fail_safe_angle_range, overall_area)

    x_left_2, y_left_2 = calculate_border_coordinates(theta3_offset, fail_safe_angle_range, overall_area)
    x_right_2, y_right_2 = calculate_border_coordinates(-theta3_offset, fail_safe_angle_range, overall_area)

    plt.figure(figsize=(10, 6))

    plot_border_lines(-x_left_1, y_left_1, (0, 0), 'gray')
    plot_border_lines(x_right_1, y_right_1, (0, 0), 'gray')
    plot_border_lines(-x_left_2, y_left_2, (distance, 0), 'gray')
    plot_border_lines(x_right_2, y_right_2, (distance, 0), 'gray')

    anchor1_coordinates = (0, 0)
    anchor2_coordinates = (distance, 0)
    plt.plot(*anchor1_coordinates, marker="o", markersize=10, markeredgecolor="blue", markerfacecolor="blue",
             label="Anchor 1")
    plt.plot(*anchor2_coordinates, marker="o", markersize=10, markeredgecolor="blue", markerfacecolor="blue",
             label="Anchor 2")

    plt.plot(x, y, marker="o", markersize=10, markeredgecolor=color, markerfacecolor=color, label="helmet")

    plt.ylim([0, overall_area])
    plt.xlim([-overall_area, overall_area])
    plt.grid(True, linestyle='--', linewidth=0.5, color='gray', alpha=0.5)
    plt.xticks(np.arange(-overall_area, overall_area + 1, 1))
    plt.yticks(np.arange(0, overall_area + 1, 1))
    plt.xlabel("[m]")
    plt.ylabel("[m]")
    plt.ion()

    # Close Event Handler
    plt.gcf().canvas.mpl_connect('close_event', on_close)
    plt.plot()
    print("plot")


# Main Function
def main():
    global fifo_anchor1
    global fifo_anchor2

    try:
        # User inputs
        distance = float(input("Bitte geben Sie den Abstand in m zwischen Empfänger eins und zwei ein "))

        theta2_offset = int(input("Bitte geben sie die Rotation von Empfänger eins im Bereich -60°...60° ein "))
        theta3_offset = int(input("Bitte geben sie die Rotation von Empfänger zwei im Bereich -60°...60° ein "))

        timestamp_1 = datetime.datetime.timestamp(datetime.datetime.now())

        initialize_serial_ports()

        plotsketch(distance, theta2_offset, theta3_offset, 1, 1, 'black')

        while 1:
            # read input datastream for anchor node 1 
            if serialdata_anchor1.in_waiting > 0:  # check wether there are data on COM Port for anchor node 1
                dataStream_anchor1 = str(serialdata_anchor1.read(
                    80))  # cast 80 bytes to string (approximated lenght of +UUDF event is 60 bytes, additionally there are empty lines between two events, 80 proved to be favourable)
                regex_anchor1 = re.split("UUDF:",
                                         dataStream_anchor1)  # regular expression split the data on the UUDF: string for work on events
                for listing in regex_anchor1:
                    if "6C1DEBA0982F" in listing:  # check if the event data is corresponding to the correct anchor node
                        parts = listing.split(",")
                        if len(parts) == 9:  # check if the splitted event data is complete
                            fifo_anchor1.put(int(parts[
                                                     2]) + theta2_offset)  # fetches the azimut data (theta2) from the string and add the rotational offset value
                            # print(datetime.datetime.now())
                serialdata_anchor1.reset_input_buffer()  # alternative mehod flush() forces a data transfer from COM port of anchor node 1 and clears the serial data on this Port afterwards
            # read input datastream for anchor node 2
            if serialdata_anchor2.in_waiting > 0:
                dataStream_anchor2 = str(serialdata_anchor2.read(80))
                regex_anchor2 = re.split("UUDF:", dataStream_anchor2)
                for listing in regex_anchor2:
                    if "6C1DEBA79E2D" in listing:
                        parts = listing.split(",")
                        if len(parts) == 9:
                            fifo_anchor2.put(int(parts[2]) + theta3_offset)
                            # print(datetime.datetime.now())
                serialdata_anchor2.reset_input_buffer()
                # averaging the azimut information of anchor node 1 and anchor node 2 about the last four records
            if fifo_anchor1.qsize() >= 4 and fifo_anchor2.qsize() >= 4:  # check the queue size
                # transfer the data of the first anchor node from FIFO queue to an array for averaging the azimut data 
                records_anchor1 = []  # create new empty array
                for i in range(4):
                    records_anchor1.append(
                        fifo_anchor1.get())  # the get function transfers and then erases the first (oldest) listing in the queue
                # calc arithmetic mean
                arithmetic_mean_fifo_anchor1 = sum(records_anchor1) / len(records_anchor1)
                # remove the listing with the largest deviation from the mean value 
                max_deviation_listing_anchor1 = max(records_anchor1,
                                                    key=lambda x: abs(x - arithmetic_mean_fifo_anchor1))
                records_anchor1.remove(max_deviation_listing_anchor1)
                # calculate the arithmetic mean of the last 3 records
                arithmetic_mean_fifo_anchor1 = sum(records_anchor1) / len(records_anchor1)
                fifo_anchor1 = queue.Queue()  # clear the FIFO queue
                # transfer the data of the second anchor node from FIFO queue to an array for averaging the azimut data 
                records_anchor2 = []
                for i in range(4):
                    records_anchor2.append(fifo_anchor2.get())
                arithmetic_mean_fifo_anchor2_listing = sum(records_anchor2) / len(records_anchor2)
                max_deviation_listing2 = max(records_anchor2,
                                             key=lambda x: abs(x - arithmetic_mean_fifo_anchor2_listing))
                records_anchor2.remove(max_deviation_listing2)
                arithmetic_mean_fifo_anchor2_listing = sum(records_anchor2) / len(records_anchor2)
                fifo_anchor2 = queue.Queue()
                # define the azimuth values in radians
                theta2 = -arithmetic_mean_fifo_anchor1 * pi / 180
                theta3 = -arithmetic_mean_fifo_anchor2_listing * pi / 180
                # calculate the position of the tag/ helmet  
                if theta2 == 0:  # exception
                    y = round(tan(pi / 2 - theta3) * distance, 3)
                    x = 0
                elif theta3 == 0:  # exception
                    y = round(tan(pi / 2 + theta2) * distance, 3)
                    x = distance
                else:  # normal case
                    y = round(distance / (tan(theta3) - tan(theta2)), 3)  #
                    x = round(-y * tan(theta2), 3)
                    # set the tag coordinates
                # calculate the inaccuracy of the measurement 
                inaccuracy = get_inaccuracy(theta2 * 180 / pi, theta3 * 180 / pi, distance, theta2_offset,
                                            theta3_offset)
                # indicate the risk of an accident
                if inaccuracy < 1:
                    color = 'green'
                elif inaccuracy < 2:
                    color = 'yellow'
                else:
                    color = 'red'
                plotsketch(distance, theta2_offset, theta3_offset, x, y, color)

                # print timestamp  for analyze timing
                print("timestamp: ", datetime.datetime.utcnow() - datetime.datetime.utcfromtimestamp(timestamp_1))
                print(f"x:{x} y:{y}")
            else:
                plt.pause(0.01)

    except KeyboardInterrupt:
        print("Programm wurde beendet.")
        on_close(event="end")
    except Exception as e:
        print("Fehler:", e)
        print("Fehler:", e)
    finally:
        # Clean up all memory access
        serialdata_anchor1.close()
        serialdata_anchor2.close()
        plt.close()


if __name__ == "__main__":
    main()
