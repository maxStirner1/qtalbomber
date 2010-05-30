#ifndef _QTALBOMBER_H
#define	_QTALBOMBER_H

/*! Defines general game constantes
 *
 *  Make sure you know what you do when editing this file
 */

/// Maximum player number in a game
#define PLAYER_NUMBER_MAX 4

/// Minimum player number to start in a game
#define PLAYER_NUMBER_MIN 2

/// Maps directory
#define MAPS_DIRECTORY "maps"

/// Game map blocks width
#define MAP_BLOCK_WIDTH 24

/// Game map blocks height
#define MAP_BLOCK_HEIGHT 24

/// Space between blocks
#define MAP_BLOCK_SPACING 4

/// Player status
#define PLAYER_OFF -1
#define PLAYER_DEAD 0
#define PLAYER_ALIVE 1

/// Game state
#define GAME_PENDING 0
#define GAME_RUNNING 1
#define GAME_WAITING 2
#define GAME_FINISHED 3

/// Player move
#define MOVE_UP 1
#define MOVE_DOWN 2
#define MOVE_RIGHT 3
#define MOVE_LEFT 4
#define DROP_BOMB 5

/// Bomb explosion countdown (in second)
#define BOMB_COUNTDOWN_TIME 3

/// Bomb blink interval (in milisecond)
#define BOMB_BLINK_INTERVAL 200

/// Bomb flam move interval (in milisecond)
#define FLAM_MOVE_INTERVAL 100

/// Round text displayer time
#define DISPLAY_CURRENT_ROUND_TIME 1000

/// Skip map XSD validation at start up
#define SKIP_XSD_VALIDATION

#endif	/* _QTALBOMBER_H */
