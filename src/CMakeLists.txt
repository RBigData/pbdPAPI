cmake_minimum_required (VERSION 2.8.1)
project(pbdpapi)
enable_language(C)

set(PAPI_HOME "${PROJECT_SOURCE_DIR}/papi-5.3.0/src/")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")


### External dependencies
find_package(R REQUIRED)
include_directories(${R_INCLUDE_DIR})

find_library(LIB_PAPI NAMES "libpapi.a"
  PATHS ${PAPI_HOME} NO_DEFAULT_PATH
)


### Flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wfatal-errors -O3")

link_directories(${PAPI_HOME})
include_directories(${PAPI_HOME}/include)


### Source files
set(RLIB_FILES
	example.c
	flops.c
)



### Make and install
add_library(pbdpapi SHARED ${RLIB_FILES})

target_link_libraries (pbdpapi ${R_LIBRARIES} ${LIB_PAPI})

set_target_properties(pbdpapi PROPERTIES PREFIX "")
if(NOT WIN32)
  set_target_properties(pbdpapi PROPERTIES SUFFIX ".so")
  install(TARGETS pbdpapi LIBRARY DESTINATION ${CMAKE_SOURCE_DIR})
else(NOT WIN32)
  install(TARGETS pbdpapi RUNTIME DESTINATION ${CMAKE_SOURCE_DIR})
endif(NOT WIN32)

