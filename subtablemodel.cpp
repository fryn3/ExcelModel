#include "subtablemodel.h"

#include <QDebug>
#include <QQmlEngine>

static bool registerMe() {
    qmlRegisterUncreatableType<SubtableModel>(
                QString("cpp.%1").arg(SubtableModel::ITEM_NAME).toUtf8(),
                12, 34, SubtableModel::ITEM_NAME.toUtf8(),
                "Not creatable as it is an enum type");
    return true;
}

const QString SubtableModel::ITEM_NAME = "SubtableModel";
const bool SubtableModel::IS_QML_REG = registerMe();


const std::array
    <QString, SubtableModel::SubtableRoleEND - SubtableModel::SubtableRoleBEGIN>
        SubtableModel::SUBTABLE_ROLE_STR {
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
    // header
    "width",
    "height",
    "resized",
    "group",
    "deploy",
    "groupSize",
    "indexInGroup",
};

SubtableModel::SubtableModel(QObject *parent) : QAbstractTableModel(parent) {
    /// \todo не делать привязку к this, для нормальной работы disconnect.
    connect(this, &QAbstractItemModel::columnsInserted, this, &SubtableModel::checkSubtableCountChanged);
    connect(this, &QAbstractItemModel::columnsRemoved, this, &SubtableModel::checkSubtableCountChanged);
    connect(this, &QAbstractItemModel::modelReset, this, &SubtableModel::checkSubtableCountChanged);
    connect(this, &QAbstractItemModel::rowsInserted, this, &SubtableModel::checkSubtableCountChanged);
    connect(this, &QAbstractItemModel::rowsRemoved, this, &SubtableModel::checkSubtableCountChanged);
}

int SubtableModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    switch (subtableOrientation()) {
    case Qt::Horizontal:
        return totalRowCount();
    case Qt::Vertical:
        return subtableSizeMax();
    default:
        return -1;
    }
}

int SubtableModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    switch (subtableOrientation()) {
    case Qt::Horizontal:
        return subtableSizeMax();
    case Qt::Vertical:
        return totalColumnCount();
    default:
        return -1;
    }
}

QVariant SubtableModel::subtableData(int subtable, int row, int column, int role) const {
    if (!isIndexValid(subtable, row, column)) {
        return QVariant();
    }
    return data(createIndex(absoluteRow(row, subtable), absoluteColumn(column, subtable)), role);
}

bool SubtableModel::subtableSetData(int subtable, int row, int column, const QVariant &value, int role) {
    qDebug() << __PRETTY_FUNCTION__ << subtable << row << column << role;
    if (!isIndexValid(subtable, row, column)) {
        return false;
    }
    return setData(createIndex(absoluteRow(row, subtable), absoluteColumn(column, subtable)), value, role);
}

QVariant SubtableModel::subtableHeaderData(int subtable, int section, Qt::Orientation orientation, int role) const {
    switch (orientation) {
    case Qt::Horizontal:
        return headerData(absoluteColumn(section, subtable), orientation, role);
    case Qt::Vertical:
        return headerData(absoluteRow(section, subtable), orientation, role);
    default:
        return QVariant();
    }
}

int SubtableModel::getStrRole(QByteArray role) const {
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

bool SubtableModel::setSubtableOrientation(int orientation) {
    if (_subTableOrientation == orientation) { return true; }
    switch (orientation) {
    case Qt::Horizontal:
    case Qt::Vertical:
        _subTableOrientation = orientation;
        checkSubtableCountChanged();
        return true;
    default:
        return false;
    }
    return true;
}

int SubtableModel::subtableSizeMax() const {
    if (_subTableSizeMax == SUBTABLE_SIZE_MAX_DEFAULT) {
        return subtableOrientation() == Qt::Horizontal
                           ? totalColumnCount() : totalRowCount();
    }
    return _subTableSizeMax;
}

bool SubtableModel::setSubtableSizeMax(int sizeMax) {
    if (sizeMax < 1) { return false; }
    if (_subTableSizeMax == sizeMax) { return true; }
    _subTableSizeMax = sizeMax;
    checkSubtableCountChanged();
    return true;
}

void SubtableModel::checkSubtableCountChanged() {
    int nowCount = subtableCount();
    if (nowCount != __subTableCountPrev) {
        __subTableCountPrev = nowCount;
        emit subtableCountChanged();
    }
}

int SubtableModel::subtableCount() const {
    switch (subtableOrientation()) {
    case Qt::Horizontal:
        return totalColumnCount() > 0 ? (totalColumnCount() / subtableSizeMax() + (totalColumnCount() % subtableSizeMax() ? 1 : 0)) : 0;
    case Qt::Vertical:
        return totalRowCount() > 0 ? (totalRowCount() / subtableSizeMax() + (totalRowCount() % subtableSizeMax() ? 1 : 0)) : 0;
    default:
        return -1;
    }
}

bool SubtableModel::isIndexValid(int subtable, int row, int column) const {
    return isSubtableValid(subtable)
            && isRowValid(row, subtable)
            && isColumnValid(column, subtable);
}

bool SubtableModel::isSubtableValid(int subtable) const {
    return subtable >= 0 && subtable < subtableCount();
}

bool SubtableModel::isRowValid(int row, int subtable) const {
    switch (subtableOrientation()) {
    case Qt::Horizontal:
        return row >= 0 && row < totalRowCount();
    case Qt::Vertical:
        if (!isSubtableValid(subtable)) { return false; }
        return row >= 0 && subtable * subtableSizeMax() + row < totalRowCount();
    default:
        return false;
    }
}

bool SubtableModel::isColumnValid(int column, int subtable) const {
    switch (subtableOrientation()) {
    case Qt::Horizontal:
        if (!isSubtableValid(subtable)) { return false; }
        return column >= 0
                && subtable * subtableSizeMax() + column < totalColumnCount();
    case Qt::Vertical:
        return column >= 0 && column < totalColumnCount();
    default:
        return false;
    }
}

int SubtableModel::absoluteRow(int row, int subtable) const {
    switch (subtableOrientation()) {
    case Qt::Horizontal:
        return row;
    case Qt::Vertical:
        if (!isSubtableValid(subtable)) { return -1; }
        return subtable * subtableSizeMax() + row;
    default:
        return -1;
    }
}

int SubtableModel::absoluteColumn(int column, int subtable) const {
    switch (subtableOrientation()) {
    case Qt::Horizontal:
        if (!isSubtableValid(subtable)) { return -1; }
        return subtable * subtableSizeMax() + column;
    case Qt::Vertical:
        return column;
    default:
        return -1;
    }
}

int SubtableModel::isGoodRole(int role) const {
    if (role >= Qt::UserRole) {
        return role;
    }
    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole:
        return SubtableRoleDisplay;
    case Qt::TextAlignmentRole:
        return SubtableRoleAlignment;
    case Qt::BackgroundRole:
        return SubtableRoleBackground;
    case Qt::ToolTipPropertyRole:
        return SubtableRoleToolTip;
    default:
        return -1;
    }
}

QHash<int, QByteArray> SubtableModel::roleNames() const {
    if (!_rolesId.isEmpty()) { return _rolesId; }

    for (int i = SubtableRoleBEGIN; i < SubtableRoleEND; ++i) {
        _rolesId.insert(i, SUBTABLE_ROLE_STR[i - SubtableRoleBEGIN].toUtf8());
    }
    return _rolesId;
}
