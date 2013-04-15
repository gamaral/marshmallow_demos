file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(MARSHMALLOW_DEMO_LIBRARIES)
	add_library(engine_demo0 SHARED "demo0.cpp")
else()
	add_executable(engine_demo0 "demo0.cpp")
endif()

if(WIN32 AND NOT MARSHMALLOW_DEBUG)
	set_target_properties(engine_demo0 PROPERTIES WIN32_EXECUTABLE ON)
endif()

target_link_libraries(engine_demo0 marshmallow_entrypoint
                                   marshmallow_game)

install(TARGETS engine_demo0
        RUNTIME DESTINATION bin COMPONENT demos
        LIBRARY DESTINATION bin COMPONENT demos)

install(DIRECTORY assets/
        DESTINATION assets COMPONENT demos)

