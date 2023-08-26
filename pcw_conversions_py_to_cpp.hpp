
#pragma once

#include <Python.h>
#include <string>
#include <iostream>

#include "py_cpp_wrap_exceptions.hpp"

namespace PyCppWrapConvPy2Cpp
{


    /**
     * @brief Converts a python object to a C++ type. It throws a PyCppWrapBadReturnType in the case
     * the PyObject type does not match the expected one
     * 
     * @tparam T destination cpp type
     * @param pObj Python object to be converted
     * @param class_name class that owns the method called, it is used for error message purposes only
     * @param file_path file where the method called is defined, it is used for error message purposes only
     * @param method_name called method, it is used for error message purposes only
     * @return T value in C++
     */
    template<typename T>
    T python2Cpp(PyObject * pObj, const std::string &class_name, const std::string &file_path, const std::string &method_name);

    template<>
    std::string python2Cpp<std::string>(PyObject * pObj, const std::string &class_name, const std::string &file_path, const std::string &method_name)
    {
        if(!PyUnicode_Check(pObj))
            throw PyCppWrapBadReturnType(class_name, file_path, method_name, "str", pObj->ob_type->tp_name);
        return PyUnicode_AsUTF8(pObj);
    }

    template<>
    int python2Cpp<int>(PyObject * pObj, const std::string &class_name, const std::string &file_path, const std::string &method_name)
    {
        if(!PyLong_Check(pObj))
            throw PyCppWrapBadReturnType(class_name, file_path, method_name, "int", pObj->ob_type->tp_name);
        return PyLong_AsLong(pObj);
    }

    template<>
    double python2Cpp<double>(PyObject * pObj, const std::string &class_name, const std::string &file_path, const std::string &method_name)
    {
        if(!PyFloat_Check(pObj))
            throw PyCppWrapBadReturnType(class_name, file_path, method_name, "float", pObj->ob_type->tp_name);
        return PyFloat_AsDouble(pObj);
    }

    template<>
    PyObject * python2Cpp<PyObject *>(PyObject * pObj, const std::string &class_name, const std::string &file_path, const std::string &method_name)
    {
        return pObj;
    }
};