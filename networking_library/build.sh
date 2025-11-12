g++ -c messages.cpp
g++ -c inner_interaction.cpp
ar rcs networking_library.a messages.o inner_interaction.o
mv networking_library.a ../libraries