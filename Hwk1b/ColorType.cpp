#include "ColorType.h"

ColorType ColorType::multiplyColor(double scalar)
{
	//Multiply a color by a scalar
	ColorType results;	
	results.r = r * scalar;
	results.g = g * scalar;
	results.b = b * scalar;	
	return results;
}

ColorType ColorType::addColor(ColorType otherColor)
{
	//add another color and this one.
	ColorType results;	
	results.r = r + otherColor.r;
	results.g = g + otherColor.g;
	results.b = b + otherColor.b;
	return results;
}
