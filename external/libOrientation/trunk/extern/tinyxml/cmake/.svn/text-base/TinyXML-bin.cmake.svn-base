# adds TINYXML executable targets
# author: Bruno VALLET
# date: 23/10/2009

# define source and header files and directories
set(tinyXmlTest_FULL_SRCS ${TINYXML_ROOT_DIR}/xmltest.cpp)

include_directories(${TINYXML_ROOT_DIR})
set(tinyXmlTest_FULL_HEADERS "")

# set definitions and dependencies
set(tinyXmlTest_DEFINITIONS _CONSOLE TUNE _CRT_SECURE_NO_WARNINGS)
if(WIN32)
	set(tinyXmlTest_DEFINITIONS ${tinyXmlTest_DEFINITIONS} -DTIXML_USE_STL)
	set(tinyXmlTest_TARGET_NAME tinyXmlTestSTL)
else(WIN32)
	set(tinyXmlTest_TARGET_NAME tinyXmlTest)
endif(WIN32)
set(tinyXmlTest_DEPENDENCIES ${tinyxml_TARGET_NAME})

__add_executable (${tinyXmlTest_TARGET_NAME} tinyXmlTest_FULL_SRCS tinyXmlTest_FULL_HEADERS tinyXmlTest_DEFINITIONS tinyXmlTest_DEPENDENCIES)

