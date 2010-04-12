/* 
 * File:   ServerUI.h
 * Author: alex
 *
 * Created on April 12, 2010, 3:40 PM
 */

#ifndef _SERVERUI_H
#define	_SERVERUI_H

#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QGridLayout>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QImage>
#include <QDir>
#include <QDebug>
#include <QComboBox>
#include <QFileInfo>

class ServerUI : public QWidget {
    Q_OBJECT

public:
    ServerUI();

signals:
    void closeUISignal();

private slots:
    void startServerSlot();
    void stopServerSlot();
    void closeUISlot();
    void nbBotSlot(int botNb);
    void nbPlayerSlot(int playerNb);
    void gameMapSlot(int index);

private:
    QGridLayout *m_mainLayout;
    QFormLayout *m_optionsLayout;
    QLineEdit *m_serverName;
    QSpinBox *m_serverPort;
    QComboBox *m_gameMap;
    QSpinBox *m_gamePlayers;
    QSpinBox *m_gameBot;
    QSpinBox *m_roundsNb;
    QHBoxLayout *m_buttonsLayout;
    QPushButton *m_startButton;
    QPushButton *m_stopButton;
    QPushButton *m_exitButton;
    QLabel *m_mapPreview;
    void connectEvent();
    void initMapList();
    void noValidMap();
    void mapPreview(int index);
};

#endif	/* _SERVERUI_H */

