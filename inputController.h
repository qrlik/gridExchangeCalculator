#ifndef INPUTCONTROLLER_H
#define INPUTCONTROLLER_H

#include "QFlag"
#include "QObject"

class inputController : public QObject
{
	Q_OBJECT
public:
	inputController() = default;

	double updateUpperPrice(QString aValue);
	double updateCurrentPrice(QString aValue);
	double updateLowerPrice(QString aValue);
	double updateStopLossPrice(QString aValue);

	double updateBuyTax(QString aValue);
	double updateSellTax(QString aValue);

	void updateGridsAmount(int aValue);

	double getUpperPrice() const;
	double getCurrentPrice() const;
	double getLowerPrice() const;
	double getStopLossPrice() const;
	double getBuyTax() const;
	double getSellTax() const;
	int getGridsAmount() const;
private:
	double updateDoubleVariable(QString aString, double& aVariable);

	double upperPrice = 0.0;
	double currentPrice = 0.0;
	double lowerPrice = 0.0;
	double stopLossPrice = 0.0;
	double buyTax = 0.0;
	double sellTax = 0.0;
	int gridsAmount = 0;
};

#endif // INPUTCONTROLLER_H
