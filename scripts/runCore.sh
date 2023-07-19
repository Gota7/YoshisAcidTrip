date > res/BUILDTIME
mkdir -p build
cd build
cmake .. $@
cmake --build . -j12
cd ..
bin/LSD