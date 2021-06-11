#ifndef STATECONTROLLER_H
#define STATECONTROLLER_H

#include "QFlag"
#include "QObject"

class StateController : public QObject
{
	Q_OBJECT
public:
	enum class eValuesState
	{
		empty = 0x0,
		upperPrice = 0x1,
		currentPrice = 0x2,
		lowerPrice = 0x4,
		stopLossPrice = 0x8,
		buyTax = 0x10,
		sellTax = 0x20,
		allCorrect = 0x3F
	};
	Q_DECLARE_FLAGS(ValuesState, eValuesState);
	Q_FLAG(ValuesState);

	StateController() = default;

	eValuesState getCurrentState() const;

public:
	double updateUpperPrice(QString aValue);
	double updateCurrentPrice(QString aValue);
	double updateLowerPrice(QString aValue);
	double updateStopLossPrice(QString aValue);

	double updateBuyTax(QString aValue);
	double updateSellTax(QString aValue);

	void updateGridsAmount(int aValue);

private:
	double updateDoubleVariable(QString aString, double& aVariable, eValuesState aState);

	double upperPrice = 0.0;
	double currentPrice = 0.0;
	double lowerPrice = 0.0;
	double stopLossPrice = 0.0;
	double buyTax = 0.0;
	double sellTax = 0.0;
	int gridsAmount = 0;
	ValuesState valuesState;
};

#endif // STATECONTROLLER_H
