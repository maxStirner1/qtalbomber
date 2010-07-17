

#include "block.h"

/*!
* Creates a rectangle to represent block
* @param breakable If block is breakable
*/
Block::Block(bool breakable) {
    setRect(0, 0, BLOCK_WIDTH, BLOCK_HEIGHT);
    m_breakable = breakable;
    if (m_breakable) {
	setPen(QPen(Qt::gray));
	setBrush(QBrush(Qt::gray, Qt::Dense4Pattern));
    } else {
	setBrush(QBrush(Qt::darkGray));
	setPen(QPen(Qt::black));
    }
}

Block::~Block() {

}

bool Block::isBreakable() {
    return m_breakable;
}


