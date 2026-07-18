/*
MIT License

Copyright (c) 2023 AntonioRamalho96

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include <Python.h>
#include <map>
#include <iostream>
#include <string>
#include <type_traits>

// Py Cpp wrap exceptions
class PyCppWrapException : public std::exception
{
public:
    explicit PyCppWrapException(std::string message) : m_message(std::move(message)) {}
    const char* what() const noexcept override { return m_message.c_str(); }

protected:
    std::string m_message;
};

class PyCppWrapBadFile : public PyCppWrapException
{
public:
    explicit PyCppWrapBadFile(const std::string &file_path);
private:
    static std::string make_msg(const std::string &file_path);
};

class PyCppWrapNonExistingClass : public PyCppWrapException
{
public:
    PyCppWrapNonExistingClass(const std::string &class_name, const std::string &file_path);
private:
    static std::string make_msg(const std::string &class_name, const std::string &file_path);
};

class PyCppWrapErrorInConstructor : public PyCppWrapException
{
public:
    PyCppWrapErrorInConstructor(const std::string &class_name, const std::string &file_path);
private:
    static std::string make_msg(const std::string &class_name, const std::string &file_path);
};


class PyCppWrapNonExistingMethod : public PyCppWrapException
{
public:
    PyCppWrapNonExistingMethod(const std::string &class_name, const std::string &file_path, const std::string &method_name);
private:
    static std::string make_msg(const std::string &class_name, const std::string &file_path, const std::string &method_name);
};

class PyCppWrapErrorInMethod : public PyCppWrapException
{
public:
    PyCppWrapErrorInMethod(const std::string &class_name, const std::string &file_path, const std::string &method_name);
private:
    static std::string make_msg(const std::string &class_name, const std::string &file_path, const std::string &method_name);
};


class PyCppWrapBadReturnType : public PyCppWrapException
{
public:
    PyCppWrapBadReturnType(const std::string &class_name, const std::string &file_path, const std::string &method_name, const std::string &expected_return_type, const std::string &actual_return_type);
private:
    static std::string make_msg(const std::string &class_name, const std::string &file_path, const std::string &method_name, const std::string &expected_return_type, const std::string &actual_return_type);
};




class PyCppWrap
{
public:
    /**
     * @brief Construct a new Py Cpp Wrap object
     * 
     * @tparam Args argument types might be int, std::string or double
     * @param entry_point_script path to python script where the class is defined
     * @param class_name name of the class to instantiate
     * @param args arguments for the constructor (from the allowed types, up to 5 arguments)
     */
    template<typename ...Args>
    PyCppWrap(const std::string &entry_point_script, const std::string &class_name, Args... args);
    ~PyCppWrap();

    /**
     * @brief Calls a function which returns something
     * 
     * @tparam T return type (might be int, string or double)
     * @tparam Args argument types might be int, std::string or double
     * @param method_name name of the method to call
     * @param args arguments to pass to that method (from the allowed types, up to 5 arguments)
     * @return T valued returned by the python method, converted to C++
     */
    template<typename T = void, typename... Args>
    T call(const std::string &method_name, Args... args);

    /**
     * @brief Returns a pointer to the python instance
     * 
     * @return PyObject* pointer to the actual python instance that is wrapped
     */
    PyObject* get() const;

    /**
     * @brief This class acquires the python GIL when constructed, 
     *        and releases it when destructed.
     * 
    */
    class PythonLock
    {
    public:
        PythonLock() : gstate{PyGILState_Ensure()} {   }
        ~PythonLock() { PyGILState_Release(gstate); }
    private:
        PyGILState_STATE gstate;
    };


private:
    // Actual instance of the python object
    PyObject * pThis;
    // These two parameters are only stored for being used in error messages
    std::string class_name;
    std::string file_path;
    class PyCppWrapModuleManager
    {
    public:
        PyCppWrapModuleManager();
        ~PyCppWrapModuleManager();
        PyObject * GetModule(const std::string & path);
    private:
        std::map<std::string, PyObject*> python_modules;
        bool m_initialized_by_this_manager{false};
        PyThreadState *m_thread_state{nullptr};
    };


    inline static PyCppWrapModuleManager g_module_manager;

    // Cpp to python conversion
    struct PyCppWrapConvCpp2Py
    {
        // Methods to handle double
        static char GetLetter(double &);
        static double GetValue(double &d);
    
        // Methods to handle PyObject *
        static char GetLetter(PyObject *);
        static PyObject *GetValue(PyObject *obj);
    
        // Methods to handle int
        static char GetLetter(int &);
        static int GetValue(int &d);
    
        // Methods to handle string
        static char GetLetter(std::string &);
        static char *GetValue(std::string &str);

        // Methods to handle string
        static char GetLetter(const char *);
        static const char *GetValue(const char *str);
    };
    
    // PyCppWrapWrgPack
    struct PyCppWrapArgPack
    {
    
        /**
        * @brief Creates a python args object
        *
        * @return PyObject* python object representing no arguments
        */
        static PyObject *PythonArgs();

        /**
        * @brief Creates a python args object. Supported types are int, std::string, double and PythonObject*
        *
        * @param argn nth rgument
        * @return PyObject* python object gathering the arguments
        */
        template <typename T1>
        static PyObject *PythonArgs(T1 arg1);
    
        template <typename T1, typename T2>
        static PyObject *PythonArgs(T1 arg1, T2 arg2);
    
        template <typename T1, typename T2, typename T3>
        static PyObject *PythonArgs(T1 arg1, T2 arg2, T3 arg3);
    
        template <typename T1, typename T2, typename T3, typename T4>
        static PyObject *PythonArgs(T1 arg1, T2 arg2, T3 arg3, T4 arg4);

        template <typename T1, typename T2, typename T3, typename T4, typename T5>
        static PyObject *PythonArgs(T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5);
    };

    // Convert Py to Cpp types
    struct PyCppWrapConvPy2Cpp
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
        static T python2Cpp(PyObject * pObj, const std::string &class_name, const std::string &file_path, const std::string &method_name);

    };



    struct PyCppWrapUtils
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
    static PyObject * GetConstructor(PyObject *module, const std::string &class_name, const std::string &file_path)
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
    static void ValidateInstance(PyObject *instance, const std::string &class_name, const std::string &file_path)
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
    static PyObject * GetMethod(PyObject *instance, const std::string &method_name, const std::string &class_name, const std::string &file_path)
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
    static void ValidateResult(PyObject *result, const std::string &class_name, const std::string &file_path, const std::string &method_name)
    {
        if (result == NULL)
        {
            PyErr_Print();
            throw PyCppWrapErrorInMethod(class_name, file_path, method_name);
        }
    }


};
};

/// Py Cpp Wrap Implementation
template<typename ...Args>
inline PyCppWrap::PyCppWrap(const std::string &entry_point_script, const std::string &class_name, Args... args) :
    class_name{class_name},
    file_path{entry_point_script}
{
    PythonLock lock{};
    PyObject *pModule = g_module_manager.GetModule(entry_point_script);
    PyObject *pConstructor = PyCppWrapUtils::GetConstructor(pModule, class_name, entry_point_script);
    PyObject *pArgs=PyCppWrapArgPack::PythonArgs(args...);
    this->pThis = PyObject_CallObject(pConstructor, pArgs);
    PyCppWrapUtils::ValidateInstance(pThis, class_name, entry_point_script);
    Py_DECREF(pArgs);
}


template<typename T , typename... Args>
inline T PyCppWrap::call(const std::string &method_name, Args... args)
{
    PythonLock lock{};
    PyObject * method = PyCppWrapUtils::GetMethod(this->pThis, method_name.c_str(), class_name, file_path);
    PyObject * pArgs = PyCppWrapArgPack::PythonArgs(args...);
    PyObject * result = PyObject_CallObject(method, pArgs);
    PyCppWrapUtils::ValidateResult(result, class_name, file_path, method_name);
    Py_DECREF(pArgs);
    if constexpr (!std::is_void_v<T>)
    {
        T result_cpp = PyCppWrapConvPy2Cpp::python2Cpp<T>(result, class_name, file_path, method_name);
        return result_cpp;
    }
}

inline PyObject* PyCppWrap::get() const{
    return this->pThis;
}

inline PyCppWrap::~PyCppWrap()
{
    PythonLock lock{};
    Py_DECREF(pThis);
}

// implement conversions from C++ to python
template<>
inline std::string PyCppWrap::PyCppWrapConvPy2Cpp::python2Cpp<std::string>(PyObject * pObj, const std::string &class_name, const std::string &file_path, const std::string &method_name)
{
    if(!PyUnicode_Check(pObj))
        throw PyCppWrapBadReturnType(class_name, file_path, method_name, "str", pObj->ob_type->tp_name);
    return PyUnicode_AsUTF8(pObj);
}

template<>
inline int PyCppWrap::PyCppWrapConvPy2Cpp::python2Cpp<int>(PyObject * pObj, const std::string &class_name, const std::string &file_path, const std::string &method_name)
{
    if(!PyLong_Check(pObj))
        throw PyCppWrapBadReturnType(class_name, file_path, method_name, "int", pObj->ob_type->tp_name);
    return PyLong_AsLong(pObj);
}

template<>
inline double PyCppWrap::PyCppWrapConvPy2Cpp::python2Cpp<double>(PyObject * pObj, const std::string &class_name, const std::string &file_path, const std::string &method_name)
{
    if(!PyFloat_Check(pObj))
        throw PyCppWrapBadReturnType(class_name, file_path, method_name, "float", pObj->ob_type->tp_name);
    return PyFloat_AsDouble(pObj);
}

template<>
inline PyObject * PyCppWrap::PyCppWrapConvPy2Cpp::python2Cpp<PyObject *>(PyObject * pObj, const std::string &class_name, const std::string &file_path, const std::string &method_name)
{
    return pObj;
}

// Definitions of conversions from C++ to python
inline char PyCppWrap::PyCppWrapConvCpp2Py::GetLetter(double &)
{
    return 'd';
}
inline double PyCppWrap::PyCppWrapConvCpp2Py::GetValue(double &d)
{
    return d;
}
inline char PyCppWrap::PyCppWrapConvCpp2Py::GetLetter(PyObject *)
{
    return 'O';
}
inline PyObject *PyCppWrap::PyCppWrapConvCpp2Py::GetValue(PyObject *obj)
{
    return obj;
}
inline char PyCppWrap::PyCppWrapConvCpp2Py::GetLetter(int &)
{
    return 'i';
}
inline int PyCppWrap::PyCppWrapConvCpp2Py::GetValue(int &d)
{
    return d;
}
inline char PyCppWrap::PyCppWrapConvCpp2Py::GetLetter(std::string &)
{
    return 'z';
}
inline char *PyCppWrap::PyCppWrapConvCpp2Py::GetValue(std::string &str)
{
    return str.data();
}
inline char PyCppWrap::PyCppWrapConvCpp2Py::GetLetter(const char *)
{
    return 'z';
}
inline const char *PyCppWrap::PyCppWrapConvCpp2Py::GetValue(const char *str)
{
    return str;
}

// Defintion of module manager
inline PyCppWrap::PyCppWrapModuleManager::PyCppWrapModuleManager()
{
    if (!Py_IsInitialized())
    {
        Py_Initialize();
        m_initialized_by_this_manager = true;
        m_thread_state = PyEval_SaveThread();
    }
}

inline PyCppWrap::PyCppWrapModuleManager::~PyCppWrapModuleManager()
{
    for(const auto &entry : python_modules)
        Py_DECREF(entry.second);
    if (m_thread_state != nullptr)
    {
        PyEval_RestoreThread(m_thread_state);
        Py_Finalize();
    }
}

inline PyObject * PyCppWrap::PyCppWrapModuleManager::GetModule(const std::string & path)
{
    PyCppWrapUtils::ScriptPath separated_path{path};
    if(python_modules.count(separated_path.filename_no_extension) == 0)
    {
        std::string import_command = std::string("import sys\nsys.path.append('") + separated_path.directory + "')\n";
        PyRun_SimpleString(import_command.c_str());

        PyObject * result = PyImport_ImportModule(separated_path.filename_no_extension.c_str());

        if(result == NULL)
        {
            std::cout << "calling py err print" << std::endl;
            PyErr_Print();
            std::cout << "calling py err print - DONE" << std::endl;
            throw PyCppWrapBadFile(path);
        }
        python_modules[separated_path.filename_no_extension] = result;
    } 
    return python_modules[separated_path.filename_no_extension];
}


// Define python argument packer
inline PyObject *PyCppWrap::PyCppWrapArgPack::PythonArgs()
{
    return Py_BuildValue("()");
}

template <typename T1>
inline PyObject *PyCppWrap::PyCppWrapArgPack::PythonArgs(T1 arg1)
{
    std::string formater = "(";
    formater += PyCppWrapConvCpp2Py::GetLetter(arg1);
    formater += ")";
    return Py_BuildValue(formater.c_str(), 
            PyCppWrapConvCpp2Py::GetValue(arg1));
}
    
template <typename T1, typename T2>
inline PyObject *PyCppWrap::PyCppWrapArgPack::PythonArgs(T1 arg1, T2 arg2)
{
    std::string formater = "(";
    formater += PyCppWrapConvCpp2Py::GetLetter(arg1);
    formater += ',';
    formater += PyCppWrapConvCpp2Py::GetLetter(arg2);
    formater += ")";
    return Py_BuildValue(formater.c_str(), 
            PyCppWrapConvCpp2Py::GetValue(arg1), 
            PyCppWrapConvCpp2Py::GetValue(arg2));
}
    
template <typename T1, typename T2, typename T3>
inline PyObject *PyCppWrap::PyCppWrapArgPack::PythonArgs(T1 arg1, T2 arg2, T3 arg3)
{
    std::string formater = "(";
    formater += PyCppWrapConvCpp2Py::GetLetter(arg1);
    formater += ',';
    formater += PyCppWrapConvCpp2Py::GetLetter(arg2);
    formater += ',';
    formater += PyCppWrapConvCpp2Py::GetLetter(arg3);
    formater += ")";
    return Py_BuildValue(formater.c_str(), 
            PyCppWrapConvCpp2Py::GetValue(arg1), 
            PyCppWrapConvCpp2Py::GetValue(arg2), 
            PyCppWrapConvCpp2Py::GetValue(arg3));
}

template <typename T1, typename T2, typename T3, typename T4>
inline PyObject *PyCppWrap::PyCppWrapArgPack::PythonArgs(T1 arg1, T2 arg2, T3 arg3, T4 arg4)
{
    std::string formater = "(";
    formater += PyCppWrapConvCpp2Py::GetLetter(arg1);
    formater += ',';
    formater += PyCppWrapConvCpp2Py::GetLetter(arg2);
    formater += ',';
    formater += PyCppWrapConvCpp2Py::GetLetter(arg3);
    formater += ',';
    formater += PyCppWrapConvCpp2Py::GetLetter(arg4);
    formater += ")";
    return Py_BuildValue(formater.c_str(), 
            PyCppWrapConvCpp2Py::GetValue(arg1), 
            PyCppWrapConvCpp2Py::GetValue(arg2), 
            PyCppWrapConvCpp2Py::GetValue(arg3), 
            PyCppWrapConvCpp2Py::GetValue(arg4));
}

template <typename T1, typename T2, typename T3, typename T4, typename T5>
inline PyObject *PyCppWrap::PyCppWrapArgPack::PythonArgs(T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5)
{
    std::string formater = "(";
    formater += PyCppWrapConvCpp2Py::GetLetter(arg1);
    formater += ',';
    formater += PyCppWrapConvCpp2Py::GetLetter(arg2);
    formater += ',';
    formater += PyCppWrapConvCpp2Py::GetLetter(arg3);
    formater += ',';
    formater += PyCppWrapConvCpp2Py::GetLetter(arg4);
    formater += ")";
    formater += PyCppWrapConvCpp2Py::GetLetter(arg5);
    formater += ")";
    return Py_BuildValue(formater.c_str(), 
                            PyCppWrapConvCpp2Py::GetValue(arg1), 
                            PyCppWrapConvCpp2Py::GetValue(arg2), 
                            PyCppWrapConvCpp2Py::GetValue(arg3), 
                            PyCppWrapConvCpp2Py::GetValue(arg4), 
                            PyCppWrapConvCpp2Py::GetValue(arg5));
}


// Py Cpp wrap exceptions implementation
// Bad file
inline PyCppWrapBadFile::PyCppWrapBadFile(const std::string &file_path) : PyCppWrapException(make_msg(file_path)) {}
inline std::string PyCppWrapBadFile::make_msg(const std::string &file_path)
{
    return std::string("Can't import file \"") + file_path + "\"";
}

// non existing class
inline PyCppWrapNonExistingClass::PyCppWrapNonExistingClass(const std::string &class_name, const std::string &file_path) : PyCppWrapException(make_msg(class_name, file_path)) {}
inline std::string PyCppWrapNonExistingClass::make_msg(const std::string &class_name, const std::string &file_path)
{
    return std::string("No such constructor \"") + class_name + "\" in file \"" + file_path + "\"";
}

// Error in contructor
inline PyCppWrapErrorInConstructor::PyCppWrapErrorInConstructor(const std::string &class_name, const std::string &file_path) : PyCppWrapException(make_msg(class_name, file_path)) {}
inline std::string PyCppWrapErrorInConstructor::make_msg(const std::string &class_name, const std::string &file_path)
{
    return std::string("Error calling constructor \"") + class_name + "\" in file \"" + file_path + "\"";
}

// Non existing method
inline PyCppWrapNonExistingMethod::PyCppWrapNonExistingMethod(const std::string &class_name, const std::string &file_path, const std::string &method_name) : PyCppWrapException(make_msg(class_name, file_path, method_name)) {}
inline std::string PyCppWrapNonExistingMethod::make_msg(const std::string &class_name, const std::string &file_path, const std::string &method_name)
{
    return std::string("No such method \"") + method_name + "\" in class \"" + class_name + "\" in file \"" + file_path + "\"";
}

// Error in method
inline PyCppWrapErrorInMethod::PyCppWrapErrorInMethod(const std::string &class_name, const std::string &file_path, const std::string &method_name) : PyCppWrapException(make_msg(class_name, file_path, method_name)) {}
inline std::string PyCppWrapErrorInMethod::make_msg(const std::string &class_name, const std::string &file_path, const std::string &method_name)
{
    return std::string("Error in method \"") + method_name + "\" in class \"" + class_name + "\" in file \"" + file_path + "\"";
}

// Bad return type
inline PyCppWrapBadReturnType::PyCppWrapBadReturnType(const std::string &class_name, const std::string &file_path, const std::string &method_name, const std::string &expected_return_type, const std::string &actual_return_type) : PyCppWrapException(make_msg(class_name, file_path, method_name, expected_return_type, actual_return_type)) {}
inline std::string PyCppWrapBadReturnType::make_msg(const std::string &class_name, const std::string &file_path, const std::string &method_name, const std::string &expected_return_type, const std::string &actual_return_type)
{
    return std::string("Error in method \"") + method_name + "\" in class \"" + class_name + "\" in file \"" + file_path +
        ". Expcting return type to be \"" + expected_return_type + "\" but was \"" + actual_return_type;
}