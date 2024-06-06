"""
@file ui.py
@brief Mock UI module for displaying coordinates and sensor data
@date 2024-05-17
@author Christoph Gruender
"""

import json
import math
import socket
import sys
import threading
import time
import tkinter as tk
import asyncio

displaydebuginfo=0
HOST = '127.0.0.1'  # localhost
PORT = 12346
firstcall = True

def server_thread():
    """
    @brief Server thread function to handle incoming connections and data.
    """
    while True:
        try:
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
                            last_occurance=msg.rfind('{"point":')
                            if(last_occurance!=-1):
                                last_msg=msg[last_occurance:]
                            if displaydebuginfo:
                                print("Received:", last_msg)
                            update_display(last_msg)
        except Exception as e:
            print(f"Exception occurred: {e}. Restarting server...")

def update_display(msg):
    """
    @brief Update the display with the received message.
    @param msg The received message in JSON format.
    @param firstcall A flag indicating if it's the first call to this function.
    """
    try:
        # Parse the JSON message
        data = json.loads(msg)
        # Extract xpos and ypos from the 'point' dictionary
        xpos, ypos = data.get('point', {}).get('position', [None, None])

        # Check if xpos and ypos are not None
        if xpos is not None and ypos is not None:
            # Update the GUI with the received coordinates
            label.config(text=f"Xpos: {xpos}    Ypos: {ypos}    Uncertainty: {data.get('point', {}).get('Uncertainty', 'N/A')}")
            update_point_on_canvas(xpos, ypos)

            # Extract sensor values
            sensor_values = data.get('sensor_values', [])

            # Visualize sensors
            visualize_sensors(sensor_values)
        else:
            print("Invalid message format:", msg)

    except json.JSONDecodeError:
        print("Invalid JSON format:", msg)

def update_point_on_canvas(xpos, ypos):
    """
    @brief Update the point on the canvas based on received coordinates.
    @param xpos X-coordinate of the point.
    @param ypos Y-coordinate of the point.
    """
    # Clear existing points
    canvas.delete("point")

    # Calculate canvas coordinates
    canvas_x = (xpos - X_MIN) * (canvas.winfo_width() / (X_MAX - X_MIN))
    canvas_y = canvas.winfo_height() - (ypos - Y_MIN) * (canvas.winfo_height() / (Y_MAX - Y_MIN))
    sys.stdout.flush()

    # Draw new point
    canvas.create_oval(canvas_x - POINT_SIZE, canvas_y - POINT_SIZE, canvas_x + POINT_SIZE, canvas_y + POINT_SIZE, fill="red", tags="point")

def visualize_sensors(sensor_values):
    """
    @brief Visualize sensor data on the canvas.
    @param sensor_values List of sensor data values.
    """
    # Clear existing sensor visualizations
    canvas_width = canvas.winfo_width()
    canvas_height = canvas.winfo_height()
    graph_height=Y_MAX-Y_MIN
    graph_width=X_MAX-X_MIN
    canvas.delete("sensordata")
    global firstcall
    for sensor in sensor_values:
        theta = math.radians(sensor.get('theta', 0))
        val = math.radians(sensor.get('val', 0))
        resAngle = theta + val
        xpos = sensor.get('pos', [0, 0])[0]
        ypos = sensor.get('pos', [0, 0])[1]

        # Calculate canvas coordinates for the sensor position
        canvas_x = (xpos - X_MIN) * (canvas_width / (X_MAX - X_MIN))
        canvas_y = canvas_height - (ypos - Y_MIN) * (canvas_height / (Y_MAX - Y_MIN))
        aspect_ratio = canvas_width / canvas_height
        


        # Calculate endpoint of the line based on angle and length
        line_length = 5000  # Adjust as needed
        end_x = canvas_x + line_length * math.cos(resAngle) * aspect_ratio #* aspect_ratio
        end_y = canvas_y - line_length * math.sin(resAngle) * (graph_width/graph_height)#* 0.29#wtf, this should be 1


        # Draw line originating from the box
        canvas.create_line(canvas_x, canvas_y, end_x, end_y, fill="black", tags="sensordata")

        
        if firstcall:
            # Draw green box
            canvas.create_rectangle(canvas_x - 5, canvas_y - 5, canvas_x + 5, canvas_y + 5, fill="green", tags="sensor")

            # Draw short lines around the sensor
            for angle in range(0, 360, 20):
                angle_rad = math.radians(angle)
                short_line_length = 50
                short_end_x = canvas_x + short_line_length * math.cos(angle_rad) *  aspect_ratio
                short_end_y = canvas_y - short_line_length * math.sin(angle_rad) * (graph_width/graph_height)
                canvas.create_line(canvas_x, canvas_y, short_end_x, short_end_y, fill="black", tags="sensor")

                # Add angle labels
                label_x = canvas_x + (short_line_length + 10) * math.cos(angle_rad) *  aspect_ratio
                label_y = canvas_y - (short_line_length + 10) * math.sin(angle_rad)* (graph_width/graph_height)
                angle_label = f"{angle}°"
                canvas.create_text(label_x, label_y, text=angle_label, fill="black", tags="sensor")
    
    firstcall=False
        

    

def draw_axes():
    """
    @brief Draw the coordinate axes on the canvas.
    """
    canvas_width = canvas.winfo_width()
    canvas_height = canvas.winfo_height()

    # Calculate baseX and baseY dynamically
    baseX = canvas_width * (-X_MIN / (X_MAX - X_MIN))
    baseY = canvas_height * ( 1-(-Y_MIN / (Y_MAX - Y_MIN)))

    # Draw x-axis
    canvas.create_line(0, baseY, canvas_width, baseY, fill="black")

    # Draw y-axis
    canvas.create_line(baseX, 0, baseX, canvas_height, fill="black")

    # Draw x-axis ticks and labels
    for x in range(int(X_MIN), int(X_MAX) + 1):
        canvas_x = (x - X_MIN) * (canvas_width / (X_MAX - X_MIN))
        canvas.create_line(canvas_x, baseY - TICK_SIZE, canvas_x, baseY + TICK_SIZE, fill="black")
        canvas.create_text(canvas_x, baseY + TICK_SIZE + 5, text=str(x), anchor="n")

    # Draw y-axis ticks and labels
    for y in range(int(Y_MIN), int(Y_MAX) + 1):
        canvas_y = canvas_height - (y - Y_MIN) * (canvas_height / (Y_MAX - Y_MIN))
        canvas.create_line(baseX - TICK_SIZE, canvas_y, baseX + TICK_SIZE, canvas_y, fill="black")
        canvas.create_text(baseX - TICK_SIZE - 15, canvas_y, text=str(y), anchor="e")

previous_width = None
previous_height = None

def resize_updates(event):
    """
    @brief Handle window resize events to redraw the canvas content.
    """
    global previous_width, previous_height
    current_width = canvas.winfo_width()
    current_height = canvas.winfo_height()
    
    # Check if the size has actually changed
    if current_width != previous_width or current_height != previous_height:
        previous_width = current_width
        previous_height = current_height
        print("Resize detected")
        canvas.delete("all")
        draw_axes()
        global firstcall
        firstcall=True

def main_thread():
    """
    @brief Main thread function for periodic print.
    """
    while True:
        print(".", end="", flush=True)
        time.sleep(1)

def main_async():
    """
    @brief Main function to run the Tkinter event loop asynchronously.
    """
    root.mainloop()

if __name__ == "__main__":
    # Initialize constants for the canvas
    POINT_SIZE = 5
    TICK_SIZE = 5
    X_MIN, X_MAX = -1, 5
    Y_MIN, Y_MAX = -1, 20

    # Initialize Tkinter GUI
    root = tk.Tk()
    root.title("Coordinates Display")
    root.geometry("800x800")  # Set initial window size

    # Create a label for displaying coordinates
    label = tk.Label(root, text="", font=("Arial", 14))
    label.pack(pady=10)

    # Create a canvas for displaying points
    canvas = tk.Canvas(root, bg="white")
    canvas.pack(fill=tk.BOTH, expand=True)

    # Bind the resize event
    root.bind("<Configure>", resize_updates)

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
