#include "py_cpp_wrap.hpp"
#include <cassert>

#define PATH_TO_SCRIPT __DIR__ "/py_files/person_class.py"

int main()
{
    PyCppWrap my_instance(PATH_TO_SCRIPT, "PersonClass", "John", 30);
    assert(my_instance.call<std::string>("greet", "Mr.") == "Mr. John"); // Returns std::string{"Mr. John"}
    assert(my_instance.call<int>("get_age") == 30); // Returns 30
    my_instance.call("increment_age"); // Does not return anything
    assert(my_instance.call<int>("get_age") == 31); // Returns 31
    std::cout << "SUCCESS!!" << std::endl;
}