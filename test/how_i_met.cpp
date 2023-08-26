#include "py_cpp_wrap.hpp"
#include <cassert>

int main()
{
    std::string py_script_path = std::string(__DIR__) + "/py_files/person.py";
    std::string class_name = "Person";

    // The Person constructor takes two arguments, name and age
    PyCppWrap barney(py_script_path, class_name, "Barney", 30);

    // The method "introduce_mydelf" takes no arguments and returns a string
    assert(barney.call<std::string>("introduce_myself") == "Hello, my name is Barney and I am 30 years old");

    // The method "set_age" takes a single argument (new age) and does not return anything
    barney.call("set_age", 35);

    // If we want to return some other type, like int, we simply pass it to the template type
    assert(barney.call<int>("get_age") == 35);

    // Now Barney is older...
    assert(barney.call<std::string>("introduce_myself") == "Hello, my name is Barney and I am 35 years old");

    // Let us create another instance
    PyCppWrap ted(py_script_path, class_name, "Ted", 30);

    // And pass it as an argument to the other instance method, using the get() method
    assert(barney.call<std::string>("introduce_other", ted.get()) == "Hi, have you met Ted");

    // Ted is obviously also capable of introducing himself
    assert(ted.call<std::string>("introduce_myself") == "Hello, my name is Ted and I am 30 years old");
    
    std::cout << "SUCCESS!!" << std::endl;

}