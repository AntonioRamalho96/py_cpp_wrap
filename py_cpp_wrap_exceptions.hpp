#pragma once

#include <stdexcept>


class PyCppWrapBadFile : public std::runtime_error
{
public:
    PyCppWrapBadFile(const std::string &file_path) : std::runtime_error(make_msg(file_path).c_str()) {}
private:
    static std::string make_msg(const std::string &file_path)
    {
        return std::string("Can't import file \"") + file_path + "\"";
    }
};

class PyCppWrapNonExistingClass : public std::runtime_error
{
public:
    PyCppWrapNonExistingClass(const std::string &class_name, const std::string &file_path) : std::runtime_error(make_msg(class_name, file_path).c_str()) {}
private:
    static std::string make_msg(const std::string &class_name, const std::string &file_path)
    {
        return std::string("No such constructor \"") + class_name + "\" in file \"" + file_path + "\"";
    }
};

class PyCppWrapErrorInConstructor : public std::runtime_error
{
public:
    PyCppWrapErrorInConstructor(const std::string &class_name, const std::string &file_path) : std::runtime_error(make_msg(class_name, file_path).c_str()) {}
private:
    static std::string make_msg(const std::string &class_name, const std::string &file_path)
    {
        return std::string("Error calling constructor \"") + class_name + "\" in file \"" + file_path + "\"";
    }
};


class PyCppWrapNonExistingMethod : public std::runtime_error
{
public:
    PyCppWrapNonExistingMethod(const std::string &class_name, const std::string &file_path, const std::string &method_name) : std::runtime_error(make_msg(class_name, file_path, method_name).c_str()) {}
private:
    static std::string make_msg(const std::string &class_name, const std::string &file_path, const std::string &method_name)
    {
        return std::string("No such method \"") + method_name + "\" in class \"" + class_name + "\" in file \"" + file_path + "\"";
    }
};

class PyCppWrapErrorInMethod : public std::runtime_error
{
public:
    PyCppWrapErrorInMethod(const std::string &class_name, const std::string &file_path, const std::string &method_name) : std::runtime_error(make_msg(class_name, file_path, method_name).c_str()) {}
private:
    static std::string make_msg(const std::string &class_name, const std::string &file_path, const std::string &method_name)
    {
        return std::string("Error in method \"") + method_name + "\" in class \"" + class_name + "\" in file \"" + file_path + "\"";
    }
};


class PyCppWrapBadReturnType : public std::runtime_error
{
public:
    PyCppWrapBadReturnType(const std::string &class_name, const std::string &file_path, const std::string &method_name, const std::string &expected_return_type, const std::string &actual_return_type) : std::runtime_error(make_msg(class_name, file_path, method_name, expected_return_type, actual_return_type).c_str()) {}
private:
    static std::string make_msg(const std::string &class_name, const std::string &file_path, const std::string &method_name, const std::string &expected_return_type, const std::string &actual_return_type)
    {
        return std::string("Error in method \"") + method_name + "\" in class \"" + class_name + "\" in file \"" + file_path +
            ". Expcting return type to be \"" + expected_return_type + "\" but was \"" + actual_return_type;
    }
};