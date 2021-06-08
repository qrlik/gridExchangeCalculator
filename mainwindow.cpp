#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QIntValidator"

namespace
{
	void setupPriceValidator(QLineEdit* aPriceEdit, QLineEdit* aFractionalEdit){
		aPriceEdit->setValidator(new QIntValidator(0, 10000));
		aFractionalEdit->setValidator(new QIntValidator(0, 99));
	}
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	setupPriceValidator(ui->UpperPriceEdit, ui->UpperFractionalEdit);
	setupPriceValidator(ui->LowerPriceEdit, ui->LowerFractionalEdit);
	setupPriceValidator(ui->StopLossPriceEdit, ui->StopLossFractionalEdit);
	ui->GridsAmountEdit->setValidator(new QIntValidator(1, 99, ui->GridsAmountEdit));

	setupTaxComboBox();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupTaxComboBox()
{
	auto* TaxComboBox = ui->TaxTypeBox;
	TaxComboBox->addItem(QStringLiteral("Tinkoff Investments"));
	TaxComboBox->addItem(QStringLiteral("Huobi"));
	connect(TaxComboBox, SIGNAL(activated(int)), SLOT(changeTaxInfo(int)));
	changeTaxInfo(0);
}

void MainWindow::changeTaxInfo(int aValue)
{
	auto buyTax = 0.f;
	auto sellTax = 0.f;
	auto commentString = QString();
	switch (aValue)
	{
	case 0:
		buyTax = 0.05f;
		sellTax = 0.05f;
		commentString = QStringLiteral("* minimum 0.01$ per share");
		break;
	case 1:
		buyTax = 0.2f;
		sellTax = 0.2f;
		break;
	}
	ui->TaxBuyAmountLabel->setText(QString::number(buyTax).append(QStringLiteral("%")));
	ui->TaxSellAmountLabel->setText(QString::number(sellTax).append(QStringLiteral("%")));
	ui->TaxCommentLabel->setText(commentString);
}

