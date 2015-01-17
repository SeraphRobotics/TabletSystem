#pragma once

#include "float.h"
#include "structs.h"
#include "vector3.h"


namespace Math {

struct Sphere {

  Vector3 center;
  Float radius;

  /**
   * Default initializer; does nothing
   */
  Sphere();

  /**
   * Initialize this sphere
   */
  explicit Sphere(const Vector3& v, Float r);

  /**
   * Copy values from the given sphere
   */
  explicit Sphere(const Sphere& s);

  /**
   * Copy values from the given sphere
   */
  explicit Sphere(const Sphere* s);

  /**
   * Initialize this sphere
   */
  Sphere& set(const Vector3& v, Float r);

  /**
   * Copy values from the given sphere
   */
  Sphere& set(const Sphere& s);

  /**
   * Copy values from the given sphere
   */
  Sphere& set(const Sphere* s);

  /**
   * Sets this sphere to contain the two vertices.  This is usually used to contain the min
   * and max coordinates of a mesh
   */
  Sphere& set(const Vector3& v1, const Vector3& v2);

  /**
   * Returns true if a ray cast from the given origin would intersect this sphere's boundaries
   */
  bool intersects(const Vector3& origin, const Vector3& ray) const;

  /**
   * Finds the distances at which a ray cast from 'origin' would enter and exit the sphere.
   * this method returns the number of intersections, which can be 0, 1, or 2.  1 is returned
   * if the ray just barely skims the surface of the sphere.
   */
  int intersectDistances(const Vector3& origin, const Vector3& ray,
                         float* enter_distance, float* exit_distance) const;

  /**
   * Finds the 3d coordinates of the closer intersection point of a ray cast toward the sphere.
   * Returns 'false' and does not modify 'intersection' if no intersection is found.
   */
  bool intersectNearLocation(const Vector3& origin, const Vector3& ray, Vector3* intersection) const;
};


}


