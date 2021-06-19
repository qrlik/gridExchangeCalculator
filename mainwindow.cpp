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
	ui->dataStateLabel->connect(ui->UpperPriceEdit, SIGNAL(textChanged(QString)), SLOT(updateUpperPrice(QString)));
	ui->UpperPriceControl->connect(ui->dataStateLabel, SIGNAL(upperPriceChanged(QString)), SLOT(setText(QString)));

	ui->dataStateLabel->connect(ui->CurrentPriceEdit, SIGNAL(textChanged(QString)), SLOT(updateCurrentPrice(QString)));
	ui->CurrentPriceControl->connect(ui->dataStateLabel, SIGNAL(currentPriceChanged(QString)), SLOT(setText(QString)));

	ui->dataStateLabel->connect(ui->LowerPriceEdit, SIGNAL(textChanged(QString)), SLOT(updateLowerPrice(QString)));
	ui->LowerPriceControl->connect(ui->dataStateLabel, SIGNAL(lowerPriceChanged(QString)), SLOT(setText(QString)));

	ui->dataStateLabel->connect(ui->StopLossPriceEdit, SIGNAL(textChanged(QString)), SLOT(updateStopLossPrice(QString)));
	ui->StopLossPriceControl->connect(ui->dataStateLabel, SIGNAL(stopLossPriceChanged(QString)), SLOT(setText(QString)));

	ui->dataStateLabel->connect(ui->TaxAmountEdit, SIGNAL(textChanged(QString)), SLOT(updateTax(QString)));
	ui->TaxControl->connect(ui->dataStateLabel, SIGNAL(taxChanged(QString)), SLOT(setText(QString)));

	connect(ui->GridsAmountSlider, SIGNAL(valueChanged(int)), SLOT(changeGridsAmount(int)));
	connect(ui->GridsAmountSpin, SIGNAL(valueChanged(int)), SLOT(changeGridsAmount(int)));

	connect(ui->dataStateLabel, SIGNAL(gridsAmountEnableChanged(bool)), SLOT(changeGridsAmountEnabled(bool)));
	connect(ui->dataStateLabel, SIGNAL(gridsAmountRangeChanged(int,int)), SLOT(changeGridsAmountRange(int,int)));

	ui->TaxRangeControl->connect(ui->dataStateLabel, SIGNAL(taxRangeChanged(QString)), SLOT(setText(QString)));
	ui->PositionProfitControl->connect(ui->dataStateLabel, SIGNAL(positionProfitChanged(QString)), SLOT(setText(QString)));
	ui->GridProfitControl->connect(ui->dataStateLabel, SIGNAL(gridProfitChanged(QString)), SLOT(setText(QString)));
	ui->TaxSpendingControl->connect(ui->dataStateLabel, SIGNAL(taxSpendingChanged(QString)), SLOT(setText(QString)));
	connect(ui->dataStateLabel, SIGNAL(gridsListChanged(const QVector<double>&)), SLOT(changeGridsList(const QVector<double>&)));
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
	ui->dataStateLabel->updateGridsAmount(aValue);
	ui->GridsAmountTipLabel->setNum(aValue);
	ui->GridsAmountSlider->setValue(aValue);
	ui->GridsAmountSpin->setValue(aValue);
}

void MainWindow::changeGridsList(const QVector<double>& aList)
{
	ui->gridsList->clear();
	for(auto gridAmount : aList)
	{
		ui->gridsList->addItem(QString::number(gridAmount, 'f', 8));
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
		ui->dataStateLabel->setPrecision(precisionStocks);
		setupMasks(priceMaskStocks);
		break;
	case 1:
		ui->dataStateLabel->setPrecision(precisionCrypto);
		setupMasks(priceMaskCrypt);
		break;
	}
}
