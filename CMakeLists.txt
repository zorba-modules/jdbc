# Copyright 2006-2016 The FLWOR Foundation.
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
# http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

MESSAGE(STATUS "")
MESSAGE(STATUS "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++")
MESSAGE(STATUS "Configuring external module <jdbc>")
MESSAGE(STATUS "-------------------------------------------------------------")
MESSAGE(STATUS "")

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT (zorba_jdbc_module)

FIND_PACKAGE (zorba_util-jvm_module QUIET)

INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/config/UtilJavaUse.cmake)

IF (zorba_util-jvm_module_FOUND)
  ZORBA_FIND_JNI()
  FIND_PACKAGE(Java)
  FIND_PACKAGE (Zorba REQUIRED HINTS "${ZORBA_BUILD_DIR}")
  INCLUDE ("${Zorba_USE_FILE}")
  
  IF (JNI_FOUND)
    INCLUDE_DIRECTORIES (${zorba_util-jvm_module_INCLUDE_DIRS})
    INCLUDE_DIRECTORIES ("include")

    # if everything is found build this module
    SET (JDBC_VERSION 1.0)

    ENABLE_TESTING ()
    INCLUDE (CTest)

    SET_CMAKE_MODULE_PATH()

    FIND_PACKAGE (Zorba REQUIRED HINTS "${ZORBA_BUILD_DIR}")
    INCLUDE ("${Zorba_USE_FILE}")

    ADD_SUBDIRECTORY ("src")

    # Attempt to find the MySQL JDBC connector module (for tests).
    IF (NOT DEFINED MYSQL_CONN_PATH)
      IF (WIN32)
        # getting connector from env var CONNECTOR_PATH
        STRING(REGEX REPLACE "\\\\" "/" MYSQL_CONN_PATH "$ENV{CONNECTOR_PATH}")
      ELSE (WIN32)
        SET (MYSQL_CONN_PATH /usr/share/java ${Java_LIBRARIES})
      ENDIF (WIN32)
    ENDIF (NOT DEFINED MYSQL_CONN_PATH)
    ZORBA_FIND_JAR (MYSQL_CONN_JAR NAMES mysql-connector-java mysql
      PATHS ${MYSQL_CONN_PATH})
    IF (MYSQL_CONN_JAR)
      MESSAGE (STATUS "MySQL connector .jar found; adding JDBC module tests.")
      DECLARE_ZORBA_JAR (FILE ${MYSQL_CONN_JAR} EXTERNAL)
      # Only add the tests if we found it, since they depend on MySQL.
      ADD_TEST_DIRECTORY("${PROJECT_SOURCE_DIR}/test")
    ELSE (MYSQL_CONN_JAR)
      MESSAGE (STATUS "MySQL connector .jar not found; skipping tests "
	"for JDBC module.")
    ENDIF (MYSQL_CONN_JAR)

    DONE_DECLARING_ZORBA_URIS ()
    
    MESSAGE(STATUS "")
    MESSAGE(STATUS "-------------------------------------------------------------")
    MESSAGE(STATUS "Configuration SUCCEEDED for external module <jdbc>")

  ELSE (JNI_FOUND)
    MESSAGE (STATUS "Java and/or JNI not found; skipping jdbc module.")
    
    MESSAGE(STATUS "")
    MESSAGE(STATUS "-------------------------------------------------------------")  
    MESSAGE(STATUS "Configuration FAILED for external module <jdbc>")
    
  ENDIF (JNI_FOUND)

ELSE (zorba_util-jvm_module_FOUND)
  MESSAGE (STATUS "Zorba's util-jvm module not found; skipping jdbc module.")
  
  MESSAGE(STATUS "")
  MESSAGE(STATUS "-------------------------------------------------------------")  
  MESSAGE(STATUS "Configuration FAILED for external module <jdbc>")
  
ENDIF (zorba_util-jvm_module_FOUND)

MESSAGE(STATUS "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++")