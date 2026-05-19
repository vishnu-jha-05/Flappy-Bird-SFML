@echo off
echo === Flappy Bird - Build Script ===
SET PATH=C:\msys64\mingw64\bin;%PATH%
SET CMAKE=C:\msys64\mingw64\bin\cmake.exe
SET MAKE=C:\msys64\mingw64\bin\mingw32-make.exe
SET BUILD_DIR=%~dp0build

if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

echo Configuring with CMake...
%CMAKE% -S "%~dp0." -B "%BUILD_DIR%" -G "MinGW Makefiles" ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DCMAKE_C_COMPILER=C:\msys64\mingw64\bin\gcc.exe ^
    -DCMAKE_CXX_COMPILER=C:\msys64\mingw64\bin\g++.exe ^
    2>&1

if %errorlevel% neq 0 (
    echo CMake configure FAILED!
    pause
    exit /b 1
)

echo Building...
%MAKE% -C "%BUILD_DIR%" -j4 2>&1

if %errorlevel% neq 0 (
    echo Build FAILED!
    pause
    exit /b 1
)

echo Copying exe and DLLs to project root...
copy /y "%BUILD_DIR%\FlappyBird.exe" "%~dp0FlappyBird.exe"
copy /y "C:\msys64\mingw64\bin\libsfml-graphics-3.dll" "%~dp0"
copy /y "C:\msys64\mingw64\bin\libsfml-window-3.dll" "%~dp0"
copy /y "C:\msys64\mingw64\bin\libsfml-system-3.dll" "%~dp0"
copy /y "C:\msys64\mingw64\bin\libsfml-audio-3.dll" "%~dp0"
copy /y "C:\msys64\mingw64\bin\libgcc_s_seh-1.dll" "%~dp0"
copy /y "C:\msys64\mingw64\bin\libstdc++-6.dll" "%~dp0"
copy /y "C:\msys64\mingw64\bin\libwinpthread-1.dll" "%~dp0"
copy /y "C:\msys64\mingw64\bin\libfreetype-6.dll" "%~dp0"
copy /y "C:\msys64\mingw64\bin\libpng16-16.dll" "%~dp0"
copy /y "C:\msys64\mingw64\bin\zlib1.dll" "%~dp0"
copy /y "C:\msys64\mingw64\bin\libbz2-1.dll" "%~dp0"
copy /y "C:\msys64\mingw64\bin\libbrotlicommon.dll" "%~dp0"
copy /y "C:\msys64\mingw64\bin\libbrotlidec.dll" "%~dp0"
copy /y "C:\msys64\mingw64\bin\libharfbuzz-0.dll" "%~dp0"
copy /y "C:\msys64\mingw64\bin\libFLAC.dll" "%~dp0"
copy /y "C:\msys64\mingw64\bin\libogg-0.dll" "%~dp0"
copy /y "C:\msys64\mingw64\bin\libvorbis-0.dll" "%~dp0"
copy /y "C:\msys64\mingw64\bin\libvorbisenc-2.dll" "%~dp0"
copy /y "C:\msys64\mingw64\bin\libvorbisfile-3.dll" "%~dp0"
copy /y "C:\msys64\mingw64\bin\libintl-8.dll" "%~dp0"
copy /y "C:\msys64\mingw64\bin\libiconv-2.dll" "%~dp0"
copy /y "C:\msys64\mingw64\bin\libglib-2.0-0.dll" "%~dp0"
copy /y "C:\msys64\mingw64\bin\libpcre2-8-0.dll" "%~dp0"
copy /y "C:\msys64\mingw64\bin\libgraphite2.dll" "%~dp0"

echo.
echo === BUILD SUCCESS! Run FlappyBird.exe to play. ===
echo Note: Make sure SFML DLLs are in the path or same folder.
pause
