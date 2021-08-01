#ifndef STATELABEL_H
#define STATELABEL_H

#include <QLabel>
#include "systems/dataController.h"

class StateLabel : public QLabel
{
	Q_OBJECT
public:
	StateLabel(QWidget* aParent = nullptr);
	void update();

signals:
	void gridsAmountEnableChanged(bool);
};

#endif // STATELABEL_H
