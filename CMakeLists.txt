cmake_minimum_required(VERSION 3.2)

project(PMX_interface VERSION 2.0)
set (CMAKE_CXX_STANDARD 20)

#Important CMAKE stuff
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set (CMAKE_CXX_STANDARD 20)

#   External utilities to configure the package
include(GNUInstallDirs)
set(INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR})
set(LIB_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR})


#   Give different names for debug and Release
set(CMAKE_RELEASE_POSTFIX "")
set(CMAKE_DEBUG_POSTFIX "-debug")



find_package (Eigen3 3.3 REQUIRED NO_MODULE)
find_package(benchmark QUIET)
find_package(utilities REQUIRED)
find_package(yaml-cpp REQUIRED)

find_package(mpi_cmake_modules REQUIRED)
find_package(real_time_tools REQUIRED)

add_subdirectory(src)


#if(benchmark_FOUND)
    #add_subdirectory(benchmarks)
#endif(benchmark_FOUND)




#   Install the library using the default routine
include(cmake/installation_module.cmake)
