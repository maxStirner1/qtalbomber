/*
 *  QtalBomber - A Qt BomberMan game <http://www.bombermanproject.com/>
 *  Copyright (C) 2010  QtalBomber team <team@bombermanproject.com>
 *
 *  Authors: Quentin Amoudruz
 *           Thomas Savineau
 *           Alexandre Joseph
 *           Lucas Courot
 *
 *  File:        GameUI.h
 *  Description: Header file for GameUI class.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _GAMEUI_H
#define	_GAMEUI_H

#include "constants.h"
#include "ui/StartUI.h"
#include "ui/ServerUI.h"
#include "ui/GameFieldUI.h"
#include "server/ConnectionHandler.h"
#include <QtNetwork>
#include <QtGui>
#include <QDebug>

class ClientUI;

class GameUI : public QWidget {
    Q_OBJECT
    
public:
    GameUI();
    StartUI* startUI();
    ClientUI* clientUI();
    ServerUI* serverUI();
    GameFieldUI* gameFieldUI();


signals:
    void quitGame();

public slots:
    void displayStartUI();
    void displayClientUI();
    void displayServerUI();
    void displayGameFieldUI();
    void exitGame();

private:
    QStackedLayout* m_layoutStack;
    StartUI* m_startUI;
    ClientUI* m_clientUI;
    ServerUI* m_serverUI;
    GameFieldUI* m_gameFieldUI;
};

#endif	/* _GAMEUI_H */

