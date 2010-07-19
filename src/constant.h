
#ifndef _CONSTANTS_H
#define	_CONSTANTS_H

#ifdef Q_WS_MAC
#define MAPS_DIR "../../../Maps"
#define SOUNDS_DIR "../../../Sounds"
#else
#define MAPS_DIR "Maps"
#define SOUNDS_DIR "Sounds"
#endif

// Map drawing
#define BLOCK_SPACING 4
#define BLOCK_HEIGHT 24
#define BLOCK_WIDTH 24

// Player event value
#define MOVE_UP 1
#define MOVE_DOWN 2
#define MOVE_RIGHT 3
#define MOVE_LEFT 4
#define DROP_BOMB 5

// Player default attributes
#define BOMB_CAPACITY 1
#define BOMB_RANGE 1
#define LIFE 1

// Game state
#define GAME_PENDING 0
#define GAME_RUNNING 1
#define GAME_PAUSED 2
#define GAME_FINISHED 3

// Head up display time
#define HUD_TIME 1000

// Bomb attributes
#define BOMB_TIMER 3000
#define BLINK_INTERVAL 200
#define ON_ROUND_TIMER 2000

// Bonus types
#define BOMB_RANGE_UP 1
#define BOMB_RANGE_DOWN 10
#define BOMB_CAPACITY_UP 2
#define BOMB_CAPACITY_DOWN 20
#define KICK_ACTIVATOR 3
#define LIFE_UP 4

// Player status
#define PLAYER_OFF 0
#define PLAYER_IN 1

#define PLAYER_NUMBER_MAX 4
#define PLAYER_NUMBER_MIN 2

#define PLAYER_MOVE_LENGTH 28

// Flams
#define FLAM_BLINK_INTERVAL 10
#define FLAM_MOVE_LENGTH 1

// Rounds
#define ROUNDS_MIN 1
#define ROUNDS_MAX 10
#define ROUNDS_DEFAULT 3

// XSD validation
#define SKIP_XSD_VALIDATION

#endif	/* _CONSTANTS_H */
