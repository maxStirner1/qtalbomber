#include "gamemap.h"

GameMap::GameMap(QFileInfo mapFile, QObject* parent)
    : QObject(parent), m_mapInfo(mapFile) {
     m_mapValid = false;

    if (!m_mapInfo.isFile())
        return;

    QFile xmlMap(m_mapInfo.absoluteFilePath());
    if (!xmlMap.open(QIODevice::ReadOnly))
        return;

    #ifndef SKIP_XSD_VALIDATION
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

    QXmlSchemaValidator validator(schema);
    if (!validator.validate(&xmlMap, QUrl::fromLocalFile(m_mapInfo.absoluteFilePath()))) {
        xmlMap.close();
        return;
    }
    #endif

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
            m_mapRepr = elem.firstChild().nodeValue();
        else if (elem.tagName() == "name")
            m_name = elem.firstChild().nodeValue().trimmed();
        else if (elem.tagName() == "author")
            m_author = elem.firstChild().nodeValue().trimmed();
        else if (elem.tagName() == "description")
            m_description = elem.firstChild().nodeValue().trimmed();
        
        node = node.nextSibling();
    }

    m_mapValid = true;
    
    // Clean the map and ensure it is valid
    cleanMapRepr();
    // Append the border to the map
    appendBorders();
}

/*! Clean the map string from the XML file
 *
 *  Remove the with space and the empty line.
 *  The size (width and height) of the map is then computed and validate to
 *  be sure all the row have the same length.
 *  If not the case the map is marked as invalid.
 */
void GameMap::cleanMapRepr() {
    QStringList lines = m_mapRepr.split('\n');
    // Remove all the space
    lines.replaceInStrings(QRegExp("\\s"), "");
    // Replace empty lines by this string and then remove it
    lines.replaceInStrings(QRegExp("$^"), "##empty##");
    lines.removeAll("##empty##");

    m_mapRepr = lines.join("");
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
void GameMap::appendBorders() {
    // Append side border
    int lines = m_mapRepr.length()/m_mapWidth;
    for (int i = 0; i < lines; i++) {
        m_mapRepr.insert(i*(m_mapWidth+2), "#");
        m_mapRepr.insert(i*(m_mapWidth+2)+m_mapWidth+1, "#");
    }
    m_mapWidth += 2;

    // Append top and bottom border
    QString lineBorder;
    for (int i = 0; i < m_mapWidth; i++) {
        lineBorder.append("#");
    }
    m_mapRepr.insert(0, lineBorder);
    m_mapRepr.append(lineBorder);
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
void GameMap::parseMapRepr() {
    // Clean attributes as the method can be call several times
    m_collisions.clear();
    m_unbreakablesBlocks.clear();
    m_breakableBlocks.clear();
    m_players.clear();
    m_bonus.clear();

    for (int i = 0; i < m_mapRepr.length(); i++) {
        if (m_mapRepr.at(i) == '_') {
            m_collisions.append(0);
            m_unbreakablesBlocks.append(0);
            m_breakableBlocks.append(0);
            m_players.append(0);
            m_bonus.append(0);
        }
        else if (m_mapRepr.at(i) == '#') {
            m_collisions.append(1);
            m_unbreakablesBlocks.append(1);
            m_breakableBlocks.append(0);
            m_players.append(0);
            m_bonus.append(0);
        }
        else if (m_mapRepr.at(i) == '@') {
            m_collisions.append(1);
            m_unbreakablesBlocks.append(0);
            m_breakableBlocks.append(1);
            m_players.append(0);
            m_bonus.append(0);
        }
        else  if (m_mapRepr.at(i).isDigit()) {
            // Don't set players in the collision tab now else if a player is
            // not used can create conflict...
            m_collisions.append(0);
            m_unbreakablesBlocks.append(0);
            m_breakableBlocks.append(0);
            m_players.append(m_mapRepr.at(i).digitValue());
            m_bonus.append(0);
        }
    }
}

/*! Getter of the map width.
 * 
 *  @return The width of the map.
 */
int GameMap::width() {
    return m_mapWidth;
}

/*! Getter of the map height.
 * 
 *  @return The height of the map.
 */
int GameMap::heigh() {
    return m_mapHeight;
}

/*! Getter of the collision table.
 * 
 *  @return Collision table.
 */
QList<int> GameMap::collisionsTab() {
    return m_collisions;
}

/*! Setter of the collision table.
 *
 *  Remeber that if you change the position of a collision you have to unset it
 *  from the old position and set it at the new position.
 *  The method can't determine you want to move a collision.
 *
 *  @param position Position of the collision element to set.
 *  @param value Value to set to collision (0 or 1)
 *
 *  @see Map::collisionTab
 */
void GameMap::setCollision(int position, int value) {
    if (position < m_collisions.length() && (value == 0 || value == 1))
        m_collisions.replace(position, value);
}

/*! Mark a position as not occuped.
 *
 *  @param position Position to mark.
 *
 *  @see GameMap::unflagCollision
 */
void GameMap::addCollision(int position) {
    if (position < m_collisions.length())
        m_collisions.replace(position, m_collisions.at(position)+1);
}

/*! Mark a position as free.
 *
 *  The position can remain occuped if it had been flagged several time.
 *
 *  @param position Position to mark.
 * 
 *  @see GameMap::unflagCollision
 */
void GameMap::removeCollision(int position) {
    if (position < m_collisions.length())
        m_collisions.replace(position, m_collisions.at(position)-1);
}

/*! Getter of the unbreakable blocks table.
 *
 *  @return Unbreakable blocks table.
 */
QList<int> GameMap::unbreakableBlocks() {
    return m_unbreakablesBlocks;
}

/*! Getter of the breakable blocks table.
 *
 *  @return Breakable blocks table.
 *
 *  @see Map::removeBreakableBlocks
 */
QList<int> GameMap::breakableBlocks() {
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
void GameMap::removeBreakableBlock(int position) {
    if (position < m_unbreakablesBlocks.length()
        && m_breakableBlocks.at(position) == 1) {
        m_breakableBlocks.replace(position, 0);
        removeCollision(position);
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
QList<int> GameMap::playersPositions() {
    return m_players;
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
void GameMap::setPlayerPosition(int player, int widthPos, int heightPos) {
    setPlayerPosition(player, (heightPos*m_mapWidth)+widthPos);
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
void GameMap::setPlayerPosition(int player, int position) {
    if (player > PLAYER_NUMBER_MAX || position > m_players.length())
        return;
   
    if (m_players.at(position) > 0)
        return;

    int oldPos = m_players.indexOf(player);
    if (oldPos == -1)
        return;

    m_players.replace(oldPos, 0);
    m_players.replace(position, player);
    removeCollision(oldPos);
    addCollision(position);
}

/*! Remove a player from the gamefield.
 *
 *  @param player Player identifier to remove.
 */
void GameMap::removePlayer(int player) {
    int playerPos = m_players.indexOf(player);
    m_players.replace(playerPos, 0);
    removeCollision(playerPos);
}

/*! Getter of bonus position table.
 *
 *  @return Bonus position.
 *
 *  @see Map::removeBonus
 */
QList<int> GameMap::bonusPositions() {
    return m_bonus;
}

/*! Remove a bonus at a given position.
 *
 *  @param position Position of the bonus to remove.
 */
void GameMap::removeBonus(int position) {
    if (position < m_bonus.length())
        m_bonus.replace(position, 0);
}

/*! Getter for the map name
 *
 *  @return The map name
 */
QString GameMap::name() {
    return m_name;
}

/*! Getter for the map author
 *
 *  @return The map author
 */
QString GameMap::author() {
    return m_author;
}

/*! Getter for the map description
 *
 *  @return The map description
 */
QString GameMap::description() {
    return m_description;
}

/*! Getter for the map validity
 *
 *  @return The validity of the map
 */
bool GameMap::isValid() {
    return m_mapValid;
}
