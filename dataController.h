#ifndef DATACONTROLLER_H
#define DATACONTROLLER_H

#include "QFlag"
#include "QObject"
#include "QVector"

using currency = double;
using percents = double;
using factor = double;

struct inputData {
	currency upperPrice = 0.0;
	currency currentPrice = 0.0;
	currency lowerPrice = 0.0;
	currency stopLossPrice = 0.0;
	factor baseTax = 0.0;
	int gridsAmount = 0;
};

struct outputData {
	QVector<currency> grids;
	factor gridFactor = 0.0;
	percents gridProfit = 0.0;
	percents positionProfit = 0.0;
	percents spendingOnTax = 0.0;
	factor lowerPriceTax = 0.0;
	factor upperPriceTax = 0.0;
	int maxGridsAmount = 0;
};

class dataController : public QObject
{
	Q_OBJECT
public:
	dataController() = default;

	currency updateUpperPrice(QString aValue);
	currency updateCurrentPrice(QString aValue);
	currency updateLowerPrice(QString aValue);
	currency updateStopLossPrice(QString aValue);
	percents updateTax(QString aValue);
	void updateGridsAmount(int aValue);

	void updateOutput();

	const inputData& getInputData() const;
	const outputData& getOutputData() const;
	int getPrecision() const;
	void setPrecision(int aPrecision);

private:
	double updateDoubleVariable(QString aString, double& aVariable);
	void updateTaxRange();
	void updateMaxGridsAmount();
	void updateProfitAndSpending();
	void updateGrids();
	QPair<currency, factor> calculateTax(currency aPrice);

	inputData inputData;
	outputData outputData;
	int precision = 0;
};

#endif // DATACONTROLLER_H
