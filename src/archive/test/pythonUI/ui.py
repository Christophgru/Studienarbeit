import socket
import threading
import time
import tkinter as tk
import asyncio
#C:\Python312\python.exe .\src\archive\test\pythonUI\ui.py

def server_thread():
    while True:
        try:
            HOST = '127.0.0.1'  # localhost
            PORT = 12346

            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                s.bind((HOST, PORT))
                s.listen()
                print(f"Server listening on port {PORT}...")

                while True:
                    conn, addr = s.accept()
                    with conn:
                        print('Connected by', addr)
                        while True:
                            data = conn.recv(1024)
                            if not data:
                                break
                            
                            msg = data.decode()
                            print("Received:", msg)
                            update_display(msg)
        except Exception as e:
             print(f"Exception occurred: {e}. Restarting server...")

def update_display(msg):
    # Extract coordinates from the message
    coordinates = {}
    parts = msg.split()
    for part in parts:
        if '=' in part:
            key, value = part.split('=')
            coordinates[key] = float(value)
        else:
            print("Invalid message format:", msg)
            return

    # Update the GUI with the received coordinates
    label.config(text=f"Xpos: {coordinates.get('Xpos', 'N/A')}    Ypos: {coordinates.get('Ypos', 'N/A')}    Unsicherheit: {coordinates.get('Unsicherheit', 'N/A')}")



def main_thread():
    while True:
        print(".", end="", flush=True)
        time.sleep(1)

def main_async():
    # Run the Tkinter event loop
    root.mainloop()

if __name__ == "__main__":

# Initialize Tkinter GUI
    root = tk.Tk()
    root.title("Coordinates Display")

    # Create a label for displaying coordinates
    label = tk.Label(root, text="", font=("Arial", 14))
    label.pack(pady=10)

    # Start the server thread
    server_thread = threading.Thread(target=server_thread)
    server_thread.daemon = True
    server_thread.start()

    # Start the main thread for printing dots
    main_thread = threading.Thread(target=main_thread)
    main_thread.daemon = True
    main_thread.start()

    # Start the Tkinter event loop in a separate thread
    asyncio.run(main_async())