C-Training

# First of all, you might want to create directory to not mix up CMake helpers and project files.
mkdir build && cd build

# Assuming you're in the build directory, generate CMake helpers.
cmake .. -DCMAKE_TOOLCHAIN_FILE=../devkita64-libnx.cmake

# Now we will generate all the binaries. These are for now just nso and nro files.
make
