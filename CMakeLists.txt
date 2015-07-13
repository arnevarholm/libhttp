# Determines what CMake APIs we can rely on
cmake_minimum_required (VERSION 2.8.11)
if (${CMAKE_VERSION} VERSION_GREATER 3.2.2)
  cmake_policy(VERSION 3.2.2)
endif()
if (${CMAKE_VERSION} VERSION_GREATER 3.1 OR
    ${CMAKE_VERSION} VERSION_EQUAL 3.1)
  cmake_policy(SET CMP0054 NEW)
endif()

# Do not allow in source builds
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# Make sure we can import out CMake functions
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Load in the needed CMake modules
include(CheckCCompilerFlag)
include(CheckCXXCompilerFlag)
include(AddCCompilerFlag)
include(AddCXXCompilerFlag)
include(DetermineTargetArchitecture)
include(CMakeDependentOption)

# Set up the project
project (civetweb)
set(CIVETWEB_VERSION "1.7.0" CACHE STRING "The version of the civetweb library")
string(REGEX MATCH "([0-9]+)\\.([0-9]+)\\.([0-9]+)" CIVETWEB_VERSION_MATCH "${CIVETWEB_VERSION}")
if ("${CIVETWEB_VERSION_MATCH}" STREQUAL "")
  message(FATAL_ERROR "Must specify a semantic version: major.minor.patch")
endif()
set(CIVETWEB_VERSION_MAJOR "${CMAKE_MATCH_1}")
set(CIVETWEB_VERSION_MINOR "${CMAKE_MATCH_2}")
set(CIVETWEB_VERSION_PATCH "${CMAKE_MATCH_3}")
determine_target_architecture(CIVETWEB_ARCHITECTURE)

# Detect the platform reliably
if(NOT MACOSX AND ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
   SET(DARWIN YES)
elseif(NOT BSD AND ${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
    SET(FREEBSD YES)
elseif(NOT LINUX AND ${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    SET(LINUX YES)
endif()

# C++ wrappers
option(CIVETWEB_ENABLE_THIRD_PARTY_OUTPUT "Shows the output of third party dependency processing" OFF)

# Max Request Size
set(CIVETWEB_MAX_REQUEST_SIZE 16384 CACHE STRING
  "The largest amount of content bytes allowed in a request")
set_property(CACHE CIVETWEB_MAX_REQUEST_SIZE PROPERTY VALUE ${CIVETWEB_MAX_REQUEST_SIZE})
message(STATUS "Max Request Size - ${CIVETWEB_MAX_REQUEST_SIZE}")

# Thread Stack Size
set(CIVETWEB_THREAD_STACK_SIZE 102400 CACHE STRING
  "The stack size in bytes for each thread created")
set_property(CACHE CIVETWEB_THREAD_STACK_SIZE PROPERTY VALUE ${CIVETWEB_THREAD_STACK_SIZE})
message(STATUS "Thread Stack Size - ${CIVETWEB_THREAD_STACK_SIZE}")

# C++ wrappers
option(CIVETWEB_ENABLE_CXX "Enables the C++ wrapper library" OFF)
message(STATUS "C++ wrappers - ${CIVETWEB_ENABLE_CXX}")

# IP Version 6
option(CIVETWEB_ENABLE_IPV6 "Enables the IP version 6 support" OFF)
message(STATUS "IP Version 6 - ${CIVETWEB_ENABLE_IPV6}")

# Websocket support
option(CIVETWEB_ENABLE_WEBSOCKETS "Enable websockets connections" OFF)
message(STATUS "Websockets support - ${CIVETWEB_ENABLE_WEBSOCKETS}")

# Memory debugging
option(CIVETWEB_ENABLE_MEMORY_DEBUGGING "Enable the memory debugging features" OFF)
message(STATUS "Memory Debugging - ${CIVETWEB_ENABLE_MEMORY_DEBUGGING}")

# LUA CGI support
option(CIVETWEB_ENABLE_LUA "Enable Lua CGIs" OFF)
message(STATUS "Lua CGI support - ${CIVETWEB_ENABLE_LUA}")

# Link to the shared LUA library
cmake_dependent_option(
  CIVETWEB_ENABLE_LUA_SHARED  "Link to the shared LUA system library" OFF
 CIVETWEB_ENABLE_LUA OFF)
if (CIVETWEB_ENABLE_LUA)
  message(STATUS "Linking shared Lua library - ${CIVETWEB_ENABLE_LUA_SHARED}")
endif()

# Lua Third Party Settings
if (CIVETWEB_ENABLE_LUA)
  if (NOT CIVETWEB_ENABLE_LUA_SHARED)
    # Lua Version
    set(CIVETWEB_LUA_VERSION 5.3.0 CACHE STRING
      "The version of Lua to build and include statically")
    set_property(CACHE CIVETWEB_LUA_VERSION PROPERTY VALUE ${CIVETWEB_LUA_VERSION})
    message(STATUS "Lua Version - ${CIVETWEB_LUA_VERSION}")
    mark_as_advanced(CIVETWEB_LUA_VERSION)

    # Lua Verification Hash
    set(CIVETWEB_LUA_MD5_HASH a1b0a7e92d0c85bbff7a8d27bf29f8af CACHE STRING
      "The hash of Lua archive to be downloaded")
    set_property(CACHE CIVETWEB_LUA_MD5_HASH PROPERTY VALUE ${CIVETWEB_LUA_MD5_HASH})
    mark_as_advanced(CIVETWEB_LUA_MD5_HASH)
  endif()

  # Lua Filesystem Version
  set(CIVETWEB_LUA_FILESYSTEM_VERSION 1.6.3 CACHE STRING
    "The version of Lua Filesystem to build and include statically")
  set_property(CACHE CIVETWEB_LUA_FILESYSTEM_VERSION PROPERTY VALUE ${CIVETWEB_LUA_FILESYSTEM_VERSION})
  message(STATUS "Lua Filesystem Version - ${CIVETWEB_LUA_FILESYSTEM_VERSION}")
  mark_as_advanced(CIVETWEB_LUA_FILESYSTEM_VERSION)

  # Lua Filesystem Verification Hash
  set(CIVETWEB_LUA_FILESYSTEM_MD5_HASH d0552c7e5a082f5bb2865af63fb9dc95 CACHE STRING
    "The hash of Lua Filesystem archive to be downloaded")
  set_property(CACHE CIVETWEB_LUA_FILESYSTEM_MD5_HASH PROPERTY VALUE ${CIVETWEB_LUA_FILESYSTEM_MD5_HASH})
  mark_as_advanced(CIVETWEB_LUA_FILESYSTEM_MD5_HASH)

  # Lua SQLite Version
  set(CIVETWEB_LUA_SQLITE_VERSION 0.9.3 CACHE STRING
    "The version of Lua SQLite to build and include statically")
  set_property(CACHE CIVETWEB_LUA_SQLITE_VERSION PROPERTY VALUE ${CIVETWEB_LUA_SQLITE_VERSION})
  message(STATUS "Lua SQLite Version - ${CIVETWEB_LUA_SQLITE_VERSION}")
  mark_as_advanced(CIVETWEB_LUA_SQLITE_VERSION)

  # Lua SQLite Verification Hash
  set(CIVETWEB_LUA_SQLITE_MD5_HASH 43234ae08197dfce6da02482ed14ec92 CACHE STRING
    "The hash of Lua SQLite archive to be downloaded")
  set_property(CACHE CIVETWEB_LUA_SQLITE_MD5_HASH PROPERTY VALUE ${CIVETWEB_LUA_SQLITE_MD5_HASH})
  mark_as_advanced(CIVETWEB_LUA_SQLITE_MD5_HASH)

  # Lua XML Version
  set(CIVETWEB_LUA_XML_VERSION 1.8.0 CACHE STRING
    "The version of Lua XML to build and include statically")
  set_property(CACHE CIVETWEB_LUA_XML_VERSION PROPERTY VALUE ${CIVETWEB_LUA_XML_VERSION})
  message(STATUS "Lua XML Version - ${CIVETWEB_LUA_XML_VERSION}")
  mark_as_advanced(CIVETWEB_LUA_XML_VERSION)

  # Lua XML Verification Hash
  set(CIVETWEB_LUA_XML_MD5_HASH 25e4c276c5d8716af1de0c7853aec2b4 CACHE STRING
    "The hash of Lua XML archive to be downloaded")
  set_property(CACHE CIVETWEB_LUA_XML_MD5_HASH PROPERTY VALUE ${CIVETWEB_LUA_XML_MD5_HASH})
  mark_as_advanced(CIVETWEB_LUA_XML_MD5_HASH)

  # SQLite Version
  set(CIVETWEB_SQLITE_VERSION 3.8.9 CACHE STRING
    "The version of SQLite to build and include statically")
  set_property(CACHE CIVETWEB_SQLITE_VERSION PROPERTY VALUE ${CIVETWEB_SQLITE_VERSION})
  message(STATUS "SQLite Version - ${CIVETWEB_SQLITE_VERSION}")
  mark_as_advanced(CIVETWEB_SQLITE_VERSION)

  # SQLite Verification Hash
  set(CIVETWEB_SQLITE_MD5_HASH 02e9c3a6daa8b8587cf6bef828c2e33f CACHE STRING
    "The hash of SQLite archive to be downloaded")
  set_property(CACHE CIVETWEB_SQLITE_MD5_HASH PROPERTY VALUE ${CIVETWEB_SQLITE_MD5_HASH})
  mark_as_advanced(CIVETWEB_SQLITE_MD5_HASH)
endif()

# SSL support
option(CIVETWEB_ENABLE_SSL "Enables the secure socket layer" ON)
message(STATUS "SSL support - ${CIVETWEB_ENABLE_SSL}")

# Dynamically load or link the SSL libraries
cmake_dependent_option(
  CIVETWEB_ENABLE_SSL_DYNAMIC_LOADING "Dynamically loads the SSL library rather than linking it" ON
  CIVETWEB_ENABLE_SSL OFF)
if (CIVETWEB_ENABLE_SSL)
  message(STATUS "Dynamically load SSL libraries - ${CIVETWEB_ENABLE_SSL_DYNAMIC_LOADING}")
endif()

# Unix systems can define the dynamic library names to load
if (CIVETWEB_ENABLE_SSL_DYNAMIC_LOADING AND NOT DARWIN AND UNIX)
  # SSL library name
  set(CIVETWEB_SSL_SSL_LIB "libssl.so" CACHE STRING
    "The name of the SSL library to load")
  set_property(CACHE CIVETWEB_SSL_SSL_LIB PROPERTY VALUE ${CIVETWEB_SSL_SSL_LIB})
  message(STATUS "SSL Library Name - ${CIVETWEB_SSL_SSL_LIB}")

  # Crytography library name
  set(CIVETWEB_SSL_CRYPTO_LIB "libcrypto.so" CACHE STRING
    "The name of the SSL Cryptography library to load")
  set_property(CACHE CIVETWEB_SSL_CRYPTO_LIB PROPERTY VALUE ${CIVETWEB_SSL_CRYPTO_LIB})
  message(STATUS "SSL Cryptography Library Name - ${CIVETWEB_SSL_CRYPTO_LIB}")
endif()

# The C and C++ standards to use
set(CIVETWEB_C_STANDARD auto CACHE STRING
  "The C standard to use; auto determines the latest supported by the compiler")
set_property(CACHE CIVETWEB_C_STANDARD PROPERTY STRINGS auto c11 c99 c89)
set(CIVETWEB_CXX_STANDARD auto CACHE STRING
  "The C++ standard to use; auto determines the latest supported by the compiler")
set_property(CACHE CIVETWEB_CXX_STANDARD PROPERTY STRINGS auto c++14 c++11 c++98)

# Configure the linker
if ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
  find_program(GCC_AR gcc-ar)
  if (GCC_AR)
    set(CMAKE_AR ${GCC_AR})
  endif()
  find_program(GCC_RANLIB gcc-ranlib)
  if (GCC_RANLIB)
    set(CMAKE_RANLIB ${GCC_RANLIB})
  endif()
endif()

# Configure the C compiler
message(STATUS "Configuring C Compiler")
if ("${CIVETWEB_C_STANDARD}" STREQUAL "auto")
  add_c_compiler_flag(-std=c11)
  if (NOT HAVE_C_FLAG_STD_C11)
    add_c_compiler_flag(-std=c99)
    if (NOT HAVE_C_FLAG_STD_C99)
      add_c_compiler_flag(-std=c89)
    endif()
  endif()
else()
  add_c_compiler_flag(-std=${CIVETWEB_C_STANDARD})
endif()
add_c_compiler_flag(-Wall)
add_c_compiler_flag(-Wextra)
add_c_compiler_flag(-Wshadow)
add_c_compiler_flag(-Wsign-conversion)
add_c_compiler_flag(-Wmissing-prototypes)
add_c_compiler_flag(-Weverything)
add_c_compiler_flag(/W4)
add_c_compiler_flag(-Wno-padded)
add_c_compiler_flag(/Wd4820) # padding
add_c_compiler_flag(-Wno-unused-macros)
add_c_compiler_flag(-Wno-format-nonliteral)
if (MINGW)
  add_c_compiler_flag(-Wno-format)
endif()
add_c_compiler_flag(-Werror)
add_c_compiler_flag(/WX)
add_c_compiler_flag(-pedantic-errors)
add_c_compiler_flag(-fvisibility=hidden)
add_c_compiler_flag(-fstack-protector-strong RELEASE)
add_c_compiler_flag(-flto RELEASE)
add_c_compiler_flag(-fsanitize=undefined DEBUG)
add_c_compiler_flag(-fsanitize=address DEBUG)
if (HAVE_C_FLAG_FSANITIZE_ADDRESS)
  add_c_compiler_flag(-static-asan DEBUG)
endif()
add_c_compiler_flag(-fstack-protector-all DEBUG)
add_c_compiler_flag(-mwindows)

# Coverage build type
set(CMAKE_C_FLAGS_COVERAGE "${CMAKE_C_FLAGS_DEBUG}" CACHE STRING
    "Flags used by the C compiler during coverage builds."
    FORCE)
set(CMAKE_EXE_LINKER_FLAGS_COVERAGE
    "${CMAKE_EXE_LINKER_FLAGS_DEBUG}" CACHE STRING
    "Flags used for linking binaries during coverage builds."
    FORCE)
set(CMAKE_SHARED_LINKER_FLAGS_COVERAGE
    "${CMAKE_SHARED_LINKER_FLAGS_DEBUG}" CACHE STRING
    "Flags used by the shared libraries linker during coverage builds."
    FORCE)
mark_as_advanced(
    CMAKE_CXX_FLAGS_COVERAGE
    CMAKE_C_FLAGS_COVERAGE
    CMAKE_EXE_LINKER_FLAGS_COVERAGE
    CMAKE_SHARED_LINKER_FLAGS_COVERAGE)
set(CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE}" CACHE STRING
    "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel Coverage."
    FORCE)
add_c_compiler_flag(--coverage COVERAGE)

# Configure the C++ compiler
if (CIVETWEB_ENABLE_CXX)
  message(STATUS "Configuring C++ Compiler")
  if ("${CIVETWEB_CXX_STANDARD}" STREQUAL "auto")
    add_cxx_compiler_flag(-std=c++14)
    if (NOT HAVE_CXX_FLAG_STD_CXX14)
      add_cxx_compiler_flag(-std=c++11)
      if (NOT HAVE_CXX_FLAG_STD_CXX11)
        add_cxx_compiler_flag(-std=c++98)
      endif()
    endif()
  else()
    add_cxx_compiler_flag(-std=${CIVETWEB_CXX_STANDARD})
  endif()
  add_cxx_compiler_flag(-Wall)
  add_cxx_compiler_flag(-Wextra)
  add_cxx_compiler_flag(-Wshadow)
  add_cxx_compiler_flag(-Wsign-conversion)
  add_cxx_compiler_flag(-Wmissing-prototypes)
  add_cxx_compiler_flag(-Weverything)
  add_cxx_compiler_flag(/W4)
  add_cxx_compiler_flag(-Wno-padded)
  add_cxx_compiler_flag(/Wd4820) # padding
  add_cxx_compiler_flag(-Wno-unused-macros)
  add_cxx_compiler_flag(-Wno-format-nonliteral)
  if (MINGW)
    add_cxx_compiler_flag(-Wno-format)
  endif()
  add_cxx_compiler_flag(-Werror)
  add_cxx_compiler_flag(/WX)
  add_cxx_compiler_flag(-pedantic-errors)
  add_cxx_compiler_flag(-Wzero-as-null-pointer-constant)
  add_cxx_compiler_flag(-fvisibility=hidden)
  add_cxx_compiler_flag(-fstack-protector-strong RELEASE)
  add_cxx_compiler_flag(-flto RELEASE)
  add_cxx_compiler_flag(-fsanitize=undefined DEBUG)
  add_cxx_compiler_flag(-fsanitize=address DEBUG)
  if (HAVE_CXX_FLAG_FSANITIZE_ADDRESS)
    add_cxx_compiler_flag(-static-asan DEBUG)
  endif()
  add_cxx_compiler_flag(-fstack-protector-all DEBUG)
  add_cxx_compiler_flag(-mwindows)
  set(CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS_DEBUG}" CACHE STRING
      "Flags used by the C++ compiler during coverage builds."
      FORCE)
  add_cxx_compiler_flag(--coverage COVERAGE)
endif()

# Set up the definitions
if (${CMAKE_BUILD_TYPE} MATCHES "[Dd]ebug")
  add_definitions(-DDEBUG)
endif()
if (HAVE_STDINT)
  add_definitions(-DHAVE_STDINT)
endif()
if (CIVETWEB_ENABLE_IPV6)
  add_definitions(-DUSE_IPV6)
endif()
if (CIVETWEB_ENABLE_WEBSOCKETS)
  add_definitions(-DUSE_WEBSOCKET)
endif()
if (CIVETWEB_ENABLE_LUA)
  add_definitions(-DUSE_LUA)
endif()
if (CIVETWEB_ENABLE_MEMORY_DEBUGGING)
  add_definitions(-DMEMORY_DEBUGGING)
endif()
if (NOT CIVETWEB_ENABLE_SSL)
  add_definitions(-DNO_SSL)
elseif (NOT CIVETWEB_ENABLE_SSL_DYNAMIC_LOADING)
  add_definitions(-DNO_SSL_DL)
else()
  if(CIVETWEB_SSL_SSL_LIB)
    add_definitions(-DSSL_LIB="${CIVETWEB_SSL_SSL_LIB}")
  endif()
  if(CIVETWEB_SSL_CRYPTO_LIB)
    add_definitions(-DCRYPTO_LIB="${CIVETWEB_SSL_CRYPTO_LIB}")
  endif()
endif()
add_definitions(-DUSE_STACK_SIZE=${CIVETWEB_THREAD_STACK_SIZE})
add_definitions(-DMAX_REQUEST_SIZE=${CIVETWEB_MAX_REQUEST_SIZE})

# Build the targets
add_subdirectory(src)

# Enable the testing of the library/executable
include(CTest)
if (BUILD_TESTING)
  # Check unit testing framework Version
  set(CIVETWEB_CHECK_VERSION 0.9.14 CACHE STRING
    "The version of Check unit testing framework to build and include statically")
  set_property(CACHE CIVETWEB_CHECK_VERSION PROPERTY VALUE ${CIVETWEB_CHECK_VERSION})
  message(STATUS "Check Unit Testing Framework Version - ${CIVETWEB_CHECK_VERSION}")
  mark_as_advanced(CIVETWEB_CHECK_VERSION)

  # Check unit testing framework Verification Hash
  set(CIVETWEB_CHECK_MD5_HASH 38263d115d784c17aa3b959ce94be8b8 CACHE STRING
    "The hash of Check unit testing framework archive to be downloaded")
  set_property(CACHE CIVETWEB_CHECK_MD5_HASH PROPERTY VALUE ${CIVETWEB_CHECK_MD5_HASH})
  mark_as_advanced(CIVETWEB_CHECK_MD5_HASH)

  # Build the testing
  add_subdirectory(test)
endif()

# Set up CPack
include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_VENDOR "civetweb Contributors")
set(CPACK_PACKAGE_CONTACT "civetweb@github.com")
set(CPACK_PACKAGE_VERSION_MAJOR "${CIVETWEB_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${CIVETWEB_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${CIVETWEB_VERSION_PATCH}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A HTTP library and server")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE.md")
set(CPACK_STRIP_FILES TRUE)
set(CPACK_PACKAGE_DEPENDS "openssl")
if (CIVETWEB_ENABLE_LUA_SHARED)
  set(CPACK_PACKAGE_DEPENDS "lua, ${CPACK_PACKAGE_DEPENDS}")
endif()

# RPM Packaging
set(CPACK_RPM_PACKAGE_GROUP "Development/Libraries")
set(CPACK_RPM_PACKAGE_LICENSE "MIT")
set(CPACK_RPM_PACKAGE_ARCHITECTURE "${CIVETWEB_ARCHITECTURE}")
set(CPACK_RPM_PACKAGE_REQUIRES "${CPACK_PACKAGE_DEPENDS}")

# Debian Packaging
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "${CIVETWEB_ARCHITECTURE}")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://github.com/bel2125/civetweb")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "${CPACK_PACKAGE_DEPENDS}")

# WiX Packaging
# TODO: www.cmake.org/cmake/help/v3.0/module/CPackWIX.html

# Finalize CPack settings
include(CPack)