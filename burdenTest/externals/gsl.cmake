SET_PROPERTY(DIRECTORY PROPERTY "EP_BASE" ${ep_base})

SET(GSL_PROJECT gsl_project CACHE INTERNAL "gsl_project name")
SET(GSL_DIR ${CMAKE_BINARY_DIR}/externals/gsl CACHE INTERNAL "gsl project directory")
ExternalProject_Add(${GSL_PROJECT}
	GIT_REPOSITORY https://github.com/ampl/gsl.git
	GIT_TAG master
	CONFIGURE_COMMAND "./configure"
	BUILD_COMMAND "make"
	INSTALL_COMMAND ""
	UPDATE_COMMAND ""
	BUILD_IN_SOURCE 1
	PREFIX ${GSL_DIR}
	CMAKE_CACHE_ARGS
		-DCMAKE_C_COMPILER:STRING=${CMAKE_C_COMPILER}
		-DCMAKE_CXX_COMPILER:STRING=${CMAKE_CXX_COMPILER}


)

ExternalProject_Get_Property(${GSL_PROJECT} SOURCE_DIR)
ExternalProject_Get_Property(${GSL_PROJECT} BINARY_DIR)

MESSAGE("BINARY_DIR: ${BINARY_DIR}")

MESSAGE("SRC_DIR: ${SOURCE_DIR}")


SET(GSL_LIB ${BINARY_DIR}/libgsl.a ${BINARY_DIR}/libgslcblas.a CACHE INTERNAL "GSL Library")
SET(GSL_INCLUDE ${SOURCE_DIR} CACHE INTERNAL "GSL Include")