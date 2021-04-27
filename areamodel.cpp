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
    : QAbstractTableModel(parent) {
    setSourceModel(model);
}

AreaModel::AreaModel(QAbstractTableModel *model, int splitOrientation, QObject *parent)
    : QAbstractTableModel(parent),
      _splitOrientation(splitOrientation) {
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
    __fromParent = true;
    setAreaRowsSize(size.height());
    setAreaColumnsSize(size.width());
    __fromParent = false;
    emit areaSizeChanged();
}

int AreaModel::areaRowsSize() const {
    return _areaSize.height();
}

void AreaModel::setAreaRowsSize(int rowsSize) {
    if (_areaSize.height() == rowsSize) { return; }
    _areaSize.setHeight(rowsSize);
    emit areaRowsSizeChanged();
    if (!__fromParent) {
        emit areaSizeChanged();
    }
}

int AreaModel::areaColumnsSize() const {
    return _areaSize.width();
}

void AreaModel::setAreaColumnsSize(int columnsSize) {
    if (_areaSize.width() == columnsSize) { return; }
    _areaSize.setWidth(columnsSize);
    emit areaColumnsSizeChanged();
    if (!__fromParent) {
        emit areaColumnsSizeChanged();
    }
}

int AreaModel::splitOrientation() const {
    return _splitOrientation;
}

void AreaModel::setSplitOrientation(int splitOrientation) {
    if (_splitOrientation == splitOrientation) { return; }
    _splitOrientation = splitOrientation;
    emit splitOrientationChanged();
}

QPoint AreaModel::first() const {
    return _first;
}

void AreaModel::setFirst(QPoint point) {
    if (_first == point) { return; }
    __fromParent = true;
    setFirstColumn(point.x());
    setFirstRow(point.y());
    __fromParent = false;
    emit firstChanged();
}

int AreaModel::firstRow() const {
    return _first.y();
}

void AreaModel::setFirstRow(int firstRow) {
    if (_first.y() == firstRow) { return; }
    _first.setY(firstRow);
    emit firstRowChanged();
    if (!__fromParent) {
        emit firstChanged();
    }
}

int AreaModel::firstColumn() const {
    return _first.x();
}

void AreaModel::setFirstColumn(int firstRow) {
    if (_first.x() == firstRow) { return; }
    _first.setX(firstRow);
    emit firstColumnChanged();
    if (!__fromParent) {
        emit firstChanged();
    }
}

QModelIndex AreaModel::index(int row, int column, const QModelIndex &parent) const {
    return _sourceModel->index(row, column, parent);
}

int AreaModel::rowCount(const QModelIndex &parent) const {
    int r;
    if (firstRow() + areaRowsSize() > _sourceModel->rowCount(parent)) {
        r = _sourceModel->rowCount(parent) - firstRow();
    } else {
        r = areaRowsSize();
    }
    return r;
}

int AreaModel::columnCount(const QModelIndex &parent) const {
    int r;
    if (firstColumn() + areaColumnsSize() > _sourceModel->columnCount(parent)) {
        r = _sourceModel->columnCount(parent) - firstColumn();
    } else {
        r = areaColumnsSize();
    }
    return r;
}

QVariant AreaModel::data(const QModelIndex &index, int role) const {
    return _sourceModel->data(_sourceModel->index(index.row() + firstRow(), index.column() + firstColumn()), role);
}

bool AreaModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    return _sourceModel->setData(index, value, role);
}

QModelIndex AreaModel::sibling(int row, int column, const QModelIndex &idx) const {
    return _sourceModel->sibling(row, column, idx);
}

QVariant AreaModel::headerData(int section, Qt::Orientation orientation, int role) const {
    return _sourceModel->headerData(section, orientation, role);
}

bool AreaModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role) {
    return _sourceModel->setHeaderData(section, orientation, value, role);
}

QMap<int, QVariant> AreaModel::itemData(const QModelIndex &index) const {
    return _sourceModel->itemData(index);
}

bool AreaModel::setItemData(const QModelIndex &index, const QMap<int, QVariant> &roles) {
    return _sourceModel->setItemData(index, roles);
}

QStringList AreaModel::mimeTypes() const {
    return _sourceModel->mimeTypes();
}

QMimeData *AreaModel::mimeData(const QModelIndexList &indexes) const {
    return _sourceModel->mimeData(indexes);
}

bool AreaModel::canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const {
    return _sourceModel->canDropMimeData(data, action, row, column, parent);
}

bool AreaModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) {
    return _sourceModel->dropMimeData(data, action, row, column, parent);
}

Qt::DropActions AreaModel::supportedDropActions() const {
    return _sourceModel->supportedDropActions();
}

Qt::DropActions AreaModel::supportedDragActions() const {
    return _sourceModel->supportedDragActions();
}

bool AreaModel::insertRows(int row, int count, const QModelIndex &parent) {
    return _sourceModel->insertRows(row, count, parent);
}

bool AreaModel::insertColumns(int column, int count, const QModelIndex &parent) {
    return _sourceModel->insertColumns(column, count, parent);
}

bool AreaModel::removeRows(int row, int count, const QModelIndex &parent) {
    return _sourceModel->removeRows(row, count, parent);
}

bool AreaModel::removeColumns(int column, int count, const QModelIndex &parent) {
    return _sourceModel->removeColumns(column, count, parent);
}

bool AreaModel::moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild) {
    return _sourceModel->moveRows(sourceParent, sourceRow, count, destinationParent, destinationChild);
}

bool AreaModel::moveColumns(const QModelIndex &sourceParent, int sourceColumn, int count, const QModelIndex &destinationParent, int destinationChild) {
    return _sourceModel->moveColumns(sourceParent, sourceColumn, count, destinationParent, destinationChild);
}

void AreaModel::fetchMore(const QModelIndex &parent) {
    return _sourceModel->fetchMore(parent);
}

bool AreaModel::canFetchMore(const QModelIndex &parent) const {
    return _sourceModel->canFetchMore(parent);
}

Qt::ItemFlags AreaModel::flags(const QModelIndex &index) const {
    return _sourceModel->flags(index);
}

void AreaModel::sort(int column, Qt::SortOrder order) {
    return _sourceModel->sort(column, order);
}

QModelIndex AreaModel::buddy(const QModelIndex &index) const {
    return _sourceModel->buddy(index);
}

QModelIndexList AreaModel::match(const QModelIndex &start, int role, const QVariant &value, int hits, Qt::MatchFlags flags) const {
    return _sourceModel->match(start, role, value, hits, flags);
}

QSize AreaModel::span(const QModelIndex &index) const {
    return _sourceModel->span(index);
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
