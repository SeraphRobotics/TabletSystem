
#include <libraries/common/todo.h>
#include <libraries/common/confirm.h>
#include "triangle.h"
#include "line.h"
#include "loopinxyplane.h"
#include <math.h>

#include <QVector>
#include <algorithm>

#include <QDebug>

namespace Math {

LoopInXYPlane::LoopInXYPlane() {
}

LoopInXYPlane::LoopInXYPlane(const LoopInXYPlane& other) {
  points += other.points;
}

LoopInXYPlane::LoopInXYPlane( LoopInXYPlane* other){
    for(int i=0; i<points.size();i++){
        points.append(Vector3(other->points.at(i).x,
                              other->points.at(i).y,
                              other->points.at(i).z));
    }
}

LoopInXYPlane::Sense LoopInXYPlane::sense() const {

  if (points.size() < 3) return kUndefined;

  Vector3 v(0.0f, 0.0f, 0.0f);
  int i;
  for (i = 0; i < points.size(); ++i) {
    v.add(points[i]);
  }
  v.scale(1.0f / i); // average coordinate

  // find angle to each other coordinate
  double angle_sum = 0.0;
  for (int i = 0; i < points.size(); ++i) {
    Vector3 v0(points.at((i-1+points.size())%points.size())), v1(points.at(i));
    v0.sub(v);
    v1.sub(v);
    const float TWO_PI = 3.1415*2;
    float diff =  atan2(v1.y,v1.x) - atan2(v0.y,v0.x);
    while (diff >= +TWO_PI/2) diff -= TWO_PI;
    while (diff <= -TWO_PI/2) diff += TWO_PI;
    angle_sum += diff;
  }

  return angle_sum > 0.0 ? kCounterClockwise : kClockwise;
}



void LoopInXYPlane::reverse() {
  QVector<Vector3> new_points;
  for (int i = points.size() - 1; i >= 0; --i) {
    new_points.push_back(points.at(i));
  }
  points.clear();
  points += new_points;
}



Vector3 LoopInXYPlane::min() const {
  Vector3 v=points[0].copy();
//  qDebug()<<"x:"<<v.x<<" y:"<<v.y<<" z:"<<v.z;
  if( !(points.size() >= 1) ) {

    for (int i = 1; i < points.size(); ++i) {
      Vector3 pt = points[i];
      if(pt.x<v.x){v.x= pt.x;}
      if(pt.y<v.y){v.y= pt.y;}
      if(pt.z<v.z){v.z= pt.z;}
//        v.min(points[i]);
    }
  }
  return v;
}



Vector3 LoopInXYPlane::max() const {
  Vector3 v;
  if( !(points.size() >= 1) ){
    v.set(points[0]);
    for (int i = 1; i < points.size(); ++i) {
      v.max(points[i]);
    }
  }
  return v;
}



Float LoopInXYPlane::areaEstimate() const {
  Vector3 v;
  v.set(max()).sub(min());
  return v.x * v.y;
}



void LoopInXYPlane::add(const Vector3& point) {
  points.push_back(point.copy());
}



void LoopInXYPlane::addFirst(const Vector3& point) {
  points.push_front(point);
}



Float LoopInXYPlane::getAverageZCoordinate() const {
  Vector3 v(0.0f, 0.0f, 0.0f);
  int i;
  for (i = 0; i < points.size(); ++i) {
    v.add(points[i]);
  }
  return v.scale(1.0f / i).z;
}



bool LoopInXYPlane::pointInside(const Vector3& point) const {
//  if( !(points.size() >= 3) )return false;


//  Vector3 newPt(point);
//  newPt.z=0;
//  // Perform the ray-intersection test: cast a ray from the given point
//  // out to infinity and count the number of segments it intersects.
//  Line test_line(newPt, max().scale((Float)2.0));
//  int intersections = 0;
//  for (int i = 1; i < points.size() + 1; ++i) {
//    Line current_segment(points[i-1], points[i%points.size()]);
//    Vector3 pt;
//    // find an intersection that is not the endpoint of the current segment.
//    // this ensures that lines that go through a vertex don't get double-counted
////    if (current_segment.intersectSegmentWithSegment2DXY(test_line, &pt) &&
////        !current_segment.b.equals(pt)) {
////      ++intersections;
////    }
//    if (current_segment.intersectSegmentWithSegment2DXY(test_line, &pt)) {
//      ++intersections;
//    }
//  }

//  // An even number of intersections (including 0) means that this point is
//  // not within the loop.  If the sense of this loop is counter-clockwise,
//  // that means that the point is not inside;
//  return ((intersections % 2) == 1);
//}
  bool inside = false;
  Vector3 p1 = points[0];
  float xinters =0;
  for(int i=0;i<points.size()+1;i++){
      Vector3 p2 = points[ i%points.size() ];
      if (point.y > std::min(p1.y,p2.y)){
          if (point.y <= std::max(p1.y,p2.y)){
              if (point.x <= std::max(p1.x,p2.x)){
                  if (p1.y != p2.y){
                      xinters = (point.y-p1.y)*(p2.x-p1.x)/(p2.y-p1.y)+p1.x;
                  }
                  if (p1.x == p2.x or point.x <= xinters){
                      inside = !inside;
                  }
              }
          }
      }
      p1=p2;
  }
  return inside;
}



void LoopInXYPlane::setZ(float z) {
  for (int i = 0; i < points.size(); ++i) {
    points[i].z = z;
  }
}


void LoopInXYPlane::findInnerSegments(
        const Line& segment,
        QVector<Line>* inner_segments) const {

  // we can't do anything with an incomplete loop
  if (!(points.size() >= 3) ) return;

  // must have an output location
  if (!(inner_segments != NULL) ) return;

  // Worklist algorithm for finding inner segments:
  // 1. Add input segment to worklist
  // 2. Create an empty finalized list
  // 3. Get the next element from the worklist
  // 4. For each edge in the loop, test for intersection with
  //    the current element.
  // 5. If there was an intersection and the point of intersection
  //    is not one of the segment's end-points, split the segment
  //    at the point of intersection and add both to the worklist.
  // 6. If there was no such intersection, add the segment to
  //    the finalized list.
  // 7. Go to (2) if the worklist still has elements in it
  // 8. For each entry in the finalized list, test its midpoint to see
  //    if it is inside the loop.  If it is not, discard the segment.

  QVector<Line> worklist;
  worklist.append(segment);
  inner_segments->clear();

  while (worklist.isEmpty() == false) {
    const Line& current_segment = worklist.at(0);
    bool intersected = false;
    for (int i = 1; i < points.size() + 1; ++i) {
      Line loop_segment(points[i-1], points[i%points.size()]);
      Vector3 intersection;
      intersected = current_segment.intersectSegmentWithSegment2DXY(loop_segment, &intersection);
      intersection.z = current_segment.a.z;
      intersected = intersected && !current_segment.isEndpoint(intersection);
      if (intersected) {
        // split the segment in two
        Line first(current_segment.a, intersection);
        Line second(intersection, current_segment.b);
        worklist.append(first);
        worklist.append(second);
        break;
      }
    }
    if (!intersected) {
      // this segment is finalized
      inner_segments->append(current_segment);
    }
    worklist.remove(0);
  }

  // Iterate through and remove segments that are not inside the loop.
  // Go backward so that removing elements doesn't change our iterator.
  for (int i = inner_segments->size() - 1; i >= 0; --i) {
    if (!pointInside(inner_segments->at(i).center())) {
      inner_segments->remove(i);
    }
  }
}


bool LoopInXYPlane::crossedBy(const Line& segment) const {
  for (int i = 1; i < points.size() + 1; ++i) {
    Line current_segment(points[i-1], points[i%points.size()]);
    Vector3 pt;
    if (current_segment.intersectSegmentWithSegment2DXY(segment, &pt)/* &&
        !segment.isEndpoint(pt)*/) {
      return true;
    }
  }

  // doesn't cross any boundary
  return false;
}


float LoopInXYPlane::distanceToPoint(Vector3 point){
    Float min = 100000;
    for (int i = 1; i < points.size() + 1; ++i) {
      Line current_segment(points[i-1], points[i%points.size()]);
      Float f = current_segment.segmentDistanceTo2DXY(point);
      min = std::min(f,min);
    }
    return min;
}

bool LoopInXYPlane::intersect(const Line& segment, QVector<Vector3>* results) const{
  for (int i = 1; i < points.size() + 1; ++i) {
    Line current_segment(points[i-1], points[i%points.size()]);
//    Vector3 pt;
    Vector3 result;
    if (current_segment.intersectSegmentWithSegment2DXY(segment, &result)/* &&
        !segment.isEndpoint(pt)*/) {

        float x = result.x-current_segment.b.x;
        float y = result.y-current_segment.b.y;
        float r = sqrt(x*x+y*y);

        float dx = current_segment.a.x-current_segment.b.x;
        float dy = current_segment.a.y-current_segment.b.y;
        float dz = current_segment.a.z-current_segment.b.z;
        float dr = sqrt(dx*dx+dy*dy);

        if(dr<0.00001){
            result.z= current_segment.b.z;
//            return true;
        }
        result.z = current_segment.b.z + dz/dr*r;
        results->append(result.copy());
//        return true;
    }
  }
  // doesn't cross any boundary
//  result->x=0;
//  result->y=0;
//  result->z=0;
  return results->size()>0;
}

bool LoopInXYPlane::contains(const LoopInXYPlane& other) const {
  for (int i = 1; i < points.size(); ++i) {
    Line border_segment(points[i-1], points[i%points.size()]);

    // check to make sure every vertex is inside the other loop
    if (!other.pointInside(border_segment.a)) return false;

    // be sure this segment doesn't intersect the other loop
    if (other.crossedBy(border_segment)) return false;
  }

  return true;
}


void LoopInXYPlane::expand(Float amount,
                           QVector<LoopInXYPlane>* expanded_loops) const {
  LoopInXYPlane simplified;
  simplified.points += points;
  simplified.simplifyAndExpand(amount, expanded_loops);
}




void LoopInXYPlane::simplify() {
  int points_at_start;
  do {
    points_at_start = points.size();

    // Is this a valid loop?
    if (points_at_start < 3) return;

    // Remove one of a pair of consecutive points that are the same for each
    // pair.  Start with "last" pointing to the end of the array since this
    // is a loop.
    int last = -1;
    for (int i = 0; i < points.size(); ++i) {
      int last_index = last < 0 ? last + points.size() : last;
      if (points.at(i).equals(points.at(last_index))) {
        points.remove(i);
        i = i - 1; // increments next loop
      } else {
        last = i;
      }
    }
    for (int i = 2; i < points.size(); ++i) {
      int low = i - 2;//0 1 2 3 4 5,
      int high = i;
      // find the last point 'high' for which all points
      // [low + 1, ... , high - 1] are on the line segment (low, high)
      for (; high < points.size(); ++high) {
        Line line(points.at(low), points.at(high));
        bool all_on_segment = true;
        for (int k = low + 1; all_on_segment && k < high; ++k) {
          all_on_segment = line.pointOnSegment(points.at(k));
        }
        if (!all_on_segment) {
          --high; // the highest was actually the one before this one
          // delete points between, but not including, low and high
          points.remove(low + 1, high - low - 1);
          break;
        }
      }

    }
  } while (points_at_start != points.size());
}

Vector3 LoopInXYPlane::center(){
    Vector3 cent(0,0,0);
    foreach(Vector3 pt, points){
        cent.x=cent.x+pt.x;
        cent.y=cent.y+pt.y;
    }
    cent.x=cent.x/points.size();
    cent.y=cent.y/points.size();
    return cent;
}

void LoopInXYPlane::simplifyAndExpand(Float amount, QVector<LoopInXYPlane>* expanded_loops) {
  simplify();

  // Don't need to do anything if not expanding
  if (Math::floatsEqual(amount, 0)) return;

  QVector<Vector3>new_points;
  new_points += points;

  for (int i = 0; i < points.size(); ++i) {
    int before = i - 1;
    if (before == -1) before = points.size() - 1;
    int after = i + 1;
    if (after == points.size()) after = 0;

    Line before_segment(points[before], points[i]);
    Line after_segment(points[i], points[after]);

    Vector3 before_ray = before_segment.ray();
    Vector3 after_ray  = after_segment.ray();

    // form the perpendicular displacement vector
    Vector3 before_displacement;
    before_displacement.x =  before_ray.y;
    before_displacement.y = -before_ray.x;
    before_displacement.z = 0.0;
    before_displacement.normalize();

    Vector3 after_displacement;
    after_displacement.x =  after_ray.y;
    after_displacement.y = -after_ray.x;
    after_displacement.z = 0.0;
    after_displacement.normalize();

    // sum the displacements and adjust the current vertex
    Vector3 pt_displacement;
    pt_displacement.set(before_displacement).add(after_displacement);
    pt_displacement.normalize().scale(amount);
    new_points[i].add(pt_displacement);
  }
/*
  for (int i = 0; i < points.size(); ++i) {
    int before = i - 1;
    if (before == -1) before = points.size() - 1;
    int after = i + 1;
    if (after == points.size()) after = 0;

    Line before_segment(points[i], points[before]);
    Line after_segment(points[i], points[after]);

    Vector3 before_ray = before_segment.ray();
    before_ray.normalize();
    Vector3 after_ray  = after_segment.ray();
    after_ray.normalize();

    Vector3 average;
    average.set(before_ray).add(after_ray).scale(0.5);

    Vector3 disp;
    disp.set(average).add(points[i]);

    if (pointInside(disp)) {
      new_points[i].sub(average.scale(amount));
    } else {
      new_points[i].add(average.scale(amount));
    }
  }
*/
  // Create new points at every location where the loop
  // self-intersects.
  { // Creating new points creates new segments, which could each intersect
    // with other segments and in turn create more points.  The 'filling' variable
    // is set each loop that  the process changes the set of points.
    bool filling;
    do { // terminates once we scan the points and find no intersections
      filling = false;
      for (int i = 0; i < new_points.size(); ++i) {
        Line segment(new_points[i], new_points[(i+1)%new_points.size()]);
        Vector3 pt;
        for (int j = i+1; j < new_points.size(); ++j) {
          Line test_segment(new_points[j], new_points[(j+1)%new_points.size()]);
          if (segment.intersectSegmentWithSegment2DXY(test_segment, &pt) &&
              !segment.isEndpoint(pt) && !test_segment.isEndpoint(pt)) {
            new_points.insert(j+1, pt);
            new_points.insert(i+1, pt); // ORDER MATTERS! j > i
            filling = true;
            break;
          }
        }
      }
    } while (filling);
  }

  // next, create loops by pruning out newly created loops
  QVector<LoopInXYPlane> loops;
  {
    LoopInXYPlane new_loop;
    new_loop.points += new_points;
    new_points.clear();
    loops.append(new_loop);
  }

  // TODO: speed could be improved by not restarting every time
  //       a loop is pruned off
RESTART_PRUNE:
  for (int i = 0; i < loops.size(); ++i) {
    LoopInXYPlane& loop = loops.operator [](i);
    int loop_size = loop.points.size();
    for (int j = 0; j < loop_size; ++j) {
      const Vector3& j_point = loop.points.at(j);
      for (int k = j+1; k < loop_size; ++k) {
        const Vector3& k_point = loop.points.at(k);
        if (j_point.equals(k_point)) {
          // form a new loop between these points
          LoopInXYPlane new_loop;
          for (int l = j; l < k; ++l) {
            new_loop.add(loop.points.at(l));
          }
          // remove the new loop's points from the original loop;
          // however, leave one copy (at k) of the duplicate point
          // in the original loop.  This must be done *before*
          // the new loop is added, since appending the loop
          // could change the referenced location
          loop.points.remove(j, k - j);
          loops.append(new_loop);
          goto RESTART_PRUNE;
        }
      }
    }
  }

  // now that we have a set of loops, remove any loops that have the
  // opposite sense of our own loop since these are invalid
  Sense my_sense = sense();
  for (int i = loops.size() - 1; i >= 0; --i) {
    if (loops.at(i).sense() != my_sense) {
      loops[i].reverse();
    }
  }

  // Finally, get rid of loops that were generated in error.  I'm
  // not quite sure if this is a correct algorithm, but it should
  // work as long as the displacement amount isn't huge.
  // 1. find the biggest loop (greatest area loop) in the list
  // 2. remove all loops that share a vertex with that loop
  // 3. remove the loop and add it to the output list
  // 4. repeat until there are no more loops


  // The below code doesn't actually implement the above algorithm; it's
  // set up to do so, but the algorithm written here simply takes the
  // biggest remaining loop and is done with it.  This means that areas
  // connected by a narrow passage will NOT be correctly contracted, since
  // only one of the two regions will be returned.
  while (!loops.isEmpty()) {
    int biggest_loop = 0;
    float biggest_area = loops.at(0).areaEstimate();
    for (int i = 1; i < loops.size(); ++i) {
      // find a bigger loop
      const LoopInXYPlane& bigger_loop = loops.at(i);
      float area = bigger_loop.areaEstimate();
      if (area > biggest_area) {
        biggest_area = area;
        biggest_loop = i;
      }
    }
    expanded_loops->append(loops.at(biggest_loop));
    loops.clear();
  }
}


}
