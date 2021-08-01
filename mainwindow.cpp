#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QStyleFactory"
#include "utils/globalvariables.h"
#include "utils/utils.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	setupSignals();
	setupPrecision();
	setupTax();

	QApplication::setStyle(QStyleFactory::create("Fusion"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupMasks(QString aMask)
{
	ui->UpperPriceEdit->setInputMask(aMask);
	ui->UpperPriceEdit->setText(aMask);

	ui->CurrentPriceEdit->setInputMask(aMask);
	ui->CurrentPriceEdit->setText(aMask);

	ui->LowerPriceEdit->setInputMask(aMask);
	ui->LowerPriceEdit->setText(aMask);

	ui->StopLossPriceEdit->setInputMask(aMask);
	ui->StopLossPriceEdit->setText(aMask);
}

void MainWindow::onVariableUpdate(QString aValue, currency(dataController::*aDataPtr)(QString), QLabel* aLabel)
{
	auto result = (DATA_CONTROLLER.*aDataPtr)(aValue);
	const auto precision = DATA_CONTROLLER.getPrecision();
	const auto tenFactor = utils::getTenFactor(precision);
	aLabel->setText(QString::number(result / tenFactor, 'f', precision));
	ui->DataStateLabel->update();
}

void MainWindow::updateUpperPrice(QString aPrice)
{
	onVariableUpdate(aPrice, &dataController::updateUpperPrice, ui->UpperPriceControl);
}

void MainWindow::updateCurrentPrice(QString aPrice)
{
	onVariableUpdate(aPrice, &dataController::updateCurrentPrice, ui->CurrentPriceControl);
}

void MainWindow::updateLowerPrice(QString aPrice)
{
	onVariableUpdate(aPrice, &dataController::updateLowerPrice, ui->LowerPriceControl);
}

void MainWindow::updateStopLossPrice(QString aPrice)
{
	onVariableUpdate(aPrice, &dataController::updateStopLossPrice, ui->StopLossPriceControl);
}

void MainWindow::updateTax(QString aTax)
{
	auto result = DATA_CONTROLLER.updateTax(aTax);
	ui->TaxControl->setText(QString::number(result , 'f', precisionTax));
	ui->DataStateLabel->update();
}

void MainWindow::setupSignals()
{
	connect(ui->UpperPriceEdit, SIGNAL(textChanged(QString)), SLOT(updateUpperPrice(QString)));
	connect(ui->CurrentPriceEdit, SIGNAL(textChanged(QString)), SLOT(updateCurrentPrice(QString)));
	connect(ui->LowerPriceEdit, SIGNAL(textChanged(QString)), SLOT(updateLowerPrice(QString)));
	connect(ui->StopLossPriceEdit, SIGNAL(textChanged(QString)), SLOT(updateStopLossPrice(QString)));
	connect(ui->TaxAmountEdit, SIGNAL(textChanged(QString)), SLOT(updateTax(QString)));

	connect(ui->GridsAmountSlider, SIGNAL(valueChanged(int)), SLOT(changeGridsAmount(int)));
	connect(ui->GridsAmountSpin, SIGNAL(valueChanged(int)), SLOT(changeGridsAmount(int)));

	connect(ui->DataStateLabel, SIGNAL(gridsAmountEnableChanged(bool)), SLOT(changeGridsAmountEnabled(bool)));
	connect(&DATA_CONTROLLER, SIGNAL(gridsAmountRangeChanged(int,int)), SLOT(changeGridsAmountRange(int,int)));

	ui->TaxRangeControl->connect(&DATA_CONTROLLER, SIGNAL(taxRangeChanged(QString)), SLOT(setText(QString)));
	ui->PositionProfitControl->connect(&DATA_CONTROLLER, SIGNAL(positionProfitChanged(QString)), SLOT(setText(QString)));
	ui->GridProfitControl->connect(&DATA_CONTROLLER, SIGNAL(gridProfitChanged(QString)), SLOT(setText(QString)));
	ui->TaxSpendingControl->connect(&DATA_CONTROLLER, SIGNAL(taxSpendingChanged(QString)), SLOT(setText(QString)));
	ui->MinPositionControl->connect(&DATA_CONTROLLER, SIGNAL(minPositionChanged(QString)), SLOT(setText(QString)));
	connect(&DATA_CONTROLLER, SIGNAL(gridsListChanged(QVector<gridInfo>)), SLOT(changeGridsList(QVector<gridInfo>)));
}

void MainWindow::changeGridsAmountEnabled(bool aEnabled)
{
	ui->GridsAmountSlider->setEnabled(aEnabled);
	ui->GridsAmountSpin->setEnabled(aEnabled);
}

void MainWindow::changeGridsAmountRange(int aMinimum, int aMaximum)
{
	ui->GridsAmountSlider->setRange(aMinimum, aMaximum);
	ui->GridsAmountSpin->setRange(aMinimum, aMaximum);
}

void MainWindow::changeGridsAmount(int aValue)
{
	DATA_CONTROLLER.updateGridsAmount(aValue);
	ui->GridsAmountTipLabel->setNum(aValue);
	ui->GridsAmountSlider->setValue(aValue);
	ui->GridsAmountSpin->setValue(aValue);
	ui->DataStateLabel->update();
}

void MainWindow::changeGridsList(QVector<gridInfo> aList)
{
	ui->GridsList->clear();
	const auto precision = DATA_CONTROLLER.getPrecision();
	const auto tenFactor = utils::getTenFactor(precision);
	for(auto gridInfo : aList)
	{
		const auto price = QString::number(gridInfo.price / tenFactor, 'f', precision);
		const auto profit = QString::number(gridInfo.profit / tenFactor, 'f', precision);
		const auto tax = QString::number(gridInfo.tax / tenFactor, 'f', precision);
		ui->GridsList->addItem(price + " | " + profit + " | " + tax);
	}
}

void MainWindow::setupTax()
{
	ui->TaxAmountEdit->setInputMask(taxMask);
	ui->TaxAmountEdit->setText(taxMask);

	auto* TaxComboBox = ui->TaxTypeBox;
	TaxComboBox->addItem(QStringLiteral("Tinkoff Investments"));
	TaxComboBox->addItem(QStringLiteral("Huobi"));
	TaxComboBox->addItem(QStringLiteral("Custom"));

	connect(TaxComboBox, SIGNAL(activated(int)), SLOT(changeTaxInfo(int)));
	changeTaxInfo(0);
}

void MainWindow::changeTaxInfo(int aValue)
{
	auto taxEdit = ui->TaxAmountEdit;
	auto commentString = QString();
	switch (aValue)
	{
	case 0:
		taxEdit->setEnabled(false);
		taxEdit->setText(QStringLiteral("0.025"));
		commentString = QStringLiteral("* minimum 0.01$ per share");
		break;
	case 1:
		taxEdit->setEnabled(false);
		taxEdit->setText(QStringLiteral("0.2"));
		break;
	case 2:
		taxEdit->setEnabled(true);
		taxEdit->setText(taxMask);
		commentString = QStringLiteral("* dynamic tax only in Stock precision");
	}
	ui->TaxCommentLabel->setText(commentString);
}

void MainWindow::setupPrecision()
{
	auto* PrecisionComboBox = ui->PrecisionBox;
	PrecisionComboBox->addItem(QStringLiteral("Stock (2)"));
	PrecisionComboBox->addItem(QStringLiteral("Crypto (8)"));

	connect(PrecisionComboBox, SIGNAL(activated(int)), SLOT(changePrecision(int)));
	changePrecision(0);
}

void MainWindow::changePrecision(int aValue)
{
	switch (aValue)
	{
	case 0:
		DATA_CONTROLLER.setPrecision(precisionStocks);
		setupMasks(priceMaskStocks);
		break;
	case 1:
		DATA_CONTROLLER.setPrecision(precisionCrypto);
		setupMasks(priceMaskCrypt);
		break;
	}
}
