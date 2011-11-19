#include "about.h"

About::About() {
    QGridLayout* mainLayout = new QGridLayout;
    QTabWidget* tabWidget = new QTabWidget;
    QLabel* tabGame = new QLabel();
    QLabel* tabPlayer1 = new QLabel();
    QLabel* tabPlayer2 = new QLabel();
    QLabel* tabPlayer3 = new QLabel();
    QLabel* tabPlayer4 = new QLabel();

    tabGame->setText(
"Bomberman is a bomber, hero of the eponymous video game, appeared on the<br />"
"Nintendo NES in 1987. We were inspired by this character to make this<br />"
"C++/Qt project. The aim of this game is to eliminate its opponents<br />"
"by placing bombs and collecting power-ups hidden beneath destructible<br />"
"blocks in a maze. the parties can play up to four players controlled by<br />"
"physical player.");
    tabPlayer1->setText("<table>"
"<tr><th>Action</th>"
"<th>Key stroke</th></tr>"
"<tr><td>Up</td><td>Top arrow</td></tr>"
"<tr><td>Down</td><td>Bottom arrow</td></tr>"
"<tr><td>Left</td><td>Left arrow</td></tr>"
"<tr><td>Right</td><td>Right arrow</td></tr>"
"<tr><td>Drop bomb</td><td>Cmd (or right Alt)</td></tr>"
"</table>");
    tabPlayer2->setText(
"<table>"
"<tr><th>Action</th>"
"<th>Key stroke</th></tr>"
"<tr><td>Up</td><td>Z</td></tr>"
"<tr><td>Down</td><td>S</td></tr>"
"<tr><td>Left</td><td>Q</td></tr>"
"<tr><td>Right</td><td>D</td></tr>"
"<tr><td>Drop bomb</td><td>A</td></tr>"
"</table>");
    tabPlayer3->setText("<table>"
"<tr><th>Action</th>"
"<th>Key stroke</th></tr>"
"<tr><td>Up</td><td>U</td></tr>"
"<tr><td>Down</td><td>J</td></tr>"
"<tr><td>Left</td><td>H</td></tr>"
"<tr><td>Right</td><td>K</td></tr>"
"<tr><td>Drop bomb</td><td>Y</td></tr>"
"</table>");
    tabPlayer4->setText("<table>"
"<tr><th>Action</th>"
"<th>Key stroke</th></tr>"
"<tr><td>Up</td><td>G</td></tr>"
"<tr><td>Down</td><td>B</td></tr>"
"<tr><td>Left</td><td>V</td></tr>"
"<tr><td>Right</td><td>N</td></tr>"
"<tr><td>Drop bomb</td><td>Space</td></tr>"
"</table>");

    tabWidget->addTab(tabGame, tr("Game"));
    tabWidget->addTab(tabPlayer1, tr("Player 1"));
    tabWidget->addTab(tabPlayer2, tr("Player 2"));
    tabWidget->addTab(tabPlayer3, tr("Player 3"));
    tabWidget->addTab(tabPlayer4, tr("Player 4"));

    mainLayout->addWidget(tabWidget, 0, 0);

    setWindowTitle(tr("About QtalBomber"));
    setLayout(mainLayout);

}
