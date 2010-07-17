/*
 *  QtalBomber - A Qt BomberMan game <http://www.bombermanproject.com/>
 *  Copyright (C) 2010  QtalBomber team <team@bombermanproject.com>
 *
 *  Authors: Quentin Amoudruz
 *           Thomas Savineau
 *           Alexandre Joseph
 *           Lucas Courot
 *
 *  File:        constants.h
 *  Description: Global constants of the game.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _CONSTANTS_H
#define	_CONSTANTS_H

#define MAPS_DIR "/maps"

// Game protocole definition
#define GAME_INFOS 1
#define JOIN_GAME 2
#define GAME_MAP 3
#define GAME_PLAYER 4
#define GAME_PLAYERS 5
#define GAME_START 6
#define PLAYER_EVENT 7
#define RIVAL_MOVE 8

// Settings
#define PLAYER_SETTINGS_MAGIC 0xA0B0C0D0
#define SERVER_SETTINGS_MAGIC 0xF0E0D0C0

// Map drawing
#define BLOCK_SPACING 4
#define BLOCK_HEIGHT 24
#define BLOCK_WIDTH 24

// In game status
#define PLAYER_REFUSED -1
#define PLAYER_NOT_CONNECTED 0
#define PLAYER_IN_GAME 1

// Player type
#define PLAYER_IS_NONE 0
#define PLAYER_IS_REAL 1
#define PLAYER_IS_SELF 2
#define PLAYER_IS_BOT 3

#define PLAYER_MOVE 28 // BLOCK_WIDTH + BLOCK_SPACING

// Player event value
#define MOVE_UP 1
#define MOVE_DOWN 2
#define MOVE_RIGHT 3
#define MOVE_LEFT 4
#define DROP_BOMB 5

// Game status
#define GAME_PENDING 0
#define GAME_STARTED 1
#define GAME_COUNTDOWN 2


#endif	/* _CONSTANTS_H */

