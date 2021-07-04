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

bool dataController::checkMaxGridsAmount(int aValue)
{
	const int minProfitAmount = 1;
	const int intFactor = std::pow(10, precision);

	const auto exp = 1.0 / (aValue + 1);
	const factor gridFactor = std::pow(inputData.upperPrice / inputData.lowerPrice, exp);

	bool isAllGridsCorrect = true;
	currency prevGrid = inputData.lowerPrice;
	for (auto i = 1, size = aValue + 2; i < size; ++i)
	{
		const auto prevGridInt = static_cast<int>(std::trunc(prevGrid * intFactor));
		const currency currentGrid = (i + 1 == size) ? inputData.upperPrice : utils::myTrunc(prevGrid * gridFactor, precision);
		const auto currentGridInt = static_cast<int>(std::trunc(currentGrid * intFactor));
		const auto gridsDiff = currentGridInt - prevGridInt;

		const auto prevTax = static_cast<int>(std::trunc(calculateTax(prevGrid).first * intFactor));
		const auto currentTax = static_cast<int>(std::trunc(calculateTax(currentGrid).first * intFactor));
		const auto profit = gridsDiff - prevTax - currentTax;

		if (profit < minProfitAmount)
		{
			isAllGridsCorrect = false;
			break;
		}
		else
		{
			prevGrid = currentGrid;
			continue;
		}
	}
	return isAllGridsCorrect;
}

void dataController::updateMaxGridsAmount()
{
	const currency minProfitAmount = 1.0 / std::pow(10, precision);
	const factor minProfitForLowerPrice = minProfitAmount / inputData.lowerPrice;
	const auto logUpper = log2(inputData.upperPrice / inputData.lowerPrice);
	const auto logLower = log2(outputData.taxRange.first * 2 + minProfitForLowerPrice + 1);
	const auto compare = logUpper / logLower - 1;
	int maxGridsAmount = std::trunc(compare);
	while(!checkMaxGridsAmount(maxGridsAmount))
	{
		--maxGridsAmount;
	}
	outputData.maxGridsAmount = maxGridsAmount;
}

void dataController::updateProfitAndSpending()
{
	auto calculateProfitAndSpending = [this](double firstPrice, double secondPrice){
		percents percents = (secondPrice / firstPrice - 1) * 100;
		currency priceDiff = secondPrice - firstPrice;
		currency profit = priceDiff - calculateTax(secondPrice).first - calculateTax(firstPrice).first;
		QPair<double, double> result;
		result.first = percents * profit / priceDiff;
		result.second = (priceDiff - profit) / priceDiff * 100;
		return result;
	};

	const auto lowerGrid = calculateProfitAndSpending(outputData.grids[0].price, outputData.grids[1].price);
	const auto upperGrid = calculateProfitAndSpending(outputData.grids[outputData.grids.size() - 2].price,
													  outputData.grids[outputData.grids.size() - 1].price);
	outputData.gridProfitRange.first = lowerGrid.first;
	outputData.gridProfitRange.second = upperGrid.first;
	outputData.spengindOnTaxRange.first = lowerGrid.second;
	outputData.spengindOnTaxRange.second = upperGrid.second;

	outputData.positionProfitRange.first = outputData.gridProfitRange.first / (inputData.gridsAmount + 1);
	outputData.positionProfitRange.second = outputData.gridProfitRange.second / (inputData.gridsAmount + 1);
}

void dataController::updateGrids()
{
	const auto exp = 1.0 / (inputData.gridsAmount + 1);
	const factor gridFactor = std::pow(inputData.upperPrice / inputData.lowerPrice, exp);

	auto& gridsVector = outputData.grids;
	gridsVector.clear();
	gridsVector.reserve(outputData.maxGridsAmount + 2);
	gridsVector.resize(inputData.gridsAmount + 2);
	gridsVector[0].price = inputData.lowerPrice;
	for(auto i = 0; i < inputData.gridsAmount; ++i)
	{
		gridsVector[i + 1].price = gridsVector[i].price * gridFactor;
	}
	gridsVector[inputData.gridsAmount + 1].price = inputData.upperPrice;

	outputData.minPosition = gridsVector[0].price;
	for (auto i = 0; i + 1 < gridsVector.size(); ++i)
	{
		gridsVector[i + 1].price = utils::myTrunc(gridsVector[i + 1].price, precision);
		gridsVector[i + 1].tax = calculateTax(gridsVector[i].price).first + calculateTax(gridsVector[i + 1].price).first;
		gridsVector[i + 1].profit = gridsVector[i + 1].price - gridsVector[i].price - gridsVector[i + 1].tax;
		outputData.minPosition += gridsVector[i + 1].price;
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
