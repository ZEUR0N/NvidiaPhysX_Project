#pragma once

#include <time.h>
#include <stdlib.h>
#include "core.hpp"
#include "RenderUtils.hpp"

#include <math.h>

#include <time.h>

namespace Constants {
	const Vector3 GRAVITY = { 0, -24.79f , 0 }; //A estamos en Jupiter
}

namespace Math {


	const double PI = 3.14159265;

	static float radians(float const& angle) {
		return angle* PI / 180.0;
	}

	static float random(const float& min, const float& max) {
		return (max - min) * ((float)rand() / RAND_MAX) + min;
		//return (rand() % max) + min;
	};
	
	static int random(const int& min, const int& max) {
		return (max - min) * ((float)rand() / RAND_MAX) + min;
		//return (rand() % max) + min;
	};

	static int random(const int& max) {
		//return (max) * ((T)rand() / RAND_MAX);
		return (rand() % max);
	};
	
	static float random(const float& max) {
		return (max) * ((float)rand() / RAND_MAX);
		//return (rand() % max);
	};

	static Vector3 random(const Vector3& v1, const Vector3& v2) {
		return Vector3(random(v1.x, v2.x), random(v1.y, v2.y), random(v1.z, v2.z));
	}
	
	static Vector4 random(const Vector4& v1, const Vector4& v2) {
		return Vector4(random(v1.x, v2.x), random(v1.y, v2.y), random(v1.z, v2.z), random(v1.w, v2.w));
	}
}




