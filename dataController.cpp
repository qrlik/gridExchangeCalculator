#include "dataController.h"
#include "globalvariables.h"
#include "utils.h"

namespace
{
#ifdef QT_DEBUG
	std::unordered_map<double, double> taxTestData
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
	auto exp = 1.0 / (inputData.gridsAmount + 1);
	outputData.gridFactor = std::pow(inputData.upperPrice / inputData.lowerPrice, exp);

	outputData.gridProfitRange.first = (outputData.gridFactor - outputData.taxRange.first * 2 - 1) * 100;
	outputData.gridProfitRange.second = (outputData.gridFactor - outputData.taxRange.second * 2 - 1) * 100;

	outputData.positionProfitRange.first = outputData.gridProfitRange.first / (inputData.gridsAmount + 1);
	outputData.positionProfitRange.second = outputData.gridProfitRange.second / (inputData.gridsAmount + 1);

	outputData.spengindOnTaxRange.first = outputData.taxRange.first * 2 / (outputData.gridFactor - 1) * 100;
	outputData.spengindOnTaxRange.second = outputData.taxRange.second * 2 / (outputData.gridFactor - 1) * 100;
}

void dataController::updateGrids()
{
	auto& gridsVector = outputData.grids;
	gridsVector.clear();
	gridsVector.reserve(outputData.maxGridsAmount + 2);
	gridsVector.resize(inputData.gridsAmount + 2);
	outputData.minPosition = inputData.lowerPrice;
	gridsVector[0] = inputData.lowerPrice;
	for(auto i = 0; i < inputData.gridsAmount; ++i)
	{
		gridsVector[i + 1] = gridsVector[i] * outputData.gridFactor;
		outputData.minPosition += gridsVector[i + 1];
	}
	gridsVector[inputData.gridsAmount + 1] = inputData.upperPrice;
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
	updateProfitAndSpending();
	updateGrids();
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
