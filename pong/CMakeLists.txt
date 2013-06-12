file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets
     DESTINATION ${PROJECT_BINARY_DIR})

set(MARSHMALLOW_PONG_SRCS "main.cpp"
                          "aicomponent.cpp"
                          "demoengine.cpp"
                          "demoscene.cpp"
                          "inputcomponent.cpp"
                          "pongball.cpp"
                          "pongcourt.cpp"
                          "ponglayer.cpp"
                          "pongpaddle.cpp"
                          "pongscore.cpp"
                          "pongwall.cpp"
                          "scoreevent.cpp"
)

if(MARSHMALLOW_DEMO_LIBRARIES)
	add_library(pong SHARED ${MARSHMALLOW_PONG_SRCS})
else()
	add_executable(pong ${MARSHMALLOW_PONG_SRCS})
endif()

if(WIN32 AND NOT MARSHMALLOW_DEBUG)
	set_target_properties(pong PROPERTIES WIN32_EXECUTABLE ON)
endif()

target_link_libraries(pong marshmallow_entrypoint
                           marshmallow_game)

install(TARGETS pong
        RUNTIME DESTINATION bin COMPONENT demos
        LIBRARY DESTINATION bin COMPONENT demos)

install(DIRECTORY assets/
        DESTINATION assets COMPONENT demos)

