@echo off
mkdir build
cls
setlocal

set LINKER_COMMON_FLAGS=-subsystem:WINDOWS -debug -machine:amd64 -INCREMENTAL:NO
set LINKER_FLAGS=%LINKER_COMMON_FLAGS% 
set LIBS=user32.lib kernel32.lib gdi32.lib Opengl32.lib Winmm.lib
set EDITOR_LIBS=user32.lib kernel32.lib gdi32.lib Opengl32.lib Winmm.lib

set REMOVE_WARNINGS=-Wno-unused-parameter -Wno-unused-variable -Wno-unused-parameter -Wno-unused-function -Wno-missing-braces

set COMPILER="C:\Program Files\LLVM\bin\clang-cl.exe"
set COMPILE_CLANG_FLAGS=-fdiagnostics-absolute-paths
set COMPILE_COMMON_FLAGS=-I../src -MT -W4 -Zi -Oi -GR- -FS -FC -utf-8 -msse4.2 -RTCu -std:c++latest -D_HAS_EXCEPTIONS=0 -D_CRT_SECURE_NO_WARNINGS %REMOVE_WARNINGS% %COMPILE_CLANG_FLAGS%
set COMPILE_DEBUG_FLAGS=-Ob2 -Ofast -fp:fast

set COMPILE_FLAGS=%COMPILE_COMMON_FLAGS% %COMPILE_DEBUG_FLAGS%
set COMPILE_CMD=%COMPILER% %COMPILE_FLAGS%


set HV_WIN32_FILES=^
    ../src/cool.cpp^
    ../src/main.cpp

set BUILD_HVWIN32_CMD=%COMPILE_CMD%         %HV_WIN32_FILES% %LIBS%                /link %LINKER_FLAGS% /OUT:cool.exe 

pushd build

echo ^>^>^>^> %BUILD_HVWIN32_CMD%
%BUILD_HVWIN32_CMD%

:end