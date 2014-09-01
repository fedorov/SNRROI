cmake_minimum_required(VERSION 2.8.7)

#-----------------------------------------------------------------------------
if(NOT Slicer_SOURCE_DIR)
  set(EXTENSION_NAME SNRROI)
  set(EXTENSION_HOMEPAGE "")
  set(EXTENSION_CATEGORY "Quantification")
  set(EXTENSION_CONTRIBUTORS "Andriy Fedorov (SPL/BWH)")
  set(EXTENSION_DESCRIPTION "Calculate approximation to Signal-to-noise ratio for each pixel, as mean/SD over ROI")
  set(EXTENSION_ICONURL "")
  set(EXTENSION_SCREENSHOTURLS "")
endif()

#-----------------------------------------------------------------------------
set(MODULE_NAME SNRROI)

#-----------------------------------------------------------------------------
if(NOT Slicer_SOURCE_DIR)
  find_package(Slicer REQUIRED)
  include(${Slicer_USE_FILE})
endif()

#-----------------------------------------------------------------------------
#add_subdirectory(Logic)

#-----------------------------------------------------------------------------
set(MODULE_INCLUDE_DIRECTORIES
  )

set(MODULE_SRCS
  )

set(MODULE_TARGET_LIBRARIES
  ${ITK_LIBRARIES}
  )

#-----------------------------------------------------------------------------
SEMMacroBuildCLI(
  NAME ${MODULE_NAME}
  TARGET_LIBRARIES ${MODULE_TARGET_LIBRARIES}
  INCLUDE_DIRECTORIES ${MODULE_INCLUDE_DIRECTORIES}
  ADDITIONAL_SRCS ${MODULE_SRCS}
  #EXECUTABLE_ONLY
  )

#-----------------------------------------------------------------------------
if(BUILD_TESTING)
  #  add_subdirectory(Testing)
endif()

#-----------------------------------------------------------------------------
if(NOT Slicer_SOURCE_DIR)
  include(${Slicer_EXTENSION_CPACK})
endif()
