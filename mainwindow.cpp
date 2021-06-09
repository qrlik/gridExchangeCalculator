#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QIntValidator"
#include "QStyleFactory"

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
	setupPriceValidator(ui->CurrentPriceEdit, ui->CurrentFractionalEdit);
	setupPriceValidator(ui->LowerPriceEdit, ui->LowerFractionalEdit);
	setupPriceValidator(ui->StopLossPriceEdit, ui->StopLossFractionalEdit);

	setupTax();

	QApplication::setStyle(QStyleFactory::create("Fusion"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupTax()
{
	ui->TaxBuyAmountEdit->setValidator(new QIntValidator(1, 999));
	ui->TaxSellAmountEdit->setValidator(new QIntValidator(1, 999));

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
		buyTaxEdit->setText(QStringLiteral("025"));
		sellTaxEdit->setText(QStringLiteral("025"));
		commentString = QStringLiteral("* minimum 0.01$ per share");
		break;
	case 1:
		buyTaxEdit->setEnabled(false);
		sellTaxEdit->setEnabled(false);
		buyTaxEdit->setText(QStringLiteral("2"));
		sellTaxEdit->setText(QStringLiteral("2"));
		break;
	case 2:
		buyTaxEdit->setEnabled(true);
		sellTaxEdit->setEnabled(true);
		buyTaxEdit->setText("");
		sellTaxEdit->setText("");
	}
	ui->TaxCommentLabel->setText(commentString);
}
