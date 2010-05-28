#ifndef _MAP_H
#define	_MAP_H

#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QFileInfo>
#include <QtCore/QUrl>
#include <QtGui/QApplication>
#include <QtXml/QtXml>
#include <QtXmlPatterns/QtXmlPatterns>
#include "qtalbomber.h"

/*! Define a map and his properties
 *
 *  Maps are created from an XML file which defines several informations such
 *  as the author, the description of the map but also the position of the
 *  breakable block, the player and the unbreakable bocks.
 *
 *  The bonus are randomly added to the map.
 *
 *  If the XML file of a map is invalid the map won't be available for playing.
 */
class GameMap : public QObject {
public:
    GameMap(QFileInfo mapFile, QObject* parent = 0);
    bool isMapValid();
    QString mapName();
    QString mapAuthor();
    QString mapDescription();
    void parseMapString();
    int mapWidth();
    int mapHeigh();
    QList<int> playersPositions();
    void setPlayerPosition(int player, int position);
    void setPlayerPosition(int player, int widthPos, int heightPos);
    QList<int> collisionsTab();
    void setCollisionsTab(QList<int> collisionsTab);
    void setCollisionsTab(int position, int value);
    QList<int> unbreakableBlocks();
    QList<int> breakableBlocks();
    void removeBreakablesBlocks(int position);
    QList<int> bonusPositions();
    void removeBonus(int position);

private:
    /// Map file informations
    QFileInfo m_mapInfo;
    /// Holds string representation of the map
    QString m_mapString;
    /// Height of the map
    int m_mapHeight;
    /// Width of the map
    int m_mapWidth;
    /// Store if the map is valid and can be used
    bool m_mapValid;
    /// Auhtor of the map
    QString m_mapAuthor;
    /// Name of the map
    QString m_mapName;
    /// Description of the map
    QString m_mapDescription;
    /// Store where collision can occure between elements of the game
    QList<int> m_collisionsTab;
    /// Define where unbreakables blocks are
    QList<int> m_unbreakablesBlocks;
    /// Defines where breakables blocks are
    QList<int> m_breakableBlocks;
    /// Defines the bonus position
    QList<int> m_bonusPositions;
    /// Store the player position
    QList<int> m_playersPositions;

    void cleanMapString();
    void appendBorders();
};

#endif	/* _MAP_H */
