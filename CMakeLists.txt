# CMakeList.txt : CMake project for ProjectSpar, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("ProjectSpar")

# Add source to this project's executable.
add_subdirectory("SparEngine")
add_executable (ProjectSpar "ProjectSpar.cpp" "ProjectSpar.h")
target_link_libraries(ProjectSpar PRIVATE SparEngine::SparEngine)

# TODO: Add tests and install targets if needed.
