/*
 *  QtalBomber - A Qt BomberMan game <http://www.bombermanproject.com/>
 *  Copyright (C) 2010  QtalBomber team <team@bombermanproject.com>
 *
 *  Authors: Quentin Amoudruz
 *           Thomas Savineau
 *           Alexandre Joseph
 *           Lucas Courot
 *
 *  File:        Map.cpp
 *  Description: Game map manager.
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

#include "game/Map.h"

Map::Map(QObject* parent) : QObject(parent) {

}

Map::Map(QString content, int height, int width, QObject* parent) : QObject(parent) {
    m_content = content;
    m_height = height; 
    m_width = width;

    appendBorders();
    /**
     * # unbreakable
     * @ breakable
     * * bonus
     * [1-4] player
     */
    for (int i = 0; i < m_content.length(); i++) {
        if (m_content.at(i) == '_') {
            m_collisionsTab.append(0);
            m_unbreakablesBlocks.append(0);
            m_breakableBlocks.append(0);
            m_playersPositions.append(0);
            m_bonusPositions.append(0);
        }
        else if (m_content.at(i) == '#') {
            m_collisionsTab.append(1);
            m_unbreakablesBlocks.append(1);
            m_breakableBlocks.append(0);
            m_playersPositions.append(0);
            m_bonusPositions.append(0);
        }
        else if (m_content.at(i) == '@') {
            m_collisionsTab.append(1);
            m_unbreakablesBlocks.append(0);
            m_breakableBlocks.append(1);
            m_playersPositions.append(0);
            m_bonusPositions.append(0);
        }
        else if (m_content.at(i) == '*') {
            m_collisionsTab.append(0);
            m_unbreakablesBlocks.append(0);
            m_breakableBlocks.append(0);
            m_playersPositions.append(0);
            m_bonusPositions.append(1);
        }
        else  if (m_content.at(i).isDigit()) {
            m_collisionsTab.append(1);
            m_unbreakablesBlocks.append(0);
            m_breakableBlocks.append(0);
            // Perhaps substract one to the value to start at zero
            m_playersPositions.append(m_content.at(i).digitValue());
            m_bonusPositions.append(0);
        }
    }

    DEBUG_displayInfos();
}

void Map::appendBorders() {
    qDebug() << "Appending side border";
    int lines = m_content.count()/m_width;
    for (int i = 0; i < lines; i++) {
        m_content.insert(i*(m_width+2), "#");
        m_content.insert(i*(m_width+2)+m_width+1, "#");
    }

    m_width += 2;

    qDebug() << "Appending top and bottom border";
    for (int i = 0; i < m_width; i++) {
        m_content.insert(0, "#");
        m_content.append("#");
    }

    m_height += 2;
}

int Map::width() {
    return m_width;
}

int Map::heigh() {
    return m_height;
}

QList<int> Map::collisionsTab() {
    return m_collisionsTab;
}

QList<int> Map::unbreakableBlocks() {
    return m_unbreakablesBlocks;
}

QList<int> Map::breakableBlocks() {
    return m_breakableBlocks;
}

QList<int> Map::bonusPositions() {
    return m_bonusPositions;
}

QList<int> Map::playersPositions() {
    return m_playersPositions;
}

void Map::setPlayersPositions(QList<int> playerTab) {
    // Have to manually update collision tab in this case
    m_playersPositions.clear();
    m_playersPositions.append(playerTab);
}

void Map::setPlayerPosition(int player, int widthPos, int heightPos) {
    setPlayerPosition(player, widthPos*heightPos);
}

void Map::setPlayerPosition(int player, int position) {
    int oldPos = m_playersPositions.indexOf(player);
    m_playersPositions.replace(oldPos, 0);
    m_playersPositions.replace(position, player);
    m_collisionsTab.replace(oldPos, 0);
    m_collisionsTab.replace(position, 1);
}

void Map::setMapHeight(int height) {
    m_height = height;
}

void Map::setMapWidth(int width) {
    m_width = width;
}

void Map::setCollisionsTab(QList<int> collisionsTab) {
    m_collisionsTab.clear();
    m_collisionsTab.append(collisionsTab);
}

void Map::setCollisionsTab(int position, int value) {
    m_collisionsTab.replace(position, value);
}

void Map::setUnbreakablesBlocks(QList<int> unbreakablesBlocks) {
    m_unbreakablesBlocks.clear();
    m_unbreakablesBlocks.append(unbreakablesBlocks);
}

void Map::setUnbreakablesBlocks(int position, int value) {
    m_unbreakablesBlocks.replace(position, value);
}

void Map::setBreakablesBlocks(QList<int> breakablesBlocks) {
    m_breakableBlocks.clear();
    m_breakableBlocks.append(breakablesBlocks);
}

void Map::setBreakablesBlocks(int position, int value) {
    m_breakableBlocks.replace(position, value);
}

void Map::setBonusPositions(QList<int> bonusPositions)  {
    m_bonusPositions.clear();
    m_bonusPositions.append(bonusPositions);
}

void Map::setBonusPositions(int position, int value) {
    m_bonusPositions.replace(position, value);
}

void Map::DEBUG_displayInfos() {
    qDebug() << "collision:" << m_collisionsTab;
    qDebug() << "unbreakable:" << m_unbreakablesBlocks;
    qDebug() << "breakable:" << m_breakableBlocks;
    qDebug() << "players:" << m_playersPositions;
    qDebug() << "bonus:" << m_bonusPositions;
    qDebug() << "Map size:" << m_width << "x" << m_height;
}

void Map::DEBUG_displayMap() {
    QString theLine;
    for (int i = 0; i <= m_content.size(); i++) {
        if (!(i%m_width)) {
            qDebug() << theLine;
            theLine = "";
        }
        theLine += m_content.at(i);
    }
}