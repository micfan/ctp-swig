include_directories(${PYTHON_INCLUDE_DIRS})
set(CMAKE_SWIG_FLAGS -features autodoc=1 -threads)
set(CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/install)

set(_target ctp-python)
set(WRAPPER_FILES ../ctp.i)

set_source_files_properties(${WRAPPER_FILES} PROPERTIES CPLUSPLUS ON)
SWIG_ADD_LIBRARY(${_target}
	LANGUAGE  python 
        SOURCES ${WRAPPER_FILES}
)
SWIG_LINK_LIBRARIES(${_target} ${PYTHON_LIBRARIES} ${CTP_LIBRARIES})
set_target_properties(${SWIG_MODULE_${_target}_REAL_NAME} PROPERTIES
    OUTPUT_NAME "_ctp"
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/install)

# file(COPY ${CTP_SHAREDLIBFILES} DESTINATION ${CMAKE_SWIG_OUTDIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/install DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/install)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/runmd.py DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/install)

execute_process(COMMAND ${PYTHON_EXECUTABLE} -c "import sysconfig; print(sysconfig.get_path('platlib'))"
    OUTPUT_VARIABLE _py_site_package_dir OUTPUT_STRIP_TRAILING_WHITESPACE)
install(TARGETS ${SWIG_MODULE_${_target}_REAL_NAME} DESTINATION ${_py_site_package_dir})
install(FILES ${swig_generated_sources} DESTINATION ${_py_site_package_dir})
