#ifndef BOMB_H
#define BOMB_H

#include "qtalbomber.h"
#include <QObject>
#include <QGraphicsEllipseItem>
#include <QTimer>
#include <QPen>
#include <QBrush>
#include <QWidget>

/*! Create a new bomb on the gamefield.
 *
 */
class Bomb : public QObject, public QGraphicsEllipseItem {
    Q_OBJECT

public:
    Bomb(int playerId, int position);
    void drop();
    int playerId();

signals:
    void blowUp(int bombPosition);

public slots:
    void blink();
    void explode();
    
private:
    /// Color status of the bomb
    bool m_bombColored;
    /// Counter for the bomb blink
    int m_blinkCounter;
    /// Identifier of the player who drop the bomb
    int m_owner;
    /// Bomb position on the gamefield grid
    int m_bombGridPosition;
};

#endif // BOMB_H