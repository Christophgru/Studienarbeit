# Studienarbeit Bluetoothtracker

The source code contains three independent software modules that communicate over WebSockets.

## HowTo

### Initialize the Project

1. **Clone the Repository:**
   `git clone https://github.com/yourusername/yourproject.git`
   `cd yourproject`
   `git submodule update --init --recursive`

2. **Ensure Python 3.12 is Installed:**
   This project requires Python 3.12. Please ensure it is installed on your machine.

3. **Create and Activate the Virtual Environment:**
 
   - On Windows:
   ```sh
     python -m venv venv_Studienarbeit
     .\venv_Studienarbeit\Scripts\activate
    ```

   - On macOS and Linux:
   ```sh
     python3 -m venv venv_Studienarbeit
     source venv_Studienarbeit/bin/activate

4. **Install Dependencies:**
   `pip install -r requirements.txt`

5. **Start the Project:**
   For Windows machines, use the `startall.bat` script to start and execute the code.

## Components
### Data Retrieval

Use Python 3.12 to execute the files `src\DataRetrievalAndUi\mockBluetooth.py` and `src\DataRetrievalAndUi\getAnchorData.py` to retrieve/mock the needed information of the [XPLR-AOA-1 Kit](https://www.u-blox.com/en/product/xplr-aoa-1-kit).

### Data Optimization

The files under `src\Backend` contain the C++ backend that channels the data and calculates a point best fit to all the sensor angles and their positions. It can be compiled using `CMakeLists.txt`.

- **C++ Version:** CMAKE_CXX_STANDARD 14
- **CMake Version:** VERSION 3.21

Make sure to have imported the external NLohmann JSON parser into `external\JSON` for automatic and valid JSON parsing for inter-process communication.

`git submodule update --init --recursive`

### Display

Use Python 3.12 for the file `src\DataRetrievalAndUi\ui.py` to display the calculated data.

### Authors

Jonathan Simon \
Christoph Gründer