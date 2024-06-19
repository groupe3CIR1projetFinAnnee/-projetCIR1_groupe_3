cmake_minimum_required(VERSION 3.25)
project(GigaTree C)

set(CMAKE_C_STANDARD 11)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ./build)

add_executable(GigaTree
        main.c
        exportLocal.c
        gigaTree.c
        person.c
        regions.c
        csvReader.c
)
