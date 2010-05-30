#ifndef _MAP_H
#define	_MAP_H

#include "qtalbomber.h"
#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QFileInfo>
#include <QtCore/QUrl>
#include <QtGui/QApplication>
#include <QtXml/QtXml>
#include <QtXmlPatterns/QtXmlPatterns>

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
    Q_OBJECT
    
public:
    GameMap(QFileInfo mapFile, QObject* parent = 0);
    bool isValid();
    QString name();
    QString author();
    QString description();
    void parseMapRepr();
    int width();
    int heigh();
    QList<int> playersPositions();
    void setPlayerPosition(int player, int position);
    void setPlayerPosition(int player, int widthPos, int heightPos);
    void removePlayer(int player);
    QList<int> collisionsTab();
    void setCollision(int position, int value);
    void addCollision(int position);
    void removeCollision(int position);
    QList<int> unbreakableBlocks();
    QList<int> breakableBlocks();
    void removeBreakableBlock(int position);
    QList<int> bonusPositions();
    void removeBonus(int position);

private:
    /// Map file informations
    QFileInfo m_mapInfo;
    /// Holds string representation of the map
    QString m_mapRepr;
    /// Height of the map
    int m_mapHeight;
    /// Width of the map
    int m_mapWidth;
    /// Store if the map is valid and can be used
    bool m_mapValid;
    /// Auhtor of the map
    QString m_author;
    /// Name of the map
    QString m_name;
    /// Description of the map
    QString m_description;
    /// Store where collision can occure between elements of the game
    QList<int> m_collisions;
    /// Define where unbreakables blocks are
    QList<int> m_unbreakablesBlocks;
    /// Defines where breakables blocks are
    QList<int> m_breakableBlocks;
    /// Defines the bonus position
    QList<int> m_bonus;
    /// Store the player position
    QList<int> m_players;

    void cleanMapRepr();
    void appendBorders();
};

#endif	/* _MAP_H */
