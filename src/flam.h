
#ifndef FLAM_H
#define FLAM_H

#include <QtGui>
#include <QTimer>
#include <QObject>
#include "constant.h"
#include "bomb.h"
#include <QtWidgets>
#include <QGraphicsRectItem>
#include <QSound>

/*!
* Manages flams
*/
class Flam : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT

private:
	/// If the flam is dangerous
    bool m_dangerous;
	/// If the flam has been collited
    bool m_hasCollided;
	/// Direction of the flam
    int m_direction;
	/// Current length of the flam
    int m_length;
	/// Max length of the flam
    int m_maxLength;
	/// Interval between two flam movements
    int m_flamInterval;
	/// Calls at regular intervals the function which manage flame's movements
    QTimer *flamTimer;

signals:
	/// Signal emitted from the flame for its disappearance
    void disappear();
	/// Signal emitted from the flame when it collided with something
    void collide();

public slots:
	/// Move of the flam
    void moveFlam();
	/// Display the flam and make it dangerous
    void triggerFlam();

public:
    Flam(int dir, int range);
    ~Flam();
    void setDangerousness(bool danger);
    bool isDangerous();
    void setHasCollided(bool collided);
    bool hasCollided();
    int direction();
};

#endif // FLAM_H
