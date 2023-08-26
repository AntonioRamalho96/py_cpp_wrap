class SimpleClass:
    def __init__(self, init : int):
        self.__val = init

    def add(self, a : int):
        self.add_no_return(a)
        return self.__val

    def add_no_return(self, a : int):
        self.__val += a
        print("Val is now " + repr(self.__val))

    def return_val_divided_by_2(self):
        return self.__val / 2.0

    def return_val_str_repr(self):
        return repr(self.__val)