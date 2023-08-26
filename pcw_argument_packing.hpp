
#pragma once

#include "pcw_conversions_cpp_to_py.hpp"

namespace PyCppWrapArgPack
{
    using namespace PyCppWrapConvCpp2Py;

    /**
     * @brief Creates a python args object
     *
     * @return PyObject* python object representing no arguments
     */
    PyObject *PythonArgs()
    {
        return Py_BuildValue("()");
    }
    /**
     * @brief Creates a python args object. Supported types are int, std::string, double and PythonObject*
     *
     * @param arg1 first rgument
     * @return PyObject* python object gathering the arguments
     */
    template <typename T1>
    PyObject *PythonArgs(T1 arg1)
    {
        std::string formater = "(";
        formater += GetLetter(arg1);
        formater += ")";
        return Py_BuildValue(formater.c_str(), GetValue(arg1));
    }

    /**
     * @brief Creates a python args object. Supported types are int, std::string, double and PythonObject*
     *
     * @param arg1 first rgument
     * @param arg2 second rgument
     * @return PyObject* python object gathering the arguments
     */
    template <typename T1, typename T2>
    PyObject *PythonArgs(T1 arg1, T2 arg2)
    {
        std::string formater = "(";
        formater += GetLetter(arg1);
        formater += ',';
        formater += GetLetter(arg2);
        formater += ")";
        return Py_BuildValue(formater.c_str(), GetValue(arg1), GetValue(arg2));
    }

    /**
     * @brief Creates a python args object. Supported types are int, std::string, double and PythonObject*
     *
     * @param arg1 first rgument
     * @param arg2 second rgument
     * @param arg3 third rgument
     * @return PyObject* python object gathering the arguments
     */
    template <typename T1, typename T2, typename T3>
    PyObject *PythonArgs(T1 arg1, T2 arg2, T3 arg3)
    {
        std::string formater = "(";
        formater += GetLetter(arg1);
        formater += ',';
        formater += GetLetter(arg2);
        formater += ',';
        formater += GetLetter(arg3);
        formater += ")";
        return Py_BuildValue(formater.c_str(), GetValue(arg1), GetValue(arg2), GetValue(arg3));
    }

    /**
     * @brief Creates a python args object. Supported types are int, std::string, double and PythonObject*
     *
     * @param arg1 first rgument
     * @param arg2 second rgument
     * @param arg3 third rgument
     * @param arg4 fourth rgument
     * @return PyObject* python object gathering the arguments
     */
    template <typename T1, typename T2, typename T3, typename T4>
    PyObject *PythonArgs(T1 arg1, T2 arg2, T3 arg3, T4 arg4)
    {
        std::string formater = "(";
        formater += GetLetter(arg1);
        formater += ',';
        formater += GetLetter(arg2);
        formater += ',';
        formater += GetLetter(arg3);
        formater += ',';
        formater += GetLetter(arg4);
        formater += ")";
        return Py_BuildValue(formater.c_str(), GetValue(arg1), GetValue(arg2), GetValue(arg3), GetValue(arg4));
    }

    /**
     * @brief Creates a python args object. Supported types are int, std::string, double and PythonObject*
     *
     * @param arg1 first rgument
     * @param arg2 second rgument
     * @param arg3 third rgument
     * @param arg4 fourth rgument
     * @param arg5 fifth rgument
     * @return PyObject* python object gathering the arguments
     */
    template <typename T1, typename T2, typename T3, typename T4, typename T5>
    PyObject *PythonArgs(T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5)
    {
        std::string formater = "(";
        formater += GetLetter(arg1);
        formater += ',';
        formater += GetLetter(arg2);
        formater += ',';
        formater += GetLetter(arg3);
        formater += ',';
        formater += GetLetter(arg4);
        formater += ")";
        formater += GetLetter(arg5);
        formater += ")";
        return Py_BuildValue(formater.c_str(), GetValue(arg1), GetValue(arg2), GetValue(arg3), GetValue(arg4), GetValue(arg5));
    }
};