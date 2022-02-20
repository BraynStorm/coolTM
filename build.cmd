@echo off
mkdir build
cls
setlocal

set LINKER_COMMON_FLAGS=-subsystem:WINDOWS -debug -machine:amd64 -INCREMENTAL:NO
set LINKER_FLAGS=%LINKER_COMMON_FLAGS% 
set LIBS=user32.lib kernel32.lib gdi32.lib Winmm.lib D3D11.lib  d3dcompiler.lib

set REMOVE_WARNINGS=-Wno-unused-parameter -Wno-unused-variable -Wno-unused-parameter -Wno-unused-function -Wno-missing-braces

set COMPILER="C:\Program Files\LLVM\bin\clang-cl.exe"
set COMPILE_CLANG_FLAGS=-fdiagnostics-absolute-paths
set COMPILE_COMMON_FLAGS=-I../src -MT -W4 -Zi -Oi -GR- -FS -FC -utf-8 -msse4.2 -RTCu -std:c++latest -D_HAS_EXCEPTIONS=0 -D_CRT_SECURE_NO_WARNINGS %REMOVE_WARNINGS% %COMPILE_CLANG_FLAGS%
set COMPILE_DEBUG_FLAGS=%COMPILE_COMMON_FLAGS%   -Ob0 -Od
set COMPILE_RELEASE_FLAGS=%COMPILE_COMMON_FLAGS% -Ob2 -Ofast -fp:fast

@REM set COMPILE_FLAGS=%COMPILE_DEBUG_FLAGS%
set COMPILE_FLAGS=%COMPILE_RELEASE_FLAGS%

set COMPILE_CMD=%COMPILER% %COMPILE_FLAGS%


set HV_WIN32_FILES=^
    ../src/cool_directx_main.cpp
    @REM ../src/main.cpp


set BUILD_HVWIN32_CMD=%COMPILE_CMD%         %HV_WIN32_FILES% %LIBS%                /link %LINKER_FLAGS% /OUT:cool.exe 

pushd build
echo %cd%

copy ..\src\simulate.hlsl .

echo ^>^>^>^> %BUILD_HVWIN32_CMD%
%BUILD_HVWIN32_CMD%


:end