cmake_minimum_required (VERSION 3.8)

project ("ProjectSpar")

add_executable (ProjectSpar "src/ProjectSpar.cpp" "src/ProjectSpar.h")
add_subdirectory("SparEngine")
target_link_libraries(ProjectSpar PUBLIC SparEngine::SparEngine)
