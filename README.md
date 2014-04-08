gr-rccBlocks
============

GNU Radio 3.7 API compatible version of the rayleigh Fading block 

CMake based project for GNU Radio (>= v3.7) custom signal processing blocks, including a multipath rayleigh fading 
simulator.

Build instructions:

mkdir build
cd build
cmake ../
make
sudo make install
sudo ldconfig

Build instructions for E100 USRP:

mkdir build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../arm_cortex_a8_native.cmake ../
make
sudo make install
sudo ldconfig

Build instructions for ARM Cortex A9 based platforms:

mkdir build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../arm_cortex_a9_native.cmake ../
make
sudo make install
sudo ldconfig
