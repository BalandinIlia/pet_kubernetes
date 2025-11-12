g++ -c controls.cpp
ar rcs parameters.a pch.o controls.o
echo parameters.a created
mv parameters.a ../libraries
