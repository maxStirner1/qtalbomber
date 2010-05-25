
#ifndef _STARTUI_H
#define	_STARTUI_H

#include <QtGui/QGridLayout>
#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QtCore/QDir>
#include "baseui.h"

/*! Splash layout of the game
 *
 *  User interface displayed at game start up.
 *  This layout regroup buttons to do the main action of the game.
 */
class StartUI : public BaseUI {
    Q_OBJECT

public:
    StartUI(QWidget* parent = 0);
    QString layoutTitle();

signals:
    void displayUI(QString layoutName);

private slots:
    void displayLayout();

private:
    /// Main layout containing the buttons and the image
    QGridLayout* m_startPanel;
    /// Multiplayer game button
    QPushButton* m_multiplayerGameButton;
    /// About game button
    QPushButton* m_aboutGameButton;
    /// Exit game button
    QPushButton* m_exitGameButton;
    /// Display splash image
    QLabel* m_gameSplash;
};

#endif	/* _STARTUI_H */