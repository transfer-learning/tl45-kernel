
set -e
set -x

mkdir -p toolchain

cd toolchain

cmake -DLLVM_TARGETS_TO_BUILD="" -DLLVM_ENABLE_PROJECTS="clang;lld" -DLLVM_EXPERIMENTAL_TARGETS_TO_BUILD="LC2200;TL45" ../toolchain-src/llvm

make -j 12


