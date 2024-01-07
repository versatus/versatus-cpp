# versatus-cpp

## Overview
This repository provides essential tools and interfaces for developing Versatus smart contracts using C++. It complements the general smart contract development guide on Versatus (https://github.com/versatus/versatus/blob/main/docs/DevelopingSmartContracts.md) with C++-specific information.

## C++ Installation

## Dependencies
C++ can be used to compile efficient smart contracts to WebAssembly (WASM). It's recommended to use the latest stable version of C++ (C++17 or later) for better compatibility and features.

## Emscripten
Emscripten is an essential tool for compiling C++ code to WASM. Install Emscripten following the instructions on its official website.


## Compilers: GCC and Clang
To develop C++ smart contracts, you will need a C++ compiler. The most common compilers are GCC (GNU Compiler Collection) and Clang.

## GCC: A widely-used open-source compiler supporting various programming languages, including C++.

To install GCC, visit the GNU project page.
For Linux users, GCC can typically be installed via the package manager, e.g., `sudo apt install gcc` on Debian-based systems.
Windows users can use MinGW or Cygwin.
MacOS users can install GCC via Homebrew using `brew install gcc`.

## Clang: Part of the LLVM project, Clang is known for its excellent diagnostics (error and warning messages).

Installation instructions are available on the LLVM download page.
For Linux, it's often as simple as `sudo apt install clang` on Debian-based systems.
Windows users can install Clang via the LLVM website or through a package manager like Chocolatey.
MacOS users have Clang installed by default, or it can be updated via Homebrew.

## CMake
CMake is an open-source family of tools designed to build, test, and package software. CMake simplifies the build process across different platforms and compilers, automating the detection of dependencies, libraries, and tools, streamlining the build process. CMake generates native scripts `(CMakeLists.txt)` that are more readable and maintainable compared to traditional makefiles.

Installing CMake:

Visit the CMake download page for official binaries and source files.
Linux users can typically install CMake via their package manager, e.g., `sudo apt install cmake`.
For Windows, CMake is available as an installer or zip file on the CMake website.
MacOS users can install CMake using Homebrew: `brew install cmake`.
With these tools installed, you'll be well-equipped to start developing smart contracts in C++ for the Versatus network.

## Building and Testing with Makefile
A Makefile is a script that simplifies the build process.

## Build Commands

## Clean Build
This command cleans the build by removing compiled files (local, contract.wasm, contract.wat).

`❯ make clean

rm -f local contract.wasm contract.wat`

Build
This command compiles your C++ code to both a local executable (for testing on your computer) and WASM (for use on Versatus).
`❯ make`

## Compiled Files

After building, the following files are generated:

`local`: The local executable for testing.
`contract.wasm`: The WebAssembly smart contract.
`contract.wat`: The human-readable WebAssembly Text Format file.

## Testing Your Contract

## Wasmer Test
Test the `contract.wasm` file using Wasmer, a tool for running WASM files. Install Wasmer and follow its installation instructions (https://wasmer.io/).

## Compiling a Contract
The Makefile includes commands for compiling the contract:

## For local testing:
`g++ -std=c++20 -o local main.cpp -I/Users/foobar/json/include/  `

## For compiling to WASM:

`em++ -std=c++20 -o contract.wasm main.cpp -I/Users/foobar/json/include/  

wasm2wat contract.wasm  -o contract.wat`

## Deploying Your Contract
Follow the Versatus network's guidelines for deploying your smart contract, as detailed in the Versatus documentation.


❯ ls

Makefile                   contract.wasm              contract.wat               local                      main.cpp                   sample-contract-input.json

❯

## 

## Local test ( Mach-O 64-bit executable arm64 )

## 



❯ ./local < sample-contract-input.json

inputs.application_input.amount:100

{"transactions":[{"amount":33,"recipient":"wallet_id_1"},{"amount":33,"recipient":"wallet_id_2"},{"amount":33,"recipient":"wallet_id_3"}]}



## 

## Wasmer test contract.wasm

## 



❯ wasmer contract.wasm \

< sample-contract-input.json

inputs.application_input.amount:100

{"transactions":[{"amount":33,"recipient":"wallet_id_1"},{"amount":33,"recipient":"wallet_id_2"},{"amount":33,"recipient":"wallet_id_3"}]}

