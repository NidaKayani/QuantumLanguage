@echo off
rem Builds quantum.exe from src\ with Quantum itself: no C++ compiler, no
rem CMake, no MSYS2. seed\quantum.exe (a Quantum VM carrying the compiler)
rem compiles the compiler's source, and the result compiles itself again.
rem
rem   build-quantum.bat            build\selfhost\quantum.exe
rem   build-quantum.bat --verify   also check the fixed point: the new compiler
rem                                rebuilds itself byte for byte
setlocal
cd /d "%~dp0"
set OUT=build\selfhost

if not exist seed\quantum.exe (
    echo [error] seed\quantum.exe is missing
    exit /b 1
)

if not exist %OUT% mkdir %OUT%

echo [1/2] seed\quantum.exe compiles the compiler from src\
seed\quantum.exe --build-self %OUT%\stage1.exe || goto :fail

echo [2/2] the new compiler compiles itself
%OUT%\stage1.exe --build-self %OUT%\quantum.exe || goto :fail

if /i "%~1"=="--verify" (
    echo [verify] quantum.exe rebuilds itself
    %OUT%\quantum.exe --build-self %OUT%\verify.exe || goto :fail
    fc /b %OUT%\quantum.exe %OUT%\verify.exe >nul || (
        echo [error] not a fixed point: quantum.exe and its rebuild differ
        exit /b 1
    )
    echo [verify] fixed point: identical
)

echo.
echo Built %OUT%\quantum.exe
echo   %OUT%\quantum.exe program.sa        run a program
echo   %OUT%\quantum.exe --bundle f o.exe  make a standalone executable
echo   %OUT%\quantum.exe --native f o.exe  native x86-64 code
exit /b 0

:fail
echo [error] build failed
exit /b 1
