/*
 *  QtalBomber - A Qt BomberMan game <http://www.bombermanproject.com/>
 *  Copyright (C) 2010  QtalBomber team <team@bombermanproject.com>
 *
 *  Authors: Quentin Amoudruz
 *           Thomas Savineau
 *           Alexandre Joseph
 *           Lucas Courot
 *
 *  File:        GameMap.cpp
 *  Description: Parse game map.
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

#include "server/GameMap.h"

// TODO: Parse mapContent and create map class

GameMap::GameMap(QFileInfo mapFile) : m_mapFile(mapFile) {
    m_mapValid = false;

    if (!m_mapFile.isFile())
        return;

    QUrl schemaUrl;
    if (QFileInfo(QObject::tr("%1/%2/qtalbomber.xsd").arg(QDir::currentPath()).arg(MAPS_DIR)).isFile()) {
        schemaUrl.setUrl(QObject::tr("%1/%2/qtalbomber.xsd").arg(QDir::currentPath()).arg(MAPS_DIR));
        qDebug() << "using local xsd";
    }
    else {
        schemaUrl.setUrl("http://www.bombermanproject.com/qtalbomber.xsd");
        qDebug() << "using remote xsd";
    }
    
    QXmlSchema schema;
    schema.load(schemaUrl);
    
    if (!schema.isValid())
        return;

    QFile map(m_mapFile.absoluteFilePath());
    if (!map.open(QIODevice::ReadOnly))
        return;

    QXmlSchemaValidator validator(schema);
    if (!validator.validate(&map, QUrl::fromLocalFile(m_mapFile.absoluteFilePath()))) {
        map.close();
        return;
    }

    map.seek(0);
    QDomDocument doc("qtalmap");
    if (!doc.setContent(&map)) {
        map.close();
        return;
    }

    QDomElement docElem = doc.documentElement();
    QDomNode node = docElem.firstChild();
    while (!node.isNull()) {
        QDomElement elem = node.toElement();

        if (elem.tagName() == "map")
            m_mapContent = elem.firstChild().nodeValue();
        else if (elem.tagName() == "name")
            m_mapName = elem.firstChild().nodeValue().trimmed();
        else if (elem.tagName() == "author")
            m_mapAuthor = elem.firstChild().nodeValue().trimmed();
        else if (elem.tagName() == "description")
            m_mapDesc = elem.firstChild().nodeValue().trimmed();
        
        node = node.nextSibling();
    }

    m_mapValid = true;
    qDebug() << "Map" << map.fileName() << "is XSD valid";

    computeContent();
    qDebug() << "Map height" << m_mapHeight << "width" << m_mapWidth;
}

void GameMap::computeContent() {
    QStringList lines = m_mapContent.split('\n');
    lines.replaceInStrings(QRegExp("\\s"), "");
    lines.replaceInStrings(QRegExp("$^"), "##empty##");
    lines.removeAll("##empty##");

    m_mapContent = lines.join("");
    m_mapHeight = lines.count();
    m_mapWidth = lines.at(0).length();

    for (int i = 1; i < m_mapHeight; i++) {
        if (lines.at(i-1).length() != lines.at(i).length()) {
            m_mapValid = false;
            qDebug() << i << lines.at(0).length() << m_mapWidth;
            break;
        }
    }
}

bool GameMap::isValid() {
    return m_mapValid;
}

QString GameMap::mapAuthor() {
    return m_mapAuthor;
}

QString GameMap::mapDescription() {
    return m_mapDesc;
}

QString GameMap::mapName() {
    return m_mapName;
}

QString GameMap::mapLocation() {
    return m_mapFile.absoluteFilePath();
}

QString GameMap::mapContent() {
    return m_mapContent;
}

int GameMap::heigh() {
    return m_mapHeight;
}

int GameMap::width() {
    return m_mapWidth;
}
