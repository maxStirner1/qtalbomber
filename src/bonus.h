#ifndef BONUS_H
#define BONUS_H

#include <QtGui>
#include <QTimer>
#include <QObject>
#include "player.h"
#include "constant.h"
#include <QtWidgets>
#include <QSound>
#include <QGraphicsRectItem>

/*!
* Manages bonuses
*/
class Bonus : public QObject, public QGraphicsRectItem
{
    Q_OBJECT

private:
	/// Type of bonus
    int m_type;
	/// List of attributes (BOMB_RANGE_UP, BOMB_CAPACITY_UP, KICK_ACTIVATOR, GOD_MODE)
    QList<int> m_attributes;

public:
    Bonus(int type);
    ~Bonus();
    int type() const;
    QList<int> attributes();
};

#endif // BONUS_H
