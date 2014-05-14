# (C) Copyright 1996-2014 ECMWF.
# 
# This software is licensed under the terms of the Apache Licence Version 2.0
# which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
# In applying this licence, ECMWF does not waive the privileges and immunities 
# granted to it by virtue of its status as an intergovernmental organisation nor
# does it submit to any jurisdiction.

##############################################################################
# macro that runs the given C code and returns its output

macro( ecbuild_check_c_source_return SOURCE )

    set( options )
    set( single_value_args VAR  OUTPUT )
    set( multi_value_args  INCLUDES LIBS DEFINITIONS )

    cmake_parse_arguments( _PAR "${options}" "${single_value_args}" "${multi_value_args}"  ${_FIRST_ARG} ${ARGN} )

    if(_PAR_UNPARSED_ARGUMENTS)
      message(FATAL_ERROR "Unknown keywords given to ecbuild_check_c_source_return(): \"${_PAR_UNPARSED_ARGUMENTS}\"")
    endif()

    if( NOT _PAR_VAR OR NOT _PAR_OUTPUT )
      message(FATAL_ERROR "The call to ecbuild_check_c_source_return() doesn't specify either SOURCE, VAR or OUTPUT")
    endif()


    if( NOT DEFINED ${_PAR_VAR} )

        set(MACRO_CHECK_FUNCTION_DEFINITIONS "-D${_PAR_VAR} ${CMAKE_REQUIRED_FLAGS}")

        set(CHECK_C_SOURCE_COMPILES_ADD_LIBRARIES)
        if( CMAKE_REQUIRED_LIBRARIES )
            list( APPEND __add_libs ${CMAKE_REQUIRED_LIBRARIES} )
        endif()
        if( _PAR_LIBS )
            list( APPEND __add_libs ${_PAR_LIBS} )
        endif()
        if( __add_libs )
            set(CHECK_C_SOURCE_COMPILES_ADD_LIBRARIES "-DLINK_LIBRARIES:STRING=${__add_libs}")
        endif()

        set(CHECK_C_SOURCE_COMPILES_ADD_INCLUDES)
        if( CMAKE_REQUIRED_INCLUDES )
            list( APPEND __add_incs ${CMAKE_REQUIRED_INCLUDES} )
        endif()
        if( _PAR_INCLUDES )
            list( APPEND __add_incs ${_PAR_INCLUDES} )
        endif()
        if( __add_libs )
            set(CHECK_C_SOURCE_COMPILES_ADD_INCLUDES "-DINCLUDE_DIRECTORIES:STRING=${__add_incs}")
        endif()
    
        # write the source file
    
        file( WRITE "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeTmp/test_${_PAR_VAR}.c" "${SOURCE}\n" )

        message( STATUS "Performing Test ${_PAR_VAR}" )
        try_run( ${_PAR_VAR}_EXITCODE ${_PAR_VAR}_COMPILED
          ${CMAKE_BINARY_DIR}
          ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeTmp/test_${_PAR_VAR}.c
          COMPILE_DEFINITIONS ${CMAKE_REQUIRED_DEFINITIONS}
          CMAKE_FLAGS -DCOMPILE_DEFINITIONS:STRING=${MACRO_CHECK_FUNCTION_DEFINITIONS}
          -DCMAKE_SKIP_RPATH:BOOL=${CMAKE_SKIP_RPATH}
          "${CHECK_C_SOURCE_COMPILES_ADD_LIBRARIES}"
          "${CHECK_C_SOURCE_COMPILES_ADD_INCLUDES}"
          COMPILE_OUTPUT_VARIABLE compile_OUTPUT 
          RUN_OUTPUT_VARIABLE     run_OUTPUT )
    
        # if it did not compile make the return value fail code of 1
        if( NOT ${_PAR_VAR}_COMPILED )
          set( ${_PAR_VAR}_EXITCODE 1 )
        endif()
    
        # if the return value was 0 then it worked
        if("${${_PAR_VAR}_EXITCODE}" EQUAL 0)
    
          message(STATUS "Performing Test ${_PAR_VAR} - Success")
          file(APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeOutput.log 
            "Performing C SOURCE FILE Test ${_PAR_VAR} succeded with the following compile output:\n"
            "${compile_OUTPUT}\n" 
            "Performing C SOURCE FILE Run ${_PAR_VAR} succeded with the following run output:\n"
            "${run_OUTPUT}\n" 
            "Return value: ${${_PAR_VAR}}\n"
            "Source file was:\n${SOURCE}\n")

          set( ${_PAR_VAR}     1              CACHE INTERNAL "Test ${_PAR_VAR}")
          set( ${_PAR_OUTPUT} "${run_OUTPUT}" CACHE INTERNAL "Test ${_PAR_VAR} output")
    
        else()
    
          if(CMAKE_CROSSCOMPILING AND "${${_PAR_VAR}_EXITCODE}" MATCHES  "FAILED_TO_RUN")
            set(${_PAR_VAR} "${${_PAR_VAR}_EXITCODE}")
            set(${OUTPUT} "")
          else()
            set(${_PAR_VAR} "" CACHE INTERNAL "Test ${_PAR_VAR}")
            set(${_PAR_OUTPUT} "" CACHE INTERNAL "Test ${_PAR_VAR} output")
          endif()
    
          message(STATUS "Performing Test ${_PAR_VAR} - Failed")
          file(APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeError.log 
            "Performing C SOURCE FILE Test ${_PAR_VAR} failed with the following compile output:\n"
            "${compile_OUTPUT}\n" 
            "Performing C SOURCE FILE Run ${_PAR_VAR} failed with the following run output:\n"
            "${run_OUTPUT}\n" 
            "Return value: ${${_PAR_VAR}_EXITCODE}\n"
            "Source file was:\n${SOURCE}\n")
        endif()
    
    endif()

endmacro()

##############################################################################
# macro that only adds a c flag if compiler supports it

macro( cmake_add_c_flags m_c_flags )

  if( NOT DEFINED N_CFLAG )
    set( N_CFLAG 0 )
  endif()

  math( EXPR N_CFLAG '${N_CFLAG}+1' )

  check_c_compiler_flag( ${m_c_flags} C_FLAG_TEST_${N_CFLAG} )

  if( C_FLAG_TEST_${N_CFLAG} )
    set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${m_c_flags}" )
	# message( STATUS "C FLAG [${m_c_flags}] added" )
  else()
	message( WARNING "Unrecognised C flag [${m_c_flags}] -- skipping" )
  endif()

endmacro()

