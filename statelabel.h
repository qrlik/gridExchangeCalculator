#ifndef STATELABEL_H
#define STATELABEL_H

#include <QLabel>
#include "statecontroller.h"

class StateLabel : public QLabel
{
	Q_OBJECT
public slots:
	void updateUpperPrice(QString aValue);
	void updateCurrentPrice(QString aValue);
	void updateLowerPrice(QString aValue);
	void updateStopLossPrice(QString aValue);

	void updateBuyTax(QString aValue);
	void updateSellTax(QString aValue);

	void updateGridsAmount(int aValue);
public:
	StateLabel(QWidget* aParent = nullptr);

signals:
	void upperPriceChanged(QString);
	void currentPriceChanged(QString);
	void lowerPriceChanged(QString);
	void stopLossPriceChanged(QString);

	void buyTaxChanged(QString);
	void sellTaxChanged(QString);

private:
	void updateStateText();

	StateController stateController;
};

#endif // STATELABEL_H
