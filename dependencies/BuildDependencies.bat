@echo off
@rem Run this batch file in a Visual Studio command prompt with CMake in the PATH
@rem Example: 
@rem BuildDependencies.bat -G"Visual Studio 10 2010" -DRAPA_USE_QT5=FALSE -DCMAKE_INSTALL_PREFIX=.. -DXInput_SDK_SEARCH_PATHS="C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)" 

set DEPENDENCIES_LIST= 	^
	RapaXInput 
	
cd %~dp0

for %%A IN (%DEPENDENCIES_LIST%) DO ( 
	if exist %%A (
		echo Building %%A
		cd %%A
		
		if not exist _build_ (
			mkdir _build_
		)
		
		cd _build_
		cmake .. %*
		
		msbuild INSTALL.vcxproj /p:Configuration=Debug
		msbuild INSTALL.vcxproj /p:Configuration=Release
		
		cd ..\..
	) else (
		echo %%A not found
	)
)

cd %~dp0