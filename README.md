# Py Cpp Wrap
This header_only library aims to provide a wrapper for using Python 3 classes in C++.

## Example
Imagine you have a python class:
```python
class PersonClass:
    def __init__(self, name, age):
        self.__name = name
        self.__age = age
    def greet(self, title):
        return title + " " + self.__name
    def get_age(self):
        return self.__age
    def increment_age(self):
        self.__age+=1
```
Here is how to create an instance and call each method with this library in C++:
```cpp
#include "py_cpp_wrap.hpp"

int main()
{
    PyCppWrap my_instance("/path/to/person_class.py", "PersonClass", "John", 30); // Instantiate the class
    my_instance.call<std::string>("greet", "Mr."); // Returns std::string{"Mr. John"}
    my_instance.call<int>("get_age"); // Returns 30
    my_instance.call("increment_age"); // Does not return anything
}
```


## Compiling
To compile your code for using this class you just need to: 
 - **include the header "py_cpp_wrap.py"** - all the other hpp files in the repo must be in the same folder
 - **Determine where "Python.h" is** - to do so you must find two first numbers of the version (e.g. 3.8) and
 tell the compiler to look for headers in the respective python directory (e.g. passing -I /usr/include/python3.8)
 - **Link "libpythonX.X.so" is** - to do so you must find two first numbers of the version (e.g. 3.8) and
 tell the compiler to link the appropriate library (e.g. passing -l python3.8)


Some nice examples of usage and how to compile the code can be found in the *test* folder
of the current repository. Look at the *Makefile* there to check on the compile commands.

To compile and run the tests run the following commands on the *test* directory of the current repository.
```
    make
    make test
```

## Supported types
 At this moment we support the following types:
  - **conversion from C++ to python** - int, std::string, double and PyObject*
  - **conversion from python to C++** - int, std::string, double and PyObject*

## Some actual documentation

This section shows briefly the methods of the main class. For documentation about all methods a doxygen documentation can be found [here](./html_doc/index.html).

Here are the methods of the **PyCppWrap** class:

___________________________________________________________________________
```cpp
    PyCppWrap(const std::string &entry_point_script, const std::string &class_name, Args... args);
```
Construct a new Py Cpp Wrap object
 - **entry_point_script** - path to python script where the class is defined
 - **class_name** - name of the class to instantiate
 - **args** - arguments for the constructor (from the allowed types, up to five arguments)
___________________________________________________________________________
```cpp
    T call<T>(const std::string &method_name, Args... args)
```
Calls a function which returns something
 - **method_name** - name of the method to call
 - **args** - arguments to pass to that method (from the allowed types, up to five arguments)
 - **return** - T value returned by the python method, converted to C++
___________________________________________________________________________

```cpp
    void call(const std::string &method_name, Args... args);
```
Calls a method discarding the return, if existing
 - **method_name** - name of the method to call
 - **args** - arguments to pass to the method (from the allowed types, up to five arguments)

___________________________________________________________________________
```cpp
    PyObject* get() const;
```
Returns a pointer to the python instance
 - **return** - PyObject* pointer to the actual python instance that is wrapped