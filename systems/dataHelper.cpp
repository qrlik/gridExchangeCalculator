#include "systems/dataHelper.h"
#include "systems/dataController.h"
#include "utils/utils.h"

QPair<currency, factor> dataHelper::calculateTax(currency aPrice)
{
	currency minimumTaxAmount = 1;
	currency calculatedTax = utils::myCeil(aPrice * DATA_CONTROLLER.getInputData().baseTax);
	if (calculatedTax <= minimumTaxAmount)
	{
		return { minimumTaxAmount, static_cast<factor>(minimumTaxAmount) / aPrice };
	}
	else {
		return { calculatedTax, static_cast<factor>(calculatedTax) / aPrice };
	}
}

QVector<double> dataHelper::calculatePureGrids(int aGridsAmount)
{
	const auto exp = 1.0 / (aGridsAmount + 1);
	const auto& inputData = DATA_CONTROLLER.getInputData();
	const factor gridFactor = std::pow(static_cast<factor>(inputData.upperPrice) / inputData.lowerPrice, exp);

	QVector<double> gridsVector(aGridsAmount + 2);
	gridsVector[0] = inputData.lowerPrice;
	for (auto i = 0; i < aGridsAmount; ++i)
	{
		gridsVector[i + 1] = gridsVector[i] * gridFactor;
	}
	gridsVector[aGridsAmount + 1] = inputData.upperPrice;
	return gridsVector;
}

bool dataHelper::checkMaxGridsAmount(int aGridsAmount)
{
	const int minProfitAmount = 1;

	const auto pureGrids = dataHelper::calculatePureGrids(aGridsAmount);

	bool isAllGridsCorrect = true;
	currency prevGrid = utils::myTrunc(pureGrids[0]);
	for (auto i = 1, size = aGridsAmount + 2; i < size; ++i)
	{
		const currency currentGrid = utils::myTrunc(pureGrids[i]);
		const auto gridsDiff = currentGrid - prevGrid;
		const auto profit = gridsDiff - calculateTax(prevGrid).first - calculateTax(currentGrid).first;

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

int dataHelper::calculateMaxGridsAmount()
{
	const auto& inputData = DATA_CONTROLLER.getInputData();
	const auto& outputData = DATA_CONTROLLER.getOutputData();
	const currency minProfitAmount = 1;

	const factor minProfitForLowerPrice = static_cast<factor>(minProfitAmount) / inputData.lowerPrice;
	const auto logUpper = log2(static_cast<factor>(inputData.upperPrice) / inputData.lowerPrice);
	const auto logLower = log2(outputData.taxRange.first * 2 + minProfitForLowerPrice + 1);
	const auto compare = logUpper / logLower - 1;
	auto maxGridsAmount = static_cast<int>(std::trunc(compare));
	while(!checkMaxGridsAmount(maxGridsAmount))
	{
		--maxGridsAmount;
	}
	return maxGridsAmount;
}

QPair<double, double> dataHelper::calculateProfitAndSpending(currency aFirstPrice, currency aSecondPrice)
{
		percents percents = (static_cast<double>(aSecondPrice) / aFirstPrice - 1) * 100;
		currency priceDiff = aSecondPrice - aFirstPrice;
		currency profit = priceDiff - dataHelper::calculateTax(aSecondPrice).first - dataHelper::calculateTax(aFirstPrice).first;
		QPair<double, double> result;
		result.first = percents * profit / priceDiff;
		result.second = static_cast<double>(priceDiff - profit) / priceDiff * 100;
		return result;
}

currency dataHelper::updateGrids(QVector<gridInfo>& aGrids)
{
	const auto& inputData = DATA_CONTROLLER.getInputData();
	const auto& outputData = DATA_CONTROLLER.getOutputData();
	const auto gridsAmount = inputData.gridsAmount;
	const auto pureGrids = dataHelper::calculatePureGrids(gridsAmount);

	aGrids.clear();
	aGrids.reserve(outputData.maxGridsAmount + 2);
	aGrids.resize(inputData.gridsAmount + 2);

	aGrids[0].price = utils::myTrunc(pureGrids[0]);
	currency minPosition = aGrids[0].price;
	for (auto i = 0; i + 1 < aGrids.size(); ++i)
	{
		aGrids[i + 1].price = utils::myTrunc(pureGrids[i + 1]);
		aGrids[i + 1].tax = dataHelper::calculateTax(aGrids[i].price).first + dataHelper::calculateTax(aGrids[i + 1].price).first;
		aGrids[i + 1].profit = aGrids[i + 1].price - aGrids[i].price - aGrids[i + 1].tax;
		minPosition += aGrids[i + 1].price;
	}
	return minPosition;
}

