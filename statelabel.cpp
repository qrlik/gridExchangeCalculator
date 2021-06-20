#include "statelabel.h"
#include "globalvariables.h"
#include "utils.h"

StateLabel::StateLabel(QWidget* aParent): QLabel(aParent)
{
	update();
}

void StateLabel::setPrecision(int aPrecision)
{
	data.setPrecision(aPrecision);
}

void StateLabel::updateAndEmit(QString aValue, double(dataController::*aDataPtr)(QString), void(StateLabel::*aThisPtr)(QString))
{
	auto result = (data.*aDataPtr)(aValue);
	emit (this->*aThisPtr)(QString::number(result, 'f', data.getPrecision()));
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
	updateAndEmit(aValue, &dataController::updateTax, &StateLabel::taxChanged);
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
	QString minTax = QString::number(utils::myCeil(outputData.taxRange.second * 100, precisionTax), 'f', precisionTax);
	QString maxTax = QString::number(utils::myCeil(outputData.taxRange.first * 100, precisionTax), 'f', precisionTax);
	bool isRanged = minTax != maxTax;
	emit taxRangeChanged((isRanged) ? (minTax + "% - " + maxTax + "%") : maxTax + "%");

	QString minProfit = QString::number(utils::myCeil(outputData.gridProfitRange.first, precisionTax), 'f', precisionTax);
	QString maxProfit = QString::number(utils::myCeil(outputData.gridProfitRange.second, precisionTax), 'f', precisionTax);
	emit gridProfitChanged((isRanged) ? (minProfit + "% - " + maxProfit + "%") : maxProfit + "%");

	QString minPosition = QString::number(utils::myCeil(outputData.positionProfitRange.first, 6), 'f', 6);
	QString maxPosition = QString::number(utils::myCeil(outputData.positionProfitRange.second, 6), 'f', 6);
	emit positionProfitChanged((isRanged) ? (minPosition + "% - " + maxPosition + "%") : maxPosition + "%");

	QString minTaxSpending = QString::number(utils::myCeil(outputData.spengindOnTaxRange.second, precisionTax), 'f', precisionTax);
	QString maxTaxSpending = QString::number(utils::myCeil(outputData.spengindOnTaxRange.first, precisionTax), 'f', precisionTax);
	emit taxSpendingChanged((isRanged) ? (minTaxSpending + "% - " + maxTaxSpending + "%") : maxTaxSpending + "%");

	emit gridsAmountRangeChanged(0, outputData.maxGridsAmount);
	emit gridsListChanged(outputData.grids);
}
