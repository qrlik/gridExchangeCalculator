#include "dataController.h"
#include "globalvariables.h"
#include "cmath"
#include "math.h"
#include "QtDebug"

namespace {
	double myCeil(double aValue, int aPrecision)
	{
		const auto tenFactor = std::pow(10, aPrecision);
		return std::ceil(aValue * tenFactor) / tenFactor;
	}
}

double dataController::updateDoubleVariable(QString aString, double& aVariable)
{
	bool success = false;
	aVariable = aString.toDouble(&success);
	return aVariable;
}

currency dataController::updateUpperPrice(QString aValue)
{
	return updateDoubleVariable(aValue, inputData.upperPrice);
}

currency dataController::updateCurrentPrice(QString aValue)
{
	return updateDoubleVariable(aValue, inputData.currentPrice);
}

currency dataController::updateLowerPrice(QString aValue)
{
	return updateDoubleVariable(aValue, inputData.lowerPrice);
}

currency dataController::updateStopLossPrice(QString aValue)
{
	return updateDoubleVariable(aValue, inputData.stopLossPrice);
}

percents dataController::updateTax(QString aValue)
{
	auto taxPercent = updateDoubleVariable(aValue, inputData.baseTax);
	inputData.baseTax /= 100;
	return taxPercent;
}

void dataController::updateGridsAmount(int aValue)
{
	inputData.gridsAmount = aValue;
}

void dataController::updateMaxGridsAmount()
{
	auto logUpper = log2(inputData.upperPrice / inputData.lowerPrice);
	auto logLower = log2(inputData.baseTax * 2 + 1);
	auto compare = logUpper / logLower - 1;
	outputData.maxGridsAmount = trunc(compare);
}

void dataController::updateProfitAndSpending()
{
	auto exp = 1.0 / (inputData.gridsAmount + 1);
	auto taxesInPercents = inputData.baseTax * 2 * 100;
	outputData.gridProfit = (std::pow(inputData.upperPrice / inputData.lowerPrice, exp) - 1) * 100 - taxesInPercents;
	outputData.positionProfit = outputData.gridProfit / (inputData.gridsAmount + 1);
	outputData.spendingOnTax = taxesInPercents / (outputData.gridProfit + taxesInPercents) * 100;
}

void dataController::updateGrids()
{
	auto& gridsVector = outputData.grids;
	const auto profitFactor = 1 + outputData.gridProfit / 100;
	gridsVector.clear();
	gridsVector.reserve(outputData.maxGridsAmount + 2);
	gridsVector.resize(inputData.gridsAmount + 2);
	gridsVector[0] = inputData.lowerPrice;
	for(auto i = 0; i < inputData.gridsAmount; ++i)
	{
		gridsVector[i + 1] = gridsVector[i] * profitFactor;
	}
	gridsVector[inputData.gridsAmount + 1] = inputData.upperPrice;
}

void dataController::updateTaxRange()
{
	outputData.upperPriceTax = calculateTax(inputData.upperPrice).second;
	outputData.lowerPriceTax = calculateTax(inputData.lowerPrice).second;
}

void dataController::updateOutput()
{
	updateTaxRange();
	updateMaxGridsAmount();
	updateProfitAndSpending();
	updateGrids();
}

QPair<currency, factor> dataController::calculateTax(currency aPrice)
{
	currency minimumTaxAmount = 1.0 / std::pow(10, precision);
	currency calculatedTax = myCeil(aPrice * inputData.baseTax, precision);
	if (calculatedTax <= minimumTaxAmount)
	{
		return { minimumTaxAmount, myCeil(minimumTaxAmount / aPrice, precisionTax + 2) };
	}
	else {
		return { calculatedTax, inputData.baseTax };
	}
}

const inputData& dataController::getInputData() const
{
	return inputData;
}

const outputData& dataController::getOutputData() const
{
	return outputData;
}

int dataController::getPrecision() const
{
	return precision;
}

void dataController::setPrecision(int aPrecision)
{
	precision = aPrecision;
}
