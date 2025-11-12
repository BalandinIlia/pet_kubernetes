g++ -c send_receive.cpp
g++ -c listen_socket.cpp
ar rcs networking_utils.a send_receive.o listen_socket.o
mv networking_utils.a ../libraries