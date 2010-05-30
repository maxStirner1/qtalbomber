#include "player.h"

/*! Create a new player and initialize his attributes.
 *
 *  @param playerId Identifier of the player
 */
Player::Player(int playerId) {
    setRect(0, 0, MAP_BLOCK_WIDTH, MAP_BLOCK_HEIGHT);
    setVisible(false);
    m_playerId = playerId;
    m_kickBonus = false;
    m_status = PLAYER_OFF;
    m_bombCapacity = 1;
    m_bombsRange = 3;
}

/*! Identifier of the player
 *
 *  @return Player identifier
 */
int Player::playerId() {
    return m_playerId;
}

/*! Set the state of the player
 *
 *  Status significations:
 *   - -1: Player is none (not in game)
 *   -  0: Player is dead
 *   -  1: Player is alive
 *   - >1: Player is god (can't died)
 *
 *  @param status Player status
 *
 *  @see Player::playerStatus
 */
void Player::setState(int status) {
    m_status = status;

    if (m_status >= 0)
        setVisible(true);
    else
        setVisible(false);
}

/*! Status of the player.
 *
 *  @return PlayerState
 *
 *  @see Player::setPlayerStatus
 */
int Player::state() {
    return m_status;
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
void Player::toggleKickBonus() {
    m_kickBonus = !m_kickBonus;
}

/*! Kick bonus available for the player
 *
 *  @return Bonus enable for the player
 *
 *  @see Player::togglePlayerKick
 */
bool Player::hasKick() {
    return m_kickBonus;
}

/*! Set the number of life of the player
 *
 *  @param lifes Player lifes
 *
 *  @see Player::playerLifes
 */
void Player::setLifes(int lifes) {
    m_lifes = lifes;
}

/*! Number of life of a player
 *
 *  @return Player life
 *
 *  @see Player::setPlayerLifes
 */
int Player::lifes() {
    return m_lifes;
}

/*! Range of the player bombs.
 *
 *  @return Player bomb range.
 */
int Player::bombsRange() {
    return m_bombsRange;
}

/*! Set the player bomb range.
 *
 *  @param range Range of the bombs
 */
void Player::setBombsRange(int range) {
    m_bombsRange = range;
}
