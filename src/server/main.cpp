/*
 * File:   main.cpp
 * Author: alex
 *
 * Created on April 12, 2010, 3:38 PM
 */

#include <QtGui/QApplication>

#include "ServerUI.h"

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);

    ServerUI ui;
    ui.show();

    return app.exec();
}
