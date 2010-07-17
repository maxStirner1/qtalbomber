
#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include <QtGui>
#include <QTimer>
#include "level.h"
#include "bomb.h"
#include "flam.h"
#include "block.h"
#include "player.h"
#include "bonus.h"
#include "constant.h"
#include "about.h"

/*!
 * Manage graphical interface and collisions between different elements
 */
class GameField : public QWidget {
    Q_OBJECT

public:
    GameField();

private slots:
    void onPlayerInfosClicked();
    void eraseBomb();
    void eraseFlam();
    void collisionChecking();
    void exitGame();
    void toggleGameState(bool notify = true);
    void selectMap(int index);
    void displayCountdown();
    void resetGamefield();
    void removeUndesiredPlayers();
    void startCountdown();
    void toggleAboutDialog();

private:
    int elementSize();
    void move(int direction, Player *item);
    int indexFromCoord(int x, int y);
    void generateBonus(QPointF position);
    void grapBonus(Bonus* bonus, Player* player);
    void dropBomb(Player* player);
    void keyPressEvent(QKeyEvent *event);
    void setPlayerOff(int playerId);
    void setPlayerAlive(int playerId);
    void setPlayerDead(int playerId);
    void manageRounds();
    void loadMaps();
    int heightFromPos(int pos);
    int widthFromPos(int pos);
    void setPlayerDefaultPos(int playerId);
    void drawArena();
    void drawCharacters();
    void writeMapText(QString text, int size);
    int countDeadPlayer();
    void displayFinalScores();

    /// Graphical scene where the graphic items are displayed and executed
    QGraphicsScene* m_scene;
    /// The view which display the scene
    QGraphicsView* m_mapView;

    /// Countdown text
    QStringList m_startText;
    /// If the music is activated
    bool m_musicPlaying;
    /// Background music
    QSound* m_music;
    /// Current level
    Level m_level;
    /// Text displayed over the GUI
    QGraphicsSimpleTextItem* m_gamefieldText;
    /// Blocks list
    QList<Block*> m_blocksArena;
    /// Bombs list
    QList<Bomb*> m_bombs;
    /// Bonuses list
    QList<Bonus*> m_bonuses;
    /// Available maps list
    QList<Level> m_availableMaps;
    /// List pf players
    QList<Player *> m_players;
    /// Number of players who are playing
    int m_playerInGame;
    /// Game players information
    QList<QPushButton *> m_playersInfosDisplay;
    /// Selected number of round
    QSpinBox* m_gameRoundsChoice;
    /// Current round
    int m_currentRound;
    /// Layout for the player information
    QVBoxLayout* m_gameInfoPanel;
    ///  Game state
    int m_gameState;
    /// Select list of maps
    QComboBox* m_mapList;
    /// List of players scores
    QList<int> m_playersScore;
    /// Start game button
    QPushButton* m_startGameButton;
    /// About dialog object
    About* m_aboutDialog;
};

#endif // GAMEFIELD_H
