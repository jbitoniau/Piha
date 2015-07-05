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
# Provider-specific source files
#
IF( CMAKE_SYSTEM_NAME MATCHES "Windows" )
	OPTION( PIHA_DIRECTINPUT_SUPPORT "Windows DirectInput support" ON )
	OPTION( PIHA_XINPUT_SUPPORT "Windows XInput support" ON )
	# ...
ELSEIF( CMAKE_SYSTEM_NAME MATCHES "Linux" )
	OPTION( PIHA_I2CDEVLIB_SUPPORT "Raspberry PI I2CDevLib support" ON )
	OPTION( PIHA_RPIOPWM_SUPPORT "Raspberry PI RPIO PWM support" ON )
ENDIF()
# ...

IF ( PIHA_DIRECTINPUT_SUPPORT )
	FIND_PACKAGE( RapaDirectInput CONFIG REQUIRED )
	IF( RapaDirectInput_FOUND  )
		SET( EXTRA_HEADERS ${EXTRA_HEADERS} "include/PihaProviderDirectInput.h" )
		SET( EXTRA_SOURCES ${EXTRA_SOURCES} "src/PihaProviderDirectInput.cpp" )
		SET( EXTRA_INCLUDE_DIRS ${EXTRA_INCLUDE_DIRS} ${RapaDirectInput_INCLUDE_DIRS} )
		SET( EXTRA_LIBS ${EXTRA_LIBS} Rapa::RapaDirectInput )
		SET( DEPENDENCY_LIB_NAMES ${DEPENDENCY_LIB_NAMES} RapaDirectInput )
		SET( DEPENDENCY_DIRS ${DEPENDENCY_DIRS} ${RapaDirectInput_DIR} )
	ELSE()
		SET( PIHA_DIRECTINPUT_SUPPORT OFF )
	ENDIF()
ENDIF()

IF ( PIHA_XINPUT_SUPPORT )
	FIND_PACKAGE( RapaXInput CONFIG REQUIRED )
	IF( RapaXInput_FOUND  )
		SET( EXTRA_HEADERS ${EXTRA_HEADERS} "include/PihaProviderXInput.h" )
		SET( EXTRA_SOURCES ${EXTRA_SOURCES} "src/PihaProviderXInput.cpp" )
		SET( EXTRA_INCLUDE_DIRS ${EXTRA_INCLUDE_DIRS} ${RapaXInput_INCLUDE_DIRS} ) # No need to add XInput to include dirs, only RapaXInput
		SET( EXTRA_LIBS ${EXTRA_LIBS} Rapa::RapaXInput )
		SET( DEPENDENCY_LIB_NAMES ${DEPENDENCY_LIB_NAMES} RapaXInput )
		SET( DEPENDENCY_DIRS ${DEPENDENCY_DIRS} ${RapaXInput_DIR} )
	ELSE()
		SET( PIHA_XINPUT_SUPPORT OFF )
	ENDIF()
ENDIF()

IF ( PIHA_I2CDEVLIB_SUPPORT )
	ADD_SUBDIRECTORY( dependencies/I2CDevLib )
	SET( EXTRA_HEADERS ${EXTRA_HEADERS} "include/PihaProviderI2CDevLib.h" )
	SET( EXTRA_SOURCES ${EXTRA_SOURCES} "src/PihaProviderI2CDevLib.cpp" )
	SET( EXTRA_INCLUDE_DIRS ${EXTRA_INCLUDE_DIRS} ${I2CDevLib_SOURCE_DIR} ${I2CDevLib_SOURCE_DIR}/I2Cdev )
	SET( EXTRA_LIBS ${EXTRA_LIBS} I2CDevLib )
ENDIF()

IF ( PIHA_RPIOPWM_SUPPORT )
	ADD_SUBDIRECTORY( dependencies/RPIOPWM )
	SET( EXTRA_HEADERS ${EXTRA_HEADERS} "include/PihaProviderRPIOPWM.h" )
	SET( EXTRA_SOURCES ${EXTRA_SOURCES} "src/PihaProviderRPIOPWM.cpp" )
	SET( EXTRA_INCLUDE_DIRS ${EXTRA_INCLUDE_DIRS} ${RPIOPWM_SOURCE_DIR} )
	SET( EXTRA_LIBS ${EXTRA_LIBS} RPIOPWM )
ENDIF()

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
# Piha install
#
#
# Install
#
INSTALL(TARGETS ${PROJECT_NAME} EXPORT PihaInstallTargets
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib
		RUNTIME DESTINATION bin )
		#INCLUDES DESTINATION include )		# If uncommented, the ${PROJECT_NAME} target contains INCLUDE_DIRECTORIES information. Importing the target automatically adds this directory to the INCLUDE_DIRECTORIES.
		
SET( TARGET_NAMESPACE Rapa:: )
INSTALL( FILES ${BASE_HEADERS} ${COMPONENTS_HEADERS} ${EXTRA_HEADERS} DESTINATION include COMPONENT Devel )
EXPORT( EXPORT PihaInstallTargets FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}Targets.cmake"  )
CONFIGURE_FILE( cmake/${PROJECT_NAME}Config.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}Config.cmake" @ONLY )
SET( ConfigPackageLocation lib/cmake/${PROJECT_NAME} )
INSTALL(EXPORT PihaInstallTargets
		FILE ${PROJECT_NAME}Targets.cmake
		NAMESPACE ${TARGET_NAMESPACE}
		DESTINATION ${ConfigPackageLocation} )
INSTALL( FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}Config.cmake" DESTINATION ${ConfigPackageLocation} COMPONENT Devel )


#
# Samples
#
ADD_SUBDIRECTORY( samples )
 