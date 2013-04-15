set(MARSHMALLOW_DEMO_CWD_OVERRIDE ON CACHE BOOL "Override Current Working Directory in demos")

configure_file(config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

if(MARSHMALLOW_DEMO_CWD_OVERRIDE)
	add_definitions(-DMARSHMALLOW_DEMO_CWD_OVERRIDE)
endif()

if(MARSHMALLOW_ANDROID)
	set(MARSHMALLOW_DEMO_LIBRARIES TRUE)
endif()

add_subdirectory(engine)
add_subdirectory(tilemap)
add_subdirectory(platformer)
if(MARSHMALLOW_WITH_BOX2D)
	add_subdirectory(angrymallows)
endif()

