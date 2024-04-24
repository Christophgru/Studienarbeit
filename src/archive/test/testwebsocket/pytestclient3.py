import socket
import time

# Set the server address and port
server_address = ('127.0.0.1', 12345)

# Create a socket (IPv4, TCP)
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    # Connect to the server
    s.connect(server_address)

    # Send data to the server
    message = "Hello from Python!"
    i=5
    while i<1000:
        messagefinal=message+str(i)
        s.sendall(messagefinal.encode())
        i=i+1
        time.sleep(0.002)
    

print("Data sent to the server.")