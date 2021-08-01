#ifndef DATACONTROLLER_H
#define DATACONTROLLER_H

#include "QFlag"
#include "QObject"
#include "QVector"
#include "cstdint"

#define DATA_CONTROLLER dataController::getInstance()

using currency = int64_t;
using percents = double;
using factor = double;

struct inputData {
	currency upperPrice = 0;
	currency currentPrice = 0;
	currency lowerPrice = 0;
	currency stopLossPrice = 0;
	factor baseTax = 0.0;
	int gridsAmount = 0;
};

struct gridInfo {
	currency price = 0;
	currency profit = 0;
	currency tax = 0;
};

struct outputData {
	QVector<gridInfo> grids;
	QPair<factor, factor> taxRange = { 0.0, 0.0 }; // max(lower price), min(upper price)
	QPair<percents, percents> gridProfitRange = { 0.0, 0.0 }; // min(lower grids), max(upper grids)
	QPair<percents, percents> positionProfitRange = { 0.0, 0.0 }; // min(lower grids), max(upper grids)
	QPair<percents, percents> spengindOnTaxRange = { 0.0, 0.0 }; // min(lower grids), max(upper grids)
	currency minPosition = 0;
	int maxGridsAmount = 0;
};

class dataController : public QObject
{
	Q_OBJECT
public:
	static dataController& getInstance();

public:
	void updateData();
	currency updateUpperPrice(QString aPrice);
	currency updateCurrentPrice(QString aPrice);
	currency updateLowerPrice(QString aPrice);
	currency updateStopLossPrice(QString aPrice);
	percents updateTax(QString aTax);
	void updateGridsAmount(int aValue);

	const inputData& getInputData() const;
	const outputData& getOutputData() const;
	int getPrecision() const;
	void setPrecision(int aPrecision);

signals:
	void gridsAmountRangeChanged(int, int);
	void taxRangeChanged(QString);
	void gridProfitChanged(QString);
	void positionProfitChanged(QString);
	void taxSpendingChanged(QString);
	void gridsListChanged(QVector<gridInfo>);
	void minPositionChanged(QString);

private:
	dataController() = default;
	~dataController() = default;
	dataController(const dataController&) = delete;
	dataController(dataController&&) = delete;
	dataController& operator=(const dataController&) = delete;
	dataController& operator=(dataController&&) = delete;

	void updateOutput();
	void emitOutput();
	currency updateVariable(QString aString, currency& aVariable);
	void updateTaxRange();
	bool checkMaxGridsAmount(int aGridsAmount);
	void updateMaxGridsAmount();
	void updateProfitAndSpending();
	void updateGrids();
	QPair<currency, factor> calculateTax(currency aPrice);
	QVector<double> calculatePureGrids(int aGridsAmount);

	inputData inputData;
	outputData outputData;
	int precision = 0;
};

#endif // DATACONTROLLER_H
