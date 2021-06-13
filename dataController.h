#ifndef DATACONTROLLER_H
#define DATACONTROLLER_H

#include "QFlag"
#include "QObject"

struct inputData {
	double upperPrice = 0.0;
	double currentPrice = 0.0;
	double lowerPrice = 0.0;
	double stopLossPrice = 0.0;
	double buyTax = 0.0;
	double sellTax = 0.0;
	int gridsAmount = 0;
};

struct outputData {
	int maxGridsAmount = 0;
};

class dataController : public QObject
{
	Q_OBJECT
public:
	dataController() = default;

	double updateUpperPrice(QString aValue);
	double updateCurrentPrice(QString aValue);
	double updateLowerPrice(QString aValue);
	double updateStopLossPrice(QString aValue);
	double updateBuyTax(QString aValue);
	double updateSellTax(QString aValue);
	void updateGridsAmount(int aValue);

	void updateOutput();

	const inputData& getInputData() const;
	const outputData& getOutputData() const;

private:
	double updateDoubleVariable(QString aString, double& aVariable);
	void updateMaxGridsAmount();

	inputData inputData;
	outputData outputData;
};

#endif // DATACONTROLLER_H
