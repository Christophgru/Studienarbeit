
import json
import math
import random
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

basetimesleep=0.02 #avg time will be 1.5x as much

def getanchor(val1_list,param1,param2):
    val=None
    if param1==0:
        val =math.cos(time.time())*param2
    if param1==1:
        val =math.sin(time.time())*param2
    print(val) 
    val1_list.append(val)
    time.sleep(basetimesleep+random.uniform(0, 1)*basetimesleep)

       

   
def on_close():
    exit()

def getValues(
              ):
    #should be passed
    theta0_offset=70
    theta1_offset=120
    thetalist=[theta0_offset, theta1_offset]
    xposlist=[0,3]
    vallist=[]
    threadlist=[]
    results=[]
    changed=False
    for i in range(2):
        vallist.append([])
        threadlist.append(None)
        results.append(None)
        threadlist[i] = threading.Thread(target=getanchor, args=(vallist[i],i,10))
        threadlist[i].start() 

    for i in range(2):
        threadlist[i].join()

    for i in range(2):
        try:
        # Retrieve the result from the list
            results[i] = vallist[i][0]
            changed=True
        except IndexError:
            # Set val1 to None if the index is out of range
            results[i] = None
            
    for i in range(2):
        results[i]={"theta":thetalist[i],"val":results[i],"xpos":xposlist[i]}

    if changed:
        return results
    else:
        return 0


    

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

    print("Webserver started, start Bluetooth simulation")

    last_value = [None, None]
    
    while True:
        try:
            temp = getValues()
            if temp!=0:
                message_final = json.dumps(temp)
                #print(message_final)
                send_data_to_all_clients(message_final)
        except Exception as e:
            print(f"Error in Bluetooth read: {e}")

if __name__ == "__main__":
    main()