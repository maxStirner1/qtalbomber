#ifndef _GAMEFIELD_H
#define	_GAMEFIELD_H

#include <QtCore/QMap>
#include <QtCore/QMapIterator>
#include <QtCore/QList>
#include <QtGui/QWidget>
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsView>
#include <QtGui/QGraphicsRectItem>
#include <QtGui/QGridLayout>
#include <QtGui/QVBoxLayout>
#include <QtCore/QString>
#include <QtGui/QBrush>
#include <QtGui/QPen>
#include <QtGui/QLabel>
#include "game/map.h"
#include "game/player.h"
#include "ui/baseui.h"

#include <QtCore/QDebug>

/*! Drawn the gamefield and all the graphics object of the game.
 *
 */
class Gamefield : public BaseUI {
    Q_OBJECT
    
public:
    Gamefield(QWidget* parent);
    void setPlayersList(QMap<int, Player *> players);
    QMap<int, Player *> playersList();
    void setGameRounds(int rounds);
    int gameRounds();
    void setMap(Map* map);
    Map* gameMap();
    void drawMap();

private:
    /// Map of the gamefield
    Map* m_map;
    /// List of the differents player
    QMap<int, Player *> m_playersList;
    /// Number of rounds of the game
    int m_gameRounds;
    /// Main layout of the interface
    QGridLayout* m_mainLayout;
    /// Map scene element
    QGraphicsScene* m_mapScene;
    /// Map scene view
    QGraphicsView* m_mapView;
    /// Layout for the player information
    QVBoxLayout* m_playersInfoLayout;
    /// Players information elements
    QMap<int, QLabel*> m_playersInfoWidget;
    /// State of the game
    int m_gameState;

    void keyPressEvent(QKeyEvent* event);
    int heightFromPos(int pos);
    int widthFromPos(int pos);
    QString layoutTitle();
};

#endif	/* _GAMEFIELD_H */

