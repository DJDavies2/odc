# (C) Copyright 1996-2014 ECMWF.
#
# This software is licensed under the terms of the Apache Licence Version 2.0
# which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
# In applying this licence, ECMWF does not waive the privileges and immunities
# granted to it by virtue of its status as an intergovernmental organisation nor
# does it submit to any jurisdiction.

############################################################################################
# check size of pointer

if( NOT DEFINED CMAKE_SIZEOF_VOID_P)
	message( FATAL_ERROR "CMake could not check sizeof void* to infer addressing mode of the OS -- try to upgrade to a more recent CMake" )
endif()

math( EXPR EC_OS_BITS "${CMAKE_SIZEOF_VOID_P} * 8" )

# we only support 32 and 64 bit operating systems
if( NOT EC_OS_BITS EQUAL "32" AND NOT EC_OS_BITS EQUAL "64" )
	message( FATAL_ERROR "operating system ${CMAKE_SYSTEM} ${EC_OS_BITS} bits -- ecbuild only supports 32 or 64 bit OS's" )
endif()

############################################################################################
# For 64 bit architectures enable PIC (position-independent code)

if( ${EC_OS_BITS} EQUAL 64 )
	set( CMAKE_POSITION_INDEPENDENT_CODE ON )
endif()

############################################################################################
# check architecture architecture

if( ENABLE_OS_TYPES_TEST )

	set( EC_SIZEOF_PTR ${CMAKE_SIZEOF_VOID_P} )

	check_type_size( char           EC_SIZEOF_CHAR        )
	check_type_size( short          EC_SIZEOF_SHORT       )
	check_type_size( int            EC_SIZEOF_INT         )
	check_type_size( long           EC_SIZEOF_LONG        )
	check_type_size( "long long"    EC_SIZEOF_LONG_LONG   )
	check_type_size( float          EC_SIZEOF_FLOAT       )
	check_type_size( double         EC_SIZEOF_DOUBLE      )
	check_type_size( "long double"  EC_SIZEOF_LONG_DOUBLE )
	check_type_size( size_t         EC_SIZEOF_SIZE_T      )
	check_type_size( ssize_t        EC_SIZEOF_SSIZE_T     )
	check_type_size( off_t          EC_SIZEOF_OFF_T       )

	message( STATUS "system sizeof:" )
	message( STATUS "  void*  [${EC_SIZEOF_PTR}]  size_t [${EC_SIZEOF_SIZE_T}]  off_t  [${EC_SIZEOF_OFF_T}]   short  [${EC_SIZEOF_SHORT}]" )
	message( STATUS "  int    [${EC_SIZEOF_INT}]  long   [${EC_SIZEOF_LONG}]  long long   [${EC_SIZEOF_LONG_LONG}]" )
	message( STATUS "  float  [${EC_SIZEOF_FLOAT}]  double [${EC_SIZEOF_DOUBLE}]  long double [${EC_SIZEOF_LONG_DOUBLE}]" )

endif()

############################################################################################
# check for large file support

# ensure we use 64bit access to files even on 32bit os -- aka Large File Support
# by making off_t 64bit and stat behave as stat64

if( ENABLE_LARGE_FILE_SUPPORT )

	check_type_size( off_t EC_SIZEOF_OFF_T )

	if( EC_SIZEOF_OFF_T LESS "8" )

	if( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" OR ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )
		add_definitions( -D_FILE_OFFSET_BITS=64 )
	endif()

	if( ${CMAKE_SYSTEM_NAME} MATCHES "AIX" )
		add_definitions( -D_LARGE_FILES=64 )
	endif()

	get_directory_property( __compile_defs COMPILE_DEFINITIONS )

	if( __compile_defs )
		foreach( def ${__compile_defs} )
			list( APPEND CMAKE_REQUIRED_DEFINITIONS -D${def} )
		endforeach()
	endif()

#	ecbuild_check_c_source_return( "#include <stdio.h>\n#include <sys/types.h>\nint main(){printf(\"%ld\",sizeof(off_t));return 0;}"
#		VAR  check_off_t_final
#		OUTPUT __sizeof_off_t_final )
#	if( NOT check_off_t_final OR __sizeof_off_t_final LESS "8" )
#		message( FATAL_ERROR "operating system ${CMAKE_SYSTEM} ${EC_OS_BITS} bits -- sizeof off_t [${__sizeof_off_t_final}]" )
#	endif()

	endif()

endif()

############################################################################################
# check endiness

if( ENABLE_OS_ENDINESS_TEST )

	test_big_endian( _BIG_ENDIAN )

	if( _BIG_ENDIAN )
		set( EC_BIG_ENDIAN    1 )
	else()
		set( EC_LITTLE_ENDIAN 1 )
	endif()

	check_c_source_runs(
		 "int compare(unsigned char* a,unsigned char* b) {
		   while(*a != 0) if (*(b++)!=*(a++)) return 1;
		   return 0;
		 }
		 int main(int argc,char** argv) {
		   unsigned char dc[]={0x30,0x61,0xDE,0x80,0x93,0x67,0xCC,0xD9,0};
		   double da=1.23456789e-75;
		   unsigned char* ca;

		   unsigned char fc[]={0x05,0x83,0x48,0x22,0};
		   float fa=1.23456789e-35;

		   if (sizeof(double)!=8) return 1;

		   ca=(unsigned char*)&da;
		   if (compare(dc,ca)) return 1;

		   if (sizeof(float)!=4) return 1;

		   ca=(unsigned char*)&fa;
		   if (compare(fc,ca)) return 1;

		   return 0;
		 }" IEEE_BE )

	if( "${IEEE_BE}" STREQUAL "" )
		set( IEEE_BE 0 CACHE INTERNAL "Test IEEE_BE")
	endif()

	check_c_source_runs(
		 "int compare(unsigned char* a,unsigned char* b) {
		   while(*a != 0) if (*(b++)!=*(a++)) return 1;
		   return 0;
		 }
		 int main(int argc,char** argv) {
		   unsigned char dc[]={0xD9,0xCC,0x67,0x93,0x80,0xDE,0x61,0x30,0};
		   double da=1.23456789e-75;
		   unsigned char* ca;

		   unsigned char fc[]={0x22,0x48,0x83,0x05,0};
		   float fa=1.23456789e-35;

		   if (sizeof(double)!=8) return 1;

		   ca=(unsigned char*)&da;
		   if (compare(dc,ca)) return 1;

		   if (sizeof(float)!=4) return 1;

		   ca=(unsigned char*)&fa;
		   if (compare(fc,ca)) return 1;

		   return 0;
		 }" IEEE_LE )

	if( "${IEEE_BE}" STREQUAL "" )
		set( IEEE_LE 0 CACHE INTERNAL "Test IEEE_LE")
	endif()

	if( EC_BIG_ENDIAN )
		message( STATUS "system is Big Endian [${EC_BIG_ENDIAN}] IEEE BE [${IEEE_BE}]" )
	endif()
	if( EC_LITTLE_ENDIAN )
		message( STATUS "system is Little Endian [${EC_LITTLE_ENDIAN}] IEEE LE [${IEEE_LE}]" )
	endif()

endif()

############################################################################################
# check operating system

set( EC_OS_NAME "UNKNOWN" )

### Unix's -- Proper operating systems

if( UNIX )

	### APPLE ###

	if( APPLE AND ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )
		set( EC_OS_NAME "macosx" )
	endif()

	### Linux ###

	if( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )

		set( EC_OS_NAME "linux" )

		# recent linkers default to --enable-new-dtags
		# which then adds both RPATH and RUNPATH to executables
		# thus invalidating RPATH setting, and making LD_LIBRARY_PATH take precedence
		# to be sure, use tool 'readelf -a <exe> | grep PATH' to see what paths are built-in
		# see:
		#  * http://blog.qt.digia.com/blog/2011/10/28/rpath-and-runpath
		#  * http://www.cmake.org/Wiki/CMake_RPATH_handling
		#  * man ld
		#  * http://blog.tremily.us/posts/rpath
		#  * http://fwarmerdam.blogspot.co.uk/2010/12/rpath-runpath-and-ldlibrarypath.html
		set(CMAKE_EXE_LINKER_FLAGS     "${CMAKE_EXE_LINKER_FLAGS}    -Wl,--disable-new-dtags")
		set(CMAKE_SHARED_LINKER_FLAGS  "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--disable-new-dtags")
		set(CMAKE_MODULE_LINKER_FLAGS  "${CMAKE_MODULE_LINKER_FLAGS} -Wl,--disable-new-dtags")

	endif()

	### Solaris ###

	if( ${CMAKE_SYSTEM_NAME} MATCHES "SunOS" )
		set( EC_OS_NAME "solaris" )
	endif()

	### AIX ###

	if( ${CMAKE_SYSTEM_NAME} MATCHES "AIX" )

		set( EC_OS_NAME "aix" )

		set( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -bbigtoc" )

		if( CMAKE_C_COMPILER_ID MATCHES "GNU" )
			set( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Xlinker" )
		endif()

		if( CMAKE_COMPILER_IS_GNUCC )
			if( EC_OS_BITS EQUAL "64" )
				cmake_add_c_flags("-maix64")
			endif()
			if( EC_OS_BITS EQUAL "32" )
				cmake_add_c_flags("-maix32")
			endif()
		endif()

		if( CMAKE_COMPILER_IS_GNUCXX )
			if( EC_OS_BITS EQUAL "64" )
				cmake_add_cxx_flags("-maix64")
			endif()
			if( EC_OS_BITS EQUAL "32" )
				cmake_add_cxx_flags("-maix32")
			endif()
		endif()

		if( CMAKE_C_COMPILER_ID MATCHES "XL" )

			cmake_add_c_flags("-qpic=large")
#            cmake_add_c_flags("-qweaksymbol")

			if(EC_OS_BITS EQUAL "32" )
				cmake_add_c_flags("-q32")
			endif()

			if(${CMAKE_BUILD_TYPE} MATCHES "Release" OR ${CMAKE_BUILD_TYPE} MATCHES "Production" )
					cmake_add_c_flags("-qstrict")
					cmake_add_c_flags("-qinline")
			endif()

			if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
					cmake_add_c_flags("-qfullpath")
					cmake_add_c_flags("-qkeepparm")
			endif()

		endif()

		if( CMAKE_CXX_COMPILER_ID MATCHES "XL" )

			cmake_add_cxx_flags("-qpic=large")
			cmake_add_cxx_flags("-bmaxdata:0x40000000")
			cmake_add_cxx_flags("-qrtti")
			cmake_add_cxx_flags("-qfuncsect")

#           cmake_add_cxx_flags("-qweaksymbol")

			if(EC_OS_BITS EQUAL "32" )
				cmake_add_cxx_flags("-q32")
			endif()

			if(${CMAKE_BUILD_TYPE} MATCHES "Release" OR ${CMAKE_BUILD_TYPE} MATCHES "Production" )
					cmake_add_cxx_flags("-qstrict")
					cmake_add_cxx_flags("-qinline")
			endif()

			if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
					cmake_add_cxx_flags("-qfullpath")
					cmake_add_cxx_flags("-qkeepparm")
			endif()

		endif()

		if( CMAKE_Fortran_COMPILER_ID MATCHES "XL" )

			cmake_add_fortran_flags("-qxflag=dealloc_cfptr")
			cmake_add_fortran_flags("-qextname")
			cmake_add_fortran_flags("-qdpc=e")
			cmake_add_fortran_flags("-bmaxdata:0x40000000")
			cmake_add_fortran_flags("-bloadmap:loadmap -bmap:loadmap")

			if(EC_OS_BITS EQUAL "32" )
				cmake_add_fortran_flags("-q32")
			endif()
		endif()

	endif()

endif()

### Windows -- are you sure?

if( WIN32 )

	### Cygwin

	if( CYGWIN )

		set( EC_OS_NAME "cygwin" )
		message( WARNING "Building on Cygwin should work but is untested -- proceed at your own risk" )

	else()

		message( FATAL_ERROR "ecBuild can only build on Windows using Cygwin" )

	endif()

endif()

### final warning / error

if( ${EC_OS_NAME} MATCHES "UNKNOWN" )
	if( DISABLE_OS_CHECK )
		message( WARNING "ecBuild is untested for this operating system: [${CMAKE_SYSTEM_NAME}]" )
	else()
		message( FATAL_ERROR "ecBuild is untested for this operating system: [${CMAKE_SYSTEM_NAME}]" )
	endif()
endif()


