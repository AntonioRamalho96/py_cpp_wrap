PYTHON_VERSION=$(shell python3 --version  | sed 's/Python //g' | sed 's/\.[[:digit:]]\+$$//g')
INIT_FLAGS=-O0 -g -I /usr/include/python${PYTHON_VERSION} -I .. -D__DIR__='"${PWD}"'
LINK_FLAGS=-lpython${PYTHON_VERSION}
SRCS=$(shell find .. -name "*.hpp")

all: build/simple build/error_cases build/how_i_met build/readme_example

clean:
	rm -rf build

test: all
	@echo "You should expect 4 'SUCCESS!!' in your terminal"
	@./build/simple 2> /dev/null | grep SUCCESS
	@./build/error_cases 2> /dev/null | grep SUCCESS
	@./build/how_i_met 2> /dev/null | grep SUCCESS
	@./build/readme_example 2> /dev/null | grep SUCCESS

build/simple: simple.cpp ${SRCS}
	mkdir -p build 
	g++ ${INIT_FLAGS} $< -o $@ ${LINK_FLAGS}

build/error_cases: error_cases.cpp ${SRCS}
	mkdir -p build 
	g++ ${INIT_FLAGS} $< -o $@ ${LINK_FLAGS}

build/how_i_met: how_i_met.cpp ${SRCS}
	mkdir -p build 
	g++ ${INIT_FLAGS} $< -o $@ ${LINK_FLAGS}

build/readme_example: readme_example.cpp ${SRCS}
	mkdir -p build 
	g++ ${INIT_FLAGS} $< -o $@ ${LINK_FLAGS}