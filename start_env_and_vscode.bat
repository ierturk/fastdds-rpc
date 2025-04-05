:: Copyright (c) 2025, Ibrahim Erturk <me@erturk.me>, ErturkMe
:: Licensed under the BSD 3-Clause License.
:: See the LICENSE file in the project root for more information.

@echo off
REM Check if an argument is provided
if "%1"=="" (
    echo Usage: start_env_and_vscode.bat [x86|x64]
    exit /b 1
)

REM Set up the environment using vcvarsall.bat
call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" %1

REM Open Visual Studio Code in the current directory
code .