
set -e
set -x

cd hardware-src/verilator

./ver_compile.sh

mkdir -p build

cd build

cmake ..

make



