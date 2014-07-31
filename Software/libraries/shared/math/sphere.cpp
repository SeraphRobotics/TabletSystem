#include "sphere.h"
#include <math.h>           // for abs/sin/cos
#include "libraries/common/confirm.h"  // code protection, debugging
#include "libraries/common/todo.h"     // error tracking

namespace Math {


Sphere::Sphere() {}


Sphere::Sphere(const Vector3& v, Float r) {
  set(v,r);
}


Sphere::Sphere(const Sphere& s) {
  set(s);
}


Sphere::Sphere(const Sphere* s) {
  set(s);
}


Sphere& Sphere::set(const Vector3& v, Float r) {
  center.set(v);
  radius = r;
  return *this;
}



Sphere& Sphere::set(const Sphere& s) {
  center.set(s.center);
  radius = s.radius;
  return *this;
}


Sphere& Sphere::set(const Sphere* s) {
  center.set(s->center);
  radius = s->radius;
  return *this;
}


Sphere& Sphere::set(const Vector3& v1, const Vector3& v2) {
  center.set(v1).add(v2).scale(0.5f); // center = average of coordinates
  radius = v2.copy().sub(center).magnitude(); // radius contains both
  return *this;
}


bool Sphere::intersects(const Vector3& origin, const Vector3& ray) const {
  float enter, exit;
  return intersectDistances(origin, ray, &enter, &exit) > 0;
}


int Sphere::intersectDistances(const Vector3& origin, const Vector3& ray,
                               float* enter_distance, float* exit_distance) const {

  if(!ray.isNormalized()) {
    if (floatsEqual(ray.length(), 0)) return 0;
    return intersectDistances(origin,
                              ray.copy().normalize(),
                              enter_distance,
                              exit_distance);
  }

  Vector3 distance;
  distance.set(origin).sub(center);
  Float b = distance.dot(ray);
  Float c = distance.magnitudeSq() - radius*radius;
  Float d = b*b - c;
  if (d > Float(0)) {
    d = sqrt(d);
    *enter_distance = -b - d;
    if (b > d) {
      // intersects twice; ray origin outside the sphere & pointing at sphere
      *exit_distance = -b + d;
      return 2;
    } else {
      // intersects once; ray origin leaving sphere or ray just barely skims surface
      *exit_distance = *enter_distance;
      return 1;
    }
  } else {
    return 0;
  }
}


bool Sphere:: intersectNearLocation(const Vector3& origin, const Vector3& ray, Vector3* intersection) const {
  float enter, exit;
  if (0 == intersectDistances(origin, ray, &enter, &exit)) return false;
  intersection->set(ray).scale(enter).add(origin);
  return true;
}




}
