#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "LDtkLoader" for configuration "Release"
set_property(TARGET LDtkLoader APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LDtkLoader PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LDtkLoader.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LDtkLoader )
list(APPEND _IMPORT_CHECK_FILES_FOR_LDtkLoader "${_IMPORT_PREFIX}/lib/LDtkLoader.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
