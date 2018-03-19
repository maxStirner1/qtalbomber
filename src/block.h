

#ifndef BLOCK_H
#define BLOCK_H

#include <QtGui>
#include <QTimer>
#include <QObject>
#include "constant.h"
#include <QGraphicsRectItem>

/*!
* Manage blocks (breakables or not)
*/
class Block : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
private:
	/// If the block is breakable
    bool m_breakable;

public:
    Block(bool breakable);
    ~Block();
    bool isBreakable();

};

#endif // BLOCK_H
