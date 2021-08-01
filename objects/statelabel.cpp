#include "statelabel.h"
#include "utils/globalvariables.h"
#include "utils/utils.h"

StateLabel::StateLabel(QWidget* aParent): QLabel(aParent)
{
	update();
}

void StateLabel::update()
{
	bool isAllCorrect = false;
	const auto& inputData = DATA_CONTROLLER.getInputData();
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
		DATA_CONTROLLER.updateData();
		setText("Correct");
	}
	emit gridsAmountEnableChanged(isAllCorrect);
}

