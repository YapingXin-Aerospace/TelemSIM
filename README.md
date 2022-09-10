# TelemSIM

Telemetry Station Simulator (TelemSIM) lets you simulate the behavior of the telemetry station to send out telemetry data through multiple ethernet channels for Functional and stress integration testing.

### Copyright Information

```bash
Author: Yaping Xin <yaping.xin@foxmail.com>
License: licensed under the MIT license. 
```

Check the [License](https://github.com/YapingXin-Aerospace/TelemSIM/blob/main/LICENSE) file.

### Motivation

I've written a number of simulation programs in C, and I always ended up writing my own post-incident processing as alternative for the real-time data process utility. Finally, I decided to develop a general multiple channels message sender for the future software V&V (Verification and validation) work.

### Advantages

* Cross-platform supported.

### Supported platforms

* Windows (x86, x64)
* macOS (i386, x86_64, arm64)
* Linux (i386, x86_64, cross-toolchains ..)
* BSD (i386, x86_64)

### Supported toolchains

```bash
msvc          Microsoft Visual C/C++ Compiler
clang         A C language family frontend for LLVM
gcc           GNU Compiler Collection
mingw         Minimalist GNU for Windows
tinycc        Tiny C Compiler
icx           Intel LLVM C/C++ Compiler
dpcpp         Intel LLVM C++ Compiler for data parallel programming model based on Khronos SYCL
```

### Uasge

===
