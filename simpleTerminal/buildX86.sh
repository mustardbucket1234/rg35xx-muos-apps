# Run this to build and test the app for x86
make
cd output/x86/bin
./app -e "ls && cd .. && pwd"