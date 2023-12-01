//main.cpp
// Created by chris on 19.10.2023.
//

#include "serial.h"
#include "iostream"
#include "calc.h"
#include "display.h"
#include <iomanip>


#include <stdio.h>
#include <stdlib.h>

#define STM32_VID 0x0403
#define STM32_PID 0x6015

#include <Python.h>

int main() {
    std::cout << "started" << std::endl;

   putenv("PYTHONPATH=C:\\Users\\chris\\AppData\\Local\\Programs\\Python\\Python310\\Lib\\site-packages");


    Py_Initialize();
    PyRun_SimpleString("import sys\nprint(sys.path)");
    
    if (!Py_IsInitialized()) {
        fprintf(stderr, "Failed to initialize Python interpreter\n");
        return -1;
    } 
    
    std::cout << "Initialized" << std::endl;

    PySys_SetPath(L"C:/Users/chris/OneDrive/Dokumente/DHBW/Studienarbeit/src/new");   
    // Import the Python module
    PyObject* pModule = PyImport_ImportModule("python_script");
    if (!pModule) {
        PyErr_Print();
        fprintf(stderr, "Failed to import Python module\n");
        Py_Finalize();
        return -1;
    }
    const wchar_t* pythonHome = Py_GetPythonHome();
    if (pythonHome) {
        // Convert the wide character string to UTF-8
        char pythonHomeUtf8[1024];
        wcstombs(pythonHomeUtf8, pythonHome, sizeof(pythonHomeUtf8));

        std::cout << "Python interpreter path: " << pythonHomeUtf8 << std::endl;
    } else {
        PyErr_Print();
        std::cout << "Python interpreter path: errror"  << std::endl;
    }

    // Call the Python function
    PyObject* pResult = PyObject_CallMethod(pModule, "python_function", NULL);
    pResult = PyObject_CallMethod(pModule, "python_function", NULL);
    if (!pResult) {
        PyErr_Print();
        fprintf(stderr, "Failed to call Python function\n");
        Py_XDECREF(pModule);
        Py_Finalize();
        return -1;
    }

    // Extract the result
    int result = PyLong_AsLong(pResult);
    if (result == -1 && PyErr_Occurred()) {
        PyErr_Print();
        fprintf(stderr, "Failed to convert Python result to int\n");
    }else {
        // Print the result
        std::cout << "Result from Python function: " << result << std::endl;
    }

    // Clean up
    Py_XDECREF(pResult);
    Py_XDECREF(pModule);
    Py_Finalize();

    return 0;
}