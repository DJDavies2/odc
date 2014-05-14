# (C) Copyright 1996-2014 ECMWF.
#
# This software is licensed under the terms of the Apache Licence Version 2.0
# which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
# In applying this licence, ECMWF does not waive the privileges and immunities
# granted to it by virtue of its status as an intergovernmental organisation nor
# does it submit to any jurisdiction.

############################################################################################
# macro to check for cxx11 features
# uses macros from the project github.com/UCL/GreatCMakeCookOff

function( ecbuild_check_cxx11 )

	# parse parameters

	set( options PRINT )
	set( single_value_args FEATURES REQUIRED )
	set( multi_value_args  )

	cmake_parse_arguments( _p "${options}" "${single_value_args}" "${multi_value_args}"  ${_FIRST_ARG} ${ARGN} )

	if(_PAR_UNPARSED_ARGUMENTS)
		message(FATAL_ERROR "Unknown keywords given to ecbuild_check_cxx11(): \"${_PAR_UNPARSED_ARGUMENTS}\"")
	endif()

	include( ${ECBUILD_MACROS_DIR}/contrib/GreatCMakeCookOff/CheckCXX11Features.cmake )

	cxx11_find_all_features( ALL_FEATURES ) # list all available features to check

	if( NOT _p_FEATURES AND NOT _p_REQUIRED ) # no input, then searhc for all features

		cxx11_feature_check()

	else()

		foreach( _f ${_p_FEATURES} )
			cxx11_feature_check( ${_f} )
		endforeach()

		foreach( _f ${_p_REQUIRED} )
			cxx11_feature_check( REQUIRED ${_f} )
		endforeach()

	endif()

	foreach( f ${ALL_FEATURES} )
		# message( "HAS_CXX11_${FEAT}" )
		string( TOUPPER ${f} FEAT )
		if( HAS_CXX11_${FEAT} )
		   list( APPEND CXX11_SUPPORTED_FEATURES ${f} )
		else()
		   list( APPEND CXX11_NOT_SUPPORTED_FEATURES ${f} )
		endif()
	endforeach()

	set( CXX11_SUPPORTED_FEATURES     ${CXX11_SUPPORTED_FEATURES}     PARENT_SCOPE )
	set( CXX11_NOT_SUPPORTED_FEATURES ${CXX11_NOT_SUPPORTED_FEATURES} PARENT_SCOPE )

	if( _p_PRINT )
		if( CXX11_SUPPORTED_FEATURES )
			join( CXX11_SUPPORTED_FEATURES " " CXX11_SUPPORTED_FEATURES_STR )
			message( STATUS "Found C++11 features: ${CXX11_SUPPORTED_FEATURES_STR}" )
		endif()
	endif()

endfunction( ecbuild_check_cxx11 )
