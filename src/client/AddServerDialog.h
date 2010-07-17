/*
 *  QtalBomber - A Qt BomberMan game <http://www.bombermanproject.com/>
 *  Copyright (C) 2010  QtalBomber team <team@bombermanproject.com>
 *
 *  Authors: Quentin Amoudruz
 *           Thomas Savineau
 *           Alexandre Joseph
 *           Lucas Courot
 *
 *  File:        AddServerDialog.h
 *  Description: Header file for AddServerDialog class.
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

#ifndef _ADDSERVERDIALOG_H
#define	_ADDSERVERDIALOG_H

#include <QtGui>

class AddServerDialog : public QDialog {
    Q_OBJECT

public:
    AddServerDialog(QWidget* parent = 0);

signals:
    void serverAdded(QStringList);

private slots:
    void addServer();
    void cancelAction();

private:
    QFormLayout* m_fieldLayout;
    QGridLayout* m_mainLayout;
    QLineEdit* m_serverAddress;
    QSpinBox* m_serverPort;
    QPushButton* m_addButton;
    QPushButton* m_cancelButton;
};

#endif	/* _ADDSERVERDIALOG_H */

