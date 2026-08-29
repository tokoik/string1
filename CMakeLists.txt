cmake_minimum_required(VERSION 3.22)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5 CACHE STRING "Minimum policy version" FORCE)

project(string1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
  add_compile_options("/utf-8")
endif()

set(SOURCES
  main.cpp
  Gg.cpp
)

set(HEADERS
  Gg.h
)

set(SHADER_FILES
  point.vert
  point.frag
  force.vert
)

source_group("Shader Files" FILES ${SHADER_FILES})

set(FETCHCONTENT_BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs")

if(APPLE)
  find_library(GLUT_LIBRARY GLUT)
  find_library(OPENGL_LIBRARY OpenGL)

  add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${SHADER_FILES})
  target_link_libraries(${PROJECT_NAME} PRIVATE ${GLUT_LIBRARY} ${OPENGL_LIBRARY})

elseif(WIN32)
  include(FetchContent)
  FetchContent_Declare(
    glew
    GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git
    GIT_TAG glew-cmake-2.3.1
  )
  set(ONLY_LIBS ON CACHE BOOL "" FORCE)
  set(BUILD_UTILS OFF CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(glew)
  if(TARGET libglew_static)
    set_target_properties(libglew_static PROPERTIES INTERFACE_COMPILE_DEFINITIONS "GLEW_STATIC")
  endif()

  FetchContent_Declare(
    freeglut
    GIT_REPOSITORY https://github.com/FreeGLUTProject/freeglut.git
    GIT_TAG master
  )
  set(FREEGLUT_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
  set(FREEGLUT_BUILD_DEMOS OFF CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(freeglut)

  find_package(OpenGL REQUIRED)

  add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${SHADER_FILES})
  target_include_directories(${PROJECT_NAME} PRIVATE
    "${freeglut_SOURCE_DIR}/include"
    "${glew_SOURCE_DIR}/include"
  )
  target_link_libraries(${PROJECT_NAME} PRIVATE libglew_static freeglut_static OpenGL::GL OpenGL::GLU)
  target_compile_definitions(${PROJECT_NAME} PRIVATE FREEGLUT_STATIC GLEW_STATIC)

  foreach(RESOURCE_FILE IN LISTS SHADER_FILES)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/${RESOURCE_FILE}"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/${RESOURCE_FILE}"
        "${CMAKE_CURRENT_BINARY_DIR}"
    )
  endforeach()

  set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

else()
  # Linux
  include(FetchContent)
  FetchContent_Declare(
    glew
    GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git
    GIT_TAG glew-cmake-2.3.1
  )
  set(ONLY_LIBS ON CACHE BOOL "" FORCE)
  set(BUILD_UTILS OFF CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(glew)
  if(TARGET libglew_static)
    set_target_properties(libglew_static PROPERTIES INTERFACE_COMPILE_DEFINITIONS "GLEW_STATIC")
  endif()

  set(OpenGL_GL_PREFERENCE "LEGACY")
  find_package(OpenGL REQUIRED)
  find_package(GLUT REQUIRED)

  add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${SHADER_FILES})
  target_include_directories(${PROJECT_NAME} PRIVATE ${glew_SOURCE_DIR}/include)
  target_compile_definitions(${PROJECT_NAME} PRIVATE GLEW_STATIC)
  target_link_libraries(${PROJECT_NAME} PRIVATE libglew_static ${OPENGL_LIBRARIES} GLUT::GLUT m)

  foreach(RESOURCE_FILE IN LISTS SHADER_FILES)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/${RESOURCE_FILE}"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
    )
  endforeach()

endif()
