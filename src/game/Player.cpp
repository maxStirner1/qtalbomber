/*
 *  QtalBomber - A Qt BomberMan game <http://www.bombermanproject.com/>
 *  Copyright (C) 2010  QtalBomber team <team@bombermanproject.com>
 *
 *  Authors: Quentin Amoudruz
 *           Thomas Savineau
 *           Alexandre Joseph
 *           Lucas Courot
 *
 *  File:        Player.cpp
 *  Description: Manage player information.
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

#include "game/Player.h"

Player::Player() {
    setRect(0, 0, BLOCK_WIDTH, BLOCK_HEIGHT);
    qDebug() << pos();
}

Player::Player(QString pseudo, QColor color, QByteArray* avatar) {
    setRect(0, 0, BLOCK_WIDTH, BLOCK_HEIGHT);
    qDebug() << pos();
    m_playerPseudo = pseudo;
    m_playerColor = color;
    m_playerAvatar.clear();
    m_playerAvatar.append(avatar->data());
}

void Player::setPlayerPseudo(QString pseudo) {
    m_playerPseudo = pseudo;
}

void Player::setPlayerColor(QColor color) {
//    QRadialGradient gradient(BLOCK_WIDTH/2, BLOCK_HEIGHT/2, (BLOCK_HEIGHT*BLOCK_WIDTH)/((BLOCK_HEIGHT*BLOCK_WIDTH)/10), BLOCK_WIDTH/2, BLOCK_HEIGHT/2);
//    gradient.setColorAt(0, QColor::fromRgb(color.red()/4, color.green()/4, color.blue()/4));
    QLinearGradient gradient(0, 0, BLOCK_WIDTH, BLOCK_HEIGHT);
    gradient.setColorAt(0, QColor::fromRgb(color.red()/2, color.green()/2, color.blue()/2));
    gradient.setColorAt(1, QColor::fromRgb(color.red(), color.green(), color.blue()));
    setBrush(QBrush(gradient));
    m_playerColor = color;
}

void Player::setPlayerAvatar(QByteArray* avatar) {
    m_playerAvatar.clear();
    m_playerAvatar.append(avatar->data());
}

QString Player::playerPseudo() {
    return m_playerPseudo;
}

QColor Player::playerColor() {
    return m_playerColor;
}

QByteArray Player::playerAvatar() {
    return m_playerAvatar;
}

int Player::playerType() {
    return m_playerType;
}

void Player::setPlayerType(int type) {
    m_playerType = type;
}

int Player::playerPosition() {
    return m_playerPosition;
}

void Player::setPlayerPosition(int position) {
    m_playerPosition = position;
}
