cmake_minimum_required(VERSION 2.8)

if (NOT CMAKE_BUILD_TYPE)
	message(STATUS "No build type selected, default to Release")
	set(CMAKE_BUILD_TYPE "Release")
endif()

if (WIN32)
	# Download Win flex-bison
	set(WINFLEXBISON_URL "http://downloads.sourceforge.net/project/winflexbison/win_flex_bison-latest.zip")
	set(WINFLEXBISON_ZIP "${CMAKE_CURRENT_BINARY_DIR}/winflexbison.zip")
	set(WIN_BISON "${CMAKE_CURRENT_BINARY_DIR}/win_bison.exe")
	set(WIN_FLEX "${CMAKE_CURRENT_BINARY_DIR}/win_flex.exe")
	set(WIN_FLEXBISON_DATA "${CMAKE_CURRENT_BINARY_DIR}/data")

	if (NOT EXISTS "${WINFLEXBISON_ZIP}")
		message(STATUS "Downloading Win flex-bison...")
		file(DOWNLOAD "${WINFLEXBISON_URL}" "${WINFLEXBISON_ZIP}" SHOW_PROGRESS)
	endif()
	if (NOT EXISTS "${WIN_BISON}" OR NOT EXISTS "${WIN_FLEX}" OR NOT EXISTS "${WIN_FLEXBISON_DATA}")
		message(STATUS "Unzipping Win flex-bison...")
		execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf "${WINFLEXBISON_ZIP}")
	endif()
endif()

# Flex/Bison stuff
find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

BISON_TARGET(parser compiler/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cc)
FLEX_TARGET(lexer compiler/lexer.l  ${CMAKE_CURRENT_BINARY_DIR}/lexer.cc)
ADD_FLEX_BISON_DEPENDENCY(lexer parser)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/compiler ${CMAKE_CURRENT_SOURCE_DIR}/sim ${CMAKE_CURRENT_SOURCE_DIR}/disasm)

set(HEADERS
  compiler/ops.h
  compiler/parser.h
  compiler/util.h
)

add_executable(compiler compiler/main.cc ${HEADERS} ${BISON_parser_OUTPUTS} ${FLEX_lexer_OUTPUTS})
add_executable(sim sim/main.cc)
add_executable(disasm disasm/main.cc)

# target_link_libraries(compiler ${FLEX_LIBRARIES} ${BISON_LIBRARIES}) # not required anymore, because of noyywrap

if (CMAKE_VERSION VERSION_LESS "3.1")
	include(CheckCXXCompilerFlag)
	CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
	CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
	if (COMPILER_SUPPORTS_CXX11)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
	elseif (COMPILER_SUPPORTS_CXX0X)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
	else()
		message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
	endif()
else()
	set_property(TARGET compiler PROPERTY CXX_STANDARD 11)
	set_property(TARGET sim PROPERTY CXX_STANDARD 11)
	set_property(TARGET disasm PROPERTY CXX_STANDARD 11)
endif()