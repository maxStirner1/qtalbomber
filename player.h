
#ifndef PLAYER_H
#define PLAYER_H
#include <QtGui>
#include <QTimer>
#include <QObject>
#include "constant.h"

class Player : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

private:
	/// Player ID
    int m_playerID;
	/// Player last direction
    int m_lastDirection;
	///  Alternate sprite pictures for players movement animation
    int m_actualPix;
	/// Alternate sprite pictures for players death animation
    int m_usedDeathPix;
	/// Is player alive
    bool m_playerAlive;
	/// Is player present in the actual round
    int m_playerStatus;
	/// Number of bombs that the player is allowed to simultanosly drop
    int m_bombCapacity;
	/// Length of the bombs flam
    int m_bombRange;
	/// If player kick is actif
    bool m_playerKick;
	/// Player lifes
    int m_playerLifes;
	/// Number of bombs currently droped
    int m_currentBombCapacity;
    void initPlayer();

private slots:
	/// Triggers animation of player death	
    void deathAnimation();

public:
    Player(int number);
    void setPlayerAttributes(QList<int> bonusAttribute);
    void setPlayerAlive(bool alive);
    bool isAlive();
    void setStatus(int type);
    int status();
    int bombRange();
    void setBombRange(int range);
    void setBombCapacity(int capacity);
    int bombCapacity();
    void togglePlayerKick();
    bool hasKick();
    void setPlayerLifes(int lifes);
    int lifes();
    void removeLifes();
    int playerID();
    int currentBombCapacity();
    void setCurrentBombCapacity(int bombCap);
    void addCurrentBombCapacity();
    void removeCurrentBombCapacity();
    void startDeathAnimation();
    void setPix(int dir);
};

#endif // PLAYER_H
