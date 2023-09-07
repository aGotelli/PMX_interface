include_directories(include)


add_library(${PROJECT_NAME} SHARED
	${PROJECT_NAME}/pmx_interface.cpp
	include/${PROJECT_NAME}/pmx_interface.hpp
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Eigen3::Eigen
        ${Boost_LIBRARIES}
        utilities
        yaml-cpp
)


install(TARGETS ${PROJECT_NAME}
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(DIRECTORY include/${PROJECT_NAME}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)



add_executable(PMX_record_data
PMX_record_data.cpp
)

target_link_libraries(PMX_record_data
    PUBLIC
        ${PROJECT_NAME}
        real_time_tools::real_time_tools
)






add_executable(conversions
    PMX_interface/conversions.cpp)
target_link_libraries(conversions
    ${Boost_LIBRARIES}
)


add_executable(interactive_commands
    PMX_interface/interactive_commands.cpp)
target_link_libraries(interactive_commands
    ${Boost_LIBRARIES}
)


add_executable(data_stream
    PMX_interface/data_stream.cpp)
target_link_libraries(data_stream
    ${Boost_LIBRARIES}
)



