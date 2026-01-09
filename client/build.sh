g++ -c utils_input.cpp
g++ -c runner.cpp
g++ -c mainC.cpp
g++ utils_input.o runner.o mainC.o -L../libraries -l:networking_library.a -l:networking_utils.a -l:logger.a -l:parameters.a -o client
mv client ../executables
