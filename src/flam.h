#ifndef _FLAM_H
#define	_FLAM_H

#include "qtalbomber.h"
#include <QTimer>
#include <QObject>
#include <QGraphicsEllipseItem>
#include <QPen>

class Flam : public QObject, public QGraphicsEllipseItem {
    Q_OBJECT

public:
    Flam(int length, int direction, int position, QPointF pos);
    int direction();
    int range();
    int currentRange();
    int position();
    void setPosition(int position);
    void triggerFlam();

signals:
    void flamGrow();

private slots:
    void stretchFlam();

private:
    /// Direction of the flam
    int m_direction;
    /// Current length of the flam
    int m_currentRange;
    /// Maximum length of the flam
    int m_range;
    /// Flam position on the map grid
    int m_position;
    /// Flam grow timer
    QTimer* m_growTimer;
};

#endif	/* _FLAM_H */

