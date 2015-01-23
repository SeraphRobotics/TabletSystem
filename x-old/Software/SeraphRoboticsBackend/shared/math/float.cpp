#include "float.h"

namespace Math {

Float toRadians(Float degrees) {
  return degrees / 180.0 * kPi;
}

Float toDegrees(Float radians) {
  return radians / kPi * 180.0;
}

bool floatsEqual(Float a, Float b) {
  static const Float kEpsilon = 1e-6;
  return a + kEpsilon > b &&
         a - kEpsilon < b;
}

}
