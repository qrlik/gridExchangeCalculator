#ifndef DATACONTROLLER_H
#define DATACONTROLLER_H

#include "QFlag"
#include "QObject"
#include "QVector"

struct inputData {
	double upperPrice = 0.0;
	double currentPrice = 0.0;
	double lowerPrice = 0.0;
	double stopLossPrice = 0.0;
	double tax = 0.0;
	int gridsAmount = 0;
};

struct outputData {
	QVector<double> grids;
    double gridProfit = 0.0;
    double positionProfit = 0.0;
    double spendingOnTax = 0.0;
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
	double updateTax(QString aValue);
	void updateGridsAmount(int aValue);

	void updateOutput();

	const inputData& getInputData() const;
	const outputData& getOutputData() const;

private:
	double updateDoubleVariable(QString aString, double& aVariable);
	void updateMaxGridsAmount();
	void updateProfitAndSpending();
	void updateGrids();

	inputData inputData;
	outputData outputData;
};

#endif // DATACONTROLLER_H
