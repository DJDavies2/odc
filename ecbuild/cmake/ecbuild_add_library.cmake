# (C) Copyright 1996-2012 ECMWF.
#
# This software is licensed under the terms of the Apache Licence Version 2.0
# which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
# In applying this licence, ECMWF does not waive the privileges and immunities
# granted to it by virtue of its status as an intergovernmental organisation nor
# does it submit to any jurisdiction.

##############################################################################
# macro for adding a library
##############################################################################

macro( ecbuild_add_library )

    set( options TEST )
    set( single_value_args TARGET TYPE COMPONENT INSTALL_HEADERS LINKER_LANGUAGE )
    set( multi_value_args  SOURCES TEMPLATES LIBS INCLUDES DEPENDS PERSISTENT DEFINITIONS CFLAGS CXXFLAGS FFLAGS CONDITION )

    cmake_parse_arguments( _PAR "${options}" "${single_value_args}" "${multi_value_args}"  ${_FIRST_ARG} ${ARGN} )

    if(_PAR_UNPARSED_ARGUMENTS)
      message(FATAL_ERROR "Unknown keywords given to ecbuild_add_library(): \"${_PAR_UNPARSED_ARGUMENTS}\"")
    endif()

    if( NOT _PAR_TARGET  )
      message(FATAL_ERROR "The call to ecbuild_add_library() doesn't specify the TARGET.")
    endif()

    if( NOT _PAR_SOURCES )
      message(FATAL_ERROR "The call to ecbuild_add_library() doesn't specify the SOURCES.")
    endif()

    get_filename_component( currdir ${CMAKE_CURRENT_SOURCE_DIR} NAME )

#    debug_var(currdir)

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

    if( _${_PAR_TARGET}_condition )

        # defines the type of library
        if( DEFINED _PAR_TYPE )
            # checks that is either SHARED or STATIC or MODULE
            if( NOT _PAR_TYPE MATCHES "STATIC" AND
                NOT _PAR_TYPE MATCHES "SHARED" AND
                NOT _PAR_TYPE MATCHES "MODULE" )
                message( FATAL_ERROR "library type must be one of [ STATIC | SHARED | MODULE ]" )
            endif()
    #    else() # default is shared unless -DENABLE_STATIC_LIBS=ON
    #        if( NOT ENABLE_STATIC_LIBS )
    #            set( _PAR_TYPE SHARED )
    #        else()
    #            set( _PAR_TYPE STATIC )
    #        endif()
        endif()
    
        # add include dirs if defined
        if( DEFINED _PAR_INCLUDES )
          foreach( path ${_PAR_INCLUDES} ) # skip NOTFOUND
            if( path )
              include_directories( ${path} )
    #        else()
    #          message( WARNING "Path ${path} was skipped" )
            endif()
          endforeach()
        endif()
    
        # add persistent layer files
        if( DEFINED _PAR_PERSISTENT )
            ecbuild_add_persistent( SRC_LIST _PAR_SOURCES FILES ${_PAR_PERSISTENT} )
        endif()
    
        # add templates to project files and remove from compilation sources
        if( DEFINED _PAR_TEMPLATES )
            list( REMOVE_ITEM _PAR_SOURCES ${_PAR_TEMPLATES} )
            ecbuild_declare_project_files( ${_PAR_TEMPLATES} )
            add_custom_target( ${_PAR_TARGET}_templates SOURCES ${_PAR_TEMPLATES} )
        endif()
    
        # add the library target
        add_library( ${_PAR_TARGET} ${_PAR_TYPE} ${_PAR_SOURCES} )
    
        # add extra dependencies
        if( DEFINED _PAR_DEPENDS)
          add_dependencies( ${_PAR_TARGET} ${_PAR_DEPENDS} )
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

        # installation

        ecbuild_separate_sources( TARGET ${_PAR_TARGET} SOURCES ${_PAR_SOURCES} )

#    debug_var( ${_PAR_TARGET}_h_srcs )
#    debug_var( ${_PAR_TARGET}_c_srcs )
#    debug_var( ${_PAR_TARGET}_cxx_srcs )
#    debug_var( ${_PAR_TARGET}_f_srcs )

        if( NOT _PAR_TEST )

            # and associate with defined component
            set( COMPONENT_DIRECTIVE "" )
            if( DEFINED _PAR_COMPONENT )
                set( COMPONENT_DIRECTIVE "${_PAR_COMPONENT}" )
            else()
                set( COMPONENT_DIRECTIVE "${PROJECT_NAME}" )
            endif()
    
            install( TARGETS ${_PAR_TARGET}
                RUNTIME DESTINATION bin
                LIBRARY DESTINATION lib
                ARCHIVE DESTINATION lib
                COMPONENT ${COMPONENT_DIRECTIVE} )
    
            # install headers
        
            set( _header_destination "include/${PROJECT_NAME}/${currdir}" )
        
            if( DEFINED _PAR_INSTALL_HEADERS )
                if( _PAR_INSTALL_HEADERS MATCHES "LISTED" )
                    install( FILES ${${_PAR_TARGET}_h_srcs}    DESTINATION ${_header_destination} )
                    if( DEFINED _PAR_TEMPLATES )
                        install( FILES ${_PAR_TEMPLATES} DESTINATION ${_header_destination} )
                    endif()
                endif()
                if( _PAR_INSTALL_HEADERS MATCHES "ALL" ) # "(\\.h|\\.b|\\.hxx|\\.hh|\\.hpp|\\.H)" ????
                    install( DIRECTORY ./  DESTINATION ${_header_destination} FILES_MATCHING PATTERN "*.h" )
                endif()
            endif()
        
            if( DEFINED _PAR_INSTALL_HEADERS_LIST )
                install( FILES ${_PAR_INSTALL_HEADERS_LIST} DESTINATION ${_header_destination} )
            endif()
        
            if( DEFINED _PAR_INSTALL_HEADERS_REGEX )
                install( DIRECTORY ./  DESTINATION ${_header_destination} FILES_MATCHING PATTERN "${_PAR_INSTALL_HEADERS_REGEX}")
            endif()
    
            # set build location
            set_property( TARGET ${_PAR_TARGET} PROPERTY LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
            set_property( TARGET ${_PAR_TARGET} PROPERTY ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )

        endif()

        # add definitions to compilation
        if( DEFINED _PAR_DEFINITIONS )
            get_property( _target_defs TARGET ${_PAR_TARGET} PROPERTY COMPILE_DEFINITIONS )
            list( APPEND _target_defs ${_PAR_DEFINITIONS} )
            set_property( TARGET ${_PAR_TARGET} PROPERTY COMPILE_DEFINITIONS ${_target_defs} )
        endif()
        
        # make sure target is removed before - some problems with AIX
        get_target_property(LIB_LOCNAME ${_PAR_TARGET} LOCATION)
        add_custom_command(
              TARGET ${_PAR_TARGET}
              PRE_BUILD
              COMMAND ${CMAKE_COMMAND} -E remove ${LIB_LOCNAME}
       )
    
        # for the links target
        if( NOT _PAR_TEST )
            set(LIB_FILENAME ${CMAKE_SHARED_LIBRARY_PREFIX}${_PAR_TARGET}${CMAKE_SHARED_LIBRARY_SUFFIX}${LIB_SUFFIX})
            ecbuild_link_lib( ${_PAR_TARGET} ${LIB_FILENAME} )
        endif()

        # set linker language
        if( DEFINED _PAR_LINKER_LANGUAGE )
            set_property( TARGET ${_PAR_TARGET} PROPERTY LINKER_LANGUAGE ${_PAR_LINKER_LANGUAGE} )
        endif()

    endif()

    # mark project files
    ecbuild_declare_project_files( ${_PAR_SOURCES} )

endmacro( ecbuild_add_library  )

