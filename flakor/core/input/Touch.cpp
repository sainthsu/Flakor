/****************************************************************************
 Copyright (c) 2013-2014 Saint Hsu
 
 http://www.flakor.org
 ****************************************************************************/

#include "core/input/Touch.h"
//#include "Game.h"

FLAKOR_NS_BEGIN

// returns the current touch location in screen coordinates
Point Touch::getLocationInView() const 
{ 
    return _point; 
}

// returns the previous touch location in screen coordinates
Point Touch::getPreviousLocationInView() const 
{ 
    return _prevPoint; 
}

// returns the start touch location in screen coordinates
Point Touch::getStartLocationInView() const 
{ 
    return _startPoint; 
}

// returns the current touch location in OpenGL coordinates
Point Touch::getLocation() const
{ 
    return _point;//Game::thisGame()->convertToGL(_point); 
}

// returns the previous touch location in OpenGL coordinates
Point Touch::getPreviousLocation() const
{ 
    return _point;//Game::thisGame()->convertToGL(_prevPoint);  
}

// returns the start touch location in OpenGL coordinates
Point Touch::getStartLocation() const
{ 
    return _point;//Game::thisGame()->convertToGL(_startPoint);  
}

// returns the delta position between the current location and the previous location in OpenGL coordinates
Point Touch::getDelta() const
{     
    return getLocation() - getPreviousLocation();
}

FLAKOR_NS_END
