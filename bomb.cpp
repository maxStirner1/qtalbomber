
#include "bomb.h"

/*!
* Instanciates a bomb
* @param playerID ID of player who drops the bomb
*/
Bomb::Bomb(int playerID) {
    setRect(BLOCK_WIDTH/3, BLOCK_HEIGHT/3, BLOCK_WIDTH/3, BLOCK_HEIGHT/3);
    m_ownerId = playerID;
    m_bombColored = false;
    m_exploded = false;
    m_blinkCounter = 0;
    QPen pen(Qt::black, 2);
    setPen(pen);
    setBrush(Qt::black);
    setZValue(-1);
}

Bomb::~Bomb() {
}

/*! Manages blinking of bombs ( 5 yellow, 5 orange, 5 red )
* 
*/
void Bomb::blink() {
    if (!m_bombColored) {
	if(m_blinkCounter <= 5) {
	    setBrush(Qt::yellow);
	    m_bombColored = true;
	    m_blinkCounter++;
	} else if (5 < m_blinkCounter && m_blinkCounter <= 10) {
	    setBrush(QColor::fromRgb (255, 150, 0));
	    m_bombColored = true;
	    m_blinkCounter++;
	} else if (m_blinkCounter < 15){
	    setBrush(Qt::red);
	    m_bombColored = true;
	    m_blinkCounter++;
	} else {
	    m_blinkTimer->stop();
	}
    } else {
	setBrush(Qt::black);
	m_bombColored = false;
	m_blinkCounter++;
    }
}

/*!
* Creates a blinkTimer and display the bomb on the player position
*/
void Bomb::dropBomb() {

    m_blinkTimer = new QTimer(this);
    m_blinkTimer->setInterval(BLINK_INTERVAL);
    m_blinkTimer->start();
    connect(m_blinkTimer, SIGNAL(timeout()), this, SLOT(blink()));

    QTimer::singleShot(BOMB_TIMER, this, SLOT(explode()));
}

/*!
* Triggers the sound of explosion, hide the bomb and trigger flams
*/
void Bomb::explode() {

    if(m_exploded) {
	return;
    }
    emit blowUp();
    QSound* explosionBomb = new QSound(tr("%1/Explosion.wav").arg(SOUNDS_DIR), this);
    explosionBomb->setLoops(1);
    explosionBomb->play();
    setVisible(false);
    m_exploded = true;
    emit disappear();
}

int Bomb::getOwnerId() {
    return m_ownerId;
}

