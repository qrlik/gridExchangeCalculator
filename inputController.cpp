#include "inputController.h"
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
	return updateDoubleVariable(aValue, upperPrice);
}

double inputController::updateCurrentPrice(QString aValue)
{
	return updateDoubleVariable(aValue, currentPrice);
}

double inputController::updateLowerPrice(QString aValue)
{
	return updateDoubleVariable(aValue, lowerPrice);
}

double inputController::updateStopLossPrice(QString aValue)
{
	return updateDoubleVariable(aValue, stopLossPrice);
}

double inputController::updateBuyTax(QString aValue)
{
	return updateDoubleVariable(aValue, buyTax);
}

double inputController::updateSellTax(QString aValue)
{
	return updateDoubleVariable(aValue, sellTax);
}

void inputController::updateGridsAmount(int aValue)
{
	gridsAmount = aValue;
}

double inputController::getUpperPrice() const
{
	return upperPrice;
}

double inputController::getCurrentPrice() const
{
	return currentPrice;
}

double inputController::getLowerPrice() const
{
	return lowerPrice;
}

double inputController::getStopLossPrice() const
{
	return stopLossPrice;
}

double inputController::getBuyTax() const
{
	return buyTax;
}

double inputController::getSellTax() const
{
	return sellTax;
}

int inputController::getGridsAmount() const
{
	return gridsAmount;
}
