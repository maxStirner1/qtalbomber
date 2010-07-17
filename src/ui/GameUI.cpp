/*
 *  QtalBomber - A Qt BomberMan game <http://www.bombermanproject.com/>
 *  Copyright (C) 2010  QtalBomber team <team@bombermanproject.com>
 *
 *  Authors: Quentin Amoudruz
 *           Thomas Savineau
 *           Alexandre Joseph
 *           Lucas Courot
 *
 *  File:        GameUI.cpp
 *  Description: Manage game layouts.
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

#include "ui/GameUI.h"
#include "ui/ClientUI.h"

GameUI::GameUI() {
    m_layoutStack = new QStackedLayout;

    m_startUI = new StartUI(this);
    m_clientUI = new ClientUI(this);
    m_serverUI = new ServerUI(this);
    m_gameFieldUI = new GameFieldUI(this);

    m_layoutStack->addWidget(m_startUI);
    m_layoutStack->addWidget(m_clientUI);
    m_layoutStack->addWidget(m_serverUI);
    m_layoutStack->addWidget(m_gameFieldUI);

    displayStartUI();
    setLayout(m_layoutStack);
    setWindowFlags(Qt::Window);
    setFixedSize(640, 515);

    setWindowIcon(QIcon(":/qtal-icon"));

    connect(this, SIGNAL(quitGame()), m_serverUI, SLOT(stopServer()));
    connect(this, SIGNAL(quitGame()), m_serverUI, SLOT(dumpServerSettings()));
    connect(this, SIGNAL(quitGame()), m_clientUI, SLOT(dumpPlayerSettings()));
    connect(this, SIGNAL(quitGame()), qApp, SLOT(quit()));
}

StartUI* GameUI::startUI() {
    return m_startUI;
}

ClientUI* GameUI::clientUI() {
    return m_clientUI;
}

ServerUI* GameUI::serverUI() {
    return m_serverUI;
}

GameFieldUI* GameUI::gameFieldUI() {
    return m_gameFieldUI;
}

/************************************************
 * SLOTS
 ***********************************************/

void GameUI::displayStartUI() {
    setWindowTitle(m_startUI->layoutTitle());
    m_layoutStack->setCurrentWidget(m_startUI);
}

void GameUI::displayClientUI() {
    setWindowTitle(m_clientUI->layoutTitle());
    m_layoutStack->setCurrentWidget(m_clientUI);
}

void GameUI::displayServerUI() {
    setWindowTitle(m_serverUI->layoutTitle());
    m_layoutStack->setCurrentWidget(m_serverUI);
    m_serverUI->checkServerReady();
}

void GameUI::displayGameFieldUI() {
    setWindowTitle(m_gameFieldUI->layoutTitle());
    m_layoutStack->setCurrentWidget(m_gameFieldUI);
}

void GameUI::exitGame() {
    emit quitGame();
}
