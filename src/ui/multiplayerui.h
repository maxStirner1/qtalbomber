#ifndef _MULTYPLAYERUI_H
#define	_MULTYPLAYERUI_H

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QList>
#include <QtGui/QApplication>
#include <QtGui/QColor>
#include <QtGui/QPushButton>
#include <QtGui/QComboBox>
#include <QtGui/QGroupBox>
#include <QtGui/QSpinBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QFormLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QLineEdit>
#include <QtGui/QLabel>
#include <QtGui/QColorDialog>
#include "ui/baseui.h"
#include "game/map.h"
#include "qtalbomber.h"

#include <QtCore/QDebug>

/*! Multiplayer settings window.
 *
 *  Create the settings window for multiplayer game.
 */
class MultiplayerUI : public BaseUI {
    Q_OBJECT

public:
    MultiplayerUI(QWidget* parent);

private slots:
    void changePlayerColor();
    void displayStartUI();

private:
    /// Pointer to the parent who created the instance
    QWidget* m_parent;
    /// Holds the players pseudo
    QMap<int, QLineEdit*> m_playersPseudo;
    /// Contains the player color
    QMap<int, QColor> m_playersColor;
    /// Button list to display player color
    QMap<int, QPushButton*> m_playersColorButton;
    /// Holds the type of the player (real, bot, none)
    QMap<int, QComboBox*> m_playersType;
    /// Player controls input
    QMap<int, QMap<QString, QLineEdit*> > m_playersControl;
    /// Player keys configuration : (playerId, (key, action))
    QMap<int, QMap<int, QString> > m_playersKey;
    /// Numbers of round
    QSpinBox* m_gameRounds;
    /// Combo box containing map list
    QComboBox* m_mapComboBox;
    /// Instances of the availables maps
    QMap<int, Map*> m_mapList;

    void loadMaps();
    void setPlayerColor(int playerId, QColor color);
    QString layoutTitle();
    void setDefaultKeys();
};

#endif	/* _MULTYPLAYERUI_H */

