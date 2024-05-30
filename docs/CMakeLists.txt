
# Require Doxygen:

find_package (Doxygen)
option (BUILD_DOCS "Create and install the HTML-based documentation (requires Doxygen)" ${DOXYGEN_FOUND})

# If we have Doxygen, build the docs:

if (BUILD_DOCS)
  if (NOT DOXYGEN_FOUND)
    message (FATAL_ERROR "Doxygen is needed to build the documentation.")
  endif()

  # Configure Doxygen.in in build directory and start the build:
  
  set (doxyfile_in ${CMAKE_SOURCE_DIR}/docs/Doxyfile.in)
  set (doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
  configure_file (${doxyfile_in} ${doxyfile} @ONLY)
  
  message ("Doxygen build started.")

  # CMAKE_CURRENT_BINARY_DIR is @build@/docs, build docs there:
  
  add_custom_target (docs ALL
    COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating documentation with Doxygen"
    VERBATIM
    )  

  # Install under @prefix@/share/

  install (DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION share)
  install (DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/latex DESTINATION share)

endif()
