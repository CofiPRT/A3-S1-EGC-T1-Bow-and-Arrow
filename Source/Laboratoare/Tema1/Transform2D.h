#pragma once

#include <include/glm.h>

namespace Transform2D
{
	// ALL OF THESE ARE TRANSPOSED

	// Translate matrix
	inline glm::mat3 Translate(float translateX, float translateY)
	{
		return glm::mat3(
			1,			0,			0,
			0,			1,			0,
			translateX, translateY, 1
		);
	}

	// Scale matrix
	inline glm::mat3 Scale(float scaleX, float scaleY)
	{
		return glm::mat3(
			scaleX, 0,		0,
			0,		scaleY, 0,
			0,		0,		1
		);
	}

	// Rotate matrix
	inline glm::mat3 Rotate(float radians)
	{
		// don't compute them ore than once
		float cosRes = cos(radians);
		float sinRes = sin(radians);

		return glm::mat3(
			cosRes,  sinRes, 0,
			-sinRes, cosRes, 0,
			0,		 0,		 1
		);
	}
}
