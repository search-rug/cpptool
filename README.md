# cpptool

A static analysis tool that uses Clang's tooling framework to export declaration trees for C/C++ projects.

This project contains the C++ exporter tool, which uses [CppTool-proto](https://github.com/search-rug/cpptool-proto) to export the information to data files.
Applications based on the [CppTool-Lib](https://github.com/search-rug/cpptool-lib) java library can read these data files and navigate the declaration tree they produce.
