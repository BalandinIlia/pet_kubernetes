g++ -c messages.cpp
g++ -c utils.cpp
ar rcs networking_library.a messages.o utils.o pch.o
echo networking_library.a created
mv networking_library.a ../libraries
