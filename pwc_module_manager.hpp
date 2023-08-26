#pragma once

#include <map>
#include <string>
#include <Python.h>
#include "py_cpp_wrap_utils.hpp"

class  PyCppWrapModuleManager
{
public:
     PyCppWrapModuleManager()
     {
        Py_Initialize();
     }
    ~ PyCppWrapModuleManager()
    {
        for(const auto &entry : python_modules)
            Py_DECREF(entry.second);
        Py_Finalize();
    }

    PyObject * GetModule(const std::string & path)
    {
        PyCppWrapUtils::ScriptPath separated_path{path};

        if(python_modules.count(separated_path.filename_no_extension) == 0)
        {
            std::string import_command = std::string("import sys\nsys.path.append('") + separated_path.directory + "')\n";
            PyRun_SimpleString(import_command.c_str());

            PyObject * result = PyImport_ImportModule(separated_path.filename_no_extension.c_str());

            if(result == NULL)
            {
                PyErr_Print();
                throw PyCppWrapBadFile(path);
            }
            python_modules[separated_path.filename_no_extension] = result;
        } 
        return python_modules[separated_path.filename_no_extension];
    }
private:
    std::map<std::string, PyObject*> python_modules;
};