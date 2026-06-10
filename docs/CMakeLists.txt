cmake_minimum_required(VERSION 3.13)

# -----------------------------------------------------------------------
# Documentation — requires Doxygen.
# This file is only included when DDASFMT_BUILD_DOCS is ON, so a missing
# Doxygen is always a hard error here rather than a silent skip.
# -----------------------------------------------------------------------

find_package(Doxygen REQUIRED)

set(doxyfile_in  ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
set(doxyfile     ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

configure_file(${doxyfile_in} ${doxyfile} @ONLY)

message(STATUS "Doxygen build started.")

add_custom_target(ddasfmt_docs ALL
  COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  COMMENT "Generating DDASFormat documentation with Doxygen"
  VERBATIM
  )

install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html  DESTINATION share)
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/latex DESTINATION share)