# python_script.py


#### includes ####

import sys
sys.path.append(r'C:\\Users\\chris\\AppData\\Local\\Programs\\Python\\Python312\\Lib')
sys.path.append(r'C:\\Users\\chris\\AppData\\Local\\Programs\\Python\\Python312\\Lib\\site-packages')


import serial
import re

serialdata_anchor1 = None
#serialdata_anchor2 = None

def init_serials():
    # baudrate = 115200, ready to send/ clear to send = 1, If a timeout is set it may return less characters as requested.
    # With no timeout it will block until the requested number of bytes is read.
    global serialdata_anchor1
    #global serialdata_anchor2
    serialdata_anchor1 = serial('COM11', 115200, timeout=0.05, rtscts=1)
   # serialdata_anchor2 = serial.Serial('COM12', 115200, timeout=0.05, rtscts=1)
    try:
        serialdata_anchor1.isOpen()
        print("COM3 (first anchor) is opened!")

    except IOError:
        serialdata_anchor1.close()
        serialdata_anchor1.open()
        print("port was already open, was closed and opened again!")



def on_close():
    serialdata_anchor1.close()
    #serialdata_anchor2.close()
    exit()

def getValues(theta2_offset=0
              #, theta3_offset=0
              ):
    val1=0
    #val2=0
    if serialdata_anchor1.in_waiting > 0:  # check wether there are data on COM Port for anchor node 1
        dataStream_anchor1 = str(serialdata_anchor1.read(
            80))  # cast 80 bytes to string (approximated lenght of +UUDF event is 60 bytes, additionally there are empty lines between two events, 80 proved to be favourable)
        regex_anchor1 = re.split("UUDF:",
                                    dataStream_anchor1)  # regular expression split the data on the UUDF: string for work on events
        for listing in regex_anchor1:
            if "6C1DEBA0982F" in listing:  # check if the event data is corresponding to the correct anchor node
                parts = listing.split(",")
                if len(parts) == 9:  # check if the splitted event data is complete
                    val1 =int(parts[2]) + theta2_offset  # fetches the azimut data (theta2) from the string and add the rotational offset value
                    # print(datetime.datetime.now())
            serialdata_anchor1.reset_input_buffer()  # alternative mehod flush() forces a data transfer from COM port of anchor node 1 and clears the serial data on this Port afterwards
   
    return val1
    