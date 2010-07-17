/*
 *  QtalBomber - A Qt BomberMan game <http://www.bombermanproject.com/>
 *  Copyright (C) 2010  QtalBomber team <team@bombermanproject.com>
 *
 *  Authors: Quentin Amoudruz
 *           Thomas Savineau
 *           Alexandre Joseph
 *           Lucas Courot
 *
 *  File:        Player.h
 *  Description: Header file for Player class.
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


#ifndef _PLAYER_H
#define	_PLAYER_H

#include "constants.h"
#include <QtGui>

class Player : public QGraphicsEllipseItem {

public:
    Player();
    Player(QString pseudo, QColor color, QByteArray* avatar);
    void setPlayerPseudo(QString pseudo);
    void setPlayerColor(QColor color);
    void setPlayerAvatar(QByteArray* avatar);
    void setPlayerType(int type);
    void setPlayerPosition(int position);
    void setPlayerStatus(int status);
    QString playerPseudo();
    QColor playerColor();
    QByteArray playerAvatar();
    int playerType();
    int playerPosition();
    int playerStatus();

private:
    QString m_playerPseudo;
    QColor m_playerColor;
    QByteArray m_playerAvatar;
    int m_playerType;
    int m_playerPosition;
    int m_playerStatus;
};

#endif	/* _PLAYER_H */

