cmake_minimum_required(VERSION 3.1)

project(SoftRendererPlayer)

################################################################################
# Source files
################################################################################
file(GLOB COMMON_SOURCE_FILES
	${PROJECT_SOURCE_DIR}/*.cpp
	${PROJECT_SOURCE_DIR}/*.h
	)
	
file(GLOB WINDOWS_RESOURCE_FILES
	${PROJECT_SOURCE_DIR}/Res/*.h
	${PROJECT_SOURCE_DIR}/Res/*.rc
	${PROJECT_SOURCE_DIR}/Res/*.ico
	)
	
set_source_files_properties(${COMMON_SOURCE_FILES} PROPERTIES HEADER_FILE_ONLY FALSE)
set_source_files_properties(${WINDOWS_RESOURCE_FILES} PROPERTIES HEADER_FILE_ONLY FALSE)

set(ALL_FILES
    ${COMMON_SOURCE_FILES}
    ${WINDOWS_RESOURCE_FILES})
	
foreach(FILE ${ALL_FILES})
    # make rel path
	file(RELATIVE_PATH REL_PATH "${PROJECT_SOURCE_DIR}" "${FILE}")

    # get the base path
    get_filename_component(BASE_PATH "${REL_PATH}" PATH)

    # changes /'s to \\'s
    string(REPLACE "/" "\\" GROUP "${BASE_PATH}")

    source_group("${GROUP}" FILES "${FILE}")
endforeach()


################################################################################
# Include directory
################################################################################	
include_directories(
    ${PROJECT_SOURCE_DIR}
	)
	
################################################################################
# Precompiled Headers #컴파일 시간을 줄이기 위해 사용 #자주 변하지않는 것들을 하나의 컴파일에 넣어둠 
################################################################################
set(ARG_PCH_HEADER WindowsPrecompiled.h)
set(ARG_PCH_SOURCE WindowsPrecompiled.cpp)
foreach (FILE ${ALL_FILES})
  file(RELATIVE_PATH REL_PATH "${PROJECT_SOURCE_DIR}" "${FILE}")
  if (REL_PATH MATCHES ".*\\.(c|cpp)$")
	if (REL_PATH STREQUAL ${ARG_PCH_SOURCE})
	  set_source_files_properties(${FILE} PROPERTIES COMPILE_FLAGS "/Yc\"${ARG_PCH_HEADER}\"")
	else()
	  set_source_files_properties(${FILE} PROPERTIES COMPILE_FLAGS "/Yu\"${ARG_PCH_HEADER}\"")
	endif()
  endif()
endforeach(FILE)

################################################################################
# Excutable Output Setting
################################################################################
add_executable(${PROJECT_NAME} WIN32 ${ALL_FILES}) #add_executable 현재 모든 파이릉ㄹ 가지고 프로젝트이름으로 win32 프로그램을 만들겠다 

set_target_properties(${PROJECT_NAME} PROPERTIES
	PREFIX ""
	OUTPUT_NAME ${PROJECT_NAME}
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Output/Win64/Binary
	)
	
################################################################################
# Import Library  ## 외부모듈사용 
################################################################################
target_link_libraries(${PROJECT_NAME})