#include "plane.h"
#include "line.h"
#include "vector3.h"
#include "triangle.h"
#include <libraries/common/todo.h>
#include <libraries/common/confirm.h>


namespace Math {

Plane::Plane(const Vector3& a,
                       const Vector3& b,
                       const Vector3& c) {
  set(a,b,c);
}


Plane::Plane(const Vector3& point,
                       const Vector3& normal) {
  set(point, normal);
}



Vector3 Plane::pointOnPlane() const {
  // ax + by + cz + d = 0
  Line line;
  line.set(Vector3::make(0,0,0), Vector3::make(a,b,c));
  return intersectLine(line);
}


bool Plane::intersectLine(const Line& line, Vector3* intersection) const {
  Vector3 abc(a,b,c);
  Float numer = d + abc.dot(line.a);
  Float denom = -abc.dot(line.ray());

  if (denom == 0) {
    // parallel to the plane
    return false;
  }

  intersection->set(line.a).add(line.ray().scale(numer/denom));

  return true;
}


bool Plane::intersectSegment(const Line& segment, Vector3* intersection) const {
  Vector3 abc(a,b,c);
  Float numer = d + abc.dot(segment.a);
  Float denom = -abc.dot(segment.ray());

  if (denom == 0) {
    // parallel to the plane
    return false;
  }

  Float t = numer / denom;

  // check to make sure it falls between the segment's endpoints
  if (t < 0 || t > 1) return false;

  intersection->set(segment.a).add(segment.ray().scale(t));

  return true;
}


Vector3 Plane::intersectLine(const Line& line) const {
  Vector3 intersection;
  if(!intersectLine(line, &intersection)){
    Vector3 v;
    return v;
  }
  return intersection;
}





Plane::RelativeLocation Plane::relativeLocationOfPoint(const Vector3& point) const {
  Float f = a*point[0] + b*point[1] + c*point[2] + d;
  if (f < 0) {
    return kBelow;
  } else if (floatsEqual(f, 0.0)) {
    return kCoincident;
  } else {
    return kAbove;
  }
}


void Plane::set(const Vector3& a, const Vector3& b, const Vector3& c) {
  struct Triangle triangle(a,b,c);
  set(a, triangle.normal());
}


void Plane::set(const Vector3& point, const Vector3& normal) {
  Vector3 abc(normal);
  abc.normalize();
  a = abc.x;
  b = abc.y;
  c = abc.z;
  d = -abc.dot(point);
}

}
