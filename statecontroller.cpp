#include "statecontroller.h"
#include "globalvariables.h"
#include "cmath"
#include "math.h"
#include "QtDebug"


double StateController::updateDoubleVariable(QString aString, double& aVariable, eValuesState aState)
{
	bool success = false;
	aVariable = aString.toDouble(&success);
	if (success)
	{
		if (aVariable > 0.f)
		{
			valuesState.setFlag(aState);
		}
		else if (valuesState.testFlag(aState))
		{
			valuesState ^= aState;
		}
	}
	else if (valuesState.testFlag(aState))
	{
		valuesState ^= aState;
	}
	return aVariable;
}

double StateController::updateUpperPrice(QString aValue)
{
	return updateDoubleVariable(aValue, upperPrice, eValuesState::upperPrice);
}

double StateController::updateCurrentPrice(QString aValue)
{
	return updateDoubleVariable(aValue, currentPrice, eValuesState::currentPrice);
}

double StateController::updateLowerPrice(QString aValue)
{
	return updateDoubleVariable(aValue, lowerPrice, eValuesState::lowerPrice);
}

double StateController::updateStopLossPrice(QString aValue)
{
	return updateDoubleVariable(aValue, stopLossPrice, eValuesState::stopLossPrice);
}

double StateController::updateBuyTax(QString aValue)
{
	return updateDoubleVariable(aValue, buyTax, eValuesState::buyTax);
}

double StateController::updateSellTax(QString aValue)
{
	return updateDoubleVariable(aValue, sellTax, eValuesState::sellTax);
}

void StateController::updateGridsAmount(int aValue)
{
	gridsAmount = aValue;
}
StateController::eValuesState StateController::getCurrentState() const
{
	for (auto flag : { eValuesState::empty, eValuesState::allCorrect })
	{
		if (valuesState.testFlag(flag))
		{
			return flag;
		}
	}
	for (auto flag : { eValuesState::upperPrice, eValuesState::currentPrice, eValuesState::lowerPrice,
		 eValuesState::stopLossPrice, eValuesState::buyTax, eValuesState::sellTax })
	{
		if (!valuesState.testFlag(flag))
		{
			return flag;
		}
	}
}
