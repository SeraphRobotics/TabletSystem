#pragma once


#include "float.h"
#include "structs.h"
#include "vector3.h"

namespace Math {

/**
 * 4-dimensional object representing a rotation about an axis in 3d space
 */
struct Quaternion {
  Float w, x, y, z;

  /**
   * Does nothing
   */
  Quaternion();

  /**
   * Copies the input quaternion
   */
  Quaternion(const Quaternion& q);

  /**
   * Copies the input quaternion
   */
  explicit Quaternion(const Quaternion* q);

  /**
   * Defines this quaternion as a rotation about the given axis by
   * a number of radians specified by 'angle'.
   * The input axis must be normalized.
   */
  explicit Quaternion(const Vector3& axis, Float angle);

  /**
   * Defines this quaternion as a rotation about the given axis by
   * a number of radians specified by 'angle'.
   * The input axis must be normalized.
   */
  explicit Quaternion(const Vector3* axis, Float angle);

  /**
   * Explicitly defines the values of this quaternion
   */
  explicit Quaternion(Float w, Float x, Float y, Float z);

  /**
   * Explicitly defines the values of this quaternion
   */
  explicit Quaternion(const Float wxyz[4]);

  /**
   * Copies the input quaternion
   */
  Quaternion& set(const Quaternion& q);

  /**
   * Copies the input quaternion
   */
  Quaternion& set(const Quaternion* q);

  /**
   * Defines this quaternion as a rotation about the given axis by
   * a number of radians specified by 'angle'.
   * The input axis must be normalized.
   */
  Quaternion& set(const Vector3& axis, Float angle);

  /**
   * Defines this quaternion as a rotation about the given axis by
   * a number of radians specified by 'angle'.
   * The input axis must be normalized.
   */
  Quaternion& set(const Vector3* axis, Float angle);

  /**
   * Explicitly defines the values of this quaternion
   */
  Quaternion& set(Float w, Float x, Float y, Float z);

  /**
   * Explicitly defines the values of this quaternion
   */
  Quaternion& set(const Float wxyz[4]);

  /**
   * Defines this quaternion using a rotation about the x/y/z axes
   */
  Quaternion& setEuler(Float x, Float y, Float z);

  /**
   * Defines this quaternion using a rotation about the x/y/z axes
   */
  Quaternion& setEuler(const Vector3& xyz);

  /**
   * Sets this quaternion to itself times the input quaternion.  This
   * results in a rotation that is the composition of this applied to q.
   * @note quaternion multiplication is NOT commutative and it's
   *       easy to confuse mul() with compose()!
   * @see Quaternion::compose
   */
  Quaternion& mul(const Quaternion& q);

  /**
   * Sets this quaternion to itself times the input quaternion.  This
   * results in a rotation that is the composition of this applied to q.
   * @note quaternion multiplication is NOT commutative and it's
   *       easy to confuse mul() with compose()!
   * @see Quaternion::compose
   */
  Quaternion& mul(const Quaternion* q);

  /**
   * Resets all values in this quaternion to zero; this is no longer
   * a valid rotation.
   */
  Quaternion& zero();

  /**
   * Sets this quaternion to the identity (no rotation)
   */
  Quaternion& identity();

  /**
   * Returns the length of this quaternion
   */
  Float magnitude() const;

  /**
   * Returns the square of the length of this quaternion
   */
  Float magnitudeSq() const;

  /**
   * Scales this quaternion to have unit magnitude
   */
  Quaternion& normalize();

  /**
   * Returns true if this quaternion has no magnitude (i.e. can't be normalized)
   */
  bool isInvalid() const;

  /**
   * Returns true if this quaternion has unit magnitude
   */
  bool isNormalized() const;

  /**
   * Duplicates the values in this quaternion to a new object
   */
  Quaternion copy() const;

  /**
   * Finds the x/y/z/ rotation angles of this quaternion
   */
  Vector3 eulerAngles() const;
};

}
