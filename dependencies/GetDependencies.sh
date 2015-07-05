#!/bin/sh
DEPENDENCIES="
	https://github.com/jbitoniau/RapaDirectShow
	https://github.com/jbitoniau/RapaDirectInput
	https://github.com/jbitoniau/RapaXInput 
	https://github.com/jbitoniau/RapaLinuxJoystick
	https://github.com/jbitoniau/RapaPhidget
	https://github.com/jbitoniau/RapaPololuMaestro"

for DEPENDENCY in $DEPENDENCIES
do
	git clone $DEPENDENCY
done
