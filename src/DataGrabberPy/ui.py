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
    label.config(text=f"Xpos: {coordinates.get('Xpos', 'N/A')}    Ypos: {coordinates.get('Ypos', 'N/A')}    Abweichung: {coordinates.get('Abweichung', 'N/A')}")
    xpos = coordinates.get('Xpos', None)
    ypos = coordinates.get('Ypos', None)
    if xpos is not None and ypos is not None:
        update_point_on_canvas(xpos, ypos)

def update_point_on_canvas(xpos, ypos):
    # Clear existing points
    canvas.delete("point")

    # Calculate canvas coordinates
    canvas_x = (xpos - X_MIN) * (CANVAS_WIDTH / (X_MAX - X_MIN))
    canvas_y = CANVAS_HEIGHT - (ypos - Y_MIN) * (CANVAS_HEIGHT / (Y_MAX - Y_MIN))

    # Draw new point
    canvas.create_oval(canvas_x - POINT_SIZE, canvas_y - POINT_SIZE, canvas_x + POINT_SIZE, canvas_y + POINT_SIZE, fill="red", tags="point")



def draw_axes():
    height=Y_MAX-Y_MIN

    ratioh=(height+Y_MIN)/height
    print(f"height: {height}, base {height+Y_MIN}, ratio: {ratioh}" )
    baseY=CANVAS_HEIGHT *ratioh

    # Draw x-axis
    canvas.create_line(0, baseY, CANVAS_WIDTH, baseY, fill="black")

    width=X_MAX-X_MIN
    ratiow=1-(width+X_MIN)/width
    print(f"width: {width}, base {width+X_MIN}, ratio: {ratiow}" )
    baseX=CANVAS_WIDTH *ratiow


    # Draw y-axis
    canvas.create_line(baseX, 0, baseX, CANVAS_HEIGHT, fill="black")
    # Draw x-axis ticks and labels
    for x in range(int(X_MIN), int(X_MAX) + 1):
        canvas_x = (x - X_MIN) * (CANVAS_WIDTH / (X_MAX - X_MIN))
        canvas.create_line(canvas_x, baseY - TICK_SIZE, canvas_x, baseY + TICK_SIZE, fill="black")
        canvas.create_text(canvas_x, baseY + TICK_SIZE + 5, text=str(x), anchor="n")

    # Draw y-axis ticks and labels
    
    for y in range(int(Y_MIN), int(Y_MAX) + 1):
        canvas_y = CANVAS_HEIGHT - (y - Y_MIN) * (CANVAS_HEIGHT / (Y_MAX - Y_MIN))
        canvas.create_line(baseX - TICK_SIZE, canvas_y, baseX + TICK_SIZE, canvas_y, fill="black")
        canvas.create_text(baseX - TICK_SIZE - 15, canvas_y, text=str(y), anchor="e")



def main_thread():
    while True:
        print(".", end="", flush=True)
        time.sleep(1)

def main_async():
    # Run the Tkinter event loop
    root.mainloop()

if __name__ == "__main__":
    POINT_SIZE = 5
    TICK_SIZE = 5
    X_MIN, X_MAX = -2, 5
    Y_MIN, Y_MAX = -1, 5
    # Initialize constants for the canvas
    CANVAS_WIDTH, CANVAS_HEIGHT = 800, 800
    POINT_SIZE = 5
    # Initialize Tkinter GUI
    root = tk.Tk()
    root.title("Coordinates Display")
    root.geometry("800x800")  # Set fixed window size

    # Create a label for displaying coordinates
    label = tk.Label(root, text="", font=("Arial", 14))
    label.pack(pady=10)

    # Create a canvas for displaying points
    canvas = tk.Canvas(root, width=CANVAS_WIDTH, height=CANVAS_HEIGHT, bg="white")
    canvas.pack()

    # Draw axes
    draw_axes()


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