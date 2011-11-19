#include "flam.h"

/*!
* Instanciates a flam
* @param dir direction
* @param range length of the flam
*/
Flam::Flam(int dir, int range)
{
    setRect(0, 0, BLOCK_WIDTH, BLOCK_HEIGHT);
    setPen(QPen(Qt::red, 2));
    setBrush(QColor::fromRgb (255, 150, 0));
    setVisible(false);
    m_dangerous = false;
    m_direction = dir;
    m_length = 0;
    m_maxLength = range*(BLOCK_SPACING+BLOCK_WIDTH);
}

Flam::~Flam() {
}

/*!
* Triggers flamTimer
*/
void Flam::triggerFlam() {
    setVisible(true);
    m_dangerous = true;

    QTimer *flamTimer = new QTimer(this);
    flamTimer->setInterval(FLAM_BLINK_INTERVAL);
    connect(flamTimer, SIGNAL(timeout()), this, SLOT(moveFlam()));
    flamTimer->start();
}

/*!
* Manages flams movement
* The flam emits a signal to know if it collides
* Prevents flams to outrun its ranges
* If the flam finished its course, it desappear
*/
void Flam::moveFlam() {
    QTimer *flamTimerCopy = qobject_cast<QTimer*>(sender());
    if (!flamTimerCopy)
	return;

    if (scenePos().x() < 0 || scenePos().y() < 0) {
	setVisible(false);
	return;
    }

    if(m_length <= m_maxLength) {
	emit collide();
	switch(m_direction)
	{
	case MOVE_UP:
	    moveBy(FLAM_MOVE_LENGTH,0);
	    break;
	case MOVE_DOWN:
	    moveBy(-FLAM_MOVE_LENGTH,0);
	    break;
	case MOVE_RIGHT:
	    moveBy(0,FLAM_MOVE_LENGTH);
	    break;
	case MOVE_LEFT:
	    moveBy(0,-FLAM_MOVE_LENGTH);
	    break;
	}
	m_length += FLAM_MOVE_LENGTH;
    } else {
	flamTimerCopy->stop();
	emit disappear();
    }
}

bool Flam::isDangerous() {
    return m_dangerous;
}

void Flam::setDangerousness(bool danger) {
    m_dangerous = danger;
}

bool Flam::hasCollided() {
    return m_hasCollided;
}

void Flam::setHasCollided(bool collided) {
    m_hasCollided = collided;
}

int Flam::direction() {
    return m_direction;
}
