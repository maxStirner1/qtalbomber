/*
 *  QtalBomber - A Qt BomberMan game <http://www.bombermanproject.com/>
 *  Copyright (C) 2010  QtalBomber team <team@bombermanproject.com>
 *
 *  Authors: Quentin Amoudruz
 *           Thomas Savineau
 *           Alexandre Joseph
 *           Lucas Courot
 *
 *  File:        AddServerDialog.cpp
 *  Description: Dialog box to add new server
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

#include "client/AddServerDialog.h"

AddServerDialog::AddServerDialog(QWidget* parent) : QDialog(parent) {
    m_mainLayout = new QGridLayout;
    m_fieldLayout = new QFormLayout;

    m_serverAddress = new QLineEdit;
    m_serverAddress->setMinimumWidth(180);
    m_serverAddress->setText("localhost");
    m_fieldLayout->addRow(tr("Server address:"), m_serverAddress);

    m_serverPort = new QSpinBox;
    m_serverPort->setRange(0, 65535);
    m_serverPort->setValue(31415);
    m_fieldLayout->addRow(tr("Port:"), m_serverPort);

    m_mainLayout->addLayout(m_fieldLayout, 0, 0, 1, 2);

    m_addButton = new QPushButton(tr("Add"));
    m_mainLayout->addWidget(m_addButton, 1, 0, Qt::AlignLeft);
    connect(m_addButton, SIGNAL(clicked()), this, SLOT(addServer()));

    m_cancelButton = new QPushButton(tr("Cancel"));
    m_mainLayout->addWidget(m_cancelButton, 1, 1, Qt::AlignRight);
    connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(cancelAction()));

    setWindowTitle(tr("Add server"));
    setLayout(m_mainLayout);
}

void AddServerDialog::addServer() {
    QStringList serverProperties;
    serverProperties << m_serverAddress->text() << QString::number(m_serverPort->value());
    
    setVisible(false);

    emit serverAdded(serverProperties);
}

void AddServerDialog::cancelAction() {
    setVisible(false);
}
