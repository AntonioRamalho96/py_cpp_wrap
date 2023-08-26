#pragma once

#include <string>
#include <Python.h>
#include "py_cpp_wrap_exceptions.hpp"

namespace PyCppWrapUtils
{
    struct ScriptPath
    {
        ScriptPath(const std::string & path)
        {
            int last_slash_pos = path.find_last_of('/');
            if(last_slash_pos != std::string::npos) 
            {
                filename_no_extension = path.substr(last_slash_pos + 1, path.size() - last_slash_pos -4);
                directory = path.substr(0, last_slash_pos);  
            } else {
                // No bar
                filename_no_extension = path;
                directory = ".";  
            }
        }

        std::string filename_no_extension{};
        std::string directory{};
    };


    /**
     * @brief Get the Constructor object
     * 
     * @param module py object with the module
     * @param class_name name of the class to call constructor
     * @param file_path path to the file where class is defined, for error message only
     * @return PyObject* callable object with constructor
     */
    PyObject * GetConstructor(PyObject *module, const std::string &class_name, const std::string &file_path)
    {
        PyObject *pDict = PyModule_GetDict(module); // pDict is a borrowed reference 
        PyObject *pConstructor = PyDict_GetItemString(pDict, class_name.c_str()); // pFunc is also a borrowed reference 
        if (!PyCallable_Check(pConstructor))
        {
            PyErr_Print();
            throw PyCppWrapNonExistingClass(class_name, file_path);
        }
        return pConstructor;
    }
    
    /**
     * @brief Validates if an instance was correctly obtainned
     * 
     * @param instance instance to validate (basically it is checked against null pointer)
     * @param class_name name of the class (just used in error message)
     * @param file_path path to the python file (just used in error message)
     */
    void ValidateInstance(PyObject *instance, const std::string &class_name, const std::string &file_path)
    {
        if (instance == NULL)
        {
            PyErr_Print();
            throw PyCppWrapErrorInConstructor(class_name, file_path);
        }
    }

    /**
     * @brief Gets a method from an instance of a class
     * 
     * @param instance instance
     * @param method_name name of the method
     * @param class_name name of the class (for error message only)
     * @param file_path path to the python file (for error message only)
     * @return PyObject* callable method
     */
    PyObject * GetMethod(PyObject *instance, const std::string &method_name, const std::string &class_name, const std::string &file_path)
    {
        PyObject *method_callable =  PyObject_GetAttrString(instance, method_name.c_str());

        if (!PyCallable_Check(method_callable))
        {
            PyErr_Print();
            throw PyCppWrapNonExistingMethod(class_name, file_path, method_name);
        }
        return method_callable;
    }

    /**
     * @brief Validates if an instance was correctly obtainned
     * 
     * @param instance instance to validate (basically it is checked against null pointer)
     * @param class_name name of the class (just used in error message)
     * @param file_path path to the python file (just used in error message)
     */
    void ValidateResult(PyObject *result, const std::string &class_name, const std::string &file_path, const std::string &method_name)
    {
        if (result == NULL)
        {
            PyErr_Print();
            throw PyCppWrapErrorInMethod(class_name, file_path, method_name);
        }
    }


};