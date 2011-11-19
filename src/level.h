#ifndef LEVEL
#define LEVEL

#include <QtGui>
#include <QtXml/QtXml>
#include <QtXmlPatterns/QXmlSchema>
#include <QtXmlPatterns/QXmlSchemaValidator>
#include "constant.h"

/*!
* Manages levels
*/
class Level {
private:
	/// Contains current map architecture
    QString m_mapRepr;
	/// Level Height
    int m_height;
	/// Level Width
    int m_width;
	/// Level Valid State
    bool m_valid;
	/// Level name
    QString m_name;
	/// Author of level
    QString m_author;
	/// Level description
    QString m_description;
	/// List of collisions (0 or 1)
    QList<int> m_collisions;
	/// List of unbreakable blocks
    QList<int> m_unbreakableBlocks;
	/// List of breakable block
    QList<int> m_breakableBlocks;
	/// List of bonuses poition
    QList<int> m_bonusesPosition;
	/// List of players position
    QList<int> m_playersPosition;

public:
    Level();
    Level(QString mapXmlPath);
    ~Level();
    void parseMapRepr();
    void appendBorders();
    void cleanMapRepr();
    bool isValid();

    QString mapRepr();
    void setMapRepr(QString map);

    int height();
    void setHeight(int lengthMap);

    int width();
    void setWidth(int widthMap);

    QString name();
    void setName(QString nameMap);

    QString author();
    void setAuthor(QString authorMap);

    QString description();
    void setDescription(QString descriptionMap);

    QList<int> collisions();
    void setCollision(int index, int number);

    QList<int> unbreakableBlocks();
    void setUnbreakableBlock(int index, int number);

    QList<int> breakableBlocks();
    void setBreakableBlock(int index, int number);
    void removeBreakbleBlock(int index);

    QList<int> playerPosition();
    void setPlayerPosition(int index, int number);

    QList<int> bonuses();
    void setBonus(int index, int number);

};

#endif
