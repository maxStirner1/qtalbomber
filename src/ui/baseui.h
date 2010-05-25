#ifndef _BASEUI_H
#define	_BASEUI_H

#include <QtGui/QWidget>
#include <QtCore/QString>

/*! Base for each interface of the game.
 *
 *  This abstract class define the methods a user interface of the game must
 *  implement.
 */
class BaseUI : public QWidget {
    Q_OBJECT
    
public:
    /// Return the title of the current layout (user interface)
    virtual QString layoutTitle() = 0;
};

#endif	/* _BASEUI_H */

