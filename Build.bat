@echo off

mkdir _build_
cd _build_
cmake .. -G"Visual Studio 10 2010" -DCMAKE_PREFIX_PATH="%~dp0\dependencies\RapaXInput" -DPIHA_USE_QT5=FALSE -DCMAKE_INSTALL_PREFIX="%~dp0"
cd ..
