#include "areamodel.h"
#include <QQmlEngine>

static bool registerMe() {
    qmlRegisterUncreatableType<AreaModel>(
                QString("cpp.%1").arg(AreaModel::ITEM_NAME).toUtf8(),
                12, 34, AreaModel::ITEM_NAME.toUtf8(),
                "Not creatable as it is an enum type");
    return true;
}

const QString AreaModel::ITEM_NAME = "AreaModel";
const bool AreaModel::IS_QML_REG = registerMe();

AreaModel::AreaModel(QAbstractTableModel *model, QObject *parent)
    : QAbstractTableModel(parent),
      _headerHModel(new HeaderModel(this, Qt::Horizontal)),
      _headerVModel(new HeaderModel(this, Qt::Vertical))  {
    setSourceModel(model);
}

AreaModel::AreaModel(QAbstractTableModel *model, int splitOrientation, QObject *parent)
    : QAbstractTableModel(parent),
      _splitOrientation(splitOrientation),
      _headerHModel(new HeaderModel(this, Qt::Horizontal)),
      _headerVModel(new HeaderModel(this, Qt::Vertical))  {
    setSourceModel(model);
}

QAbstractTableModel *AreaModel::sourceModel() const {
    return _sourceModel;
}

void AreaModel::setSourceModel(QAbstractTableModel *model) {
    if (_sourceModel == model) { return; }
    beginResetModel();
    if (sourceModel()) {
        disconnect(sourceModel(), &QAbstractTableModel::destroyed,
                            this, &QAbstractTableModel::destroyed);
        disconnect(sourceModel(), &QAbstractTableModel::rowsAboutToBeInserted,
                            this, &QAbstractTableModel::rowsAboutToBeInserted);
        disconnect(sourceModel(), &QAbstractTableModel::rowsInserted,
                            this, &QAbstractTableModel::rowsInserted);
        disconnect(sourceModel(), &QAbstractTableModel::rowsAboutToBeRemoved,
                            this, &QAbstractTableModel::rowsAboutToBeRemoved);
        disconnect(sourceModel(), &QAbstractTableModel::rowsRemoved,
                            this, &QAbstractTableModel::rowsRemoved);
        disconnect(sourceModel(), &QAbstractTableModel::rowsAboutToBeMoved,
                            this, &QAbstractTableModel::rowsAboutToBeMoved);
        disconnect(sourceModel(), &QAbstractTableModel::rowsMoved,
                            this, &QAbstractTableModel::rowsMoved);
        disconnect(sourceModel(), &QAbstractTableModel::columnsAboutToBeInserted,
                            this, &QAbstractTableModel::columnsAboutToBeInserted);
        disconnect(sourceModel(), &QAbstractTableModel::columnsInserted,
                            this, &QAbstractTableModel::columnsInserted);
        disconnect(sourceModel(), &QAbstractTableModel::columnsAboutToBeRemoved,
                            this, &QAbstractTableModel::columnsAboutToBeRemoved);
        disconnect(sourceModel(), &QAbstractTableModel::columnsRemoved,
                            this, &QAbstractTableModel::columnsRemoved);
        disconnect(sourceModel(), &QAbstractTableModel::columnsAboutToBeMoved,
                            this, &QAbstractTableModel::columnsAboutToBeMoved);
        disconnect(sourceModel(), &QAbstractTableModel::columnsMoved,
                            this, &QAbstractTableModel::columnsMoved);
        disconnect(sourceModel(), &QAbstractTableModel::modelAboutToBeReset,
                            this, &QAbstractTableModel::modelAboutToBeReset);
        disconnect(sourceModel(), &QAbstractTableModel::modelReset,
                            this, &QAbstractTableModel::modelReset);
        disconnect(sourceModel(), &QAbstractTableModel::dataChanged,
                            this, &QAbstractTableModel::dataChanged);
        disconnect(sourceModel(), &QAbstractTableModel::headerDataChanged,
                            this, &QAbstractTableModel::headerDataChanged);
        disconnect(sourceModel(), &QAbstractTableModel::layoutAboutToBeChanged,
                            this, &QAbstractTableModel::layoutAboutToBeChanged);
        disconnect(sourceModel(), &QAbstractTableModel::layoutChanged,
                            this, &QAbstractTableModel::layoutChanged);
    }
    _sourceModel = model;
    if (sourceModel()) {
        connect(sourceModel(), &QAbstractTableModel::destroyed,
                         this, &QAbstractTableModel::destroyed);
        connect(sourceModel(), &QAbstractTableModel::rowsAboutToBeInserted,
                         this, &QAbstractTableModel::rowsAboutToBeInserted);
        connect(sourceModel(), &QAbstractTableModel::rowsInserted,
                         this, &QAbstractTableModel::rowsInserted);
        connect(sourceModel(), &QAbstractTableModel::rowsAboutToBeRemoved,
                         this, &QAbstractTableModel::rowsAboutToBeRemoved);
        connect(sourceModel(), &QAbstractTableModel::rowsRemoved,
                         this, &QAbstractTableModel::rowsRemoved);
        connect(sourceModel(), &QAbstractTableModel::rowsAboutToBeMoved,
                         this, &QAbstractTableModel::rowsAboutToBeMoved);
        connect(sourceModel(), &QAbstractTableModel::rowsMoved,
                         this, &QAbstractTableModel::rowsMoved);
        connect(sourceModel(), &QAbstractTableModel::columnsAboutToBeInserted,
                         this, &QAbstractTableModel::columnsAboutToBeInserted);
        connect(sourceModel(), &QAbstractTableModel::columnsInserted,
                         this, &QAbstractTableModel::columnsInserted);
        connect(sourceModel(), &QAbstractTableModel::columnsAboutToBeRemoved,
                         this, &QAbstractTableModel::columnsAboutToBeRemoved);
        connect(sourceModel(), &QAbstractTableModel::columnsRemoved,
                         this, &QAbstractTableModel::columnsRemoved);
        connect(sourceModel(), &QAbstractTableModel::columnsAboutToBeMoved,
                         this, &QAbstractTableModel::columnsAboutToBeMoved);
        connect(sourceModel(), &QAbstractTableModel::columnsMoved,
                         this, &QAbstractTableModel::columnsMoved);
        connect(sourceModel(), &QAbstractTableModel::modelAboutToBeReset,
                         this, &QAbstractTableModel::modelAboutToBeReset);
        connect(sourceModel(), &QAbstractTableModel::modelReset,
                         this, &QAbstractTableModel::modelReset);
        connect(sourceModel(), &QAbstractTableModel::dataChanged,
                         this, &QAbstractTableModel::dataChanged);
        connect(sourceModel(), &QAbstractTableModel::headerDataChanged,
                         this, &QAbstractTableModel::headerDataChanged);
        connect(sourceModel(), &QAbstractTableModel::layoutAboutToBeChanged,
                         this, &QAbstractTableModel::layoutAboutToBeChanged);
        connect(sourceModel(), &QAbstractTableModel::layoutChanged,
                         this, &QAbstractTableModel::layoutChanged);
    }
    endResetModel();
    emit sourceModelChanged();
}

QSize AreaModel::areaSize() const {
    return _areaSize;
}

void AreaModel::setAreaSize(const QSize &size) {
    if (_areaSize == size) { return; }
    /// \todo можно вместо ресета вызывать Insert/Remove
    beginResetModel();
    __fromParent = true;
    setAreaRowsSize(size.height());
    setAreaColumnsSize(size.width());
    __fromParent = false;
    endResetModel();
    emit areaSizeChanged();
}

int AreaModel::areaRowsSize() const {
    return _areaSize.height();
}

void AreaModel::setAreaRowsSize(int rowsSize) {
    if (_areaSize.height() == rowsSize) { return; }
    if (!__fromParent) {
        beginResetModel();
    }
    _areaSize.setHeight(rowsSize);
    emit areaRowsSizeChanged();
    if (!__fromParent) {
        endResetModel();
        emit areaSizeChanged();
    }
}

int AreaModel::areaColumnsSize() const {
    return _areaSize.width();
}

void AreaModel::setAreaColumnsSize(int columnsSize) {
    if (_areaSize.width() == columnsSize) { return; }
    if (!__fromParent) {
        beginResetModel();
    }
    _areaSize.setWidth(columnsSize);
    emit areaColumnsSizeChanged();
    if (!__fromParent) {
        endResetModel();
        emit areaColumnsSizeChanged();
    }
}

int AreaModel::splitOrientation() const {
    return _splitOrientation;
}

void AreaModel::setSplitOrientation(int splitOrientation) {
    if (_splitOrientation == splitOrientation) { return; }
    beginResetModel();
    _splitOrientation = splitOrientation;
    endResetModel();
    emit splitOrientationChanged();
}

QPoint AreaModel::first() const {
    return _first;
}

void AreaModel::setFirst(QPoint point) {
    if (_first == point) { return; }
    if (point.x() >= _sourceModel->columnCount()
            || point.y() >= _sourceModel->rowCount()) {
        return;
    }
    beginResetModel();
    __fromParent = true;
    setFirstColumn(point.x());
    setFirstRow(point.y());
    __fromParent = false;
    endResetModel();
    emit firstChanged();
}

int AreaModel::firstRow() const {
    return _first.y();
}

void AreaModel::setFirstRow(int firstRow) {
    if (_first.y() == firstRow) { return; }
    if (firstRow >= _sourceModel->rowCount()) { return; }
    if (!__fromParent) {
        beginResetModel();
    }
    _first.setY(firstRow);
    emit firstRowChanged();
    if (!__fromParent) {
        endResetModel();
        emit firstChanged();
    }
}

int AreaModel::firstColumn() const {
    return _first.x();
}

void AreaModel::setFirstColumn(int firstColumn) {
    if (_first.x() == firstColumn) { return; }
    if (firstColumn >= _sourceModel->columnCount()) { return; }
    if (!__fromParent) {
        beginResetModel();
    }
    _first.setX(firstColumn);
    emit firstColumnChanged();
    if (!__fromParent) {
        endResetModel();
        emit firstChanged();
    }
}

HeaderModel *AreaModel::headerHModel() const {
    return _headerHModel;
}

HeaderModel *AreaModel::headerVModel() const {
    return _headerVModel;
}

int AreaModel::rowMapToSource(int areaRow) const {
    return firstRow() + areaRow;
}

int AreaModel::columnMapToSource(int areaColumn) const {
    return firstColumn() + areaColumn;
}

QModelIndex AreaModel::indexMapToSource(const QModelIndex &indexArea) const {
    return _sourceModel->index(rowMapToSource(indexArea.row()),
                               columnMapToSource(indexArea.column()));
}

int AreaModel::rowCount(const QModelIndex &parent) const {
    int r;
    auto sourceIndex = indexMapToSource(parent);
    if (firstRow() + areaRowsSize() > _sourceModel->rowCount(sourceIndex)) {
        r = _sourceModel->rowCount(sourceIndex) - firstRow();
    } else {
        r = areaRowsSize();
    }
    return r;
}

int AreaModel::columnCount(const QModelIndex &parent) const {
    int r;
    auto sourceIndex = indexMapToSource(parent);
    if (firstColumn() + areaColumnsSize() > _sourceModel->columnCount(sourceIndex)) {
        r = _sourceModel->columnCount(sourceIndex) - firstColumn();
    } else {
        r = areaColumnsSize();
    }
    return r;
}

QVariant AreaModel::data(const QModelIndex &index, int role) const {
    return _sourceModel->data(indexMapToSource(index), role);
}

bool AreaModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    return _sourceModel->setData(indexMapToSource(index), value, role);
}

QModelIndex AreaModel::sibling(int row, int column, const QModelIndex &idx) const {
    return _sourceModel->sibling(row, column, indexMapToSource(idx));
}

QVariant AreaModel::headerData(int section, Qt::Orientation orientation, int role) const {
    switch (orientation) {
    case Qt::Horizontal:
        return _sourceModel->headerData(columnMapToSource(section), orientation, role);
    case Qt::Vertical:
        return _sourceModel->headerData(rowMapToSource(section), orientation, role);
    default:
        qDebug() << __PRETTY_FUNCTION__ << "orientation is bad:" << orientation;
        return QVariant();
    }
}

bool AreaModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role) {
    switch (orientation) {
    case Qt::Horizontal:
        return _sourceModel->setHeaderData(columnMapToSource(section), orientation, value, role);
    case Qt::Vertical:
        return _sourceModel->setHeaderData(rowMapToSource(section), orientation, value, role);
    default:
        qDebug() << __PRETTY_FUNCTION__ << "orientation is bad:" << orientation;
        return false;
    }
}

QMap<int, QVariant> AreaModel::itemData(const QModelIndex &index) const {
    return _sourceModel->itemData(indexMapToSource(index));
}

bool AreaModel::setItemData(const QModelIndex &index, const QMap<int, QVariant> &roles) {
    return _sourceModel->setItemData(indexMapToSource(index), roles);
}

QStringList AreaModel::mimeTypes() const {
    return _sourceModel->mimeTypes();
}

QMimeData *AreaModel::mimeData(const QModelIndexList &indexes) const {
    return _sourceModel->mimeData(indexes);
}

bool AreaModel::canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const {
    return _sourceModel->canDropMimeData(data, action, row, column, indexMapToSource(parent));
}

bool AreaModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) {
    return _sourceModel->dropMimeData(data, action, row, column, indexMapToSource(parent));
}

Qt::DropActions AreaModel::supportedDropActions() const {
    return _sourceModel->supportedDropActions();
}

Qt::DropActions AreaModel::supportedDragActions() const {
    return _sourceModel->supportedDragActions();
}

bool AreaModel::insertRows(int row, int count, const QModelIndex &parent) {
    return _sourceModel->insertRows(row, count, indexMapToSource(parent));
}

bool AreaModel::insertColumns(int column, int count, const QModelIndex &parent) {
    return _sourceModel->insertColumns(column, count, indexMapToSource(parent));
}

bool AreaModel::removeRows(int row, int count, const QModelIndex &parent) {
    return _sourceModel->removeRows(row, count, indexMapToSource(parent));
}

bool AreaModel::removeColumns(int column, int count, const QModelIndex &parent) {
    return _sourceModel->removeColumns(column, count, indexMapToSource(parent));
}

bool AreaModel::moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild) {
    return _sourceModel->moveRows(indexMapToSource(sourceParent), sourceRow, count, indexMapToSource(destinationParent), destinationChild);
}

bool AreaModel::moveColumns(const QModelIndex &sourceParent, int sourceColumn, int count, const QModelIndex &destinationParent, int destinationChild) {
    return _sourceModel->moveColumns(indexMapToSource(sourceParent), sourceColumn, count, indexMapToSource(destinationParent), destinationChild);
}

void AreaModel::fetchMore(const QModelIndex &parent) {
    return _sourceModel->fetchMore(indexMapToSource(parent));
}

bool AreaModel::canFetchMore(const QModelIndex &parent) const {
    return _sourceModel->canFetchMore(indexMapToSource(parent));
}

Qt::ItemFlags AreaModel::flags(const QModelIndex &index) const {
    return _sourceModel->flags(indexMapToSource(index));
}

void AreaModel::sort(int column, Qt::SortOrder order) {
    return _sourceModel->sort(column, order);
}

QModelIndex AreaModel::buddy(const QModelIndex &index) const {
    return _sourceModel->buddy(indexMapToSource(index));
}

QModelIndexList AreaModel::match(const QModelIndex &start, int role, const QVariant &value, int hits, Qt::MatchFlags flags) const {
    return _sourceModel->match(start, role, value, hits, flags);
}

QSize AreaModel::span(const QModelIndex &index) const {
    return _sourceModel->span(indexMapToSource(index));
}

QHash<int, QByteArray> AreaModel::roleNames() const {
    return _sourceModel->roleNames();
}

bool AreaModel::submit() {
    return _sourceModel->submit();
}

void AreaModel::revert() {
    return _sourceModel->revert();
}
