
#include "player.h"

/*!
* Instanciates a player and initialize attributs
* @param number Player ID
*/
Player::Player(int number)
{
    m_playerID = number;
    m_actualPix = 0;
    m_playerKick = false;
    m_playerAlive = false;
    m_bombCapacity = BOMB_CAPACITY;
    m_currentBombCapacity = 0;
    m_bombRange = BOMB_RANGE;
    m_playerLifes = LIFE;
    m_playerStatus = PLAYER_OFF;
    m_usedDeathPix = 0;
    setPix(MOVE_DOWN);
    setTransformOriginPoint(boundingRect().center());
    setPos(0, 0);
}

/*!
* Chooses appropriate picture for player direction movement
*/
void Player::setPix(int dir) {
    setPixmap(QPixmap(tr(":/player-%1-%2-%3").arg(m_playerID).arg(dir).arg(m_actualPix)));
    m_actualPix = (m_actualPix) ? 0 : 1;
}

/*!
* Triggers deaths movement timer
*/
void Player::startDeathAnimation() {
    QTimer* deathTimer = new QTimer(this);
    deathTimer->setInterval(200);
    connect(deathTimer, SIGNAL(timeout()), this, SLOT(deathAnimation()));
    deathTimer->start();
}

/*!
* Manages death animation
*/
void Player::deathAnimation() {
    QTimer* deathTimer = qobject_cast<QTimer*>(sender());

    if (!deathTimer)
	return;

    if (m_usedDeathPix > 5) {
	deathTimer->stop();
	setZValue(-1);
	return;
    }

    setPixmap(QPixmap(tr(":/player-death-%1-%2").arg(m_playerID).arg(m_usedDeathPix)));
    //setRotation(60*m_usedDeathPix);
    m_usedDeathPix++;
}

/*!
* Affects bonuses to player
*/
void Player::setPlayerAttributes(QList<int> bonusAttributes) {

    m_bombCapacity += bonusAttributes[BOMB_CAPACITY_UP];
    m_bombRange += bonusAttributes[BOMB_RANGE_UP];

    if (m_bombCapacity < BOMB_CAPACITY) {
	m_bombCapacity = BOMB_CAPACITY;
    }
    if (m_bombRange < BOMB_RANGE) {
	m_bombRange = BOMB_RANGE;
    }
    if (m_playerKick == false && bonusAttributes[KICK_ACTIVATOR] > 0) {
	m_playerKick = true;
	qDebug() << "Bomb kick upgraded :" << bonusAttributes[KICK_ACTIVATOR];
    }
    if (m_playerLifes < 3 && bonusAttributes[LIFE_UP] > 0) {
	m_playerLifes += bonusAttributes[LIFE_UP];
	qDebug() << "Life upgraded :" << bonusAttributes[LIFE_UP];
    }
    qDebug() << "Player Stats list :";
    qDebug() << "Capacity :" << m_bombCapacity << "Range :" << m_bombRange << "Life :" << m_playerLifes << "Kick :" << m_playerKick;
}

void Player::setStatus(int type) {
    m_playerStatus = type;

    if (m_playerStatus)
	setVisible(true);
    else
	setVisible(false);
}

int Player::status() {
    return m_playerStatus;
}

void Player::setPlayerAlive(bool alive) {
    m_playerAlive = alive;
}

bool Player::isAlive() {
    return m_playerAlive;
}

/*! Bomb capacity setter.
 *
 *  @param capacity Number of bomber
 *
 *  Define the number of bombs that can be dropped
 *
 *  @see Player::bombCapacity
 */
void Player::setBombCapacity(int capacity) {
    m_bombCapacity = capacity;
}

/*! Number of bomb a player can drop.
 *
 *  @return Bomb capacity of a player
 *
 *  @see Player::setBombCapacity
 */
int Player::bombCapacity() {
    return m_bombCapacity;
}

void Player::togglePlayerKick() {
    m_playerKick = !m_playerKick;
}

bool Player::hasKick() {
    return m_playerKick;
}

void Player::setPlayerLifes(int lifes) {
    m_playerLifes = lifes;
}

int Player::lifes() {
    return m_playerLifes;
}

void Player::setBombRange(int range) {
    m_bombRange = range;
}

int Player::playerID() {
    return m_playerID;
}

int Player::currentBombCapacity() {
    return m_currentBombCapacity;
}

void Player::setCurrentBombCapacity(int bombCap) {
    m_currentBombCapacity = bombCap;
}

void Player::addCurrentBombCapacity() {
    m_currentBombCapacity++;
}

void Player::removeCurrentBombCapacity() {
    m_currentBombCapacity--;
}

int Player::bombRange() {
    return m_bombRange;
}

void Player::removeLifes() {
    m_playerLifes--;
}
