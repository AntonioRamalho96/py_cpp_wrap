class ErrorConstructor:
    def __init__(self):
        self.a = asdf

class ErrorMethod:
    def __init__(self):
        pass
    def method(self):
        print("Here 1")
        self.a = asdf
        print("Here 2")
