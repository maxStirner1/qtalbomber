/* 
 * File:   ServerUI.cpp
 * Author: alex
 * 
 * Created on April 12, 2010, 3:40 PM
 */

#include <QtCore/qcoreapplication.h>
#include <QtGui/qdialog.h>
#include <QtGui/qmessagebox.h>

#include "ServerUI.h"

ServerUI::ServerUI() {
    // Any better idea?
    setWindowTitle("BomberMan Server");

    m_startButton = new QPushButton(tr("Start"));
    m_stopButton = new QPushButton(tr("Stop"));
    m_stopButton->setDisabled(true);
    m_exitButton = new QPushButton(tr("Exit"));

    m_buttonsLayout = new QHBoxLayout;
    m_buttonsLayout->addWidget(m_startButton);
    m_buttonsLayout->addWidget(m_stopButton);
    m_buttonsLayout->addWidget(m_exitButton);

    // Save value in prefs?
    m_gameMap = new QComboBox();
    // Strange policy application...
    m_gameMap->setInsertPolicy(QComboBox::InsertAlphabetically);
    initMapList();
    m_gameMap->setCurrentIndex(0);

    m_serverName = new QLineEdit;
    // Save value in prefs?
    m_serverName->setText("BomberMan Server");

    m_serverPort = new QSpinBox();
    m_serverPort->setRange(0, 65535);
    // Save value in prefs?
    m_serverPort->setValue(31415);

    // Between 1 and 4 players
    m_gamePlayers = new QSpinBox();
    m_gamePlayers->setRange(1, 4);
    // Save value in prefs?
    m_gamePlayers->setValue(4);

    // Between 0 and 3 bots
    m_gameBot = new QSpinBox();
    m_gameBot->setRange(0, 3);
    // Save value in prefs?
    m_gameBot->setValue(0);

    // Between 1 and 10 rounds
    m_roundsNb = new QSpinBox();
    m_roundsNb->setRange(1, 10);
    m_roundsNb->setValue(4);

    m_optionsLayout = new QFormLayout;
    m_optionsLayout->addRow(tr("Server name:"), m_serverName);
    m_optionsLayout->addRow(tr("Server port:"), m_serverPort);
    m_optionsLayout->addRow(tr("Game map:"), m_gameMap);
    m_optionsLayout->addRow(tr("Player number:"), m_gamePlayers);
    m_optionsLayout->addRow(tr("Bot number:"), m_gameBot);

    m_mapPreview = new QLabel;
    mapPreview(m_gameMap->currentIndex());


    m_mainLayout = new QGridLayout;
    m_mainLayout->addLayout(m_optionsLayout, 0, 0);
    m_mainLayout->addLayout(m_buttonsLayout, 1, 0);
    m_mainLayout->addWidget(m_mapPreview, 0, 1, 2, 1);

    connectEvent();

    setLayout(m_mainLayout);
}

void ServerUI::connectEvent() {
    QObject::connect(m_exitButton, SIGNAL(clicked()), this, SLOT(closeUISlot()));
    QObject::connect(this, SIGNAL(closeUISignal()), qApp, SLOT(quit()));

    QObject::connect(m_startButton, SIGNAL(clicked()), this, SLOT(startServerSlot()));
    QObject::connect(m_stopButton, SIGNAL(clicked()), this, SLOT(stopServerSlot()));

    QObject::connect(m_gameBot, SIGNAL(valueChanged(int)), this, SLOT(nbBotSlot(int)));
    QObject::connect(m_gamePlayers, SIGNAL(valueChanged(int)), this, SLOT(nbPlayerSlot(int)));

    QObject::connect(m_gameMap, SIGNAL(activated(int)), this, SLOT(gameMapSlot(int)));
}

void ServerUI::startServerSlot() {
    m_startButton->setEnabled(false);
    m_stopButton->setEnabled(true);
}

void ServerUI::stopServerSlot() {
    m_startButton->setEnabled(true);
    m_stopButton->setEnabled(false);
}

void ServerUI::closeUISlot() {
    if (m_stopButton->isEnabled()) {
        QMessageBox::warning(this, tr("Close window"), tr("The server will still run until you click on the stop button"));
    }

    emit closeUISignal();
}

void ServerUI::nbBotSlot(int botNb) {
   m_gameBot->setValue(botNb);

   if ((m_gamePlayers->value() + botNb) > 4 ) {
       m_gamePlayers->setValue(4 - botNb);
   }
}

void ServerUI::nbPlayerSlot(int playerNb) {
    m_gamePlayers->setValue(playerNb);

    if ((m_gameBot->value() + playerNb) > 4) {
        m_gameBot->setValue(4 - playerNb);
    }
}

void ServerUI::initMapList() {
    QDir mapDir;

    if (!mapDir.cd("maps")) {
        noValidMap();
    }
    else {
        mapDir.setFilter(QDir::Dirs | QDir::NoSymLinks);
        mapDir.setSorting(QDir::Name);

        QFileInfoList mapList = mapDir.entryInfoList();
        for (int i = 0; i < mapList.size(); i++) {
            // Check map is valid
            if (mapDir.exists(mapList.at(i).fileName() + "/map.xml")) {
                m_gameMap->addItem(mapList.at(i).fileName().replace('_', ' '), mapList.at(i).absoluteFilePath());
            }
        }

        if (m_gameMap->count() < 1) {
            noValidMap();
        }
        else {
            m_gameMap->insertItem(0, "[Random]", "random");
            m_gameMap->insertItem(1, "[Autogenerate]", "auto");
        }
    }
}

void ServerUI::noValidMap() {
    QMessageBox::critical(this, "Map directory", "Map directory does not exists in server folder.\n"
        "Please download some map and restart the server");

    m_startButton->setEnabled(false);
}

void ServerUI::mapPreview(int index) {
    QString previewPix = m_gameMap->itemData(index).toString();

    if (previewPix == "auto" || previewPix == "random") {
        m_mapPreview->setPixmap(QPixmap("bomberman.png"));
    }
    else {
        m_mapPreview->setPixmap(QPixmap(previewPix + "/preview.png"));
    }
}

void ServerUI::gameMapSlot(int index) {
    mapPreview(index);
}