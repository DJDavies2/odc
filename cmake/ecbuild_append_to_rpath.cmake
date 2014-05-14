# (C) Copyright 1996-2014 ECMWF.
#
# This software is licensed under the terms of the Apache Licence Version 2.0
# which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
# In applying this licence, ECMWF does not waive the privileges and immunities
# granted to it by virtue of its status as an intergovernmental organisation nor
# does it submit to any jurisdiction.

############################################################################################
# macro to append paths to rpath

# if dir is absolute, it simply appends
# if dir is relative,
#    then it will try to make it relative to the executables
#    else it will fallback to making it absolute by prepending the install path

function( _path_append var path )
	if( "${${var}}" STREQUAL "" )
		set( ${var} "${path}" PARENT_SCOPE )
	else()
		set( ${var} "${${var}}:${path}" PARENT_SCOPE )
	endif()
endfunction()

macro( ecbuild_append_to_rpath RPATH_DIRS )
   
   if( NOT ${ARGC} EQUAL 1 )
	   message( SEND_ERROR "ecbuild_append_to_rpath takes 1 argument")
   endif()

   foreach( RPATH_DIR ${RPATH_DIRS} )
     
		if( NOT ${RPATH_DIR} STREQUAL "" )
        
			file( TO_CMAKE_PATH ${RPATH_DIR} RPATH_DIR ) # sanitize the path

			if( IS_ABSOLUTE ${RPATH_DIR} )

				_path_append( CMAKE_INSTALL_RPATH "${RPATH_DIR}" )

			else()

				set( _done 0 )

				if( EC_OS_NAME STREQUAL "macosx" )
					_path_append( CMAKE_INSTALL_RPATH "@loader_path/../${RPATH_DIR}" )
					set( _done 1 )
				endif()

				if( EC_OS_NAME STREQUAL "linux" )
					_path_append( CMAKE_INSTALL_RPATH "$ORIGIN/../${RPATH_DIR}" )
					set( _done 1 )
				endif()

				if( EC_OS_NAME STREQUAL "solaris" )
					_path_append( CMAKE_INSTALL_RPATH "$ORIGIN/../${RPATH_DIR}" )
					set( _done 1 )
				endif()

				# fallback

				if( NOT _done )
					_path_append( CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${RPATH_DIR}" )
				endif()

			endif()

     endif()

   endforeach()

endmacro( ecbuild_append_to_rpath )
