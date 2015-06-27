CMAKE_MINIMUM_REQUIRED( VERSION 3.0 )

PROJECT( "Piha" )

SET( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake" )

IF( MSVC )
	INCLUDE( PihaConfigureVisualStudio )
ENDIF()

#
# Piha source files
#
SET( 	BASE_HEADERS
		"include/PihaConfig.h"
		"include/PihaVector3.h"
		"include/PihaTime.h"			#!!!
		"include/PihaMemoryBuffer.h"
		"include/PihaImageFormat.h"
		"include/PihaImage.h"
		"include/PihaDevice.h"
		"include/PihaDeviceProvider.h"
	)
SET( 	BASE_SOURCES
		"src/PihaTime.cpp"
		"src/PihaMemoryBuffer.cpp"
		"src/PihaImageFormat.cpp"
		"src/PihaImageFormat.cpp"
		"src/PihaImage.cpp"
		"src/PihaDevice.cpp"
		"src/PihaDeviceProvider.cpp"
	)
	
SET(	COMPONENTS_HEADERS
		"include/PihaComponent.h"
		"include/PihaAccelerometer.h"
		"include/PihaGyroscope.h"
		"include/PihaMagnetometer.h"
		#"include/PihaTemperatureSensor.h"
		"include/PihaBarometer.h"
		#"include/PihaGPSReceiver.h"
		"include/PihaButton.h"
		"include/PihaAxis.h"
		"include/PihaVibrator.h"
		"include/PihaBattery.h"
		"include/PihaCamera.h"
		"include/PihaPWM.h"
	)
	
SET(	COMPONENTS_SOURCES
		"src/PihaComponent.cpp"		
		"src/PihaAccelerometer.cpp"
		"src/PihaGyroscope.cpp"
		"src/PihaMagnetometer.cpp"
		"src/PihaTemperatureSensor.cpp"
		"src/PihaBarometer.cpp"
		#"src/PihaGPSReceiver.cpp"
		"src/PihaButton.cpp"
		"src/PihaAxis.cpp"
		"src/PihaVibrator.cpp"
		"src/PihaBattery.cpp"
		"src/PihaCamera.cpp"
		"src/PihaPWM.cpp"
	)
	
#
# Platform-specific source files
#
IF( CMAKE_SYSTEM_NAME MATCHES "Windows" )
	SET( EXTRA_HEADERS ${EXTRA_HEADERS} 
		 "include/PihaWindowsDevice.h" )
	SET( EXTRA_SOURCES ${EXTRA_SOURCES}
		"src/PihaWindowsDevice.cpp" )
ELSEIF( CMAKE_SYSTEM_NAME MATCHES "Linux" )
	SET( EXTRA_HEADERS ${EXTRA_HEADERS} 
		 "include/PihaLinuxDevice.h" )
	SET( EXTRA_SOURCES ${EXTRA_SOURCES}
		 "src/PihaLinuxDevice.cpp" )
	SET( EXTRA_LIBS ${EXTRA_LIBS} rt )			# for time functions?
ELSEIF( CMAKE_SYSTEM_NAME MATCHES "Darwin" )
	# ...
ENDIF()
	
	
#
# Piha source configuration
#
CONFIGURE_FILE( "${CMAKE_CURRENT_SOURCE_DIR}/include/PihaConfig.h.in"
				"${CMAKE_CURRENT_SOURCE_DIR}/include/PihaConfig.h" )

SET(	SOURCES
		${BASE_HEADERS} ${BASE_SOURCES} 
		${COMPONENTS_HEADERS} ${COMPONENTS_SOURCES}
		${EXTRA_HEADERS} ${EXTRA_SOURCES}
	)

SOURCE_GROUP("Base" FILES ${BASE_HEADERS}  ${BASE_SOURCES}  )
SOURCE_GROUP("Components" FILES ${COMPONENTS_HEADERS} ${COMPONENTS_SOURCES} )
SOURCE_GROUP("DeviceProviders" FILES ${EXTRA_HEADERS} ${EXTRA_SOURCES} )	

#
# Piha library 
#
IF( CMAKE_SYSTEM_NAME MATCHES "Windows" )
	SET(CMAKE_DEBUG_POSTFIX "d")
ENDIF()

INCLUDE_DIRECTORIES("include")
INCLUDE_DIRECTORIES(${EXTRA_INCLUDE_DIRS})
ADD_LIBRARY( ${PROJECT_NAME} STATIC ${SOURCES} )
TARGET_LINK_LIBRARIES( ${PROJECT_NAME} ${EXTRA_LIBS}  ) 


#
# Samples
#
ADD_SUBDIRECTORY( samples )
 