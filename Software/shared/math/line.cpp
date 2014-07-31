#include "line.h"
#include <common/confirm.h>  // code protection, debugging
#include <common/todo.h>     // error tracking
#include "math.h"

namespace Math {


Line::Line() {}


Line::Line(const Line& v) : a(v.a), b(v.b) {}


Line::Line(const Line* v) : a(v->a), b(v->b) {}


Line::Line(const Vector3& a, const Vector3& b) : a(a), b(b) {}


void Line::set(const Line& other) {
  a.set(other.a);
  b.set(other.b);
}


void Line::set(const Line* other) {
  if(other){
    a.set(other->a);
    b.set(other->b);
  }
}


void Line::set(const Vector3& a, const Vector3& b) {
  this->a.set(a);
  this->b.set(b);
}


bool Line::hasLength() const {
  return a.equals(b) == false;
}


Float Line::length() const {
  return b.copy().sub(a).length();
}




void Line::reverse() {
  Vector3 temp(a);
  a.set(b);
  b.set(temp);
}


bool Line::pointOnLine(const Vector3& v) const {
  // a + ray(from:a, to:v) * scalar = b
  // ray(from:a, to:v) * scalar = b - a
  // abs(ray(from:a, to:v)) = abs(ray(from:a, to:b))
  Vector3 t0, t1;
  return a.equals(v) ||
         b.equals(v) || 
         t0.ray(a, v).abs().equals(t1.ray(a, b).abs());
}


bool Line::pointOnSegment(const Vector3& v) const {
  // the point is in the segment if a ray from a->v is the reverse of
  // the ray from b->v, or either endpoint is equal to the point.
  bool point_on_segment = a.equals(v) || b.equals(v);
  if (!point_on_segment) {
    Vector3 t0, t1;
    float dot_product = t0.ray(a, v).dot(t1.ray(b, v));
    point_on_segment = floatsEqual(dot_product, -1.0);
  }
  return point_on_segment;
}



Vector3 Line::center() const {
  return a.copy().add(b).scale(0.5);
}


bool Line::isEndpoint(const Vector3& other) const {
  return a.equals(other) || b.equals(other);
}



bool Line::coincidentWithLine(const Line& line) const {
  return line.pointOnLine(a) && line.pointOnLine(b);
}



bool Line::equalsSegment(const Line& segment) const {
  return (a.equals(segment.a) && b.equals(segment.b)) ||
         (a.equals(segment.b) && b.equals(segment.a));
}



Line Line::intersectLineWithLine(const Line& other) const {
  if( !(!"intersectLineWithLine is not implemented") ) {
//    todo("kwg8","implement intersectLineWithLine");
  }
  Line invalid;
  return invalid.zero();
}


 Line& Line::zero() {
  a.zero();
  b.zero();
  return *this;
}


Line& Line::changeLength(Float amount) {
  if(!(hasLength()) ) {
    // can't do anything if there is no length, since we can't find a
    // direction in which to extend the line
    return *this;
  }
  amount /= Float(2.0);
  Vector3 v(ray());
  v.normalize();
  a.sub(v.copy().scale(amount)); // a += norm(b-a) * amount/2
  b.add(v.copy().scale(amount)); // b -= norm(b-a) * amount/2
  return *this;
}


 Vector3 Line::ray() const {
  return b.copy().sub(a);
}



bool Line::intersectSegmentWithSegment2DXY(
        const Line& other,
        Vector3* result) const {

  // algorithm from:
  // http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/

  Float ua = (other.b.x - other.a.x)*(a.y - other.a.y)
           - (other.b.y - other.a.y)*(a.x - other.a.x);
  Float ub = (b.x - a.x)*(a.y - other.a.y) - (b.y - a.y)*(a.x - other.a.x);
  Float denom = (other.b.y - other.a.y)*(b.x - a.x)
              - (other.b.x - other.a.x)*(b.y - a.y);

  if (denom == Float(0.0)) {
    return false; // no intersection; parallel or coincident lines
  }

  ua /= denom;
  ub /= denom;

  // ensure it is within both segments
  if (ua < 0.0 || ua > 1.0 || ub < 0.0 || ub > 1.0) {
    return false;
  }

  if(result){
    result->set(a.x + ua * (b.x - a.x),
                a.y + ua * (b.y - a.y),
                (a.z+b.z+other.a.z+other.b.z)/4.0);
  }
  return true;
}



Float Line::evaluate2DLineXY(
        const Vector3& point) const {
  Float dx = b.x - a.x, dy = b.y - a.y;
  // y - y0 = m(x - x0)
  // y - m(x - x0) - y0 = 0
  if (dx == Float(0.0)) {
    // this is an exactly vertical line
    Float difference = point.x - a.x;
    if (floatsEqual(difference, 0.0)) {
      return point.y - /* Float(0.0) -*/ a.y;
    } else if (difference > Float(0.0)) {
      return /* point.y */- dy * Float(+1e10) /* - a.y */;
    } else { //if (difference < Float(0.0)) {
      return /* point.y */- dy * Float(-1e10) /* - a.y */;
    }
  } else {
    return (point.y - (dy / dx) * (point.x - a.x) - a.y);
  }
}


Float Line::segmentDistanceTo2DXY(const Vector3& point) const {
  Float A = point.x - a.x;
  Float B = point.y - a.y;
  Float C = b.x - a.x;
  Float D = b.y - a.y;
  Float dot = A*C + B*D;
  Float len_sq = C*C + D*D;
  Float param = dot / len_sq;

  Float xx, yy;
  if (param < (Float)0.0) {
    xx = a.x;
    yy = a.y;
  } else if (param > (Float)1.0) {
    xx = b.x;
    yy = b.y;
  } else {
    xx = a.x + param * C;
    yy = a.y + param * D;
  }

  Float dx = point.x - xx;
  Float dy = point.y - yy;

  return sqrt(dx*dx + dy*dy);
}



Float Line::lineDistanceTo2DXY(const Vector3& point) const {
  Float A = point.x - a.x;
  Float B = point.y - a.y;
  Float C = b.x - a.x;
  Float D = b.y - a.y;
  Float dot = A*C + B*D;
  Float len_sq = C*C + D*D;
  Float param = dot / len_sq;

  Float xx, yy; // closest point on line
  xx = a.x + param * C;
  yy = a.y + param * D;

  Float dx = point.x - xx;
  Float dy = point.y - yy;

  return sqrt(dx*dx + dy*dy);
}

}
