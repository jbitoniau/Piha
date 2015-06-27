#pragma once

#include "PihaComponent.h"

namespace Piha
{

class Button : public Component
{
public:
	Button( const std::string& name );
	
	bool	isPressed() const			{ return mIsPressed; }
	void	setPressed( bool pressed );
	
	virtual std::string	toString() const;

protected:
	bool	mIsPressed;
};

}