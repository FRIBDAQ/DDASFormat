
# Require Doxygen:

find_package (Doxygen)

if (BUILD_DOCS)

  # If docs were requested but Doxygen is missing, warn and skip the docs
  # rather than aborting the whole configure: the library itself does not
  # need Doxygen.

  if (NOT DOXYGEN_FOUND)
    message (WARNING "Doxygen not found: skipping documentation build.")
    return()
  endif()

  # Configure Doxyfile.in in build directory and start the build:

  set (doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
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

  # Install the generated docs under @prefix@/share/. The HTML output is
  # always produced (GENERATE_HTML = YES). The LaTeX output is only produced
  # when GENERATE_LATEX is enabled in the Doxyfile, so install it only if it
  # was actually generated. The directories are created at build time, so the
  # existence check is deferred to install time via install(CODE).

  install (DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION share)

  install (CODE "
    if (EXISTS \"${CMAKE_CURRENT_BINARY_DIR}/latex\")
      file (INSTALL
        DESTINATION \"\${CMAKE_INSTALL_PREFIX}/share\"
        TYPE DIRECTORY
        FILES \"${CMAKE_CURRENT_BINARY_DIR}/latex\")
    endif ()
  ")

endif()
