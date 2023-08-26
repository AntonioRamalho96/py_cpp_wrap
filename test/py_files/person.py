class Person:
    def __init__(self, name, age) -> None:
        self.__name = name
        self.__age = age

    def set_age(self, new_age):
        self.__age = new_age

    def get_age(self):
        return self.__age

    def get_name(self):
        return self.__name

    def introduce_myself(self):
        return "Hello, my name is {} and I am {} years old".format(self.__name, self.__age)

    def introduce_other(self, other):
        return "Hi, have you met {}".format(other.get_name())

