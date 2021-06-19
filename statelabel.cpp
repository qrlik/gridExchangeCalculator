#include "statelabel.h"
#include "globalvariables.h"

StateLabel::StateLabel(QWidget* aParent): QLabel(aParent)
{
	update();
}

void StateLabel::setPrecision(int aPrecision)
{
	data.setPrecision(aPrecision);
}

void StateLabel::updateUpperPrice(QString aValue)
{
	auto result = data.updateUpperPrice(aValue);
	emit upperPriceChanged(QString::number(result, 'f', data.getPrecision()));
	update();
}

void StateLabel::updateCurrentPrice(QString aValue)
{
	auto result = data.updateCurrentPrice(aValue);
	emit currentPriceChanged(QString::number(result, 'f', data.getPrecision()));
	update();
}

void StateLabel::updateLowerPrice(QString aValue)
{
	auto result = data.updateLowerPrice(aValue);
	emit lowerPriceChanged(QString::number(result, 'f', data.getPrecision()));
	update();
}

void StateLabel::updateStopLossPrice(QString aValue)
{
	auto result = data.updateStopLossPrice(aValue);
	emit stopLossPriceChanged(QString::number(result, 'f', data.getPrecision()));
	update();
}

void StateLabel::updateTax(QString aValue)
{
	auto result = data.updateTax(aValue);
	emit taxChanged(QString::number(result, 'f', data.getPrecision()));
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
	else if (inputData.tax == 0.0)
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
	else if (inputData.upperPrice / inputData.lowerPrice <= inputData.tax * 2)
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

	emit gridsAmountRangeChanged(0, outputData.maxGridsAmount);
	emit gridProfitChanged(QString::number(outputData.gridProfit, 'f', precisionCrypto));
	emit positionProfitChanged(QString::number(outputData.positionProfit, 'f', precisionCrypto));
	emit taxSpendingChanged(QString::number(outputData.spendingOnTax, 'f', precisionCrypto));
	emit gridsListChanged(outputData.grids);
}
