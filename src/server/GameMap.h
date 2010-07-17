/*
 *  QtalBomber - A Qt BomberMan game <http://www.bombermanproject.com/>
 *  Copyright (C) 2010  QtalBomber team <team@bombermanproject.com>
 *
 *  Authors: Quentin Amoudruz
 *           Thomas Savineau
 *           Alexandre Joseph
 *           Lucas Courot
 *
 *  File:        GameMap.h
 *  Description: Header file for GameMap class.
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

#ifndef _GAMEMAP_H
#define	_GAMEMAP_H

#include "../constants.h"
#include <QtXml/QtXml>
#include <QtXmlPatterns/QtXmlPatterns>
#include <QDebug>

class GameMap {
public:
    GameMap(QFileInfo mapFile);
    bool isValid();
    QString mapName();
    QString mapAuthor();
    QString mapDescription();
    QString mapLocation();
    QString mapContent();
    int heigh();
    int width();

private:
    void computeContent();
    QFileInfo m_mapFile;
    bool m_mapValid;
    QString m_mapName;
    QString m_mapAuthor;
    QString m_mapDesc;
    QString m_mapContent;
    int m_mapHeight;
    int m_mapWidth;
};

#endif	/* _GAMEMAP_H */

