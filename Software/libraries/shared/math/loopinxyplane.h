#include "structs.h"
#include "triangle.h"
#include <QVector>


namespace Math {

struct Line;

/**
 * Stores a set of points that defines a loop.  The loop must not be self-
 * intersecting, and its direction matters.  A loop that is counter-clockwise
 * when viewed from above on the +Z axis defines an "outer" boundary such that
 * the area within the loop is considered the inside.  Winding points in the
 * opposite order defines a loop whose inside contains everything except
 * what is within the loop's boundaries.  In this way, taking the union of
 * several loops can define disjoint areas and areas with inner holes.
 */
struct LoopInXYPlane {
public:

  /**
   * Defines the rotation direction of this loop
   */
  enum Sense {

    /// Loop points progress counter-clockwise 
    kCounterClockwise = 0,

    /// Loop points progress clockwise
    kClockwise = 1,

    /// The sense can't be determined
    kUndefined = 2
  };


public:

  /// List of points representing the loop
  QVector<Vector3> points;


public:

  LoopInXYPlane();

  /**
   * Copies the points from the input loop
   */
  LoopInXYPlane(const LoopInXYPlane& other);

  /**
   * Copies the points from the input loop
   */
  LoopInXYPlane( LoopInXYPlane* other);

  /**
   * Obtains the sense of this loop from the order of the points it contains
   */
  Sense sense() const;

  /**
   * Changes the sense of this loop from clockwise to counter-clockwise or
   * vice-versa.
   */
  void reverse();

  /**
   * Returns the average z coordinate in the loop
   */
  Float getAverageZCoordinate() const;

  /**
   * Sets the z coordinate for every point to be the average z for over all
   * points in the loop
   */
  void snapZCoordinates();

  /**
   * Takes a guess at the area of this loop by finding the area of the
   * rectangle created by the min/max coordinates.  The value will never be
   * lower than the true area.
   */
  Float areaEstimate() const;

  /**
   * Returns lowest values from each dimension
   */
  Vector3 min() const;

  /**
   * Returns the highest values from each dimension
   */
  Vector3 max() const;

  /**
   * Adds a point at the end of member 'points'
   */
  void add(const Vector3& point);

  /**
   * Adds a point to the beginning of member 'points'
   */
  void addFirst(const Vector3& point);

  /**
   * Determines whether or not a point is inside this loop.  The z-coordinate
   * of the point is ignored.
   */
  bool pointInside(const Vector3& point) const;

  /**
   * Changes the Z-coordinate of all elements
   */
  void setZ(float z);

  /**
   * Returns in inner_segments the longest components of 'segment' for which
   * all points are inside this loop and whose ends are either one of segment's
   * ends or are on a boundary of this loop.
   * For counter-clockwise loops, this will return only segments inside the loop.
   * For clockwise loops, this method returns segments outside the loop.
   */
  void findInnerSegments(const Line& segment,
                         QVector<Line>* inner_segments) const;

  /**
   * Returns 'true' if the given segment intersects with the boundary of this loop
   */
  bool crossedBy(const Line& segment) const;

  /**
   * Returns true if all of the points in the other loop are within this loop, and
   * none of the edges of the two loops intersect.
   */
  bool contains(const LoopInXYPlane& other) const;


  /**
   *returns true if given segment intersects with boundary, returns point of intersection
   **/
  bool intersect(const Line& segment,Vector3* result) const;

  /**
   * Creates a new set of zero or more loops that are displaced by 'amount' from this
   * loop.  For some loop shapes, this can cause a single loop to generate multiple
   * loops (by contracting a bean shape, for example).  Other times, a high displacement
   * can cause no valid loops to be found at all.
   * @note BE SURE before invoking this method that the loop has been simplified!  If
   *       at all possible, use the simplifyAndExpand function to call both.
   */
  void expand(Float amount, QVector<LoopInXYPlane>* expanded_loops) const;

  /**
   * Eliminates consecutive points in the loop that are coincident and replaces N>2
   * points in a straight line with 2 points.
   */
  void simplify();

  /**
   * Invokes simplify(), then expand().  This is the safest way to call 'expand'.
   */
  void simplifyAndExpand(Float amount, QVector<LoopInXYPlane>* expanded_loops);
};


}
