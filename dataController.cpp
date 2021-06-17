#include "dataController.h"
#include "globalvariables.h"
#include "cmath"
#include "math.h"
#include "QtDebug"


double dataController::updateDoubleVariable(QString aString, double& aVariable)
{
	bool success = false;
	aVariable = aString.toDouble(&success);
	return aVariable;
}

double dataController::updateUpperPrice(QString aValue)
{
	return updateDoubleVariable(aValue, inputData.upperPrice);
}

double dataController::updateCurrentPrice(QString aValue)
{
	return updateDoubleVariable(aValue, inputData.currentPrice);
}

double dataController::updateLowerPrice(QString aValue)
{
	return updateDoubleVariable(aValue, inputData.lowerPrice);
}

double dataController::updateStopLossPrice(QString aValue)
{
	return updateDoubleVariable(aValue, inputData.stopLossPrice);
}

double dataController::updateTax(QString aValue)
{
	auto taxPercent = updateDoubleVariable(aValue, inputData.tax);
	inputData.tax /= 100;
	return taxPercent;
}

void dataController::updateGridsAmount(int aValue)
{
	inputData.gridsAmount = aValue;
}

void dataController::updateMaxGridsAmount()
{
	auto logUpper = log2(inputData.upperPrice / inputData.lowerPrice);
	auto logLower = log2(inputData.tax * 2 + 1);
	auto compare = logUpper / logLower - 1;
	outputData.maxGridsAmount = trunc(compare);
}

void dataController::updateProfitAndSpending()
{
	auto exp = 1.0 / (inputData.gridsAmount + 1);
	auto taxesInPercents = inputData.tax * 2 * 100;
	outputData.gridProfit = (pow(inputData.upperPrice / inputData.lowerPrice, exp) - 1) * 100 - taxesInPercents;
	outputData.positionProfit = outputData.gridProfit / (inputData.gridsAmount + 1);
	outputData.spendingOnTax = taxesInPercents / (outputData.gridProfit + taxesInPercents) * 100;
}

void dataController::updateOutput()
{
	updateMaxGridsAmount();
	updateProfitAndSpending();
}

const inputData& dataController::getInputData() const
{
	return inputData;
}

const outputData& dataController::getOutputData() const
{
	return outputData;
}
