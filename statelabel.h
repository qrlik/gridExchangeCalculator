#ifndef STATELABEL_H
#define STATELABEL_H

#include <QLabel>
#include "dataController.h"

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
	void setPrecision(int aPrecision);

signals:
	void upperPriceChanged(QString);
	void currentPriceChanged(QString);
	void lowerPriceChanged(QString);
	void stopLossPriceChanged(QString);
	void buyTaxChanged(QString);
	void sellTaxChanged(QString);
	void gridsAmountSliderEnableChanged(bool);
	void gridsAmountSliderRangeChanged(int, int);

private:
	void update();
	void updateData();

	dataController stateController;
	int precision = 0;
};

#endif // STATELABEL_H
