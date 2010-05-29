#ifndef _GAMEFIELD_H
#define	_GAMEFIELD_H

#include "gamemap.h"
#include "player.h"
#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QSpinBox>
#include <QList>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QComboBox>
#include <QPushButton>
#include <QMessageBox>
#include <QComboBox>

class Gamefield : public QWidget {
    Q_OBJECT
    
public:
    Gamefield();

private slots:
    void onExitGame();
    void onGameStart();
    void onMapSelected(int index);
    void onPlayerInfosClicked();

private:
    /// Availables maps
    QList<GameMap *> m_gameMapList;
    /// Current map of the gamefield
    GameMap* m_gameMap;
    /// Game players
    QList<Player *> m_playersList;
    /// Number of player which are playing
    int m_playerNumber;
    /// Game players information
    QList<QPushButton *> m_playerInfosList;
    /// Number of rounds of the game
    QSpinBox* m_gameRoundsField;
    /// Map scene element
    QGraphicsScene* m_mapScene;
    /// Map scene view
    QGraphicsView* m_mapView;
    /// Layout for the player information
    QVBoxLayout* m_gamePanel;
    /// Status of the game
    int m_gameState;
    /// Map list combo box
    QComboBox* m_gameMapListBox;

    void keyPressEvent(QKeyEvent* event);
    int heightFromPos(int pos);
    int widthFromPos(int pos);
    void drawMap();
    void loadMaps();
    void initPlayers();
};

#endif	/* _GAMEFIELD_H */

