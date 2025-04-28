@echo off
echo Unit Test:
cd G-TestFile
rmdir /s /q build
mkdir build
cd build
cmake -G Ninja ..
ninja
ctest --verbose

cd../..
rmdir /s /q BinFile
mkdir BinFile
echo ---------------------------------------------
echo Build and flash the gateway example to board:
echo ---------------------------------------------
cd Examples/Gateway
rmdir /s /q build
mkdir build
cd build
cmake -G Ninja -DCMAKE_TOOLCHAIN_FILE=../../arm-none-eabi-gcc.cmake ..
ninja
copy "Gateway.bin" "../../../BinFile"

echo ---------------------------------------------
echo Build and flash the Node01 example to board:
echo ---------------------------------------------
cd ../../Node01
rmdir /s /q build
mkdir build
cd build
cmake -G Ninja -DCMAKE_TOOLCHAIN_FILE=../../arm-none-eabi-gcc.cmake ..
ninja
copy "Node_01.bin" "../../../BinFile"

echo ---------------------------------------------
echo Build and flash the Node02 example to board:
echo ---------------------------------------------
cd ../../Node02
rmdir /s /q build
mkdir build
cd build
cmake -G Ninja -DCMAKE_TOOLCHAIN_FILE=../../arm-none-eabi-gcc.cmake ..
ninja
copy "Node_02.bin" "../../../BinFile"

echo -----------------------------------------------
echo Build and flash the Test_Node example to board:
echo -----------------------------------------------
cd ../../Test_Node
rmdir /s /q build
mkdir build
cd build
cmake -G Ninja -DCMAKE_TOOLCHAIN_FILE=../../arm-none-eabi-gcc.cmake ..
ninja
copy "Test_Node.bin" "../../../BinFile"
cd ../../../BinFile

Rem Flash Node 01
STM32_Programmer_CLI -c port=SWD sn=52FF6D064849825053310667 -w Node_01.bin 0x08000000 -v -rst
Rem Flash Node 02
STM32_Programmer_CLI -c port=SWD sn=52FF71064849825056350467 -w Node_02.bin 0x08000000 -v -rst
Rem Flash Test Node
STM32_Programmer_CLI -c port=SWD sn=52FF6F064849825057460467 -w Test_Node.bin 0x08000000 -v -rst
Rem Flash Gateway
STM32_Programmer_CLI -c port=SWD sn=52FF6C064849825033360667 -w Gateway.bin 0x08000000 -v -rst

Rem get the serial port number of the board
mode

cd ..
pytest -s test_uart.py
echo Done
