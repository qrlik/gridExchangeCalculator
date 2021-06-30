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
	QPair<factor, factor> taxRange = { 0.0, 0.0 }; // max(lower price), min(upper price)
	QPair<percents, percents> gridProfitRange = { 0.0, 0.0 }; // min(lower grids), max(upper grids)
	QPair<percents, percents> positionProfitRange = { 0.0, 0.0 }; // min(lower grids), max(upper grids)
	QPair<percents, percents> spengindOnTaxRange = { 0.0, 0.0 }; // min(lower grids), max(upper grids)
	currency minPosition = 0.0;
	int maxGridsAmount = 0;
};

class dataController : public QObject
{
	Q_OBJECT
public:
	dataController();

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
#ifdef QT_DEBUG
	void testTax();
#endif

	inputData inputData;
	outputData outputData;
	int precision = 0;
};

#endif // DATACONTROLLER_H
