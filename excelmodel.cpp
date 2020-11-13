#include "excelmodel.h"

#include <QDebug>

const std::array<QString, ExcelModel::ExcelRoleCOUNT> ExcelModel::EXCEL_ROLE_STR {
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

ExcelModel::ExcelModel(QObject *parent) : QAbstractTableModel(parent) {
    /// \todo не делать привязку к this, для нормальной работы disconnect.
    connect(this, &QAbstractItemModel::columnsInserted, this, &ExcelModel::checkSubTableCountChanged);
    connect(this, &QAbstractItemModel::columnsRemoved, this, &ExcelModel::checkSubTableCountChanged);
    connect(this, &QAbstractItemModel::modelReset, this, &ExcelModel::checkSubTableCountChanged);
    connect(this, &QAbstractItemModel::rowsInserted, this, &ExcelModel::checkSubTableCountChanged);
    connect(this, &QAbstractItemModel::rowsRemoved, this, &ExcelModel::checkSubTableCountChanged);
}

int ExcelModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    switch (subTableOrientation()) {
    case Qt::Horizontal:
        return totalRowCount();
    case Qt::Vertical:
        return subTableSizeMax();
    default:
        return -1;
    }
}

int ExcelModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    switch (subTableOrientation()) {
    case Qt::Horizontal:
        return _subTableSizeMax;
    case Qt::Vertical:
        return totalColumnCount();
    default:
        return -1;
    }
}

QVariant ExcelModel::excelData(int subtable, int row, int column, int role) const {
    qDebug() << __PRETTY_FUNCTION__ << subtable << row << column << role;
    if (!isIndexValid(subtable, row, column)) {
        return QVariant();
    }
    return data(createIndex(absoluteRow(row, subtable), absoluteColumn(column, subtable)), role);
}

bool ExcelModel::excelSetData(int subtable, int row, int column, const QVariant &value, int role) {
    qDebug() << __PRETTY_FUNCTION__ << subtable << row << column << role;
    if (!isIndexValid(subtable, row, column)) {
        return false;
    }
    return setData(createIndex(absoluteRow(row, subtable), absoluteColumn(column, subtable)), value, role);
}

QVariant ExcelModel::excelHeaderData(int subtable, int section, Qt::Orientation orientation, int role) const {
    qDebug() << __PRETTY_FUNCTION__ << subtable << section << orientation << role;
    switch (orientation) {
    case Qt::Horizontal:
        return headerData(absoluteColumn(section, subtable), orientation, role);
    case Qt::Vertical:
        return headerData(absoluteRow(section, subtable), orientation, role);
    default:
        return QVariant();
    }
}

int ExcelModel::getStrRole(QByteArray role) const {
    if (_rolesStr.isEmpty()) {
        if (_rolesId.isEmpty()) {
            roleNames();
        }
        for (auto it = _rolesId.cbegin(); it != _rolesId.cend(); ++it) {
            _rolesStr[it.value()] = it.key();
        }
    }
    if (!_rolesStr.contains(role)) {
        qDebug() << __PRETTY_FUNCTION__ << "can't find role" << role;
        qDebug() << _rolesId;
        qDebug() << _rolesStr;
        return -1;
    }
    return _rolesStr[role];
}

bool ExcelModel::setSubTableOrientation(int orientation) {
    if (_subTableOrientation == orientation) { return true; }
    switch (orientation) {
    case Qt::Horizontal:
    case Qt::Vertical:
        _subTableOrientation = orientation;
        checkSubTableCountChanged();
        return true;
    default:
        return false;
    }
    return true;
}

int ExcelModel::subTableSizeMax() const {
    if (_subTableSizeMax == SUBTABLE_SIZE_MAX_DEFAULT) {
        _subTableSizeMax = subTableOrientation() == Qt::Horizontal
                           ? totalColumnCount() : totalRowCount();
    }
    return _subTableSizeMax;
}

bool ExcelModel::setSubTableSizeMax(int sizeMax) {
    if (sizeMax < 1) { return false; }
    if (_subTableSizeMax == sizeMax) { return true; }
    _subTableSizeMax = sizeMax;
    checkSubTableCountChanged();
    return true;
}

void ExcelModel::checkSubTableCountChanged() {
    int nowCount = subTableCount();
    if (nowCount != __subTableCountPrev) {
        __subTableCountPrev = nowCount;
        emit checkSubTableCountChanged();
    }
}

int ExcelModel::subTableCount() const {
    switch (subTableOrientation()) {
    case Qt::Horizontal:
        return totalColumnCount() / subTableSizeMax() + (totalColumnCount() % subTableSizeMax() ? 1 : 0);
    case Qt::Vertical:
        return totalRowCount() / subTableSizeMax() + (totalRowCount() % subTableSizeMax() ? 1 : 0);
    default:
        return -1;
    }
}

bool ExcelModel::isIndexValid(int subtable, int row, int column) const {
    return isSubTableValid(subtable)
            && isRowValid(row, subtable)
            && isColumnValid(column, subtable);
}

bool ExcelModel::isSubTableValid(int subtable) const {
    return subtable >= 0 && subtable < subTableCount();
}

bool ExcelModel::isRowValid(int row, int subtable) const {
    switch (subTableOrientation()) {
    case Qt::Horizontal:
        return row >= 0 && row < totalRowCount();
    case Qt::Vertical:
        if (!isSubTableValid(subtable)) { return false; }
        return row >= 0 && subtable * subTableSizeMax() + row < totalRowCount();
    default:
        return false;
    }
}

bool ExcelModel::isColumnValid(int column, int subtable) const {
    switch (subTableOrientation()) {
    case Qt::Horizontal:
        if (!isSubTableValid(subtable)) { return false; }
        return column >= 0
                && subtable * subTableSizeMax() + column < totalColumnCount();
    case Qt::Vertical:
        return column >= 0 && column < totalColumnCount();
    default:
        return false;
    }
}

int ExcelModel::absoluteRow(int row, int subtable) const {
    switch (subTableOrientation()) {
    case Qt::Horizontal:
        return row;
    case Qt::Vertical:
        if (!isSubTableValid(subtable)) { return -1; }
        return subtable * subTableSizeMax() + row;
    default:
        return -1;
    }
}

int ExcelModel::absoluteColumn(int column, int subtable) const {
    switch (subTableOrientation()) {
    case Qt::Horizontal:
        if (!isSubTableValid(subtable)) { return -1; }
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
        return ExcelRoleDisplay + Qt::UserRole;
    case Qt::TextAlignmentRole:
        return ExcelRoleAlignment + Qt::UserRole;
    case Qt::BackgroundRole:
        return ExcelRoleBackground + Qt::UserRole;
    case Qt::ToolTipPropertyRole:
        return ExcelRoleToolTip + Qt::UserRole;
    default:
        return -1;
    }
}

QHash<int, QByteArray> ExcelModel::roleNames() const {
    if (!_rolesId.isEmpty()) { return _rolesId; }

    for (int i = 0; i < ExcelRoleCOUNT; ++i) {
        _rolesId.insert(Qt::UserRole + i, EXCEL_ROLE_STR[i].toUtf8());
    }
    return _rolesId;
}
