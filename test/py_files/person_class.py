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