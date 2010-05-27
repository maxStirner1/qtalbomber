#include "game/player.h"

Player::Player() {
    initPlayer();
}

Player::Player(QString pseudo, QColor color, int type) {
    m_playerPseudo = pseudo;
    m_playerType = type;
    setPlayerColor(color);
    initPlayer();
}


/*! Initialize player attributes.
 *
 *  Set the default value for the bonus and set set the size of the graphics item.
 */
void Player::initPlayer() {
    setRect(0, 0, BLOCK_WIDTH, BLOCK_HEIGHT);
    m_playerKick = false;
    m_playerAlive = true;
    m_bombCapacity = 1;
    m_bombRange = 3;
    m_playerPosition = 0;
}

/*! Setter for the player color.
 *
 *  @param color Color of the player.
 *
 *  This method also set a linear gradient and set it in the graphic item brush.
 *
 *  @see Player::playerColor
 */
void Player::setPlayerColor(QColor color) {
    QLinearGradient gradient(0, 0, BLOCK_WIDTH, BLOCK_HEIGHT);
    gradient.setColorAt(0, QColor::fromRgb(color.red()/2, color.green()/2, color.blue()/2));
    gradient.setColorAt(1, QColor::fromRgb(color.red(), color.green(), color.blue()));
    setBrush(QBrush(gradient));
    m_playerColor = color;
}

/*! Getter for the player color.
 *
 *  @return The player color.
 *
 *  @see Player::setPlayerColor
 */
QColor Player::playerColor() {
    return m_playerColor;
}

/*! Set the player type.
 *
 *  @param type Type of the player
 *
 *  The type of the playe define which kind of player it is (bot, real, ...).
 *  The value of the type is not verify.
 *
 *  @see Player::playerType
 */
void Player::setPlayerType(int type) {
    m_playerType = type;
}

/*! Player type getter.
 *
 *  @return The type of the player.
 *
 *  @see Player::setPlayerType
 */
int Player::playerType() {
    return m_playerType;
}

/*! Set the state of the player
 *
 *  @param alive Player alive or not.
 *
 *  If the value is 0, player is dead.
 *  The player is alive with a value of 1.
 *  The god mode can be set with a value > 1
 *
 *  @see Player::isAlive
 */
void Player::playerAlive(int alive) {
    m_playerAlive = alive;
}

/*! Status of the player.
 *
 *  @return If the player is alive or not/
 *
 *  @see Player::playerAlive
 */
bool Player::isAlive() {
    return m_playerAlive > 0;
}

/*! Set the player pseudo
 *
 *  @param pseudo Player pseudo
 *
 *  @see Player::playerPseudo
 */
void Player::setPlayerPseudo(QString pseudo) {
    m_playerPseudo = pseudo;
}

/*! Getter of the player pseudo
 *
 *  @return Player pseudo
 *
 *  @see Player::setPlayerPseudo
 */
QString Player::playerPseudo() {
    return m_playerPseudo;
}

/*! Bomb capacity setter.
 *
 *  @param capacity Number of bombe
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

/*! Toggle the kick bonus.
 *
 *  @see Player::hasKick
 */
void Player::togglePlayerKick() {
    m_playerKick = !m_playerKick;
}

/*! Kick bonus available for the player
 *
 *  @return Bonus enable for the player
 *
 *  @see Player::togglePlayerKick
 */
bool Player::hasKick() {
    return m_playerKick;
}

/*! Set the number of life of the player
 *
 *  @param lifes Player lifes
 *
 *  @see Player::playerLifes
 */
void Player::setPlayerLifes(int lifes) {
    m_playerLifes = lifes;
}

/*! Number of life of a player
 *
 *  @return Player life
 *
 *  @see Player::setPlayerLifes
 */
int Player::playerLifes() {
    return m_playerLifes;
}
