# This is the README for Studienarbeit Bluetoothtracker

The sourcecoe above contains three independent software modules, that communicate over websockets.

## HowTo
To start and execute the code, try the `startall.bat` script for Windows machines. 
### Dataretrieval
Use Py 3.12 to execute the files src\DataRetrievalAndUi\mockBluetooth.py and src\DataRetrievalAndUi\getAnchorData.py retrieve/mock the needed information of the XPLR-AOA-1 Kit https://www.u-blox.com/en/product/xplr-aoa-1-kit.
### Dataoptimisation
The Files under src\Backend contain the C++ Backend that channels the Data and calculates a Point best fit to all the Sensorangles and their Positions. It can be compiled by CMakeLists.txt.
C++ Version: CMAKE_CXX_STANDARD 14
CMake Version: VERSION 3.21
Make sure to have importet the external NLohammParser into external\JSON that we use for json parsing for automatic and valid JSON-Parsing for the inter process communication.
`git submodule update --init --recursive`

### Display
Use Py3.12 for the file src\DataRetrievalAndUi\ui.py to display the calculated Data.


### Authors

Jonathan Simon \
Christoph Gründer