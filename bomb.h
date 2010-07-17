
#ifndef BOMB_H
#define BOMB_H

#include <QtGui>
#include <QTimer>
#include <QObject>
#include "constant.h"

/*! Create a new bomb on the gamefield.
 *
 */
class Bomb : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT

private:
	/// If the bomb has exploded
    bool m_exploded;
	/// Color status of the bomb
    bool m_bombColored;
	/// Counter for the bomb blink
    int m_blinkCounter;
	/// Identifier of the player who drop the bomb
    int m_ownerId;
	/// Calls at regular intervals the function which manage bomb's blinks
    QTimer *m_blinkTimer;

public slots:
    void dropBomb();
    void blink();
    void explode();

signals:
    void disappear();
    void blowUp();

public:
    Bomb(int playerID);
    ~Bomb();
    int getOwnerId();

};

#endif // BOMB_H
