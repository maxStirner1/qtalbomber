/*
 *  QtalBomber - A Qt BomberMan game <http://www.bombermanproject.com/>
 *  Copyright (C) 2010  QtalBomber team <team@bombermanproject.com>
 *
 *  Authors: Quentin Amoudruz
 *           Thomas Savineau
 *           Alexandre Joseph
 *           Lucas Courot
 *
 *  File:        StartUI.cpp
 *  Description: Game start layout.
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

#include "ui/StartUI.h"

StartUI::StartUI(QWidget* parent) :QWidget(parent) {
    m_playGameButton = new QPushButton(tr("Start game"));
    m_hostGameButton = new QPushButton(tr("Host server"));
    m_exitGameButton = new QPushButton(tr("Exit game"));
    
    m_gameSplash = new QLabel();
    m_gameSplash->setPixmap(QPixmap(":/qtal-splash"));

    m_startPanel = new QGridLayout;
    m_startPanel->addWidget(m_gameSplash, 0, 0, 3, 1);
    m_startPanel->addWidget(m_playGameButton, 1, 1);
    m_startPanel->addWidget(m_hostGameButton, 2, 1);
    m_startPanel->addWidget(m_exitGameButton, 3, 1);

    setLayout(m_startPanel);

    connect(m_playGameButton, SIGNAL(clicked()), parent, SLOT(displayClientUI()));
    connect(m_hostGameButton, SIGNAL(clicked()), parent, SLOT(displayServerUI()));
    connect(m_exitGameButton, SIGNAL(clicked()), parent, SLOT(exitGame()));
}

QString StartUI::layoutTitle() {
    return tr("BomberMan Game");
}