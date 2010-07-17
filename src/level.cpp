#include "level.h"

Level::Level() {
	
}

/*! Instanciates a level and check if it's valid
 *
 *   @param mapXmlPath xml path to the level
 */
Level::Level(QString mapXmlPath) {
    m_valid = false;

    QFile xmlMap(mapXmlPath);
    if (!xmlMap.open(QIODevice::ReadOnly))
        return;

    #ifndef SKIP_XSD_VALIDATION
    // Try to use the local XSD if available to be more fast
    QUrl schemaUrl;
    QFileInfo localXSD(QObject::tr("%1/%2/qtalbomber.xsd")
        .arg(QApplication::applicationDirPath()).arg(MAPS_DIR));

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
    if (!validator.validate(&xmlMap, QUrl::fromLocalFile(mapXmlPath))) {
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

    m_valid = true;

    // Clean the map and ensure it is valid
    cleanMapRepr();
    // Append the border to the map
    appendBorders();
    parseMapRepr();
}

Level::~Level() {
}

/*! Clean the map string from the XML file
 *
 *  Remove the with space and the empty line.
 *  The size (width and height) of the map is then computed and validate to
 *  be sure all the row have the same length.
 *  If not the case the map is marked as invalid.
 */
void Level::cleanMapRepr() {
    QStringList lines = m_mapRepr.split('\n');
    // Remove all the space
    lines.replaceInStrings(QRegExp("\\s"), "");
    // Replace empty lines by this string and then remove it
    lines.replaceInStrings(QRegExp("$^"), "##empty##");
    lines.removeAll("##empty##");
	
    m_mapRepr = lines.join("");
    m_height = lines.count();
    m_width = lines.at(0).length();
	
    for (int i = 1; i < m_height; i++) {
		if (lines.at(i - 1).length() != lines.at(i).length()) {
			m_valid = false;
			break;
		}
    }
}

/*! Parse map string to usable to draw a gamefield and play with it.
 *
 *  This parsing function read each character of the map string which have been
 *  retrieved in the XML file and interpret it.
 *  The value of the differents tables such as the breakable blocks are set here.
 *
 *  Known characters:
 *      - unbreakable: #
 *      - breakable: b
 *      - player: [1-4]
 *
 *  Bonus are generated randomly.
 */
void Level::parseMapRepr() {
    // Clean attributes as the method can be call several times
    m_collisions.clear();
    m_unbreakableBlocks.clear();
    m_breakableBlocks.clear();
    m_playersPosition.clear();
    m_bonusesPosition.clear();
	
    for (int i = 0; i < m_mapRepr.length(); i++) {
		if (m_mapRepr.at(i) == '_') {
			m_collisions.append(0);
			m_unbreakableBlocks.append(0);
			m_breakableBlocks.append(0);
			m_playersPosition.append(0);
			m_bonusesPosition.append(0);
		}
		else if (m_mapRepr.at(i) == '#') {
			m_collisions.append(1);
			m_unbreakableBlocks.append(1);
			m_breakableBlocks.append(0);
			m_playersPosition.append(0);
			m_bonusesPosition.append(0);
		}
		else if (m_mapRepr.at(i) == 'b') {
			m_collisions.append(1);
			m_unbreakableBlocks.append(0);
			m_breakableBlocks.append(1);
			m_playersPosition.append(0);
			m_bonusesPosition.append(0);
		}
		else if (m_mapRepr.at(i).isDigit()) {
			m_collisions.append(0);
			m_unbreakableBlocks.append(0);
			m_breakableBlocks.append(0);
			m_playersPosition.append(m_mapRepr.at(i).digitValue());
			m_bonusesPosition.append(0);
		}
    }
}

bool Level::isValid() {
    return m_valid;
}

/*!
* Is used to add side borders around the map
*/
void Level::appendBorders() {
    // Append side border
    int lines = m_mapRepr.length() / m_width;
    for (int i = 0; i < lines; i++) {
		m_mapRepr.insert(i * (m_width + 2), "#");
		m_mapRepr.insert(i * (m_width + 2) + m_width + 1, "#");
    }
    m_width += 2;
	
    // Append top and bottom border
    QString lineBorder;
    for (int i = 0; i < m_width; i++) {
		lineBorder.append("#");
    }
    m_mapRepr.insert(0, lineBorder);
    m_mapRepr.append(lineBorder);
    m_height += 2;
}


//From here, getter and setter only...

QString Level::mapRepr() {
    return m_mapRepr;
}

void Level::setMapRepr(QString map) {
    m_mapRepr = map;
}

int Level::height() {
    return m_height;
}

void Level::setHeight(int lengthMap) {
    m_height = lengthMap;
}

int Level::width() {
    return m_width;
}

void Level::setWidth(int widthMap) {
    m_width = widthMap;
}

QString Level::name() {
    return m_name;
}

void Level::setName(QString nameMap) {
    m_name = nameMap;
}

QString Level::author() {
    return m_author;
}

void Level::setAuthor(QString authorMap) {
    m_author = authorMap;
}

QString Level::description() {
    return m_description;
}

void Level::setDescription(QString descriptionMap) {
    m_description = descriptionMap;
}

QList<int> Level::collisions() {
    return m_collisions;
}

void Level::setCollision(int index, int number) {
    m_collisions[index] = number;
}

QList<int> Level::unbreakableBlocks() {
    return m_unbreakableBlocks;
}

void Level::setUnbreakableBlock(int index, int number) {
    m_unbreakableBlocks[index] = number;
}

QList<int> Level::breakableBlocks() {
    return m_breakableBlocks;
}

void Level::setBreakableBlock(int index, int number) {
    m_breakableBlocks[index] = number;
}

QList<int> Level::playerPosition() {
    return m_playersPosition;
}

void Level::setPlayerPosition(int index, int number) {
    m_playersPosition[index] = number;
}

QList<int> Level::bonuses() {
    return m_bonusesPosition;
}

void Level::setBonus(int index, int number) {
    m_bonusesPosition[index] = number;
}
