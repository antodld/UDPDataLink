cmake_minimum_required(VERSION 3.1)

set(PROJECT_NAME UDPDataLink)
set(PROJECT_DESCRIPTION "")
set(PROJECT_URL "")
set(PROJECT_VERSION 0.0.1)
set(PROJECT_USE_CMAKE_EXPORT TRUE)
set(CMAKE_CXX_STANDARD 17)

# include(cmake/base.cmake)

project(${PROJECT_NAME} LANGUAGES CXX VERSION ${PROJECT_VERSION})

find_package(Boost REQUIRED COMPONENTS serialization)

set(HDR_DIR
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/UDPDataLink>$<INSTALL_INTERFACE:include/UDPDataLink>)

set(HDR
  ${HDR_DIR}/Publisher.h
  ${HDR_DIR}/Receiver.h
  ${HDR_DIR}/Serialize.h
)

# header-only
add_library(${PROJECT_NAME} INTERFACE)
if(COMMAND target_sources)
  target_sources(${PROJECT_NAME} INTERFACE ${HDR})
endif()
# target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include/UDPDataLink)

target_link_libraries(${PROJECT_NAME} INTERFACE ${Boost_LIBRARIES})

set(TARGETS_EXPORT_NAME "${PROJECT_NAME}Config")

install(TARGETS ${PROJECT_NAME}
  EXPORT "${TARGETS_EXPORT_NAME}")

install(
  DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION include)

export(TARGETS ${PROJECT_NAME} FILE "${PROJECT_NAME}Config.cmake" )

# Install the export file
install(EXPORT ${TARGETS_EXPORT_NAME}
    FILE "${TARGETS_EXPORT_NAME}.cmake"
    DESTINATION lib/cmake/${PROJECT_NAME}
)

