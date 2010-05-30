#include "flam.h"
#include <QDebug>

Flam::Flam(int length, int direction, int position, QPointF pos) {
    m_range = length;
    m_currentRange = 0;
    m_direction = direction;
    m_position = position;

    // Set flam properties
    setRect(0, 0, MAP_BLOCK_WIDTH, MAP_BLOCK_HEIGHT);
    setPen(QPen(Qt::red, 2));
    setBrush(QColor::fromRgb (255, 150, 0));
    setPos(pos);

    // Start the flam stretching
    m_growTimer = new QTimer(this);
    m_growTimer->setInterval(FLAM_MOVE_INTERVAL);

    connect(m_growTimer, SIGNAL(timeout()), this, SLOT(stretchFlam()));

    m_growTimer->start();
}

/*! Move direction of the flam.
 *
 *  @return Move direction.
 */
int Flam::direction() {
    return m_direction;
}

/*! Flam position on the map grid.
 *
 *  @return Position of the flam.
 *
 *  @see Flam::setFlamPosition
 */
int Flam::position() {
    return m_position;
}

/*! Set the position of the flam.
 *
 *  @param position New position of the flam.
 *
 *  @see Flam::flamPosition
 */
void Flam::setPosition(int position) {
    m_position = position;
}

/*! Length of the flam.
 *
 *  @return Flam length.
 */
int Flam::currentRange() {
    return m_currentRange;
}

/*! Maximum length of the flam.
 *
 *  @return Flam maximum length.
 */
int Flam::range() {
    return m_range;
}

/*! Stretch the flam length
 *
 */
void Flam::stretchFlam() {
    m_currentRange++;

    if (m_currentRange > m_range)
        m_growTimer->stop();

    emit flamGrow();
}