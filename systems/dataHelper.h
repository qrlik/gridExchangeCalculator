#ifndef DATAHELPER_H
#define DATAHELPER_H

#include <QVector>
#include "systems/dataController.h"

class dataHelper
{
public:
	static QPair<currency, factor> calculateTax(currency aPrice);
	static int calculateMaxGridsAmount();
	static QPair<double, double> calculateProfitAndSpending(currency aFirstPrice, currency aSecondPrice); // gridProfit, spengindOnTax
	static currency updateGrids(QVector<gridInfo>& aGrids);
private:
	static QVector<double> calculatePureGrids(int aGridsAmount);
	static bool checkMaxGridsAmount(int aGridsAmount);
};

#endif // DATAHELPER_H
