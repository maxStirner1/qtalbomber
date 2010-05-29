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
#define BLOCK_WIDTH 24

/// Game map blocks height
#define BLOCK_HEIGHT 24

/// Space between blocks
#define BLOCK_SPACING 4

/// Player status
#define PLAYER_NONE -1
#define PLAYER_DEAD 0
#define PLAYER_ALIVE 1

/// Game state
#define GAME_PENDING 0
#define GAME_RUNNING 1
#define GAME_FINISHED 2

#endif	/* _QTALBOMBER_H */

