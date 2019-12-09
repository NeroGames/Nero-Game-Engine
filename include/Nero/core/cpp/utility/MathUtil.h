////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 SANOU A. K. Landry
////////////////////////////////////////////////////////////
#ifndef MATHUTIL_H
#define MATHUTIL_H
///////////////////////////HEADERS//////////////////////////
////////////////////////////////////////////////////////////

#endif // MATHUTIL_H

namespace nero
{
	const int RAND_LIMIT	                    = 32767;
	const float PI                              = 3.141592653589793238462643383f;

	int                 randomInt(int exclusiveMax);
	float               randomFloat();                        // Random number in range [-1,1]
	float               randomFloat(float lo, float hi);      // Random floating point number in range [lo, hi]
}
