include_directories(${RUBY_INCLUDE_DIRS})
set(CMAKE_SWIG_FLAGS -autorename)
set(CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR})

set(_target callback-ruby)
set(WRAPPER_FILES ../callback.i)
set_source_files_properties(${WRAPPER_FILES} PROPERTIES CPLUSPLUS ON)
swig_add_module(${_target} ruby ${WRAPPER_FILES})
swig_link_libraries(${_target} ${RUBY_LIBRARY} ${CTP_LIBRARIES})
set_target_properties(${_target} PROPERTIES PREFIX "" SUFFIX ".so")
set_target_properties(${_target} PROPERTIES OUTPUT_NAME "swigmt")

execute_process(COMMAND ${RUBY_EXECUTABLE} -e "print RbConfig::CONFIG['sitearchdir']" OUTPUT_VARIABLE _ruby_site_dir)
install(TARGETS ${_target} DESTINATION ${_ruby_site_dir})

#file(TO_NATIVE_PATH ${CMAKE_SWIG_OUTDIR} CMAKE_SWIG_OUTDIR)
#file(COPY runme.rb DESTINATION ${CMAKE_SWIG_OUTDIR})
