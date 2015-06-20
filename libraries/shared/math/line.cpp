#include "line.h"
#include <libraries/common/confirm.h>  // code protection, debugging
#include <libraries/common/todo.h>     // error tracking
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


bool Line::pointOnSegment2D(const Vector3& v) const{
    Vector3 ap = a.copy();
    ap.z=0;
    Vector3 bp = b.copy();
    bp.z=0;
    Vector3 vp = v.copy();
    vp.z=0;
    bool point_on_segment = ap.equals(vp) || bp.equals(vp);
    if (!point_on_segment) {
      Vector3 t0, t1;
      float dot_product = t0.ray(ap, vp).dot(t1.ray(bp, vp));
      point_on_segment = floatsEqual(dot_product, -1.0);
    }
    return point_on_segment;
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



bool Line::intersectLineWithLine(const Line& other, Line *result) const {
    //http://paulbourke.net/geometry/pointlineplane/
    result->zero();
    double EPS = 0.000001;
    Vector3 p1,p2,p3,p4;
    p1 = a.copy();
    p2 = b.copy();
    p3 = other.a.copy();
    p4 = other.b.copy();
    double mua,mub;
    Vector3 pa,pb;

    Vector3 p13,p43,p21;
    double d1343,d4321,d1321,d4343,d2121;
    double numer,denom;

    p13.x = p1.x - p3.x;
    p13.y = p1.y - p3.y;
    p13.z = p1.z - p3.z;
    p43.x = p4.x - p3.x;
    p43.y = p4.y - p3.y;
    p43.z = p4.z - p3.z;
    if (abs(p43.x) < EPS && abs(p43.y) < EPS && abs(p43.z) < EPS){
      return false;}
    p21.x = p2.x - p1.x;
    p21.y = p2.y - p1.y;
    p21.z = p2.z - p1.z;
    if (abs(p21.x) < EPS && abs(p21.y) < EPS && abs(p21.z) < EPS){
      return false;
    }

    d1343 = p13.x * p43.x + p13.y * p43.y + p13.z * p43.z;
    d4321 = p43.x * p21.x + p43.y * p21.y + p43.z * p21.z;
    d1321 = p13.x * p21.x + p13.y * p21.y + p13.z * p21.z;
    d4343 = p43.x * p43.x + p43.y * p43.y + p43.z * p43.z;
    d2121 = p21.x * p21.x + p21.y * p21.y + p21.z * p21.z;

    denom = d2121 * d4343 - d4321 * d4321;
    if (abs(denom) < EPS){
      return false;
    }
    numer = d1343 * d4321 - d1321 * d4343;

    mua = numer / denom;
    mub = (d1343 + d4321 * (mua)) / d4343;

    pa.x = p1.x + mua * p21.x;
    pa.y = p1.y + mua * p21.y;
    pa.z = p1.z + mua * p21.z;
    pb.x = p3.x + mub * p43.x;
    pb.y = p3.y + mub * p43.y;
    pb.z = p3.z + mub * p43.z;

    result->a = pa.copy();
    result->b = pb.copy();
    return true;
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


bool Line::intersectSegmentWithSegment3DXY(const Line& other, Vector3* result){
    //http://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
    result->set(0,0,0);
    Vector3 q,p,r,s;
    q = a.copy();
    p = other.a.copy();
    r = b.copy()-q;
    s = other.b.copy();

    if(r.cross(s).magnitude()==0){return false;}

    float t = ((q-p).cross((s/(r.cross(s).magnitude())))).magnitude();
    float u = ((q-p).cross((r/(r.cross(s).magnitude())))).magnitude();
    if( (u<1 && u>0 && t>0 && t<1) || u==0 || u==1 || t==1 || t==0){
        Vector3 pt = q+u*s;
        result->set(pt);
        return true;
    }
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


Float Line::lineDistanceTo3D(const Vector3 &point) const{
    //http://mathworld.wolfram.com/Point-LineDistance3-Dimensional.html
    Vector3 p1 = point-a;
    Vector3 p2 = point-b;
    Vector3 d = b-a;
    Vector3 c = p1.cross(p2);
    return c.magnitude()/d.magnitude();
}

Float Line::valueAtXY(Float X,Float Y) const{
    //http://tutorial.math.lamar.edu/Classes/CalcIII/EqnsOfLines.aspx
    Vector3 abc = b-a;
    if (0==abc.z) return a.z;
    if (0==abc.y && 0!=abc.x){
        return abc.z/abc.x*(X-a.x)+a.z;
    }
    if (0==abc.x && 0!=abc.y){
        return abc.z/abc.y*(Y-a.y)+a.z;
    }
    return a.z;



}

}
