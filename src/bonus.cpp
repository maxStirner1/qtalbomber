#include "bonus.h"

/*!
* Instanciates a bonus depending its type
* @param type Type of bonus
*/
Bonus::Bonus(int type)
{
    setRect(10, 10, 5, 5);
    m_type = type;
    for (int i = 0; i < 4; i++) {
	m_attributes.append(0);
    }

    switch(m_type) {
    case BOMB_RANGE_UP:
	if (qrand() % 2 == 1) {
	    setBrush(QColor::fromRgb (255, 150, 0));
	    m_attributes.insert(BOMB_RANGE_UP, -1);
	} else {
	    setBrush(Qt::red);
	    m_attributes.insert(BOMB_RANGE_UP, 1);
	}
	break;
    case BOMB_CAPACITY_UP:
	if (qrand() % 2 == 1) {
	    setBrush(Qt::cyan);
	    m_attributes.insert(BOMB_CAPACITY_UP, -1);
	} else {
	    setBrush(Qt::blue);
	    m_attributes.insert(BOMB_CAPACITY_UP, 1);
	}
	break;

    case KICK_ACTIVATOR:
	setBrush(Qt::darkMagenta);
	m_attributes.insert(KICK_ACTIVATOR, 1);
	break;
    case LIFE_UP:
	setBrush(Qt::green);
	m_attributes.insert(LIFE_UP, 1);
	break;
    }
}

Bonus::~Bonus() {
}

int Bonus::type() const {
    return m_type;
}

QList<int> Bonus::attributes() {
    return m_attributes;
}

