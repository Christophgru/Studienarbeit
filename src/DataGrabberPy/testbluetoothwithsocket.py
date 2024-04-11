
import signal
import sys
import serial
import re
import socket
import time
import threading
import asyncio
import websockets



# Set the server address and port
port= 12345
clients = set()

serialdata_anchor1 = None
serialdata_anchor2 = None

def init_serials():
    # baudrate = 115200, ready to send/ clear to send = 1, If a timeout is set it may return less characters as requested.
    # With no timeout it will block until the requested number of bytes is read.
    global serialdata_anchor1
    global serialdata_anchor2
    serialdata_anchor1 = serial.Serial('COM11', 115200, timeout=0.05, rtscts=1)
    serialdata_anchor2 = serial.Serial('COM12', 115200, timeout=0.05, rtscts=1)
    try:
        serialdata_anchor1.isOpen()
        print("anchor2 (first anchor) is opened!")

    except IOError:
        serialdata_anchor1.close()
        serialdata_anchor1.open()
        print("port was already open, was closed and opened again!")
    try:
        serialdata_anchor2.isOpen()
        print("anchor2 (first anchor) is opened!")

    except IOError:
        serialdata_anchor2.close()
        serialdata_anchor2.open()
        print("port was already open, was closed and opened again!")



def getanchor1(val1_list,theta2_offset=0):
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
                    val1_list.append(val1)
                    # print(datetime.datetime.now())"""
        serialdata_anchor1.reset_input_buffer()  # alternative mehod flush() forces a data transfer from COM port of anchor node 1 and clears the serial data on this Port afterwards
     
def getanchor2(val2_list,theta3_offset=0):
    if serialdata_anchor2.in_waiting > 0:
        dataStream_anchor2 = str(serialdata_anchor2.read(80))
        regex_anchor2 = re.split("UUDF:", dataStream_anchor2)
        for listing in regex_anchor2:
            if "6C1DEBA79E2D" in listing:
                parts = listing.split(",")
                if len(parts) == 9:
                    val2=int(parts[2]) + theta3_offset
                    val2_list.append(val2)
                    # print(datetime.datetime.now())"""
        serialdata_anchor2.reset_input_buffer()
        # averaging the azimut information of anchor node 1 and anchor node 2 about the last four records

def on_close():
    serialdata_anchor1.close()
    serialdata_anchor2.close()
    exit()

def getValues(theta2_offset=0
              , theta3_offset=0
              ):
    val1_list=[]
    val2_list=[]

    thread1 = threading.Thread(target=getanchor1, args=(val1_list,theta2_offset,))

    thread2 = threading.Thread(target=getanchor2, args=(val2_list,theta2_offset,))
    thread1.start() 
    thread2.start()

    thread1.join()
    thread2.join()

    try:
    # Retrieve the result from the list
        val1 = val1_list[0]
    except IndexError:
        # Set val1 to None if the index is out of range
        val1 = None
    try:
    # Retrieve the result from the list
        val2 = val2_list[0]
    except IndexError:
        # Set val1 to None if the index is out of range
        val2 = None
    
    return [val1,val2]











    

server_running = True
server_socket = None
client_sockets = []

def handle_client(client_socket, client_address):
    """
    Function to handle incoming client connections.
    """
    print(f"Connection from {client_address}")
    while server_running:
        try:
            data = client_socket.recv(1024)
            if not data:
                break
            print(f"Received from {client_address}: {data.decode('utf-8')}")
        except Exception as e:
            print(f"Error receiving data from {client_address}: {e}")
            break

    print(f"Client {client_address} disconnected")
    client_sockets.remove(client_socket)
    client_socket.close()

def send_data_to_all_clients(data):
    """
    Function to send data to all connected clients.
    """
    print(f"send data to {len(client_sockets) } clients")
    for client_socket in client_sockets:
        try:
            client_socket.sendall(data.encode('utf-8'))
        except Exception as e:
            print(f"Error sending data to client: {e}")

def signal_handler(sig, frame):
    """
    Signal handler for keyboard interrupt.
    """
    global server_running, server_socket
    print("Stopping server...")
    server_running = False
    # Close server socket
    if server_socket:
        server_socket.close()
    # Close client sockets
    for client_socket in client_sockets:
        client_socket.close()
    sys.exit()


def accept_connections():
    """
    Function to accept incoming connections in a separate thread.
    """
    global server_socket
    try:
        while server_running:
            # Accept incoming connection
            client_socket, client_address = server_socket.accept()
            client_sockets.append(client_socket)

            # Start a new thread to handle the client
            client_thread = threading.Thread(target=handle_client, args=(client_socket, client_address))
            client_thread.start()
    except Exception as e:
        print(f"Error accepting connection: {e}")


def main():
    """
    Main function to start the server and manage Bluetooth read.
    """
    # Register signal handler for keyboard interrupt
    signal.signal(signal.SIGINT, signal_handler)

    # Create a socket object
    global server_socket 
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    server_socket.bind(('127.0.0.1', 12345))
    server_socket.listen(5)
    print("Server listening on port 12345")

    connection_thread = threading.Thread(target=accept_connections)
    connection_thread.start()

    print("Webserver started, start Bluetooth read")

    # Your Bluetooth read logic goes here
    init_serials()
    last_value = [None, None]
    
    while True:
        try:
            temp = getValues()
            changed = False
            if temp[0] is not None:
                last_value[0] = temp[0]
                changed = True
            if temp[1] is not None:
                last_value[1] = temp[1]
                changed = True
            if changed:
                message_final = "\t\t" + str(last_value) + "\t\t\t\t\t[]"
              #  print(message_final)
                send_data_to_all_clients(message_final)
        except Exception as e:
            print(f"Error in Bluetooth read: {e}")

if __name__ == "__main__":
    main()