#pragma once




//----[  MicronTolerance  ]----------------------------------------------------
// Pass this struct as the _Tolerance parameter when specifying the type of
// a Vector3.  This will make any two points in space less than 0.000001 units
// apart register as equal, and any distance greater than that amount as
// nonequal.
// Other uses include:
//  - two planes are coincident if they are parallel and the distance between
//    them is less than the tolerance
//  - two lines intersect if the shortest line that can be created between
//    them has a length less than the tolerance
struct MicronTolerance {
  static const double kTolerance;
  static const double kToleranceSq;
};

