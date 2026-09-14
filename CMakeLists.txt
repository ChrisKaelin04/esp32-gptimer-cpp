cmake_minimum_required(VERSION 3.16)
project(gptimer_cpp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(gptimer_test test/main.cpp)
target_include_directories(gptimer_test PRIVATE include test/fake)

enable_testing()
add_test(NAME gptimer COMMAND gptimer_test)

target_compile_options(gptimer_test PRIVATE -fsanitize=address,undefined -g)
target_link_options(gptimer_test PRIVATE -fsanitize=address,undefined)
