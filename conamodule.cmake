list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR}/Conan)
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})


if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/0.18.1/conan.cmake"
            "${CMAKE_BINARY_DIR}/conan.cmake"
            TLS_VERIFY ON)
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_configure(REQUIRES
        catch2/2.13.8
        GENERATORS cmake_find_package)

conan_cmake_autodetect(settings)

conan_cmake_install(PATH_OR_REFERENCE .
        BUILD missing
        REMOTE conancenter
        INSTALL_FOLDER ${CMAKE_BINARY_DIR}/Conan
        SETTINGS ${settings})

find_package(catch2 REQUIRED)