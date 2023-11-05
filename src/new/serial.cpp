//
// Created by chris on 19.10.2023.
//
#include "iostream"
#include "serial.h"
#include <windows.h>
#include <tchar.h>
using namespace std;
serial::serial(int port) {
    this->port=port;

    std::cout<<"port "<< port<<" wird erstellt"<<std::endl;
    this->initialize();
}
void PrintCommState(DCB dcb)
{
    //  Print some of the DCB structure values
    _tprintf( TEXT("\nBaudRate = %d, ByteSize = %d, Parity = %d, StopBits = %d\n"),
              dcb.BaudRate,
              dcb.ByteSize,
              dcb.Parity,
              dcb.StopBits );
}
int serial::initialize() {
    std::string portName = "COM"+ std::to_string(this->port); // Change this to your specific COM port

    // Open the COM port
    const char* cstr;
    cstr=portName.c_str();


   hSerial = CreateFile(cstr, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING,
                          FILE_ATTRIBUTE_NORMAL, 0);
    if (hSerial == INVALID_HANDLE_VALUE) {
        if (GetLastError() == ERROR_FILE_NOT_FOUND) {
            // serial port not found. Handle error here.
            cerr<<"port not found"<<endl;
        }
        // any other error. Handle error here.
        cerr<<"error during handle creation"<<endl;
    }
    cout<<"Errorcode: "<<GetLastError()<<"handle: "<<hSerial<<endl;

    DCB dcbSerialParam = {0};
    dcbSerialParam.DCBlength = sizeof(dcbSerialParam);

    if (!GetCommState(hSerial, &dcbSerialParam)) {
        // handle error here
        cerr<<"error during GetCommState"<<endl;
    }

    dcbSerialParam.BaudRate = CBR_19200;
    dcbSerialParam.ByteSize = 8;
    dcbSerialParam.StopBits = ONESTOPBIT;
    dcbSerialParam.Parity = NOPARITY;

    if (!SetCommState(hSerial, &dcbSerialParam)) {
        // handle error here
        cerr<<"error during SetCommState"<<endl;
    }


}
float parseangle(DWORD bytesread){
    std::cout<<bytesread<<std::endl;
    return 0;
}

float serial::getAngle() {
    // Read data from the COM port
    const int n=255;
    char buffer[n + 1]={0};
    DWORD bytesRead=0;

    if (ReadFile(hSerial, buffer, n, &bytesRead, NULL)) {
        std::cout << "Received " << bytesRead << " bytes: " << buffer << std::endl;
    } else {
        std::cerr << "Error reading from COM port." << std::endl;
    }
    float returner= parseangle(bytesRead);
    // Close the COM port
    std::cout<<"port:"<<port<<" Winkel empfangen:"<<returner<<std::endl;
    CloseHandle(hSerial);
    return returner;
}