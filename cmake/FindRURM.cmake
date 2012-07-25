# - Try to find RUR module library
# Once done, this will define
#
#  RURM_FOUND - system has RUR module library
#  RURM_INCLUDE_DIRS - the RUR module include directories
#  RURM_LIBRARIES - link these to use the RUR module 

include(LibFindMacros)

# Dependencies 
libfind_package(YARP YARP)

# Use pkg-config to get hints about paths (not useful, but it cannot hurt)
libfind_pkg_check_modules(RURM_PKGCONF RURM)

# Include dir
find_path(RURM_INCLUDE_DIR
  NAMES rurm
  PATHS ${RURM_PKGCONF_INCLUDE_DIRS}
)

# Finally the library itself
find_library(RURM_LIBRARY
  NAMES rurm
  PATHS ${RURM_PKGCONF_LIBRARY_DIRS}
)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(RURM_PROCESS_INCLUDES RURM_INCLUDE_DIR RURM_INCLUDE_DIRS)
set(RURM_PROCESS_LIBS RURM_LIBRARY RURM_LIBRARIES)
libfind_process(RURM)

