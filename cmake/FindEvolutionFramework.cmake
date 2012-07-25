# - Try to find EvolutionFramework
# Once done, this will define
#
#  EvolutionFramework_FOUND - system has EvolutionFramework
#  EvolutionFramework_INCLUDE_DIRS - the EvolutionFramework include directories
#  EvolutionFramework_LIBRARIES - link these to use EvolutionFramework

include(LibFindMacros)

# Dependencies (not useful for EvolutionFramework)
#libfind_package(EvolutionFramework symbicator-simulation1)

# Use pkg-config to get hints about paths (not useful for EvolutionFramework, but it cannot hurt)
libfind_pkg_check_modules(EvolutionFramework_PKGCONF EvolutionFramework)

# Include dir
find_path(EvolutionFramework_INCLUDE_DIR
  NAMES evolutionframework
  PATHS ${EvolutionFramework_PKGCONF_INCLUDE_DIRS}
)

# Finally the library itself
find_library(EvolutionFramework_LIBRARY
  NAMES evolutionframework
  PATHS ${EvolutionFramework_PKGCONF_LIBRARY_DIRS}
)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(EvolutionFramework_PROCESS_INCLUDES EvolutionFramework_INCLUDE_DIR EvolutionFramework_INCLUDE_DIRS)
set(EvolutionFramework_PROCESS_LIBS EvolutionFramework_LIBRARY EvolutionFramework_LIBRARIES)
libfind_process(EvolutionFramework)

