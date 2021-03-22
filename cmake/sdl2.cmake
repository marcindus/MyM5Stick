include(ExternalProject)
ExternalProject_Add(
        SimpleDirectLayer2
        PREFIX ${CMAKE_BINARY_DIR}/external/sdl2
        URL https://www.libsdl.org/release/SDL2-2.0.8.tar.gz
        UPDATE_COMMAND ""
        INSTALL_COMMAND ""
        LOG_DOWNLOAD ON
        LOG_CONFIGURE ON
        LOG_BUILD ON
        CMAKE_CACHE_ARGS "-DSDL_STATIC:BOOL=0 -DSDL_AUDIO:BOOL=0")


find_package(X11 REQUIRED)

ExternalProject_Get_Property(SimpleDirectLayer2 source_dir)
ExternalProject_Get_Property(SimpleDirectLayer2 binary_dir)

set(SDL2_INCLUDE_DIRS ${binary_dir}/include)

# The cloning of the above repo doesn't happen until make, however if the dir doesn't
# exist, INTERFACE_INCLUDE_DIRECTORIES will throw an error.
# To make it work, we just create the directory now during config.
file(MAKE_DIRECTORY ${SDL2_INCLUDE_DIRS})

set(SDL2_LIBRARY_PATH ${binary_dir}/${CMAKE_FIND_LIBRARY_PREFIXES}SDL2.a)
set(SDL2_LIBRARY SDL2)
add_library(${SDL2_LIBRARY} UNKNOWN IMPORTED)
set_target_properties(${SDL2_LIBRARY} PROPERTIES
        "IMPORTED_LOCATION" "${SDL2_LIBRARY_PATH}"
        "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
        "INTERFACE_INCLUDE_DIRECTORIES" "${SDL2_INCLUDE_DIRS}")
add_dependencies(${SDL2_LIBRARY} SimpleDirectLayer2)

ExternalProject_Add_Step(SimpleDirectLayer2 fixHeaders
        COMMAND ${CMAKE_COMMAND} -E make_directory ${binary_dir}/include/SDL2
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${source_dir}/include ${binary_dir}/include/SDL2
        COMMAND ${CMAKE_COMMAND} -E copy ${binary_dir}/include/SDL_config.h ${binary_dir}/include/SDL2
        DEPENDEES build
        )