# AURA_ADD_TARGET
# creates an executable target with name TARGET_NAME from a number
# of source files and a number of libraries
# the first source file name has to be specified right after the library
# additional source files and libraries can be specified after the target name
FUNCTION(AURA_ADD_TARGET TARGET_NAME FIRST_SOURCE_FILENAME)
  SET(EXECUTABLE_SOURCE_FILES "${FIRST_SOURCE_FILENAME}")
  SET(EXECUTABLE_LINK_LIBRARIES "")
  FOREACH(ARG ${ARGN})
    # check if source file or library
    IF(${ARG} MATCHES "(^[a-z_]*.cpp$)|^[a-z_]*.cu$|^[a-z_]*.cl$")
      SET(EXECUTABLE_SOURCE_FILES ${ARG} ${EXECUTABLE_SOURCE_FILES})
    ELSE()
      SET(EXECUTABLE_LINK_LIBRARIES ${ARG} ${EXECUTABLE_LINK_LIBRARIES})
    ENDIF()
  ENDFOREACH()
  #MESSAGE("source files ${EXECUTABLE_SOURCE_FILES}")
  #MESSAGE("link files ${EXECUTABLE_LINK_LIBRARIES}")
  ADD_EXECUTABLE(${TARGET_NAME} ${EXECUTABLE_SOURCE_FILES})
  TARGET_LINK_LIBRARIES(${TARGET_NAME} ${EXECUTABLE_LINK_LIBRARIES})
ENDFUNCTION()

# AURA_ADD_TEST
# creates a unit test, the name of the test ist derived from the
# folder and name of the first source file
FUNCTION(AURA_ADD_TEST FIRST_SOURCE_FILENAME)
  GET_FILENAME_COMPONENT(TARGET_NAME1 ${FIRST_SOURCE_FILENAME} PATH)
  GET_FILENAME_COMPONENT(TARGET_NAME2 ${FIRST_SOURCE_FILENAME} NAME_WE)
  SET(TARGET_NAME "test.${TARGET_NAME1}.${TARGET_NAME2}")
  SET(TEST_NAME "${TARGET_NAME1}.${TARGET_NAME2}")
  #MESSAGE("${TARGET_NAME} ${TEST_NAME}")
  AURA_ADD_TARGET(${TARGET_NAME} ${FIRST_SOURCE_FILENAME} ${ARGN})
  TARGET_LINK_LIBRARIES(${TARGET_NAME} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
  ADD_TEST(${TEST_NAME} ${TARGET_NAME})
ENDFUNCTION()

