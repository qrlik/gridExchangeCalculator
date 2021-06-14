#include "statelabel.h"
#include "globalvariables.h"

StateLabel::StateLabel(QWidget* aParent): QLabel(aParent)
{
	update();
}

void StateLabel::setPrecision(int aPrecision)
{
	precision = aPrecision;
}

void StateLabel::updateUpperPrice(QString aValue)
{
	auto result = stateController.updateUpperPrice(aValue);
	emit upperPriceChanged(QString::number(result, 'f', precision));
	update();
}

void StateLabel::updateCurrentPrice(QString aValue)
{
	auto result = stateController.updateCurrentPrice(aValue);
	emit currentPriceChanged(QString::number(result, 'f', precision));
	update();
}

void StateLabel::updateLowerPrice(QString aValue)
{
	auto result = stateController.updateLowerPrice(aValue);
	emit lowerPriceChanged(QString::number(result, 'f', precision));
	update();
}

void StateLabel::updateStopLossPrice(QString aValue)
{
	auto result = stateController.updateStopLossPrice(aValue);
	emit stopLossPriceChanged(QString::number(result, 'f', precision));
	update();
}

void StateLabel::updateBuyTax(QString aValue)
{
	auto result = stateController.updateBuyTax(aValue);
	emit buyTaxChanged(QString::number(result, 'f', precisionTax));
	update();
}

void StateLabel::updateSellTax(QString aValue)
{
	auto result = stateController.updateSellTax(aValue);
	emit sellTaxChanged(QString::number(result, 'f', precisionTax));
	update();
}

void StateLabel::updateGridsAmount(int aValue)
{
	stateController.updateGridsAmount(aValue);
	update();
}

void StateLabel::update()
{
	bool isAllCorrect = false;
	const auto& inputData = stateController.getInputData();
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
	else if (inputData.buyTax == 0.0)
	{
		setText("Enter buy tax");
	}
	else if (inputData.sellTax == 0.0)
	{
		setText("Enter sell tax");
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
	else if (inputData.upperPrice / inputData.lowerPrice <= inputData.buyTax + inputData.sellTax)
	{
		setText("Profit without grids must be less than tax");
	}
	else
	{
		isAllCorrect = true;
		updateData();
		setText("Correct");
	}
	emit gridsAmountSliderEnableChanged(isAllCorrect);
}

void StateLabel::updateData()
{
	stateController.updateOutput();
	const auto& outputData = stateController.getOutputData();

	emit gridsAmountSliderRangeChanged(0, outputData.maxGridsAmount);
	emit gridProfitChanged(QString::number(outputData.gridProfit, 'f', precisionCrypto));
	emit positionProfitChanged(QString::number(outputData.positionProfit, 'f', precisionCrypto));
	emit taxSpendingChanged(QString::number(outputData.spendingOnTax, 'f', precisionCrypto));
}
