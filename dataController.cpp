#include "dataController.h"
#include "globalvariables.h"
#include "utils.h"

namespace
{
#ifdef QT_DEBUG
	const std::unordered_map<double, double> taxTestData
	{
		{ 1.0, 0.01 },
		{ 5.0, 0.01 },
		{ 10.0, 0.01 },
		{ 20.0, 0.01 },
		{ 20.01, 0.02 },
		{ 40.01, 0.03 },
		{ 60.01, 0.04 },
		{ 80.01, 0.05 },
		{ 100.01, 0.06 }
	};
#endif
}

dataController::dataController()
{
#ifdef QT_DEBUG
	testTax();
#endif
}

#ifdef QT_DEBUG
void dataController::testTax()
{
	precision = 2;
	inputData.baseTax = 0.0005;
	for(auto [price, tax] :taxTestData){
		auto calculatedTax = calculateTax(price).first;
		assert(calculatedTax == tax);
	}
	precision = 0;
	inputData.baseTax = 0.0;
}
#endif

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
	const currency minProfitAmount = 1.0 / std::pow(10, precision);
	const factor minProfitForLowerPrice = minProfitAmount / inputData.lowerPrice;
	const auto logUpper = log2(inputData.upperPrice / inputData.lowerPrice);
	const auto logLower = log2(outputData.taxRange.first * 2 + minProfitForLowerPrice + 1);
	const auto compare = logUpper / logLower - 1;
	outputData.maxGridsAmount = std::trunc(compare);
}

void dataController::updateProfitAndSpending()
{
	const percents lowerGridPercents = (outputData.grids[1] / outputData.grids[0] - 1) * 100;
	const currency lowerGridDiff = outputData.grids[1] - outputData.grids[0];
	const currency lowerGridProfit = lowerGridDiff - calculateTax(outputData.grids[1]).first
									 - calculateTax(outputData.grids[0]).first;
	outputData.gridProfitRange.first = lowerGridPercents * lowerGridProfit / lowerGridDiff;
	const auto lastIndex = outputData.grids.size() - 1;
	const percents upperGridPercents = (outputData.grids[lastIndex] / outputData.grids[lastIndex - 1] - 1) * 100;
	const currency upperGridDiff = outputData.grids[lastIndex] - outputData.grids[lastIndex - 1];
	const currency upperGridProfit = upperGridDiff - calculateTax(outputData.grids[lastIndex]).first
									 - calculateTax(outputData.grids[lastIndex - 1]).first;
	outputData.gridProfitRange.second = upperGridPercents * upperGridProfit / upperGridDiff;

	outputData.positionProfitRange.first = outputData.gridProfitRange.first / (inputData.gridsAmount + 1);
	outputData.positionProfitRange.second = outputData.gridProfitRange.second / (inputData.gridsAmount + 1);

	outputData.spengindOnTaxRange.first = (lowerGridDiff - lowerGridProfit) / lowerGridDiff * 100;
	outputData.spengindOnTaxRange.second = (upperGridDiff - upperGridProfit) / upperGridDiff * 100;
}

void dataController::updateGrids()
{
	const auto exp = 1.0 / (inputData.gridsAmount + 1);
	const factor gridFactor = std::pow(inputData.upperPrice / inputData.lowerPrice, exp);

	auto& gridsVector = outputData.grids;
	gridsVector.clear();
	gridsVector.reserve(outputData.maxGridsAmount + 2);
	gridsVector.resize(inputData.gridsAmount + 2);
	outputData.minPosition = inputData.lowerPrice;
	gridsVector[0] = inputData.lowerPrice;
	for(auto i = 0; i < inputData.gridsAmount; ++i)
	{
		gridsVector[i + 1] = gridsVector[i] * gridFactor;
		outputData.minPosition += gridsVector[i + 1];
	}
	gridsVector[inputData.gridsAmount + 1] = inputData.upperPrice;
	for(auto& gridAmount : gridsVector)
	{
		gridAmount = utils::myTrunc(gridAmount, precision);
	}
}

void dataController::updateTaxRange()
{
	outputData.taxRange.first = calculateTax(inputData.lowerPrice).second;
	outputData.taxRange.second = calculateTax(inputData.upperPrice).second;
}

void dataController::updateOutput()
{
	updateTaxRange();
	updateMaxGridsAmount();
	updateGrids();
	updateProfitAndSpending();
}

QPair<currency, factor> dataController::calculateTax(currency aPrice)
{
	currency minimumTaxAmount = 1.0 / std::pow(10, precision);
	currency calculatedTax = utils::myCeil(aPrice * inputData.baseTax, precision);
	if (calculatedTax <= minimumTaxAmount)
	{
		return { minimumTaxAmount, minimumTaxAmount / aPrice };
	}
	else {
		return { calculatedTax, calculatedTax / aPrice };
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
