#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "systems/dataController.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct gridInfo;
class QLabel;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
	void updateUpperPrice(QString aPrice);
	void updateCurrentPrice(QString aPrice);
	void updateLowerPrice(QString aPrice);
	void updateStopLossPrice(QString aPrice);
	void updateTax(QString aTax);

	void changeTaxInfo(int aValue);
	void changePrecision(int aValue);
	void changeGridsAmountEnabled(bool aEnabled);
	void changeGridsAmountRange(int aMinimum, int aMaximum);
	void changeGridsAmount(int aValue);
	void changeGridsList(QVector<gridInfo> aList);

private:
	void setupSignals();
	void setupMasks(QString aMasks);
	void setupTax();
	void setupPrecision();
	void onVariableUpdate(QString aValue, currency(dataController::*aDataPtr)(QString), QLabel* aLabel);

	Ui::MainWindow* ui;
};
#endif // MAINWINDOW_H
