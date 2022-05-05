#cmake_minimum_required command sets the policies so that the build is exactly like it would be on the listed version of CMake
#project(<PROJECT-NAME> [<language-name>...]) by default c/c++
cmake_minimum_required(VERSION 3.22.1)

project(cmakedemo)

add_executable(
   tsparser
   tsparser.cpp
)
