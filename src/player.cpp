#include "player.h"

/*! Create a new player and initialize his attributes.
 *
 *  @param playerId Identifier of the player
 */
Player::Player(int playerId) {
    setRect(0, 0, BLOCK_WIDTH, BLOCK_HEIGHT);
    setVisible(false);
    m_playerId = playerId;
    m_playerKick = false;
    m_playerStatus = PLAYER_NONE;
    m_bombCapacity = 1;
    m_bombRange = 3;
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
void Player::setPlayerStatus(int status) {
    m_playerStatus = status;

    if (m_playerStatus >= 0)
        setVisible(true);
}

/*! Status of the player.
 *
 *  @return PlayerState
 *
 *  @see Player::setPlayerStatus
 */
int Player::playerStatus() {
    return m_playerStatus;
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
