cmake_minimum_required(VERSION 3.10)
project(SnSupear)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Add the executable
add_executable(SnSupear EditorUI.cpp) # Replace 'main.cpp' with your actual source file(s)

find_package(Curses REQUIRED)
target_link_libraries(SnSupear PRIVATE Curses::Curses)

# Include any additional libraries or directories if needed
# target_link_libraries(SnSupear PRIVATE your_library)
