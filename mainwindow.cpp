#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QStyleFactory"
#include "globalvariables.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	setupSignals();
	setupTax();
	setupPrecision();

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

void MainWindow::setupSignals()
{
	ui->inputInfoStateLabel->connect(ui->UpperPriceEdit, SIGNAL(textChanged(QString)), SLOT(updateUpperPrice(QString)));
	ui->UpperPriceControl->connect(ui->inputInfoStateLabel, SIGNAL(upperPriceChanged(QString)), SLOT(setText(QString)));

	ui->inputInfoStateLabel->connect(ui->CurrentPriceEdit, SIGNAL(textChanged(QString)), SLOT(updateCurrentPrice(QString)));
	ui->CurrentPriceControl->connect(ui->inputInfoStateLabel, SIGNAL(currentPriceChanged(QString)), SLOT(setText(QString)));

	ui->inputInfoStateLabel->connect(ui->LowerPriceEdit, SIGNAL(textChanged(QString)), SLOT(updateLowerPrice(QString)));
	ui->LowerPriceControl->connect(ui->inputInfoStateLabel, SIGNAL(lowerPriceChanged(QString)), SLOT(setText(QString)));

	ui->inputInfoStateLabel->connect(ui->StopLossPriceEdit, SIGNAL(textChanged(QString)), SLOT(updateStopLossPrice(QString)));
	ui->StopLossPriceControl->connect(ui->inputInfoStateLabel, SIGNAL(stopLossPriceChanged(QString)), SLOT(setText(QString)));

	ui->inputInfoStateLabel->connect(ui->TaxBuyAmountEdit, SIGNAL(textChanged(QString)), SLOT(updateBuyTax(QString)));
	ui->TaxBuyControl->connect(ui->inputInfoStateLabel, SIGNAL(buyTaxChanged(QString)), SLOT(setText(QString)));

	ui->inputInfoStateLabel->connect(ui->TaxSellAmountEdit, SIGNAL(textChanged(QString)), SLOT(updateSellTax(QString)));
	ui->TaxSellControl->connect(ui->inputInfoStateLabel, SIGNAL(sellTaxChanged(QString)), SLOT(setText(QString)));

	ui->inputInfoStateLabel->connect(ui->GridsAmountSlider, SIGNAL(valueChanged(int)), SLOT(updateGridsAmount(int)));
	ui->GridsAmountTipLabel->connect(ui->GridsAmountSlider, SIGNAL(valueChanged(int)), SLOT(setNum(int)));
}

void MainWindow::setupTax()
{
	ui->TaxBuyAmountEdit->setInputMask(taxMask);
	ui->TaxBuyAmountEdit->setText(taxMask);

	ui->TaxSellAmountEdit->setInputMask(taxMask);
	ui->TaxSellAmountEdit->setText(taxMask);

	auto* TaxComboBox = ui->TaxTypeBox;
	TaxComboBox->addItem(QStringLiteral("Tinkoff Investments"));
	TaxComboBox->addItem(QStringLiteral("Huobi"));
	TaxComboBox->addItem(QStringLiteral("Custom"));

	connect(TaxComboBox, SIGNAL(activated(int)), SLOT(changeTaxInfo(int)));
	changeTaxInfo(0);
}

void MainWindow::changeTaxInfo(int aValue)
{
	auto buyTaxEdit = ui->TaxBuyAmountEdit;
	auto sellTaxEdit = ui->TaxSellAmountEdit;
	auto commentString = QString();
	switch (aValue)
	{
	case 0:
		buyTaxEdit->setEnabled(false);
		sellTaxEdit->setEnabled(false);
		buyTaxEdit->setText(QStringLiteral("0.025"));
		sellTaxEdit->setText(QStringLiteral("0.025"));
		commentString = QStringLiteral("* minimum 0.01$ per share");
		break;
	case 1:
		buyTaxEdit->setEnabled(false);
		sellTaxEdit->setEnabled(false);
		buyTaxEdit->setText(QStringLiteral("0.2"));
		sellTaxEdit->setText(QStringLiteral("0.2"));
		break;
	case 2:
		buyTaxEdit->setEnabled(true);
		sellTaxEdit->setEnabled(true);
		buyTaxEdit->setText(taxMask);
		sellTaxEdit->setText(taxMask);
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
		ui->inputInfoStateLabel->setPrecision(precisionStocks);
		setupMasks(priceMaskStocks);
		break;
	case 1:
		ui->inputInfoStateLabel->setPrecision(precisionCrypto);
		setupMasks(priceMaskCrypt);
		break;
	}
}
