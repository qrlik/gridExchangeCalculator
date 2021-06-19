#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
	void changeTaxInfo(int aValue);
	void changePrecision(int aValue);
	void changeGridsAmountEnabled(bool aEnabled);
	void changeGridsAmountRange(int aMinimum, int aMaximum);
	void changeGridsAmount(int aValue);
	void changeGridsList(const QStringList& aList);

private:
	void setupSignals();
	void setupMasks(QString aMasks);
	void setupTax();
	void setupPrecision();

	Ui::MainWindow* ui;
};
#endif // MAINWINDOW_H
