#include "about.h"

About::About() {
    QGridLayout* mainLayout = new QGridLayout;
    QTabWidget* tabWidget = new QTabWidget;
    QTextEdit* tabGame = new QTextEdit;
    QTextEdit* tabPlayer1 = new QTextEdit;
    QTextEdit* tabPlayer2 = new QTextEdit;
    QTextEdit* tabPlayer3 = new QTextEdit;
    QTextEdit* tabPlayer4 = new QTextEdit;

    tabGame->setText("About the game");
    tabPlayer1->setText("About player 1");
    tabPlayer2->setText("About player 2");
    tabPlayer3->setText("About player 3");
    tabPlayer4->setText("About player 4");

    tabWidget->addTab(tabGame, tr("Game"));
    tabWidget->addTab(tabPlayer1, tr("Player 1"));
    tabWidget->addTab(tabPlayer2, tr("Player 2"));
    tabWidget->addTab(tabPlayer3, tr("Player 3"));
    tabWidget->addTab(tabPlayer4, tr("Player 4"));

    mainLayout->addWidget(tabWidget, 0, 0);

    setWindowTitle(tr("About QtalBomber"));
    setLayout(mainLayout);

}
