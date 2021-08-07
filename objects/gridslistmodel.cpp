#include "gridslistmodel.h"
#include "systems/dataController.h"
#include "utils/utils.h"

gridsModel::gridsModel(QObject *aParent) : QAbstractTableModel(aParent)
{

}

int gridsModel::rowCount(const QModelIndex& aIndex) const
{
	return DATA_CONTROLLER.getOutputData().grids.size();
}

int gridsModel::columnCount(const QModelIndex& aIndex) const
{
	return 3;
}

QVariant gridsModel::data(const QModelIndex& aIndex, int aRole) const
{
	if (!aIndex.isValid() || aRole != Qt::DisplayRole)
	{
		return QVariant();
	}
	const auto precision = DATA_CONTROLLER.getPrecision();
	const auto tenFactor = utils::getTenFactor(precision);
	const auto& gridInfo = DATA_CONTROLLER.getOutputData().grids[aIndex.row()];
	switch (aIndex.column())
	{
		case 0:
			return QString::number(gridInfo.price / tenFactor, 'f', precision);
		case 1:
			return QString::number(gridInfo.profit / tenFactor, 'f', precision);
		case 2:
			return QString::number(gridInfo.tax / tenFactor, 'f', precision);
		default:
			return QVariant();
	}
}

QVariant gridsModel::headerData(int aSection, Qt::Orientation aOrientation, int aRole) const
{
	if (aOrientation != Qt::Horizontal || aRole != Qt::DisplayRole)
	{
		return QVariant();
	}
	switch (aSection)
	{
		case 0:
			return "Price";
		case 1:
			return "Profit";
		case 2:
			return "Tax";
		default:
			return QVariant();
	}
}

void gridsModel::update()
{
	beginResetModel();
	emit layoutChanged();
	emit dataChanged(index(0, 0), index(DATA_CONTROLLER.getOutputData().grids.size(), 3));
	endResetModel();
}
