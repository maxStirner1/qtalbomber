/*! @mainpage QtalBomber game project
 *
 *  @section authors Author of the project
 *
 *  @author Quentin Amoudruz
 *  @author Thomas Savineau
 *  @author Alexandre Joseph
 *  @author Lucas Courot
 */

#include <QApplication>
#include "gamefield.h"
#include <QTime>
#include <QWidget>
#include <QtGui>


int main(int argc, char *argv[])
{
	QApplication app(argc,argv);
	qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
	
	GameField ui;
	ui.show();

	return app.exec();
}
