CMAKE_MINIMUM_REQUIRED( VERSION 3.0 )

# The code below defines the following variables:
# PIHA_QT_FOUND: whether Qt was found (Qt4 or 5 depending on the user choice)
# PIHA_QT_LINK_LIBRARIES: the imported Qt targets to link against
# PIHA_QT_DEBUG_DLLS and PIHA_QT_RELEASE_DLLS: on Windows platform, the DLLs to copy next to the binary to get a self-contained release

OPTION( PIHA_USE_QT5 "Use Qt5 instead of Qt4" ON)

SET( CMAKE_INCLUDE_CURRENT_DIR ON )
SET( CMAKE_AUTOMOC ON )
SET( PIHA_QT_FOUND OFF )

IF( PIHA_USE_QT5 )
	
	FIND_PACKAGE( Qt5Widgets )
	IF( Qt5Widgets_FOUND )

		SET( PIHA_QT_FOUND ON )
		SET( PIHA_QT_LINK_LIBRARIES Qt5::Widgets ) 
		
		# Configure deployment/install on Windows
		IF( CMAKE_SYSTEM_NAME MATCHES "Windows" )
	
			# This is very hacky. We find the location of QtCore and assume a particular directory structure.
			# We get two directories up and consider it as the root folder for Qt
			# From there, we assume that the DLLs we need are in the bin and platform sub-directories
			GET_TARGET_PROPERTY( PIHA_QT_CORE_DEBUG_DLL Qt5::Core LOCATION_DEBUG )
			SET( PIHA_QT_ROOT_DIR "${PIHA_QT_CORE_DEBUG_DLL}/../.." )
			GET_FILENAME_COMPONENT( PIHA_QT_ROOT_DIR ${PIHA_QT_ROOT_DIR} ABSOLUTE)

			FILE( GLOB ICU_DLLS ${PIHA_QT_ROOT_DIR}/bin/icu*.dll )
			SET( PIHA_QT_DEBUG_DLLS 
				${ICU_DLLS}
				${PIHA_QT_ROOT_DIR}/bin/Qt5Cored.dll 
				${PIHA_QT_ROOT_DIR}/bin/Qt5Guid.dll
				${PIHA_QT_ROOT_DIR}/bin/Qt5Widgetsd.dll )
			SET( PIHA_QT_DEBUG_PLATFORMS_DLLS
				${PIHA_QT_ROOT_DIR}/plugins/platforms/qwindowsd.dll ) 

			SET( PIHA_QT_RELEASE_DLLS 
				${ICU_DLLS}
				${PIHA_QT_ROOT_DIR}/bin/Qt5Core.dll 
				${PIHA_QT_ROOT_DIR}/bin/Qt5Gui.dll
				${PIHA_QT_ROOT_DIR}/bin/Qt5Widgets.dll )
			SET( PIHA_QT_RELEASE_PLATFORMS_DLLS
				${PIHA_QT_ROOT_DIR}/plugins/platforms/qwindows.dll ) 
		ENDIF()
	ENDIF()

ELSE()
	
	FIND_PACKAGE( Qt4 COMPONENTS QtCore QtGui )
	IF( QT4_FOUND )
		SET( PIHA_QT_FOUND ON )
		SET( PIHA_QT_LINK_LIBRARIES Qt4::QtCore Qt4::QtGui ) 
		
		# Configure deployment/install on Windows
		IF( CMAKE_SYSTEM_NAME MATCHES "Windows" )
			SET( PIHA_QT_DEBUG_DLLS ${QT_BINARY_DIR}/QtCored4.dll ${QT_BINARY_DIR}/QtGuid4.dll )
			SET( PIHA_QT_RELEASE_DLLS ${QT_BINARY_DIR}/QtCore4.dll ${QT_BINARY_DIR}/QtGui4.dll )
		ENDIF()
	ENDIF()

ENDIF()