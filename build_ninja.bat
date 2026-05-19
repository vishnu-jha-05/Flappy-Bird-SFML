@echo off
SET PATH=C:\msys64\mingw64\bin;%PATH%
cmake -S . -B build_ninja -G Ninja -DCMAKE_BUILD_TYPE=Release
ninja -C build_ninja
copy /y build_ninja\FlappyBird.exe FlappyBird.exe
