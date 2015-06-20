#include "triangle.h"
#include <libraries/common/confirm.h>  // code protection, debugging
#include <libraries/common/todo.h>     // error tracking
#include "line.h"            // edges are lines
#include "plane.h"
#include <QDebug>



namespace Math {


Triangle::Triangle() {
}


Triangle::Triangle(const Triangle& other) {
  v[0].set(other.v[0]);
  v[1].set(other.v[1]);
  v[2].set(other.v[2]);
}


Triangle::Triangle(const Triangle* other) {
  if(other){
    v[0].set(other->v[0]);
    v[1].set(other->v[1]);
    v[2].set(other->v[2]);
  }
}


Triangle::Triangle(const Vector3& a,
                             const Vector3& b,
                             const Vector3& c) {
  set(a, b, c);
}


void Triangle::edge(int edge_number, Line* segment) const {
  switch (edge_number) {
  default: {
      //confirm(!"trying to access invalid edge; returning empty segment instead");
      segment->zero();
    } break;
  case 0: segment->set(edge0()); break;
  case 1: segment->set(edge1()); break;
  case 2: segment->set(edge2()); break;
  }
}


Line Triangle::edge0() const {
  Line e(v[0], v[1]);
  return e;
}


Line Triangle::edge1() const {
  Line e(v[1], v[2]);
  return e;
}


Line Triangle::edge2() const {
  Line e(v[2], v[0]);
  return e;
}


Vector3 Triangle::center() const {
  static const Float kOneThird = 1.0 / 3.0;
  return v[0].copy().add(v[1]).add(v[2]).scale(kOneThird);
}


Vector3 Triangle::normal() const {
  // normal is found by:
  //  normalize( (v1-v0) cross (v2 - v0) )

  Vector3 normal = v[1].copy().sub(v[0]).cross(v[2].copy().sub(v[0]));
  if(normal.isInvalid()){
      return normal;
  }
  return normal.normalize();

}


void Triangle::set(const Vector3& a,
                   const Vector3& b,
                   const Vector3& c) {
  v[0].set(a);
  v[1].set(b);
  v[2].set(c);
}

bool Triangle::intersectXYPlaneAtZ(Float z, Line* intersection) const {

  if ((v[0].z >= z && v[1].z >= z && v[2].z >= z) ||
      (v[0].z <= z && v[1].z <= z && v[2].z <= z)) {
    return false;
  }

  Vector3 p, p1, p2;
  int points_found = 0;

  for (int i0=0; i0<3; i0++) {
    int i1 = (i0+1)%3;
    if ((v[i0].z <= z && v[i1].z > z) || (v[i1].z <= z && v[i0].z > z)) {
      p.set(v[i0]).add(v[i1].copy().sub(v[i0]).scale((z - v[i0].z)/(v[i1].z-v[i0].z)));
      //p = v[i0]+(v[i1]-v[i0])*(z-v[i0].z)/(v[i1].z-v[i0].z);
      if (points_found == 0) {
        p1.set(p);
        points_found++;
      } else {
        p2.set(p);
        // set right order
        if ((normal().cross(p2.copy().sub(p1))).dot(Vector3::make(0,0,1)) < 0) { // swap
          p.set(p1);
          p1.set(p2);
          p2.set(p);
        }
        intersection->set(p1, p2);
        return true;
      }
    }
  }

  return false; // less than 2 points found
}



bool Triangle::intersectedByRay(const Vector3& origin,
                                const Vector3& ray) {
  Vector3 e1,e2;
  e1.set(v[1]).sub(v[0]);
  e2.set(v[2]).sub(v[0]);

  Vector3 q;
  q.set(ray).cross(e2);
  double a=e1.dot(q);

  // vector is parallel to plane
  if (floatsEqual(a, 0)) return false;


  Line l(origin.copy(),origin.copy()+1000*ray.copy());
  Vector3 pt;
  if(     edge0().intersectSegmentWithSegment3DXY(l,&pt) ||
          edge1().intersectSegmentWithSegment3DXY(l,&pt)||
          edge2().intersectSegmentWithSegment3DXY(l,&pt)){
      return true;
  }

  double f=1/a;
  Vector3 s;
  s.set(origin).sub(v[0]);
  double u=f*s.dot(q);

  // the intersection is outside the triangle
  if (u<0.0) return false;

  Vector3 r;
  r=s.cross(e1);
  double v=f*ray.dot(r);

  // the intersection is outside the triangle
  if (v<0.0 || u+v>1.0) return false;

  return true;
}


float Triangle::heightAt(const Vector3& origin,
               const Vector3& ray){
    /// http://geomalgorithms.com/a05-_intersect-1.html
    Vector3 n = normal();
    Vector3 w;
    float ndotR = n.dot(ray);
    if(floatsEqual(ndotR,0)){return -100000000;}
    w.set(v[0]).sub(origin);
    float h = n.dot(w)/ndotR;
    return h;
}

}
