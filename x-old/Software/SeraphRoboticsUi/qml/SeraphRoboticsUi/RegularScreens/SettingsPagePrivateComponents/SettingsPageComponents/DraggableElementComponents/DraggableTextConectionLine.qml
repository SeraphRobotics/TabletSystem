import QtQuick 2.4

Rectangle {
    id: descriptionConnectionElement

    property alias x1: descriptionConnectionElement.x
    property alias y1: descriptionConnectionElement.y

    property Item currentElement: null
    property Item typeNameText: null
    property Scale m_scale: null

    property real x2: (parent.height/2)
    property real y2: (parent.width/2)

    x1: currentElement.labelDirection === "left" ? typeNameText.x+typeNameText.width : typeNameText.x
    y1:  (typeNameText.y+(typeNameText.height/2)*(1/m_scale.yScale))

    color : "#666666"
    height: 2
    smooth: true;
    visible : settingsPageManager.modificationState === "3dReview" ? true : false

    transformOrigin: Item.TopLeft;

    width: getWidth(x1,y1,x2,y2);
    rotation: getSlope(x1,y1,x2,y2);

    function getWidth(sx1,sy1,sx2,sy2)
    {
        var w=Math.sqrt(Math.pow((sx2-sx1),2)+Math.pow((sy2-sy1),2));
        return w;
    }

    function getSlope(sx1,sy1,sx2,sy2)
    {
        var a,m,d;
        var b=sx2-sx1;
        if (b===0)
            return 0;
        a=sy2-sy1;
        m=a/b;
        d=Math.atan(m)*180/Math.PI;

        if (a<0 && b<0)
            return d+180;
        else if (a>=0 && b>=0)
            return d;
        else if (a<0 && b>=0)
            return d;
        else if (a>=0 && b<0)
            return d+180;
        else
            return 0;
    }
}
