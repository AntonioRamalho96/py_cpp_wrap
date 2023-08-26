#pragma once

#include <string>
#include <map>

#include "pcw_argument_packing.hpp"
#include "pcw_conversions_py_to_cpp.hpp"
#include "py_cpp_wrap_utils.hpp"
#include "pwc_module_manager.hpp"

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
    PyCppWrap(const std::string &entry_point_script, const std::string &class_name, Args... args) :
        class_name{class_name},
        file_path{entry_point_script}
    {
        PyObject *pModule = module_manager.GetModule(entry_point_script);
        PyObject *pConstructor = PyCppWrapUtils::GetConstructor(pModule, class_name, entry_point_script);
        PyObject *pArgs=PyCppWrapArgPack::PythonArgs(args...);
        this->pThis = PyObject_CallObject(pConstructor, pArgs);
        PyCppWrapUtils::ValidateInstance(pThis, class_name, entry_point_script);
        Py_DECREF(pArgs);
    }

    /**
     * @brief Calls a function which returns something
     * 
     * @tparam T return type (might be int, string or double)
     * @tparam Args argument types might be int, std::string or double
     * @param method_name name of the method to call
     * @param args arguments to pass to that method (from the allowed types, up to 5 arguments)
     * @return T valued returned by the python method, converted to C++
     */
    template<typename T, typename... Args>
    T call(const std::string &method_name, Args... args)
    {
        PyObject * method = PyCppWrapUtils::GetMethod(this->pThis, method_name.c_str(), class_name, file_path);
        PyObject * pArgs = PyCppWrapArgPack::PythonArgs(args...);
        PyObject * result = PyObject_CallObject(method, pArgs);
        PyCppWrapUtils::ValidateResult(result, class_name, file_path, method_name);
        T result_cpp = PyCppWrapConvPy2Cpp::python2Cpp<T>(result, class_name, file_path, method_name);
        Py_DECREF(pArgs);
        return result_cpp;
    }

    /**
     * @brief Calls a method discarding the return, if existing
     * 
     * @tparam Args argument types might be int, std::string or double
     * @param method_name name of the method to call
     * @param args arguments to pass to the method (from the allowed types, up to 5 arguments)
     */
    template<typename... Args>
    void call(const std::string &method_name, Args... args)
    {
        PyObject * method = PyCppWrapUtils::GetMethod(this->pThis, method_name.c_str(), class_name, file_path);
        PyObject * pArgs = PyCppWrapArgPack::PythonArgs(args...);
        PyObject * result = PyObject_CallObject(method, pArgs);
        PyCppWrapUtils::ValidateResult(result, class_name, file_path, method_name);
        Py_DECREF(pArgs);
    }

    /**
     * @brief Returns a pointer to the python instance
     * 
     * @return PyObject* pointer to the actual python instance that is wrapped
     */
    PyObject* get() const{
        return this->pThis;
    }

    ~PyCppWrap()
    {
        Py_DECREF(pThis);
    }

    private:
        // Actual instance of the python object
        PyObject * pThis;
        // These two parameters are only stored for being used in error messages
        std::string class_name;
        std::string file_path;
        // Module manager
        static PyCppWrapModuleManager module_manager;
};

PyCppWrapModuleManager PyCppWrap::module_manager{};