
#pragma once

#include <Python.h>
#include <string>
#include <iostream>

#include "py_cpp_wrap_exceptions.hpp"

// Cpp to python
namespace PyCppWrapConvCpp2Py
{

    // Methods to handle double
    char GetLetter(double &)
    {
        return 'd';
    }
    double GetValue(double &d)
    {
        return d;
    }

    // Methods to handle PyObject *
    char GetLetter(PyObject *)
    {
        return 'O';
    }
    PyObject *GetValue(PyObject *obj)
    {
        return obj;
    }

    // Methods to handle int
    char GetLetter(int &)
    {
        return 'i';
    }
    int GetValue(int &d)
    {
        return d;
    }

    // Methods to handle string
    char GetLetter(std::string &)
    {
        return 'z';
    }
    char *GetValue(std::string &str)
    {
        return &str[0];
    }

    // Methods to handle string
    char GetLetter(const char *)
    {
        return 'z';
    }
    const char *GetValue(const char *str)
    {
        return str;
    }
};