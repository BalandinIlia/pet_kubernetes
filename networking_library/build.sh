g++ -c messages.cpp
g++ -c utils.cpp
ar rcs networking_library.a messages.o utils.o pch.o
mv networking_library.a ../libraries