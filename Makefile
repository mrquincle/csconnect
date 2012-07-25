#!/bin/make

# Author: Anne C. van Rossum
# Date: Aug. 04, 2011

all: 
	@mkdir -p build
	cd build && cmake $(CMAKE_FLAGS) .. $(FLAGS)
	cd build && make

clean:
	cd build && make clean

install:
	cd build && make install

uninstall:
	rm -f /usr/local/lib/libcsconnect.so
	rm -f /usr/local/include/csconnect/*
	rmdir /usr/local/include/csconnect
