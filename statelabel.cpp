#include "statelabel.h"
#include "globalvariables.h"

StateLabel::StateLabel(QWidget* aParent): QLabel(aParent)
{
	updateStateText();
}

void StateLabel::updateUpperPrice(QString aValue)
{
	auto result = stateController.updateUpperPrice(aValue);
	emit upperPriceChanged(QString::number(result, 'f', precisionStocks));
	updateStateText();
}

void StateLabel::updateCurrentPrice(QString aValue)
{
	auto result = stateController.updateCurrentPrice(aValue);
	emit currentPriceChanged(QString::number(result, 'f', precisionStocks));
	updateStateText();
}

void StateLabel::updateLowerPrice(QString aValue)
{
	auto result = stateController.updateLowerPrice(aValue);
	emit lowerPriceChanged(QString::number(result, 'f', precisionStocks));
	updateStateText();
}

void StateLabel::updateStopLossPrice(QString aValue)
{
	auto result = stateController.updateStopLossPrice(aValue);
	emit stopLossPriceChanged(QString::number(result, 'f', precisionStocks));
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
	auto state = stateController.getCurrentState();
	switch (state)
	{
	case StateController::eValuesState::empty:
		setText("Enter some data");
		break;
	case StateController::eValuesState::upperPrice:
		setText("Enter upper price!");
		break;
	case StateController::eValuesState::currentPrice:
		setText("Enter current price!");
		break;
	case StateController::eValuesState::lowerPrice:
		setText("Enter lower price!");
		break;
	case StateController::eValuesState::stopLossPrice:
		setText("Enter stop loss price!");
		break;
	case StateController::eValuesState::buyTax:
		setText("Enter buy tax!");
		break;
	case StateController::eValuesState::sellTax:
		setText("Enter sell tax!");
		break;
	case StateController::eValuesState::allCorrect:
		setText("Correct");
		break;
	}
}
