#pragma once


namespace Math {

/// This is the floating-point type used throughout the math namespace
typedef double Float;

static const Float kPi = Float(3.14159265358979323846);
static const Float k2Pi = Float(3.14159265358979323846 * 2);

Float toRadians(Float degrees);
Float toDegrees(Float radians);

/**
 * Returns true only if the floats are within some epsilon of each other
 */
bool floatsEqual(Float a, Float b);

}
