/*
 *  QtalBomber - A Qt BomberMan game <http://www.bombermanproject.com/>
 *  Copyright (C) 2010  QtalBomber team <team@bombermanproject.com>
 *
 *  Authors: Quentin Amoudruz
 *           Thomas Savineau
 *           Alexandre Joseph
 *           Lucas Courot
 *
 *  File:        StartUI.h
 *  Description: Header file for StartUI class.
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

#ifndef _STARTUI_H
#define	_STARTUI_H

#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QDir>

class StartUI : public QWidget {
    Q_OBJECT
    
public:
    StartUI(QWidget* parent = 0);
    QString layoutTitle();

private:
    QGridLayout* m_startPanel;
    QPushButton* m_playGameButton;
    QPushButton* m_hostGameButton;
    QPushButton* m_exitGameButton;
    QLabel* m_gameSplash;
};

#endif	/* _STARTUI_H */

