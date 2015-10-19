#ifndef _ColorType_
#define _ColorType_

class ColorType
{
	public:
	double r, g, b;

	ColorType multiplyColor(double scalar);
	ColorType addColor(ColorType);
	ColorType clamp();
};

#endif