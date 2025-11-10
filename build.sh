cd math_core
chmod ++x build.sh
./build.sh
cd ..

cd networking_library
chmod ++x build.sh
./build.sh
cd ..

cd parameters
chmod ++x build.sh
./build.sh
cd ..

cd client
chmod ++x build.sh
./build.sh
cd ..

cd server
chmod ++x build.sh
./build.sh
cd ..

echo "Printing directory contents after build"
cd libraries
echo "Libraries"
ls
cd ..
cd executables
echo "Executables"
ls
cd ..
