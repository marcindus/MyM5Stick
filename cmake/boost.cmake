include(ExternalProject)

set(BOOST_URL https://dl.bintray.com/boostorg/release/1.67.0/source/boost_1_67_0.tar.gz)

ExternalProject_Add(
        boost
        PREFIX ${CMAKE_BINARY_DIR}/external/boost
        URL ${BOOST_URL}
        URL_HASH SHA256=8aa4e330c870ef50a896634c931adf468b21f8a69b77007e45c444151229f665
        CONFIGURE_COMMAND ./bootstrap.sh --prefix=${CMAKE_BINARY_DIR}/external/boost/boost-installed
        BUILD_COMMAND ./b2 --with-system toolset=gcc variant=debug link=static install --prefix=${CMAKE_BINARY_DIR}/external/boost/boost-installed
        BUILD_IN_SOURCE 1
        INSTALL_COMMAND ""
)

set(Boost_INCLUDE_DIRS ${CMAKE_BINARY_DIR}/external/boost/boost-installed/include)
set(Boost_LIBRARIES ${CMAKE_BINARY_DIR}/external/boost/boost-installed/lib/libboost_system.a)