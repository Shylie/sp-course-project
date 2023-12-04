# SIC/XE Assembler

## How to build

#### Dependencies:

- CMake (minimal version of 3.18)
- C89/90 complient compiler system


#### Building:

- Clone the repo, navigate to the project's directory and execute cmake build command:

`git clone https://github.com/Shylie/sp-course-project && cd sp-course-project && cmake -S . -B build && cmake --build build`


## Running the assembler

#### Linux & Apple

- The binary for the assembler will be in build/assembler 

#### Windows

- The binary for the assembler will be in build\assembler\Debug

#### Executable arguments

- In order to generate machine code from you assembly file you'll need to pass its path to the assembler executable in the following manner

`.\assembler <file-path>`


## Documentation

- [SIC](documentation/SIC.md)
- [SIC/XE](documentation/SICXE.md)
