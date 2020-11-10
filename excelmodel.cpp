#include "excelmodel.h"

#include <QDebug>

const std::array<QString, ExcelModel::ExcelRoleCNT> ExcelModel::EXCEL_ROLE_STR {
    // alias std roles
    "display",
    "alignment",    // не уверен что стандартное название такое
    "background",   // не уверен что стандартное название такое
    "toolTip",      // не уверен что стандартное название такое
    // cell
    "readOnly",
    "enabled",
    "spanH",
    "spanV",
    "validator",
    "dropdown",
//    "selected",
//    "inSelectedArea",
    // header
    "width",
    "height",
    "resized",
    "group",
    "indexInGroup",
    "deploy",
};

ExcelModel::ExcelModel(QObject *parent) : QAbstractTableModel(parent) { }

QVariant ExcelModel::excelData(int subtable, int row, int column, int role) const
{
    qDebug() << __PRETTY_FUNCTION__ << subtable << row << column << role;
    if (!isIndexValid(subtable, row, column)) {
        return QVariant();
    }
    return data(index(absoluteRow(row, subtable), absoluteColumn(column, subtable)), role);
}

bool ExcelModel::excelSetData(int subtable, int row, int column, const QVariant &value, int role)
{
    qDebug() << __PRETTY_FUNCTION__ << subtable << row << column << role;
    if (!isIndexValid(subtable, row, column)) {
        return false;
    }
    return setData(index(absoluteRow(row, subtable), absoluteColumn(column, subtable)), value, role);
}

bool ExcelModel::setSubTableOrientation(int orientation) {
    if (orientation != Qt::Horizontal
            && orientation != Qt::Vertical) {
        return false;
    }
    _subTableOrientation = orientation;
    return true;
}

bool ExcelModel::setSubTableSizeMax(int sizeMax) {
    if (sizeMax < 1) { return false; }
    _subTableSizeMax = sizeMax;
    return true;
}

bool ExcelModel::isIndexValid(int subtable, int row, int column) const
{
    return isGoodSubTable(subtable)
            && isGoodRow(row, subtable)
            && isGoodColumn(column, subtable);
}

bool ExcelModel::isGoodSubTable(int subtable) const
{
    return subtable > 0 && subtable < _subTableCnt;
}

bool ExcelModel::isGoodRow(int row, int subtable) const
{
    switch (subTableOrientation()) {
    case Qt::Horizontal:
        return row > 0 && row < rowCount();
    case Qt::Vertical:
        if (!isGoodSubTable(subtable)) { return false; }
        return row > 0 && subtable * subTableSizeMax() + row < rowCount();
    default:
        return false;
    }
}

bool ExcelModel::isGoodColumn(int column, int subtable) const
{
    switch (subTableOrientation()) {
    case Qt::Horizontal:
        if (!isGoodSubTable(subtable)) { return false; }
        return column > 0
                && subtable * subTableSizeMax() + column < columnCount();
    case Qt::Vertical:
        return column > 0 && column < columnCount();
    default:
        return false;
    }
}

int ExcelModel::absoluteRow(int row, int subtable) const
{
    switch (subTableOrientation()) {
    case Qt::Horizontal:
        return row;
    case Qt::Vertical:
        if (!isGoodSubTable(subtable)) { return -1; }
        return subtable * subTableSizeMax() + row;
    default:
        return -1;
    }
}

int ExcelModel::absoluteColumn(int column, int subtable) const
{
    switch (subTableOrientation()) {
    case Qt::Horizontal:
        if (!isGoodSubTable(subtable)) { return -1; }
        return subtable * subTableSizeMax() + column;
    case Qt::Vertical:
        return column;
    default:
        return -1;
    }
}


int ExcelModel::isGoodRole(int role) const {
    if (role >= Qt::UserRole) {
        return role;
    }
    switch (role) {
    case Qt::DisplayRole:
        return Qt::UserRole + ExcelRoleDisplay;
    case Qt::TextAlignmentRole:
        return Qt::UserRole + ExcelRoleAlignment;
    case Qt::BackgroundRole:
        return Qt::UserRole + ExcelRoleBackground;
    case Qt::ToolTipPropertyRole:
        return Qt::UserRole + ExcelRoleToolTip;
    default:
        return -1;
    }
}

QHash<int, QByteArray> ExcelModel::roleNames() const {
    if (!_rolesId.isEmpty()) { return _rolesId; }

    for (int i = 0; i < ExcelRoleCNT; ++i) {
        _rolesId.insert(Qt::UserRole + i, EXCEL_ROLE_STR[i].toUtf8());
    }
    return _rolesId;
}
