#include "dataController.h"
#include "utils/globalvariables.h"
#include "utils/utils.h"

dataController& dataController::getInstance(){
	static dataController instance;
	return instance;
}

currency dataController::updateVariable(QString aString, currency& aVariable)
{
	aVariable = utils::myTrunc(aString.toDouble() * std::pow(10, precision));
	return aVariable;
}

currency dataController::updateUpperPrice(QString aValue)
{
	return updateVariable(aValue, inputData.upperPrice);
}

currency dataController::updateCurrentPrice(QString aValue)
{
	return updateVariable(aValue, inputData.currentPrice);
}

currency dataController::updateLowerPrice(QString aValue)
{
	return updateVariable(aValue, inputData.lowerPrice);
}

currency dataController::updateStopLossPrice(QString aValue)
{
	return updateVariable(aValue, inputData.stopLossPrice);
}

percents dataController::updateTax(QString aValue)
{
	auto taxPercent = aValue.toDouble();
	inputData.baseTax = taxPercent / 100;
	return taxPercent;
}

void dataController::updateGridsAmount(int aValue)
{
	inputData.gridsAmount = aValue;
}

QVector<double> dataController::calculatePureGrids(int aGridsAmount)
{
	const auto exp = 1.0 / (aGridsAmount + 1);
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

bool dataController::checkMaxGridsAmount(int aGridsAmount)
{
	const int minProfitAmount = 1;

	const auto pureGrids = calculatePureGrids(aGridsAmount);

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

void dataController::updateMaxGridsAmount()
{
	const currency minProfitAmount = 1;
	const factor minProfitForLowerPrice = static_cast<factor>(minProfitAmount) / inputData.lowerPrice;
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
	auto calculateProfitAndSpending = [this](currency firstPrice, currency secondPrice){
		percents percents = (static_cast<double>(secondPrice) / firstPrice - 1) * 100;
		currency priceDiff = secondPrice - firstPrice;
		currency profit = priceDiff - calculateTax(secondPrice).first - calculateTax(firstPrice).first;
		QPair<double, double> result;
		result.first = percents * profit / priceDiff;
		result.second = static_cast<double>(priceDiff - profit) / priceDiff * 100;
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
	const auto gridsAmount = inputData.gridsAmount;
	const auto pureGrids = calculatePureGrids(gridsAmount);

	auto& gridsVector = outputData.grids;
	gridsVector.clear();
	gridsVector.reserve(outputData.maxGridsAmount + 2);
	gridsVector.resize(inputData.gridsAmount + 2);

	gridsVector[0].price = utils::myTrunc(pureGrids[0]);
	outputData.minPosition = gridsVector[0].price;
	for (auto i = 0; i + 1 < gridsVector.size(); ++i)
	{
		gridsVector[i + 1].price = utils::myTrunc(pureGrids[i + 1]);
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

void dataController::updateData()
{
	updateOutput();
	emitOutput();
}

void dataController::updateOutput()
{
	updateTaxRange();
	updateMaxGridsAmount();
	updateGrids();
	updateProfitAndSpending();
}

void dataController::emitOutput()
{
	const auto& outputData = DATA_CONTROLLER.getOutputData();
	const auto [minTax, maxTax] = std::minmax(outputData.taxRange.first, outputData.taxRange.second);
	const auto taxFactor = utils::getTenFactor(precisionTax);
	QString minTaxStr = QString::number(utils::myCeil(minTax * taxFactor) / taxFactor, 'f', precisionTax);
	QString maxTaxStr = QString::number(utils::myCeil(maxTax * taxFactor) / taxFactor, 'f', precisionTax);
	emit taxRangeChanged((minTax != maxTax) ? (minTaxStr + " - " + maxTaxStr) : maxTaxStr);

	QString minProfit = QString::number(utils::myTrunc(outputData.gridProfitRange.first * taxFactor) / taxFactor, 'f', precisionTax);
	QString maxProfit = QString::number(utils::myTrunc(outputData.gridProfitRange.second * taxFactor) / taxFactor, 'f', precisionTax);
	emit gridProfitChanged((minProfit != maxProfit) ? (minProfit + " - " + maxProfit) : maxProfit);

	const auto profitFactor = utils::getTenFactor(6);
	QString minPosition = QString::number(utils::myTrunc(outputData.positionProfitRange.first * profitFactor) / profitFactor, 'f', 6);
	QString maxPosition = QString::number(utils::myTrunc(outputData.positionProfitRange.second * profitFactor) / profitFactor, 'f', 6);
	emit positionProfitChanged((minPosition != maxPosition) ? (minPosition + " - " + maxPosition) : maxPosition);

	QString minTaxSpending = QString::number(utils::myCeil(outputData.spengindOnTaxRange.second * taxFactor) / taxFactor, 'f', precisionTax);
	QString maxTaxSpending = QString::number(utils::myCeil(outputData.spengindOnTaxRange.first * taxFactor) /  taxFactor, 'f', precisionTax);
	emit taxSpendingChanged((minTaxSpending != maxTaxSpending) ? (minTaxSpending + " - " + maxTaxSpending) : maxTaxSpending);

	const auto precisionFactor = utils::getTenFactor(DATA_CONTROLLER.getPrecision());
	emit minPositionChanged(QString::number(outputData.minPosition / precisionFactor, 'f', DATA_CONTROLLER.getPrecision()));
	emit gridsAmountRangeChanged(0, outputData.maxGridsAmount);
	emit gridsListChanged(outputData.grids);
}

QPair<currency, factor> dataController::calculateTax(currency aPrice)
{
	currency minimumTaxAmount = 1;
	currency calculatedTax = utils::myCeil(aPrice * inputData.baseTax);
	if (calculatedTax <= minimumTaxAmount)
	{
		return { minimumTaxAmount, static_cast<factor>(minimumTaxAmount) / aPrice };
	}
	else {
		return { calculatedTax, static_cast<factor>(calculatedTax) / aPrice };
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
