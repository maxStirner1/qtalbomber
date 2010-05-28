
#ifndef _PLAYER_H
#define	_PLAYER_H

#include <QtGui/QGraphicsEllipseItem>
#include <QtCore/QString>
#include <QtGui/QColor>
#include <QtGui/QBrush>
#include <QtGui/QLinearGradient>
#include "qtalbomber.h"

class Player : public QGraphicsEllipseItem {
public:
    Player();
    Player(QString pseudo, QColor color, int type);
    void setPlayerPseudo(QString pseudo);
    QString playerPseudo();
    void setPlayerColor(QColor color);
    QColor playerColor();
    void playerAlive(int alive);
    bool isAlive();
    void setPlayerType(int type);
    int playerType();
    void setBombCapacity(int capacity);
    int bombCapacity();
    void togglePlayerKick();
    bool hasKick();
    void setPlayerLifes(int lifes);
    int playerLifes();

private:
    /// Player pseudo
    QString m_playerPseudo;
    /// Player status
    int m_playerAlive;
    /// Player type (real, bot)
    int m_playerType;
    /// Player color
    QColor m_playerColor;
    /// Number of bomb which can be dropped by the user
    int m_bombCapacity;
    /// Bombs range
    int m_bombRange;
    /// Kick bonus
    bool m_playerKick;
    /// Number of life of the player
    int m_playerLifes;

    void initPlayer();
    int heightFromPos(int pos);
    int widthFromPos(int pos);
};

#endif	/* _PLAYER_H */

