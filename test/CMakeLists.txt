
AURA_ADD_TEST(backend/device_info.cpp ${AURA_BACKEND_LIBRARIES})
AURA_ADD_TEST(backend/factorize.cpp ${AURA_BACKEND_LIBRARIES})
AURA_ADD_TEST(backend/device_ptr.cpp ${AURA_BACKEND_LIBRARIES})
AURA_ADD_TEST(backend/feed.cpp ${AURA_BACKEND_LIBRARIES})

AURA_ADD_TEST(backend/host_allocator.cpp ${AURA_BACKEND_LIBRARIES})
AURA_ADD_TEST(backend/host_memory.cpp ${AURA_BACKEND_LIBRARIES})
AURA_ADD_TEST(backend/complex.cpp backend/complex.cc 
	${AURA_BACKEND_LIBRARIES})
AURA_ADD_TEST(backend/kernel.cpp backend/kernel.cc 
	${AURA_BACKEND_LIBRARIES})

# TODO this test is broken because the code is broken, remove me
#AURA_ADD_TEST(backend/map.cpp backend/map.cc 
#	${AURA_BACKEND_LIBRARIES})

AURA_ADD_TEST(backend/mark.cpp ${AURA_BACKEND_LIBRARIES})

AURA_ADD_TEST(detail/svec.cpp)

IF(NOT ${AURA_FFT_LIBRARIES} STREQUAL "")
	AURA_ADD_TEST(math/fft.cpp  
		${AURA_BACKEND_LIBRARIES} ${AURA_FFT_LIBRARIES} pthread)
ENDIF()

# this comparison is intentionally written like this because it is a list
IF(NOT "${AURA_FFT_HOST_LIBRARIES}" STREQUAL " ")
	MESSAGE("${AURA_FFT_HOST_LIBRARIES}")
	AURA_ADD_TEST(math/fft_host.cpp pthread 
		${AURA_BACKEND_LIBRARIES} ${AURA_FFT_HOST_LIBRARIES})
ENDIF()

AURA_ADD_TEST(misc/profile.cpp pthread)

# cprofile is a C interface for the profiler, it requires
# a C++ library that exports C functions
ADD_LIBRARY(cprofile ${PROJECT_SOURCE_DIR}/source/misc/cprofile.cpp)
AURA_ADD_TEST(misc/cprofile.c cprofile pthread)
AURA_ADD_TEST(misc/sequence.cpp)
AURA_ADD_TEST(misc/coo.cpp)

AURA_ADD_TEST(device_array.cpp ${AURA_BACKEND_LIBRARIES})
AURA_ADD_TEST(device_buffer.cpp ${AURA_BACKEND_LIBRARIES})
AURA_ADD_TEST(device_lock.cpp ${AURA_BACKEND_LIBRARIES} 
	${Boost_FILESYSTEM_LIBRARY})
AURA_ADD_TEST(device_range.cpp ${AURA_BACKEND_LIBRARIES})

AURA_ADD_TEST(usingdirective.cpp ${AURA_BACKEND_LIBRARIES})

IF(${AURA_BACKEND} STREQUAL CUDA)
	AURA_ADD_TEST(backend/p2p.cpp ${AURA_BACKEND_LIBRARIES})
ENDIF()

AURA_ADD_TEST(inlinefunctions.cpp inlinefunctions2.cpp 
	${AURA_BACKEND_LIBRARIES})

ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/math/)

