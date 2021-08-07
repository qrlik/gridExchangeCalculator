#include "dataController.h"
#include "dataHelper.h"
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

void dataController::updateProfitAndSpending()
{
	const auto lowerGrid = dataHelper::calculateProfitAndSpending(outputData.grids[0].price, outputData.grids[1].price);
	const auto upperGrid = dataHelper::calculateProfitAndSpending(outputData.grids[outputData.grids.size() - 2].price,
																  outputData.grids[outputData.grids.size() - 1].price);
	outputData.gridProfitRange.first = lowerGrid.first;
	outputData.gridProfitRange.second = upperGrid.first;
	outputData.spengindOnTaxRange.first = lowerGrid.second;
	outputData.spengindOnTaxRange.second = upperGrid.second;

	outputData.positionProfitRange.first = outputData.gridProfitRange.first / (inputData.gridsAmount + 1);
	outputData.positionProfitRange.second = outputData.gridProfitRange.second / (inputData.gridsAmount + 1);
}

void dataController::updateTaxRange()
{
	outputData.taxRange.first = dataHelper::calculateTax(inputData.lowerPrice).second;
	outputData.taxRange.second = dataHelper::calculateTax(inputData.upperPrice).second;
}

void dataController::updateData()
{
	updateOutput();
	emitOutput();
}

void dataController::updateOutput()
{
	updateTaxRange();
	outputData.maxGridsAmount = dataHelper::calculateMaxGridsAmount();
	outputData.minPosition = dataHelper::updateGrids(outputData.grids);
	updateProfitAndSpending();
}

void dataController::emitOutput()
{
	const auto& outputData = DATA_CONTROLLER.getOutputData();
	const auto [minTax, maxTax] = std::minmax(outputData.taxRange.first, outputData.taxRange.second);
	const auto taxFactor = utils::getTenFactor(precisionTax);
	QString minTaxStr = QString::number(utils::myCeil(minTax * taxFactor * 100) / taxFactor, 'f', precisionTax);
	QString maxTaxStr = QString::number(utils::myCeil(maxTax * taxFactor * 100) / taxFactor, 'f', precisionTax);
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
	emit gridsListChanged();
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
