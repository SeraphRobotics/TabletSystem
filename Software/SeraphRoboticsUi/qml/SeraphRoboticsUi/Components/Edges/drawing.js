// drawing.js

// Helper function for drawSpline.
//   Based on http://scaledinnovation.com/analytics/splines/splines.html
function getControlPoints(x0, y0, x1, y1, x2, y2, t) {
    //  x0,y0,x1,y1 are the coordinates of the end (knot) pts of this segment
    //  x2,y2 is the next knot -- not connected here but needed to calculate p2
    //  p1 is the control point calculated here, from x1 back toward x0.
    //  p2 is the next control point, calculated here and returned to become the
    //  next segment's p1.
    //  t is the 'tension' which controls how far the control points spread.

    //  Scaling factors: distances from this knot to the previous and following knots.
    var d01 = Math.sqrt( Math.pow(x1-x0, 2) + Math.pow(y1-y0,2) );
    var d12 = Math.sqrt( Math.pow(x2-x1, 2) + Math.pow(y2-y1,2) );

    var fa = t*d01/(d01+d12);
    var fb = t - fa;

    var p1x = x1 + fa*(x0-x2);
    var p1y = y1 + fa*(y0-y2);

    var p2x = x1 - fb*(x0-x2);
    var p2y = y1 - fb*(y0-y2);

    return [p1x, p1y, p2x, p2y]
}

function drawSpline(ctx, pts, t) {
    var cp = [];   // array of control points, as x0,y0,x1,y1,...
    var n = pts.length;
    //   Append and prepend knots and control points to close the curve
    pts.push(pts[0], pts[1], pts[2], pts[3]);
    pts.unshift(pts[n-1]);
    pts.unshift(pts[n-1]);
    var i;
    for(i = 0; i < n; i += 2) {
        cp = cp.concat(getControlPoints(pts[i], pts[i+1], pts[i+2], pts[i+3], pts[i+4], pts[i+5], t));
    }
    cp = cp.concat(cp[0], cp[1]);
    ctx.moveTo(pts[2], pts[3])
    for(i = 2; i < n + 2; i += 2) {
        ctx.bezierCurveTo(cp[2*i-2], cp[2*i-1], cp[2*i], cp[2*i+1], pts[i+2], pts[i+3]);
        ctx.stroke();
    }
}


function drawEdge(ctx, centerX, centerY, radius) {
    var xStart = centerX,
            yStart = centerY,
            radiusStart = 0,
            xEnd = centerX,
            yEnd = centerY,
            radiusEnd = radius;

    var radgrad = ctx.createRadialGradient(xStart, yStart, radiusStart,
                                           xEnd, yEnd, radiusEnd);
    radgrad.addColorStop(0,    'rgba(255, 255, 255, 1)');
    radgrad.addColorStop(0.95, 'rgba(0, 0, 120, 1)');
    radgrad.addColorStop(0.99, 'rgba(0, 0, 120, 1)');
    radgrad.addColorStop(1,    'rgba(0, 0, 120, 0)');

    ctx.fillStyle = radgrad;

    var x1 = centerX-radius >= 0 ? centerX-radius : 0;
    var y1 = centerY-radius >= 0 ? centerY-radius : 0;

    ctx.fillRect(x1, y1, centerX+radius, centerY+radius);
}
