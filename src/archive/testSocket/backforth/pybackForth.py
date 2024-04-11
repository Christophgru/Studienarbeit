import socket
import time

# Set the server address and port
server_address = ('127.0.0.1', 12345)

# Create a socket (IPv4, TCP)
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    # Connect to the server
    s.connect(server_address)

    i=0
    while i<10000:
            # Send data to the server
            message = "Hello from Python!"
            messagefinal=message
            s.sendall(messagefinal.encode())

            data = s.recv(1024)  # Adjust the buffer size based on your message size
            if not data:
                print("No Data could be found")
            print(f"Received data from server[{i}]: {data.decode()}")
            i=i+1

print("Data sent to the server.")