#include "ColorType.h"

ColorType ColorType::multiplyColor(double scalar)
{
	//Multiply a color by a scalar
	ColorType results;	
	results.r = r * scalar;
	results.g = g * scalar;
	results.b = b * scalar;	
	return results.clamp();
}

ColorType ColorType::addColor(ColorType otherColor)
{
	//add another color and this one.
	//Does this make sense?
	ColorType results;	
	results.r = r + otherColor.r;
	results.g = g + otherColor.g;
	results.b = b + otherColor.b;
	return results.clamp();
}

ColorType ColorType::clamp()
{
	ColorType results;	
	results.r = (r>1)?1:(r<0)?0:r;
	results.g = (g>1)?1:(g<0)?0:g;
	results.b = (b>1)?1:(b<0)?0:b;
	return results;

}
