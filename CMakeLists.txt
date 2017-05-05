set(SRC main.cpp)

set(CMAKE_CXX_FLAGS "-std=c++14")

add_executable(toto ${SRC})

target_link_libraries(toto dl)