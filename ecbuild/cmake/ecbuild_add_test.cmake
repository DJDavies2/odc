# (C) Copyright 1996-2012 ECMWF.
# 
# This software is licensed under the terms of the Apache Licence Version 2.0
# which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
# In applying this licence, ECMWF does not waive the privileges and immunities 
# granted to it by virtue of its status as an intergovernmental organisation nor
# does it submit to any jurisdiction.

##############################################################################
# macro for adding a test
##############################################################################

macro( ecbuild_add_test )

    set( options ) # no options
    set( single_value_args TARGET ENABLED COMMAND TYPE LINKER_LANGUAGE )
    set( multi_value_args  SOURCES LIBS INCLUDES DEPENDS ARGS PERSISTENT DEFINITIONS RESOURCES CFLAGS CXXFLAGS FFLAGS CONDITION ENVIRONMENT )

    cmake_parse_arguments( _PAR "${options}" "${single_value_args}" "${multi_value_args}"  ${_FIRST_ARG} ${ARGN} )

    if(_PAR_UNPARSED_ARGUMENTS)
      message(FATAL_ERROR "Unknown keywords given to ecbuild_add_test(): \"${_PAR_UNPARSED_ARGUMENTS}\"")
    endif()

    # check test type

    if( NOT _PAR_TYPE )  # may be EXE or PYTHON
        set( _PAR_TYPE "EXE" )
    endif()

    if( _PAR_TYPE MATCHES "PYTHON" )
        if( PYTHONINTERP_FOUND )
            set( _PAR_COMMAND ${PYTHON_EXECUTABLE} )
        else()
            message( FATAL_ERROR "Requested a python test but python interpreter not found - PYTHON_EXECUTABLE: [${PYTHON_EXECUTABLE}]" )
        endif()
    endif()

    set( _TEST_DIR ${CMAKE_CURRENT_BINARY_DIR} )

    # further checks

    if( NOT _PAR_TARGET AND NOT _PAR_COMMAND )
        message(FATAL_ERROR "The call to ecbuild_add_test() doesn't specify the TARGET or COMMAND.")
    endif()

    if( _PAR_TYPE MATCHES "SCRIPT" AND NOT _PAR_COMMAND )
        message(FATAL_ERROR "The call to ecbuild_add_test() defines a 'script' but doesn't specify the COMMAND.")
    endif()

    if( DEFINED _PAR_TARGET AND _PAR_TYPE MATCHES "EXE" AND NOT _PAR_SOURCES )
      message(FATAL_ERROR "The call to ecbuild_add_test() defined TARGET but doesn't specify the SOURCES.")
    endif()

    ### conditional build

    if( DEFINED _PAR_CONDITION )
        set(_target_condition_file "${CMAKE_CURRENT_BINARY_DIR}/set_${_PAR_TARGET}_condition.cmake")
        file( WRITE  ${_target_condition_file} "  if( ")
        foreach( term ${_PAR_CONDITION} )
            file( APPEND ${_target_condition_file} " ${term}")
        endforeach()
        file( APPEND ${_target_condition_file} " )\n    set(_${_PAR_TARGET}_condition TRUE)\n  else()\n    set(_${_PAR_TARGET}_condition FALSE)\n  endif()\n")
        include( ${_target_condition_file} )
    else()
        set( _${_PAR_TARGET}_condition TRUE )
    endif()

    if( ENABLE_TESTS AND _${_PAR_TARGET}_condition )

        if( _PAR_TYPE MATCHES "EXE" )

            # TARGET defined so build the test
    
            if( DEFINED _PAR_TARGET )
    
                # add include dirs if defined
                if( DEFINED _PAR_INCLUDES )
                  foreach( path ${_PAR_INCLUDES} ) # skip NOTFOUND
                    if( path )
                      include_directories( ${path} )
                    endif()
                  endforeach()
                endif()
        
                # add persistent layer files
                if( DEFINED _PAR_PERSISTENT )
                    ecbuild_add_persistent( SRC_LIST _PAR_SOURCES FILES  ${_PAR_PERSISTENT} )
                endif()
        
                # add the test target
                
                add_executable( ${_PAR_TARGET} ${_PAR_SOURCES} )
        
                # add extra dependencies
                if( DEFINED _PAR_DEPENDS)
                  add_dependencies( ${_PAR_TARGET} ${_PAR_DEPENDS} )
                endif()
        
                # add resources
                if( DEFINED _PAR_RESOURCES)
                        foreach( rfile ${_PAR_RESOURCES} )
                            add_custom_command( TARGET ${_PAR_TARGET} POST_BUILD
                                COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/${rfile} ${CMAKE_CURRENT_BINARY_DIR} )
                        endforeach()
                endif()
        
                # add the link libraries
                if( DEFINED _PAR_LIBS )
                  list(REMOVE_ITEM _PAR_LIBS debug)
                  list(REMOVE_ITEM _PAR_LIBS optimized)
                  foreach( lib ${_PAR_LIBS} ) # skip NOTFOUND
                    if( lib )
                      target_link_libraries( ${_PAR_TARGET} ${lib} )
                    else()
                      message( WARNING "Lib ${lib} was skipped" )
                    endif()
                  endforeach()
                endif()
        
                # add local flags
                if( DEFINED _PAR_CFLAGS )
                    set_source_files_properties( ${${_PAR_TARGET}_c_srcs}   PROPERTIES COMPILE_FLAGS "${_PAR_CFLAGS}" )
                endif()
                if( DEFINED _PAR_CXXFLAGS )
                    set_source_files_properties( ${${_PAR_TARGET}_cxx_srcs} PROPERTIES COMPILE_FLAGS "${_PAR_CXXFLAGS}" )
                endif()
                if( DEFINED _PAR_FFLAGS )
                    set_source_files_properties( ${${_PAR_TARGET}_f_srcs}   PROPERTIES COMPILE_FLAGS "${_PAR_FFLAGS}" )
                endif()
        
                # add definitions to compilation
                if( DEFINED _PAR_DEFINITIONS )
                    get_property( _target_defs TARGET ${_PAR_TARGET} PROPERTY COMPILE_DEFINITIONS )
                    list( APPEND _target_defs ${_PAR_DEFINITIONS} )
                    set_property( TARGET ${_PAR_TARGET} PROPERTY COMPILE_DEFINITIONS ${_target_defs} )
                endif()
        
                # set build location to local build dir
                # not the project base as defined for libs and execs
                set_property( TARGET ${_PAR_TARGET} PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} )

                # set linker language
                if( DEFINED _PAR_LINKER_LANGUAGE )
                    set_property( TARGET ${_PAR_TARGET} PROPERTY LINKER_LANGUAGE ${_PAR_LINKER_LANGUAGE} )
                endif()
        
                # make sure target is removed before - some problems with AIX
                get_target_property(EXE_FILENAME ${_PAR_TARGET} OUTPUT_NAME)
                add_custom_command(
                      TARGET ${_PAR_TARGET}
                      PRE_BUILD
                      COMMAND ${CMAKE_COMMAND} -E remove ${EXE_FILENAME}
                )

        endif() # EXE
    
      endif() # condition
    
      # define the arguments
      set( TEST_ARGS "" )
      if( DEFINED _PAR_ARGS  )
        list( APPEND TEST_ARGS ${_PAR_ARGS} )
      endif()

      # setup enable flag    
      if( NOT DEFINED _PAR_ENABLED )
          set( _PAR_ENABLED 1 )
      endif()

      # define the test
      if( _PAR_ENABLED )

          if( DEFINED _PAR_COMMAND AND NOT _PAR_TARGET )
              set( _PAR_TARGET ${_PAR_COMMAND} )
          endif()

          if( DEFINED _PAR_COMMAND )
              add_test( ${_PAR_TARGET} ${_PAR_COMMAND} ${TEST_ARGS} ) # run a command as test
          else()
              add_test( ${_PAR_TARGET} ${_PAR_TARGET}  ${TEST_ARGS} ) # run the test that was generated
          endif()


          if( DEFINED _PAR_ENVIRONMENT )
              set_tests_properties( ${_PAR_TARGET} PROPERTIES ENVIRONMENT "${_PAR_ENVIRONMENT}")
          endif()

      endif()
    
      # add to the overall list of tests
      list( APPEND ECBUILD_ALL_TESTS ${_PAR_TARGET} )
      set( ECBUILD_ALL_TESTS ${ECBUILD_ALL_TESTS} CACHE INTERNAL "" )

    endif() # _condition

    # finally mark project files
    ecbuild_declare_project_files( ${_PAR_SOURCES} )

endmacro( ecbuild_add_test )
