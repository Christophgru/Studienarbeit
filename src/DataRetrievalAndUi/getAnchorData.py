
import json
import math
import os
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
sensors=None




def init_serials():
    """
    @brief Initialize the serial connections for anchor nodes.
    @details Sets up serial connections for two anchor nodes with specific configurations.
    """
    # baudrate = 115200, ready to send/ clear to send = 1, If a timeout is set it may return less characters as requested.
    # With no timeout it will block until the requested number of bytes is read.
    global sensors
    for i in range(len(sensors)):
        sensors[i]['serial']= serial.Serial(sensors[i]["serial_port"], 115200, timeout=0.05, rtscts=1)
        try:
            sensors[i]['serial'].isOpen()
            print("anchor2 (first anchor) is opened!")

        except IOError:
            sensors[i]['serial'].close()
            sensors[i]['serial'].open()
            print("port was already open, was closed and opened again!")
    




def getanchor(sensor):
    """
    @brief Retrieve data from the second anchor node.
    @param val2_list List to store the azimuth data from the second anchor node.
    """
    if sensor['serial'].in_waiting > 0:
        dataStream_anchor = str(sensor['serial'].read(80))
        #print(dataStream_anchor2)
        regex_anchor = re.split("UUDF:", dataStream_anchor)
        for listing in regex_anchor:
            if sensor['id'] in listing:
                parts = listing.split(",")
                if len(parts) == 9:
                    val=-int(parts[2])
                    sensor['val'].append(val)
        sensor['serial'].reset_input_buffer()

def on_close():
    """
    @brief Close the serial connections and exit the program.
    """
    sensors[0]['serial'].close()
    sensors[1]['serial'].close()
    exit()

def getValues(lastvalue):
    """
    @brief Retrieve and process values from the anchor nodes.
    @param theta2_offset Azimuth offset for the first anchor node.
    @param theta3_offset Azimuth offset for the second anchor node.
    @return A list of dictionaries with processed values or 0 if no change.
    """
    global sensors
    numSensors=len(sensors)
    changed=False
    results=[]
    for i in range(numSensors):
        results.append(None)
        sensors[i]['val']=[]
        sensors[i]['thread'] = threading.Thread(target=getanchor, args=(sensors[i],))
        sensors[i]['thread'].start()
    

    for i in range(numSensors):
       sensors[i]['thread'].join()
        
    for i in range(numSensors):
        try:
        # Retrieve the result from the list
            sensors[i]['result'] = sensors[i]['val'][0]
            changed=True
        except IndexError:
            # Keep old Value if the index is out of range (No Value read)
            None
    
    for i in range(numSensors):
        results[i]={"theta":sensors[i]['theta'],"val":sensors[i]['result'],"xpos":sensors[i]['xpos']}
    
    if changed:
        #print(results)
        return results
    else: 
        return 0


server_running = True
server_socket = None
client_sockets = []

def handle_client(client_socket, client_address):
    """
    @brief Handle incoming client connections.
    @param client_socket The socket object for the connected client.
    @param client_address The address of the connected client.
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
    @brief Send data to all connected clients.
    @param data The data to be sent to all clients.
    """
    print(f"send data to {len(client_sockets) } clients")
    for client_socket in client_sockets:
        try:
            client_socket.sendall(data.encode('utf-8'))
        except Exception as e:
            print(f"Error sending data to client: {e}")

def signal_handler(sig, frame):
    """
    @brief Signal handler for keyboard interrupt.
    @param sig The signal number.
    @param frame The current stack frame.
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
    @brief Accept incoming connections in a separate thread.
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
    @brief Main function to start the server and manage Bluetooth read.
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

    global sensors
    script_dir = os.path.dirname(__file__)
    json_file_path = os.path.join(script_dir, 'Sensor_Config.json')
    with open(json_file_path, 'r') as file:
        sensors = json.load(file)
    print(sensors)

    # Your Bluetooth read logic goes here
    init_serials()
    temp=[None]
    while True:
        try:
            val = getValues(temp)
            if val!=0:
                temp=val
                message_final = json.dumps(temp)
                #  print(message_final)
                send_data_to_all_clients(message_final)
        except Exception as e:
            print(f"Error in Bluetooth read: {e}(No Data to read)")

if __name__ == "__main__":
    main()