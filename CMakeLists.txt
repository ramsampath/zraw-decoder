cmake_minimum_required(VERSION 3.10)
project(zraw-decoder)

set(CMAKE_CXX_STANDARD 17)

# Set output directories for builds
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Define build configurations
if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# Define source files
set(SOURCES
    ${CMAKE_SOURCE_DIR}/src/main.cpp
)

set(HEADERS
    ${CMAKE_SOURCE_DIR}/zraw-container/ZrawConverter.hpp
	${CMAKE_SOURCE_DIR}/zraw-container/ZrawConverterThread.hpp 
	${CMAKE_SOURCE_DIR}/zraw-container/ZrawNewExtractor.hpp
	${CMAKE_SOURCE_DIR}/zraw-container/ZrawProcessingModel.hpp
	${CMAKE_SOURCE_DIR}/zraw-container/TinyMovFileLibrary.hpp
	${CMAKE_SOURCE_DIR}/zraw-container/MovAvInfoDetect.hpp
	${CMAKE_SOURCE_DIR}/zraw-container/IConsoleOutput.hpp
	${CMAKE_SOURCE_DIR}/zraw-container/IProgressBar.hpp
	${CMAKE_SOURCE_DIR}/zraw-container/ZRawProcess.hpp
	${CMAKE_SOURCE_DIR}/zraw-container/Event.h 
)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/zraw-decoder-lib/install/include
    ${CMAKE_SOURCE_DIR}/tinydngloader
	${CMAKE_SOURCE_DIR}/tinydngloader/examples/dngwriter
    ${CMAKE_SOURCE_DIR}/quick_arg_parser
	${CMAKE_SOURCE_DIR}/zraw-container
)

# Create executable
add_executable(zraw-decoder ${SOURCES} ${HEADERS})

# Find OpenSSL
set(OPENSSL_ROOT_DIR "F:/OpenSSL-Win64") # Update with the correct path


set(OPENSSL_CRYPTO_LIBRARY "${OPENSSL_ROOT_DIR}/lib/VC/x64/MT/libcrypto.lib")
set(OPENSSL_SSL_LIBRARY "${OPENSSL_ROOT_DIR}/lib/VC/x64/MT/libssl.lib")

find_package(OpenSSL REQUIRED)

# Link Libraries
target_link_libraries(zraw-decoder PRIVATE ${OPENSSL_SSL_LIBRARY} ${OPENSSL_CRYPTO_LIBRARY})

# Link additional libraries
target_link_directories(zraw-decoder PRIVATE
    ${CMAKE_SOURCE_DIR}/zraw-decoder-lib/install/lib
    ${OPENSSL_ROOT_DIR}/lib
)

target_link_libraries(zraw-decoder PRIVATE libzraw)

# Set Debug/Release specific flags
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(zraw-decoder PRIVATE DEBUG)
    target_compile_options(zraw-decoder PRIVATE /MDd)
else()
    target_compile_definitions(zraw-decoder PRIVATE NDEBUG)
    target_compile_options(zraw-decoder PRIVATE /MD)
endif()

# Add pre-build commands
add_custom_target(pre-build
    COMMAND git submodule update --init
#    COMMAND "${CMAKE_VS_DEVENV_COMMAND}" "${CMAKE_SOURCE_DIR}/../zraw-decoder-lib/vc2017/zraw-decoder-lib.sln" /Build ${CMAKE_BUILD_TYPE}
#    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_dependencies(zraw-decoder pre-build)

# Add post-build commands to copy the output binary
#add_custom_command(TARGET zraw-decoder POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/../build
#    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:zraw-decoder> ${CMAKE_SOURCE_DIR}/../build/
#)

# Install the executable
install(TARGETS zraw-decoder RUNTIME DESTINATION bin)

# Install OpenSSL DLLs (for Windows)
if(WIN32)
    install(FILES
        ${OPENSSL_ROOT_DIR}/bin/libcrypto-3-x64.dll
        ${OPENSSL_ROOT_DIR}/bin/libssl-3-x64.dll
		${CMAKE_SOURCE_DIR}/zraw-decoder-lib/install/lib/libzraw.dll
        DESTINATION bin
    )
endif()
