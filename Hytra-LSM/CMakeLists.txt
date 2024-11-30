cmake_minimum_required(VERSION 2.8.12.2)
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
set(CMAKE_OSX_SYSROOT /Library/Developer/CommandLineTools/SDKs/MacOSX13.3.sdk)
project(Inverted_LSM)

set(CMAKE_CXX_STANDARD 14)
# set(CMAKE_SYSTEM_PROCESSOR arm)

add_executable(Inverted_LSM
        main.cpp
        LSM.cpp
        DiskLevel.cpp
        DiskRUN.cpp
        FileMapper.cpp
        MemRUN.cpp
        MemLevel.cpp
        Server.cpp
        LSMUtils.cpp
        Server.cpp)

add_executable(test_insert_operation
        test_insert_operation.cpp
        LSM.cpp
        DiskLevel.cpp
        DiskRUN.cpp
        FileMapper.cpp
        MemRUN.cpp
        MemLevel.cpp
        LSMUtils.cpp

)

add_executable(test_filemapper_put_and_get
        test_filemapper_put_and_get.cpp
        DiskLevel.cpp
        DiskRUN.cpp
        FileMapper.cpp)

add_executable(test_file_write
        test_file_write.cpp
        DiskLevel.cpp
        DiskRUN.cpp
        FileMapper.cpp)

add_executable(test_filemapper
        test_filemapper.cpp
        FileMapper.cpp)