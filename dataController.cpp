#include "dataController.h"
#include "globalvariables.h"
#include "cmath"
#include "math.h"
#include "QtDebug"


double inputController::updateDoubleVariable(QString aString, double& aVariable)
{
	bool success = false;
	aVariable = aString.toDouble(&success);
	return aVariable;
}

double inputController::updateUpperPrice(QString aValue)
{
	return updateDoubleVariable(aValue, inputData.upperPrice);
}

double inputController::updateCurrentPrice(QString aValue)
{
	return updateDoubleVariable(aValue, inputData.currentPrice);
}

double inputController::updateLowerPrice(QString aValue)
{
	return updateDoubleVariable(aValue, inputData.lowerPrice);
}

double inputController::updateStopLossPrice(QString aValue)
{
	return updateDoubleVariable(aValue, inputData.stopLossPrice);
}

double inputController::updateBuyTax(QString aValue)
{
	auto taxPercent = updateDoubleVariable(aValue, inputData.buyTax);
	inputData.buyTax /= 100;
	return taxPercent;
}

double inputController::updateSellTax(QString aValue)
{
	auto taxPercent = updateDoubleVariable(aValue, inputData.sellTax);
	inputData.sellTax /= 100;
	return taxPercent;
}

void inputController::updateGridsAmount(int aValue)
{
	inputData.gridsAmount = aValue;
}

int inputController::updateMaxGridsAmount()
{
	auto logUpper = std::log2(inputData.upperPrice / inputData.lowerPrice);
	auto logLower = std::log2(inputData.buyTax + inputData.sellTax + 1);
	auto compare = logUpper / logLower - 1;
	auto result = std::trunc(compare);
	return result;
}

double inputController::getUpperPrice() const
{
	return inputData.upperPrice;
}

double inputController::getCurrentPrice() const
{
	return inputData.currentPrice;
}

double inputController::getLowerPrice() const
{
	return inputData.lowerPrice;
}

double inputController::getStopLossPrice() const
{
	return inputData.stopLossPrice;
}

double inputController::getBuyTax() const
{
	return inputData.buyTax;
}

double inputController::getSellTax() const
{
	return inputData.sellTax;
}

int inputController::getGridsAmount() const
{
	return inputData.gridsAmount;
}
