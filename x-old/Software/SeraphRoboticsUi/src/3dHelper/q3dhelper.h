#ifndef Q3DHELPER_H
#define Q3DHELPER_H

#include <QQuickItem>
#include <QQuickMesh>
#include <QGLSceneNode>

#include <QQmlListProperty>
#include <QDebug>

#define DBG_INFO (qDebug()<<__FILE__<<":"<<__LINE__<<__FUNCTION__<<":")

/**
 * @brief The Q3DHelper class
 * Class to help getting the bounding box from stl file and importing it
 * to Qt Quick.
 */

class Q3DHelper : public QObject
{
    Q_OBJECT

public:
    explicit Q3DHelper(QObject *parent = 0);
    ~Q3DHelper();

    /**
      Load Qt3d mesh into QGLSceneNode.
      */
    Q_INVOKABLE void setNode(QQuickMesh *n);

    /**
      Functions used to find the bounding box of the current 3d Item
      @pre setNode()
      */
    Q_INVOKABLE QVariant boundingBoxCenter() const;
    Q_INVOKABLE QVariant boundingBoxFrontCenter() const;
    Q_INVOKABLE QVariant boundingBoxTrueCenter() const;

private:
    QGLSceneNode *m_node;
};

#endif // Q3DHELPER_H
