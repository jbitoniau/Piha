#!/bin/sh
if [ ! -d "RapaXInput" ]; then
	echo "Cloning RapaXInput"
	git clone https://github.com/jbitoniau/RapaXInput
else
	echo "RapaXInput already exists"
fi
