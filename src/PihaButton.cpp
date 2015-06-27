#include "PihaButton.h"

#include <sstream>

namespace Piha
{

Button::Button( const std::string& name )
	: Component(kButton, name),
	  mIsPressed(false)
{
}

void Button::setPressed( bool pressed )
{
	if ( mIsPressed==pressed )
		return;
	mIsPressed = pressed;
	notifyComponentChanged();
}
	
std::string	Button::toString() const
{
	std::stringstream stream;
	stream << Component::toString();
	if ( isPressed() )
		stream << " pressed";
	else
		stream << " not pressed";
	return stream.str();
}

}

