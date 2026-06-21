@echo off
setlocal EnableDelayedExpansion

:: ============================================================
::  build.bat — Build script for llvm_devirt project
::  Usage: build.bat <input.bin> <output.ll> [Release|Debug]
::
::  Arguments:
::    %1 — input binary file  (passed to llvm_devirt.exe)
::    %2 — output LLVM IR file (passed to llvm_devirt.exe + clang)
:: ============================================================

set "SCRIPT_DIR=%~dp0"
set "ROOT_DIR=%SCRIPT_DIR%.."
set "BUILD_DIR=%ROOT_DIR%\build"
set "SCRIPTS_DIR=%SCRIPT_DIR%"
set "CLANG=C:\Program Files\LLVM\bin\clang.exe"
set "CONFIG=Release"

set "INPUT_BIN=%~f1"
set "OUTPUT_LL=%~f2"

:: ── Validate arguments ────────────────────────────────────────
if "%INPUT_BIN%"=="" (
    echo [ERROR] Missing argument 1: input binary file.
    echo Usage: build.bat ^<input.bin^> ^<output.ll^> [Release^|Debug]
    exit /b 1
)
if "%OUTPUT_LL%"=="" (
    echo [ERROR] Missing argument 2: output LLVM IR file.
    echo Usage: build.bat ^<input.bin^> ^<output.ll^> [Release^|Debug]
    exit /b 1
)

:: Resolve output name for clang (strip .ll -> .exe)
set "OUTPUT_EXE=%OUTPUT_LL:.ll=.exe%"

echo.
echo ============================================================
echo  llvm_devirt Build Script
echo  Config     : %CONFIG%
echo  Input bin  : %INPUT_BIN%
echo  Output IR  : %OUTPUT_LL%
echo  Output exe : %OUTPUT_EXE%
echo  Root dir   : %ROOT_DIR%
echo ============================================================
echo.

:: ── 1. CMake configure ───────────────────────────────────────
echo [1/4] Configuring CMake...
cmake -S "%ROOT_DIR%" -B "%BUILD_DIR%"
if errorlevel 1 (
    echo [ERROR] CMake configuration failed.
    exit /b 1
)

:: ── 2. CMake build ───────────────────────────────────────────
echo.
echo [2/4] Building project ^(%CONFIG%^)...
cmake --build "%BUILD_DIR%" --config %CONFIG% -- /verbosity:normal
if errorlevel 1 (
    echo [ERROR] Build failed.
    exit /b 1
)

:: ── 3. Copy binary ───────────────────────────────────────────
echo.
echo [3/4] Copying llvm_devirt.exe to scripts\...
set "EXE_SRC=%BUILD_DIR%\bin\%CONFIG%\llvm_devirt.exe"
if not exist "%EXE_SRC%" (
    echo [ERROR] Binary not found: %EXE_SRC%
    exit /b 1
)
copy /Y "%EXE_SRC%" "%SCRIPTS_DIR%"
if errorlevel 1 (
    echo [ERROR] Failed to copy binary.
    exit /b 1
)

:: ── 4. Run devirt + clang ────────────────────────────────────
echo.
echo [4/4] Running llvm_devirt.exe + clang compilation...
pushd "%SCRIPTS_DIR%"

if not exist "%INPUT_BIN%" (
    echo [ERROR] Input file not found: %INPUT_BIN%
    popd
    exit /b 1
)

llvm_devirt.exe "%INPUT_BIN%" "%OUTPUT_LL%"
if errorlevel 1 (
    echo [ERROR] llvm_devirt.exe failed.
    popd
    exit /b 1
)

if not exist "%OUTPUT_LL%" (
    echo [ERROR] Expected output IR not found: %OUTPUT_LL%
    popd
    exit /b 1
)

if not exist "%CLANG%" (
    echo [ERROR] Clang not found at: %CLANG%
    popd
    exit /b 1
)

"%CLANG%" devirt_body.c -x ir "%OUTPUT_LL%" ^
    -O1 ^
    -mllvm -unroll-threshold=0 ^
    -mllvm -unroll-max-upperbound=0 ^
    -o "%OUTPUT_EXE%" ^
    -Xlinker /SUBSYSTEM:CONSOLE ^
    -m32

if errorlevel 1 (
    echo [ERROR] Clang compilation failed.
    popd
    exit /b 1
)

:: ── Cleanup ──────────────────────────────────────────────────
echo [Cleanup] Removing llvm_devirt.exe from scripts\...
if exist "%SCRIPTS_DIR%llvm_devirt.exe" (
    del /Q "%SCRIPTS_DIR%llvm_devirt.exe"
    echo [Cleanup] Done.
) else (
    echo [Cleanup] llvm_devirt.exe not found, skipping.
)

popd

echo.
echo ============================================================
echo  Build complete: scripts\%OUTPUT_EXE%
echo ============================================================
echo.
endlocal
exit /b 0