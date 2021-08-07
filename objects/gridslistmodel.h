#ifndef GRIDSLISTMODEL_H
#define GRIDSLISTMODEL_H

#include <QAbstractTableModel>

class gridsModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	explicit gridsModel(QObject *aParent = nullptr);
	int rowCount(const QModelIndex& aIndex = QModelIndex()) const override;
	int columnCount(const QModelIndex& aIndex = QModelIndex()) const override;
	QVariant data(const QModelIndex &aIndex, int aRole) const override;
	QVariant headerData(int aSection, Qt::Orientation aOrientation, int aRole) const override;

public slots:
	void update();
};

#endif // GRIDSLISTMODEL_H
