////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
/////////////////////////////////////////////////////////////
///////////////////////////HEADERS///////////////////////////
#include <random>
#include <cmath>
#include <ctime>
#include <Nero/core/cpp/utility/MathUtil.h>
/////////////////////////////////////////////////////////////


namespace
{
	std::default_random_engine createRandomEngine()
	{
		//seed: graine aleatoire
		auto seed = static_cast<unsigned long>(std::time(nullptr));
		return std::default_random_engine(seed);
	}

	auto RandomEngine = createRandomEngine();
}

namespace nero
{
	int randomInt(int exclusiveMax)
	{
		std::uniform_int_distribution<int> distr(0, exclusiveMax - 1);
		return distr(RandomEngine);
	}

	float randomFloat()
	{
		float r = (float)(std::rand() & (RAND_LIMIT));
		r /= RAND_LIMIT;
		r = 2.0f * r - 1.0f;
		return r;
	}

	float randomFloat(float lo, float hi)
	{
		float r = (float)(std::rand() & (RAND_LIMIT));
		r /= RAND_LIMIT;
		r = (hi - lo) * r + lo;
		return r;
	}
}
