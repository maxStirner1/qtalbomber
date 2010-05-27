#ifndef _QTALBOMBER_H
#define	_QTALBOMBER_H

/*! Defines general game constantes
 *
 *  Make sure you know what you do when editing this file
 */

/// Number of player of the game.
/// If changed remenber to modify default keys in multiplayerUI
#define PLAYER_NUMBER 4

/// Default player color. A Qt color constants
#define PLAYER_DEFAULT_COLOR "#00FF00"

/// Minimum number of player in the game
#define PLAYER_NUMBER_MIN 2

/// Directory of the maps
#define MAPS_DIRECTORY "maps"

/// Block width
#define BLOCK_WIDTH 24

/// Block height
#define BLOCK_HEIGHT 24

/// Space between blocks
#define BLOCK_SPACING 4

/// Player type
#define PLAYER_IS_REAL 1
#define PLAYER_IS_BOT 2
// Used only with network
#define PLAYER_IS_SELF 3

#endif	/* _QTALBOMBER_H */

