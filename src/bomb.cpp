#include "bomb.h"

/*! Create a new bomb.
 *
 *  Until the Bomb::dropBomb function is called, the countdown of the bomb
 *  will not start.
 *
 *  @param playerId Identifier of the player who drop the bomb.
 *  @param parent Gamefield on which the bomb is dropped.
 *
 */
Bomb::Bomb(int playerId, int position) {
    setRect(MAP_BLOCK_WIDTH/4, MAP_BLOCK_HEIGHT/4, MAP_BLOCK_WIDTH/2, MAP_BLOCK_HEIGHT/2);
    
    m_bombColored = false;
    m_blinkCounter = 0;
    m_owner = playerId;
    m_bombGridPosition = position;

    setPen(QPen(Qt::black, 2));
    setBrush(Qt::black);
}

/*! Make the bomb blink at regular interval.
 *
 *  @see Bomb::dropBomb
 */
void Bomb::blink() {
    if (!m_bombColored) {
        if (m_blinkCounter < 6)
            setBrush(Qt::yellow);
        else if (m_blinkCounter < 11)
            setBrush(QColor::fromRgb(255, 150, 0));
        else if (m_blinkCounter < 16)
            setBrush(Qt::red);
        else {
            QTimer* timer = qobject_cast<QTimer *>(sender());
            if (!timer)
                return;

            timer->stop();
        }
    }
    else
        setBrush(Qt::black);

    m_bombColored= !m_bombColored;
    m_blinkCounter++;
}

/*! Drop the bomb on the gamefield.
 *
 *  This function start the countdown of the bomb explosion and the blinking
 *  signal.
 */
void Bomb::drop() {
    QTimer* blinkTimer = new QTimer(this);
    blinkTimer->setInterval(BOMB_BLINK_INTERVAL);

    connect(blinkTimer, SIGNAL(timeout()), this, SLOT(blink()));
    blinkTimer->start();

    QTimer::singleShot(BOMB_COUNTDOWN_TIME*1000, this, SLOT(explode()));
}

/*! Identifier of the player who drop the bomb.
 *
 *  @return Player identifier.
 */
int Bomb::playerId() {
    return m_owner;
}

/*! Make the bomb blow up
 *
 */
void Bomb::explode() {
    emit blowUp(m_bombGridPosition);
}