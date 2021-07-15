#include "statelabel.h"
#include "utils/globalvariables.h"
#include "utils/utils.h"

StateLabel::StateLabel(QWidget* aParent): QLabel(aParent)
{
	update();
}

void StateLabel::setPrecision(int aPrecision)
{
	data.setPrecision(aPrecision);
}

int StateLabel::getPrecision() const
{
	return data.getPrecision();
}

void StateLabel::updateAndEmit(QString aValue, currency(dataController::*aDataPtr)(QString), void(StateLabel::*aThisPtr)(QString))
{
	auto result = (data.*aDataPtr)(aValue);
	const auto precision = data.getPrecision();
	const auto tenFactor = utils::getTenFactor(precision);
	emit (this->*aThisPtr)(QString::number(result / tenFactor, 'f', data.getPrecision()));
	update();
}

void StateLabel::updateUpperPrice(QString aValue)
{
	updateAndEmit(aValue, &dataController::updateUpperPrice, &StateLabel::upperPriceChanged);
}

void StateLabel::updateCurrentPrice(QString aValue)
{
	updateAndEmit(aValue, &dataController::updateCurrentPrice, &StateLabel::currentPriceChanged);
}

void StateLabel::updateLowerPrice(QString aValue)
{
	updateAndEmit(aValue, &dataController::updateLowerPrice, &StateLabel::lowerPriceChanged);
}

void StateLabel::updateStopLossPrice(QString aValue)
{
	updateAndEmit(aValue, &dataController::updateStopLossPrice, &StateLabel::stopLossPriceChanged);
}

void StateLabel::updateTax(QString aValue)
{
	auto result = data.updateTax(aValue);
	emit taxChanged(QString::number(result, 'f', precisionTax));
	update();
}

void StateLabel::updateGridsAmount(int aValue)
{
	data.updateGridsAmount(aValue);
	update();
}

void StateLabel::update()
{
	bool isAllCorrect = false;
	const auto& inputData = data.getInputData();
	if (inputData.upperPrice == 0.0)
	{
		setText("Enter upper price");
	}
	else if (inputData.currentPrice == 0.0)
	{
		setText("Enter current price");
	}
	else if (inputData.lowerPrice == 0.0)
	{
		setText("Enter lower price");
	}
	else if (inputData.stopLossPrice == 0.0)
	{
		setText("Enter stop loss price");
	}
	else if (inputData.baseTax == 0.0)
	{
		setText("Enter tax");
	}
	else if (inputData.upperPrice <= inputData.lowerPrice)
	{
		setText("Upper price must be greater than lower");
	}
	else if (inputData.lowerPrice <= inputData.stopLossPrice)
	{
		setText("Lower price must be greater than Stop Loss price");
	}
	else if (inputData.upperPrice <= inputData.currentPrice)
	{
		setText("Upper price must be greater than Current price");
	}
	else if (inputData.currentPrice <= inputData.stopLossPrice)
	{
		setText("Current price must be greater than Stop Loss price");
	}
	else if (inputData.upperPrice / inputData.lowerPrice <= inputData.baseTax * 2)
	{
		setText("Profit without grids must be less than tax");
	}
	else
	{
		isAllCorrect = true;
		updateData();
		setText("Correct");
	}
	emit gridsAmountEnableChanged(isAllCorrect);
}

void StateLabel::updateData()
{
	data.updateOutput();
	const auto& outputData = data.getOutputData();
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

	const auto precisionFactor = utils::getTenFactor(data.getPrecision());
	emit minPositionChanged(QString::number(outputData.minPosition / precisionFactor, 'f', data.getPrecision()));
	emit gridsAmountRangeChanged(0, outputData.maxGridsAmount);
	emit gridsListChanged(outputData.grids);
}
