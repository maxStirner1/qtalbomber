/*
 *  QtalBomber - A Qt BomberMan game <http://www.bombermanproject.com/>
 *  Copyright (C) 2010  QtalBomber team <team@bombermanproject.com>
 *
 *  Authors: Quentin Amoudruz
 *           Thomas Savineau
 *           Alexandre Joseph
 *           Lucas Courot
 *
 *  File:        Map.h
 *  Description: Header file for GameUI class.
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

#ifndef _MAP_H
#define	_MAP_H

#include <QtGui>
#include <QDebug>

class Map : public QObject {
public:
    Map(QObject* parent = 0);
    Map(QString content, int height, int width, QObject *parent = 0);
    int width();
    int heigh();
    QList<int> collisionsTab();
    QList<int> unbreakableBlocks();
    QList<int> breakableBlocks();
    QList<int> bonusPositions();
    QList<int> playersPositions();
    void setPlayersPositions(QList<int> playerTab);
    void setPlayerPosition(int player, int position);
    void setPlayerPosition(int player, int widthPos, int heightPos);
    void setMapWidth(int width);
    void setMapHeight(int height);
    void setCollisionsTab(QList<int> collisionsTab);
    void setCollisionsTab(int position, int value);
    void setUnbreakablesBlocks(QList<int> unbreakablesBlocks);
    void setUnbreakablesBlocks(int position, int value);
    void setBreakablesBlocks(QList<int> breakablesBlocks);
    void setBreakablesBlocks(int position, int value);
    void setBonusPositions(QList<int> bonusPositions);
    void setBonusPositions(int position, int value);
    void DEBUG_displayInfos();
    void DEBUG_displayMap();

private:
    void computeSize();
    void appendBorders();
    QString m_content;
    int m_height;
    int m_width;
    bool m_valid;
    QList<int> m_collisionsTab;
    QList<int> m_unbreakablesBlocks;
    QList<int> m_breakableBlocks;
    QList<int> m_bonusPositions;
    QList<int> m_playersPositions;
};

#endif	/* _MAP_H */

