#ifndef _GAMEFIELD_H
#define	_GAMEFIELD_H

#include "gamemap.h"
#include "player.h"
#include "bomb.h"
#include "flam.h"
#include <QApplication>
#include <QWidget>
#include <QMap>
#include <QMapIterator>
#include <QLabel>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QSpinBox>
#include <QKeyEvent>
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
    void exitGame();
    void toggleGameState();
    void selectMap(int index);
    void onPlayerInfosClicked();
    void onBombExplode(int position);
    void stretchFlam();
    void displayeCurrentRound(int round = 0);

private:
    /// Availables maps
    QList<GameMap *> m_availableMaps;
    /// Current map of the gamefield
    GameMap* m_currentMap;
    /// Game players
    QList<Player *> m_players;
    /// Number of player which are playing
    int m_playerInGame;
    /// Game players information
    QList<QPushButton *> m_playersInfosDisplay;
    /// Number of rounds of the game
    QSpinBox* m_gameRoundsChoice;
    /// Current round
    int m_currentRound;
    /// Map scene element
    QGraphicsScene* m_mapScene;
    /// Map scene view
    QGraphicsView* m_mapView;
    /// Layout for the player information
    QVBoxLayout* m_gameInfoPanel;
    /// Status of the game
    int m_gameState;
    /// Map list combo box
    QComboBox* m_mapList;
    /// Player keys configuration : (key, (playerId, action))
    QMap<int, QMap<int, int> > m_playersControlKeys;
    /// Bomb list (position, instance)
    QMap<int, Bomb*> m_gamefieldBomb;
    /// Bomb flam position (position, instance)
    QMap<int, Flam*> m_gamefieldFlam;
    /// Player finish game postion
    QMap<int, int> m_playersScore;
    /// Breakable blocks (position, instance)
    QMap<int, QGraphicsItem*> m_breakableBlocks;
    /// Gamefield texte
    QGraphicsSimpleTextItem* m_gamefieldText;

    void keyPressEvent(QKeyEvent* event);
    int heightFromPos(int pos);
    int widthFromPos(int pos);
    void drawMap();
    void loadMaps();
    void initPlayers();
    void initPlayersControlKeys();
    void setPlayerDefaultPos(int playerId);
    void setPlayerDead(int playerId);
    void setPlayerAlive(int playerId);
    void setPlayerOff(int playerId);
    void movePlayer(int playerId, int direction);
    void dropBomb(int playerId);
    void manageRounds();
    void startNewRound();
};

#endif	/* _GAMEFIELD_H */

