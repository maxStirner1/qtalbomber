#include "map.h"

Map::Map(QFileInfo mapFile, QObject* parent) : m_mapInfo(mapFile) {
     m_mapValid = false;

    if (!m_mapInfo.isFile())
        return;

    // Try to use the local XSD if available to be more fast
    QUrl schemaUrl;
    QFileInfo localXSD(QObject::tr("%1/%2/qtalbomber.xsd")
        .arg(QApplication::applicationDirPath()).arg(MAPS_DIRECTORY));
    
    if (localXSD.isFile())
        schemaUrl.setUrl(localXSD.absoluteFilePath());
    else
        schemaUrl.setUrl("http://www.bombermanproject.com/qtalbomber.xsd");

    // Load the schema and validate it
    QXmlSchema schema;
    schema.load(schemaUrl);
    if (!schema.isValid())
        return;

    // Open the XML map file and validate it
    QFile xmlMap(m_mapInfo.absoluteFilePath());
    if (!xmlMap.open(QIODevice::ReadOnly))
        return;
    QXmlSchemaValidator validator(schema);
    if (!validator.validate(&xmlMap, QUrl::fromLocalFile(m_mapInfo.absoluteFilePath()))) {
        xmlMap.close();
        return;
    }

    // Get map DOM
    xmlMap.seek(0);
    QDomDocument doc("qtalmap");
    if (!doc.setContent(&xmlMap)) {
        xmlMap.close();
        return;
    }

    // Parse the XML map
    QDomElement docElem = doc.documentElement();
    QDomNode node = docElem.firstChild();
    while (!node.isNull()) {
        QDomElement elem = node.toElement();

        if (elem.tagName() == "map")
            m_mapString = elem.firstChild().nodeValue();
        else if (elem.tagName() == "name")
            m_mapName = elem.firstChild().nodeValue().trimmed();
        else if (elem.tagName() == "author")
            m_mapAuthor = elem.firstChild().nodeValue().trimmed();
        else if (elem.tagName() == "description")
            m_mapDescription = elem.firstChild().nodeValue().trimmed();
        
        node = node.nextSibling();
    }

    m_mapValid = true;
    // Clean the map and ensure it is valid
    cleanMapString();
}

/*! Clean the map string from the XML file
 *
 *  Remove the with space and the empty line.
 *  The size (width and height) of the map is then computed and validate to
 *  be sure all the row have the same length.
 *  If not the case the map is marked as invalid.
 */
void Map::cleanMapString() {
    QStringList lines = m_mapString.split('\n');
    // Remove all the space
    lines.replaceInStrings(QRegExp("\\s"), "");
    // Replace empty lines by this string and then remove it
    lines.replaceInStrings(QRegExp("$^"), "##empty##");
    lines.removeAll("##empty##");

    m_mapString = lines.join("");
    m_mapHeight = lines.count();
    m_mapWidth = lines.at(0).length();

    for (int i = 1; i < m_mapHeight; i++) {
        if (lines.at(i-1).length() != lines.at(i).length()) {
            m_mapValid = false;
            break;
        }
    }
}

/*! Append border to a map.
 *
 *  When created, the global border of the map are not set because implied.
 *  Add it now and then update the new size and width.
 */
void Map::appendBorders() {    
    // Append side border
    int lines = m_mapString.length()/m_mapWidth;
    for (int i = 0; i < lines; i++) {
        m_mapString.insert(i*(m_mapWidth+2), "#");
        m_mapString.insert(i*(m_mapWidth+2)+m_mapWidth+1, "#");
    }
    m_mapWidth += 2;

    // Append top and bottom border
    QString lineBorder;
    for (int i = 0; i < m_mapWidth; i++) {
        lineBorder.append("#");
    }
    m_mapString.insert(0, lineBorder);
    m_mapString.append(lineBorder);
    m_mapHeight += 2;
}

/*! Parse map string to usable to draw a gamefield and play with it.
 *
 *  This parsing function read each character of the map string which have been
 *  retrieved in the XML file and interpret it.
 *  The value of the differents tables such as the breakable blocks are set here.
 *
 *  Known characters:
 *      - unbreakable: #
 *      - breakable: @
 *      - player: [1-4]
 *
 *  Bonus are generate randomly.
 */
void Map::parseMapString() {
    // Append the border to the map
    appendBorders();

    for (int i = 0; i < m_mapString.length(); i++) {
        if (m_mapString.at(i) == '_') {
            m_collisionsTab.append(0);
            m_unbreakablesBlocks.append(0);
            m_breakableBlocks.append(0);
            m_playersPositions.append(0);
            m_bonusPositions.append(0);
        }
        else if (m_mapString.at(i) == '#') {
            m_collisionsTab.append(1);
            m_unbreakablesBlocks.append(1);
            m_breakableBlocks.append(0);
            m_playersPositions.append(0);
            m_bonusPositions.append(0);
        }
        else if (m_mapString.at(i) == '@') {
            m_collisionsTab.append(1);
            m_unbreakablesBlocks.append(0);
            m_breakableBlocks.append(1);
            m_playersPositions.append(0);
            m_bonusPositions.append(0);
        }
        else  if (m_mapString.at(i).isDigit()) {
            m_collisionsTab.append(1);
            m_unbreakablesBlocks.append(0);
            m_breakableBlocks.append(0);
            m_playersPositions.append(m_mapString.at(i).digitValue());
            m_bonusPositions.append(0);
        }
    }
}

/*! Getter of the map width.
 * 
 *  @return The width of the map.
 */
int Map::mapWidth() {
    return m_mapWidth;
}

/*! Getter of the map height.
 * 
 *  @return The height of the map.
 */
int Map::mapHeigh() {
    return m_mapHeight;
}

/*! Getter of the collision table.
 * 
 *  @return Collision table.
 */
QList<int> Map::collisionsTab() {
    return m_collisionsTab;
}

/*! Setter of the collision table.
 *
 *  @param position Position of the collision element to set.
 *  @param value Value to set to collision (0 or 1)
 *
 *  Remeber that if you change the position of a collision you have to unset it
 *  from the old position and set it at the new position.
 *  The method can't determine you want to move a collision.
 *
 *  @see Map::collisionTab
 */
void Map::setCollisionsTab(int position, int value) {
    if (value < m_collisionsTab.length() && (value == 0 || value == 1))
        m_collisionsTab.replace(position, value);
}

/*! Getter of the unbreakable blocks table.
 *
 *  @return Unbreakable blocks table.
 */
QList<int> Map::unbreakableBlocks() {
    return m_unbreakablesBlocks;
}

/*! Getter of the breakable blocks table.
 *
 *  @return Breakable blocks table.
 *
 *  @see Map::removeBreakableBlocks
 */
QList<int> Map::breakableBlocks() {
    return m_breakableBlocks;
}

/*! Remove a breakable block from the information tables.
 *
 *  @param position Position of the breakable block to remove.
 *
 *  If the breakable block does not exists, nothing is done.
 *  When a breakable block is removed, his collision is also removed.
 *
 *  @see Map::breakablesBlocks
 */
void Map::removeBreakablesBlocks(int position) {
    if (position < m_unbreakablesBlocks.length()
        && m_unbreakablesBlocks.at(position) == 1) {
        m_unbreakablesBlocks.replace(position, 0);
        m_collisionsTab.replace(position, 0);
    }
}

/*! Getter of player position table
 *
 *  @return Player position table
 *
 *  Players are identify by a number from 1 to 4.
 *
 *  @see Map::setPlayerPosition
 */
QList<int> Map::playersPositions() {
    return m_playersPositions;
}

/*! Setter for player position table.
 *
 *  @param player ID of the player to set the position.
 *  @param widthPos Position in block from the left of the map.
 *  @param heightPos Position in block from the top of the map.
 *
 *  The player ID start from 0.
 *  This function call the other overloaded setPlayerPosition.
 *
 *  @see Map::playerPosition
 *  @seee Map::setPlayerPosition(int player, int position)
 */
void Map::setPlayerPosition(int player, int widthPos, int heightPos) {
    setPlayerPosition(player, widthPos*heightPos);
}

/*! Setter for the player position table.
 *
 *  @param player ID of the player to set the position.
 *  @param position Position of the player from the beginning
 *                  of the position tab
 *
 *  The player ID starts from 0.
 *  The position of the player is an integer which starts from 0.
 *
 *  @see Map::playerPosition
 */
void Map::setPlayerPosition(int player, int position) {
    if (player > PLAYER_NUMBER || position > m_playersPositions.length())
        return;
   
    if (m_playersPositions.at(position) > 0)
        return;

    int oldPos = m_playersPositions.indexOf(player);
    if (oldPos == -1)
        return;

    m_playersPositions.replace(oldPos, 0);
    m_playersPositions.replace(position, player);
    m_collisionsTab.replace(oldPos, 0);
    m_collisionsTab.replace(position, 1);
}

/*! Getter of bonus position table.
 *
 *  @return Bonus position.
 *
 *  @see Map::removeBonus
 */
QList<int> Map::bonusPositions() {
    return m_bonusPositions;
}

/*! Remove a bonus at a given position.
 *
 *  @param position Position of the bonus to remove.
 */
void Map::removeBonus(int position) {
    if (position < m_bonusPositions.length())
        m_bonusPositions.replace(position, 0);
}

/*! Getter for the map name
 *
 *  @return The map name
 */
QString Map::mapName() {
    return m_mapName;
}

/*! Getter for the map author
 *
 *  @return The map author
 */
QString Map::mapAuthor() {
    return m_mapAuthor;
}

/*! Getter for the map description
 *
 *  @return The map description
 */
QString Map::mapDescription() {
    return m_mapDescription;
}

/*! Getter for the map validity
 *
 *  @return The validity of the map
 */
bool Map::isValid() {
    return m_mapValid;
}
