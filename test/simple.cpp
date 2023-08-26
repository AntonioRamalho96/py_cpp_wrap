#include "py_cpp_wrap.hpp"
#include <cassert>

int main()
{
    PyCppWrap my_class(std::string(__DIR__) + "/py_files/simple_class.py", "SimpleClass", 4);
    int result =  my_class.call<int>("add", 5);
    assert(result == 9);
    my_class.call("add_no_return", 1);
    result = my_class.call<int>("add", 5);
    assert(result == 15);


    double result_d;
    std::string result_s;
    result_s = my_class.call<std::string>("return_val_str_repr");
    assert(result_s == "15");

    result_d = my_class.call<double>("return_val_divided_by_2");
    assert(result_d == 7.5);
    std::cout << "SUCCESS!!" << std::endl;

}