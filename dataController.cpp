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

double dataController::updateBuyTax(QString aValue)
{
	auto taxPercent = updateDoubleVariable(aValue, inputData.buyTax);
	inputData.buyTax /= 100;
	return taxPercent;
}

double dataController::updateSellTax(QString aValue)
{
	auto taxPercent = updateDoubleVariable(aValue, inputData.sellTax);
	inputData.sellTax /= 100;
	return taxPercent;
}

void dataController::updateGridsAmount(int aValue)
{
	inputData.gridsAmount = aValue;
}

void dataController::updateMaxGridsAmount()
{
	auto logUpper = std::log2(inputData.upperPrice / inputData.lowerPrice);
	auto logLower = std::log2(inputData.buyTax + inputData.sellTax + 1);
	auto compare = logUpper / logLower - 1;
	outputData.maxGridsAmount = std::trunc(compare);
}

void dataController::updateOutput()
{
	updateMaxGridsAmount();
}

const inputData& dataController::getInputData() const
{
	return inputData;
}

const outputData& dataController::getOutputData() const
{
	return outputData;
}
