g++ -c controls.cpp
ar rcs parameters.a pch.o controls.o
mv parameters.a ../libraries