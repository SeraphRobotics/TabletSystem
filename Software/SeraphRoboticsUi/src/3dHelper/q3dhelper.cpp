#include <QDebug>
#include "q3dhelper.h"


Q3DHelper::Q3DHelper(QObject *parent)
    : QObject(parent)
    , m_node(NULL)
{

}

Q3DHelper::~Q3DHelper()
{

}

void Q3DHelper::setNode(QQuickMesh *n)
{
    if (n == NULL)
    {
        DBG_INFO << "Mesh is NULL!";
        return;
    }
    m_node = n->getSceneObject();
}

QVariant Q3DHelper::boundingBoxCenter() const
{
    // Used for center
    if (m_node == NULL)
    {
        DBG_INFO << "Mesh is NULL!";
        return QVariant();
    }
    QVector3D center(m_node->boundingBox().center());
    center.setY(m_node->boundingBox().maximum().y()*0.65);
    center.setX(m_node->boundingBox().maximum().x()*0.65);
    return center;
}

QVariant Q3DHelper::boundingBoxFrontCenter() const
{
    // Used for eye position
    if (m_node == NULL)
    {
        DBG_INFO << "Mesh is NULL!";
        return QVariant();
    }

    QBox3D bbox(m_node->boundingBox());

    float z = qMax(
                qMax( bbox.size().x(), bbox.size().y() ),
                bbox.size().z());
    z += bbox.maximum().z();
    QVector3D frontCenter(0.0, 0.0, 2*z);
    return frontCenter;
}

QVariant Q3DHelper::boundingBoxTrueCenter() const
{
    if (m_node == NULL)
    {
        DBG_INFO << "Mesh is NULL!";
        return QVariant();
    }
    QVector3D center(m_node->boundingBox().center());
    return center;
}
