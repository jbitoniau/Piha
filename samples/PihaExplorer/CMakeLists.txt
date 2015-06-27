CMAKE_MINIMUM_REQUIRED( VERSION 3.0 )

PROJECT( "PihaExplorer" )

IF( MSVC )
	INCLUDE( PihaConfigureVisualStudio )
ENDIF()

SET(	SOURCES
			PihaImageConverter.h
			PihaImageConverter.cpp
			PihaQImageWidget.h
			PihaQImageWidget.cpp
			PihaQDeviceUpdater.h
			PihaQDeviceUpdater.cpp
			PihaQComponentTreeItemEditorFactory.h
			PihaQComponentTreeItemEditorFactory.cpp
			PihaQComponentTreeItem.h
			PihaQComponentTreeItem.cpp
			PihaQDeviceTreeItem.h
			PihaQDeviceTreeItem.cpp
			PihaQDeviceTreeWidget.h
			PihaQDeviceTreeWidget.cpp
			Main.cpp
	)
	
SOURCE_GROUP("" FILES ${SOURCES} )		# Avoid "Header Files" and "Source Files" virtual folders in VisualStudio

INCLUDE( PihaFindQt )
IF( PIHA_QT_FOUND )
	
	INCLUDE_DIRECTORIES( ${PihaLib_SOURCE_DIR} )
	ADD_EXECUTABLE( ${PROJECT_NAME} WIN32 ${SOURCES} )
	TARGET_LINK_LIBRARIES( ${PROJECT_NAME} Piha )
	TARGET_LINK_LIBRARIES( ${PROJECT_NAME} ${PIHA_QT_LINK_LIBRARIES} )

	#
	# Install
	#
	IF( CMAKE_SYSTEM_NAME MATCHES "Windows" )
		INSTALL( TARGETS  ${PROJECT_NAME}
				 CONFIGURATIONS Debug
				 RUNTIME DESTINATION "bin/debug" 
				 LIBRARY DESTINATION "lib"
				 ARCHIVE DESTINATION "lib" )
		INSTALL( TARGETS  ${PROJECT_NAME}
				 CONFIGURATIONS Release
				 RUNTIME DESTINATION "bin/release" 
				 LIBRARY DESTINATION "lib"
				 ARCHIVE DESTINATION "lib" )
		INSTALL( FILES ${PIHA_QT_DEBUG_DLLS} CONFIGURATIONS Debug DESTINATION "bin/debug")
		INSTALL( FILES ${PIHA_QT_DEBUG_PLATFORMS_DLLS} CONFIGURATIONS Debug DESTINATION "bin/debug/platforms")
		INSTALL( FILES ${PIHA_QT_RELEASE_DLLS} CONFIGURATIONS Release DESTINATION "bin/release")
		INSTALL( FILES ${PIHA_QT_RELEASE_PLATFORMS_DLLS} CONFIGURATIONS Release DESTINATION "bin/release/platforms")
	ELSE()
		INSTALL( TARGETS  ${PROJECT_NAME}
				 RUNTIME DESTINATION "bin" 
				 LIBRARY DESTINATION "lib"
				 ARCHIVE DESTINATION "lib" )
	ENDIF()

ELSE()
	MESSAGE("QT not found")
ENDIF()
	


