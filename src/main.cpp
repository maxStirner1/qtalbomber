/*! @mainpage QtalBomber game project
 * 
 *  @section authors Author of the project
 *
 *  @author Quentin Amoudruz
 *  @author Thomas Savineau
 *  @author Alexandre Joseph
 *  @author Lucas Courot
 */

#include <QtGui/QApplication>
#include <QtCore/QTime>
#include "ui/gameui.h"

/*! Start point of the application.
 * @param argc Number of argument passed to the program.
 * @param argv Argument list passed to the program.
 *
 * Create a instance of the game user interface and initialize random function.
 * @see StartUI
 */
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    GameUI ui;
    ui.show();

    return app.exec();
}
