# Fast DDS QML Example with Conan 2 (using conanfile.py)

This project demonstrates how to use Fast DDS with a QML GUI, managed by Conan 2 using a conanfile.py recipe. The example also implements a RPC (remote procedure call) over DDS.

## Prerequisites
- FastDDSGen V2 tool from eProsima (included in FastDDS installation for Windows) requires Java Runtime Environment (JRE) installation
- CMake 3.15+
- MSVC C++ compiler supporting C++20
- Conan 2.0+
- Python 3.6+ (for Conan)
- Ninja build
- Clink integration with Cmder for enhancing the Windows `cmd` shell experience
- GNU Make
- VS Code
- A third-party prebuilt 32-bit library, `ExampleLib`, used for enabling communication between the 32-bit proxy side and the 64-bit invoker side.

## Libraries from Conan center
- FastDDS
- FastDDS CDR
- Boost asio
- Boost signals2
- Qt 6.5.3 LTS

## Building on Windows
Start a `Cmder` console
```bash

# For building invoker (64-bit) app
λ "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" x64
# Note: Adjust the path to match your Visual Studio installation if it differs.
λ make all
λ code .

# ctrl+shift+p
# CMake: Select Configure Preset
# select conan-windows-msvc-194-x86_64-debug config

# For building proxy (32-bit) app
λ "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" x86
# Note: Modify the path above if your Visual Studio installation is in a different location.
λ make all
λ code .

# ctrl+shift+p
# CMake: Select Configure Preset
# select conan-windows-msvc-194-x86-debug config
```
## License
This project is licensed under the BSD 3-Clause License, which permits use, distribution, and modification with proper attribution, while disclaiming liability and warranty. See the [LICENSE](./LICENSE) file for full details.