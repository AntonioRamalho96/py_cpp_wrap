#include "py_cpp_wrap.hpp"
#include "py_cpp_wrap_exceptions.hpp"

#define ASSERT_EXCEPTION_THROWN(expression, my_exception)                     \
    try                                                                       \
    {                                                                         \
        expression;                                                           \
        std::cout << "Error, exception should have been thrown" << std::endl; \
        assert(false);                                                        \
    }                                                                         \
    catch (const my_exception &e)                                             \
    {                                                                         \
        std::cerr << e.what() << '\n';                                        \
    }

namespace Paths
{
    std::string dir = std::string{__DIR__} + "/py_files/";
    std::string simple_class =  dir + "simple_class.py";
    std::string bad_sintax =  dir + "bad_sintax.py";
    std::string classes_with_errors =  dir + "classes_with_errors.py";
    std::string return_your_type =  dir + "return_your_type.py";
};

void NonExistingFile();
void BadFileSintax();
void NonExistingClass();
void ErrorInConstructor();
void NonExistingMethod();
void ErrorInMethod();
void BadArgumentNumber();
void BadArgumentType();
void BadReturnType();

int main()
{
    NonExistingFile();
    BadFileSintax();
    NonExistingClass();
    ErrorInConstructor();
    NonExistingMethod();
    ErrorInMethod();
    BadArgumentNumber();
    BadArgumentType();
    BadReturnType();
    std::cout << "SUCCESS!!" << std::endl;
}

void NonExistingFile()
{
    ASSERT_EXCEPTION_THROWN(PyCppWrap("bad/path", "useless"), PyCppWrapBadFile);
}

void BadFileSintax()
{
    ASSERT_EXCEPTION_THROWN(PyCppWrap(Paths::bad_sintax, "useless"), PyCppWrapBadFile);
}

void NonExistingClass()
{
    ASSERT_EXCEPTION_THROWN(PyCppWrap(Paths::simple_class, "bad_class_name"), PyCppWrapNonExistingClass);
}

void ErrorInConstructor()
{
    ASSERT_EXCEPTION_THROWN(PyCppWrap(Paths::classes_with_errors, "ErrorConstructor"), PyCppWrapErrorInConstructor);
}
void NonExistingMethod()
{
    PyCppWrap my_class(Paths::simple_class, "SimpleClass", 3);
    ASSERT_EXCEPTION_THROWN(my_class.call("bad_method_name"), PyCppWrapNonExistingMethod);
    std::cout << "Here" << std::endl;
    ASSERT_EXCEPTION_THROWN(my_class.call<int>("bad_method_name"), PyCppWrapNonExistingMethod);
}
void ErrorInMethod()
{
    PyCppWrap my_class(Paths::classes_with_errors, "ErrorMethod");
    ASSERT_EXCEPTION_THROWN(my_class.call("method"), PyCppWrapErrorInMethod);
}
void BadArgumentNumber()
{
    PyCppWrap my_class(Paths::simple_class, "SimpleClass", 4);
    ASSERT_EXCEPTION_THROWN(my_class.call<int>("add", 5, 2), PyCppWrapErrorInMethod);
}
void BadArgumentType()
{
    PyCppWrap my_class(Paths::simple_class, "SimpleClass", 4);
    ASSERT_EXCEPTION_THROWN(my_class.call<int>("add", "bad type arg"), PyCppWrapErrorInMethod);
}
void BadReturnType()
{
    PyCppWrap my_class(Paths::return_your_type, "ReturnYourType");

    my_class.call<std::string>("return_str");
    my_class.call<int>("return_int");
    my_class.call<double>("return_double");

    ASSERT_EXCEPTION_THROWN(my_class.call<int>("return_str"), PyCppWrapBadReturnType);
    ASSERT_EXCEPTION_THROWN(my_class.call<int>("return_double"), PyCppWrapBadReturnType);


    ASSERT_EXCEPTION_THROWN(my_class.call<std::string>("return_int"), PyCppWrapBadReturnType);
    ASSERT_EXCEPTION_THROWN(my_class.call<std::string>("return_double"), PyCppWrapBadReturnType);
    
    ASSERT_EXCEPTION_THROWN(my_class.call<double>("return_str"), PyCppWrapBadReturnType);
    ASSERT_EXCEPTION_THROWN(my_class.call<double>("return_int"), PyCppWrapBadReturnType);

    ASSERT_EXCEPTION_THROWN(my_class.call<std::string>("return_nothing"), PyCppWrapBadReturnType);
    ASSERT_EXCEPTION_THROWN(my_class.call<int>("return_nothing"), PyCppWrapBadReturnType);
    ASSERT_EXCEPTION_THROWN(my_class.call<double>("return_nothing"), PyCppWrapBadReturnType);

}