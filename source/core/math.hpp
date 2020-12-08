#ifndef CORE_MATH_HPP
#define CORE_MATH_HPP

// Clamp value
template<typename T> T clamp(T value, T min, T max)
{
	return ((value < min) ? min : ((value > max) ? max : value));
}

#endif