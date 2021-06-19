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
	void updateTax(QString aValue);
	void updateGridsAmount(int aValue);

public:
	StateLabel(QWidget* aParent = nullptr);
	void setPrecision(int aPrecision);

signals:
	void upperPriceChanged(QString);
	void currentPriceChanged(QString);
	void lowerPriceChanged(QString);
	void stopLossPriceChanged(QString);
	void taxChanged(QString);
	void gridsAmountEnableChanged(bool);
	void gridsAmountRangeChanged(int, int);

	void gridProfitChanged(QString);
	void positionProfitChanged(QString);
	void taxSpendingChanged(QString);
	void gridsListChanged(const QStringList&);
private:
	void update();
	void updateData();

	dataController stateController;
	int precision = 0;
};

#endif // STATELABEL_H
