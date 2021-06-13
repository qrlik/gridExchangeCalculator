#include "statelabel.h"
#include "globalvariables.h"

StateLabel::StateLabel(QWidget* aParent): QLabel(aParent)
{
	updateStateText();
}

void StateLabel::setPrecision(int aPrecision)
{
	precision = aPrecision;
}

void StateLabel::updateUpperPrice(QString aValue)
{
	auto result = stateController.updateUpperPrice(aValue);
	emit upperPriceChanged(QString::number(result, 'f', precision));
	updateStateText();
}

void StateLabel::updateCurrentPrice(QString aValue)
{
	auto result = stateController.updateCurrentPrice(aValue);
	emit currentPriceChanged(QString::number(result, 'f', precision));
	updateStateText();
}

void StateLabel::updateLowerPrice(QString aValue)
{
	auto result = stateController.updateLowerPrice(aValue);
	emit lowerPriceChanged(QString::number(result, 'f', precision));
	updateStateText();
}

void StateLabel::updateStopLossPrice(QString aValue)
{
	auto result = stateController.updateStopLossPrice(aValue);
	emit stopLossPriceChanged(QString::number(result, 'f', precision));
	updateStateText();
}

void StateLabel::updateBuyTax(QString aValue)
{
	auto result = stateController.updateBuyTax(aValue);
	emit buyTaxChanged(QString::number(result, 'f', precisionTax));
	updateStateText();
}

void StateLabel::updateSellTax(QString aValue)
{
	auto result = stateController.updateSellTax(aValue);
	emit sellTaxChanged(QString::number(result, 'f', precisionTax));
	updateStateText();
}

void StateLabel::updateGridsAmount(int aValue)
{
	stateController.updateGridsAmount(aValue);
}

void StateLabel::updateStateText()
{
	bool isAllCorrect = false;
	if (stateController.getUpperPrice() == 0.0)
	{
		setText("Enter upper price");
	}
	else if (stateController.getCurrentPrice() == 0.0)
	{
		setText("Enter current price");
	}
	else if (stateController.getLowerPrice() == 0.0)
	{
		setText("Enter lower price");
	}
	else if (stateController.getStopLossPrice() == 0.0)
	{
		setText("Enter stop loss price");
	}
	else if (stateController.getBuyTax() == 0.0)
	{
		setText("Enter buy tax");
	}
	else if (stateController.getSellTax() == 0.0)
	{
		setText("Enter sell tax");
	}
	else if (stateController.getUpperPrice() <= stateController.getLowerPrice())
	{
		setText("Upper price must be greater than lower");
	}
	else if (stateController.getLowerPrice() <= stateController.getStopLossPrice())
	{
		setText("Lower price must be greater than Stop Loss price");
	}
	else if (stateController.getUpperPrice() <= stateController.getCurrentPrice())
	{
		setText("Upper price must be greater than Current price");
	}
	else if (stateController.getCurrentPrice() <= stateController.getStopLossPrice())
	{
		setText("Current price must be greater than Stop Loss price");
	}
	else if (stateController.getUpperPrice() / stateController.getLowerPrice()
			 <= stateController.getBuyTax() + stateController.getSellTax())
	{
		setText("Profit without grids must be less than tax");
	}
	else
	{
		isAllCorrect = true;
		emit gridsAmountSliderRangeChanged(0, stateController.updateMaxGridsAmount());
		setText("Correct");
	}
	emit gridsAmountSliderEnableChanged(isAllCorrect);
}
