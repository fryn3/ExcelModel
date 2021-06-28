#include "headermodel.h"

#include <QQmlEngine>

static bool registerMe() {
    qmlRegisterUncreatableType<HeaderModel>(
                QString("cpp.%1").arg(HeaderModel::ITEM_NAME).toUtf8(),
                12, 34, HeaderModel::ITEM_NAME.toUtf8(),
                "Not creatable as it is an enum type");
    return true;
}

const QString HeaderModel::ITEM_NAME = "HeaderModel";
const bool HeaderModel::IS_QML_REG = registerMe();


HeaderModel::HeaderModel(QAbstractTableModel *model,
                         Qt::Orientation orientation)
        : QAbstractListModel(model->parent()), _or(orientation), _m(model) {
    Q_ASSERT(model);
    if (_or == Qt::Horizontal) {
        connect(_m, &QAbstractItemModel::columnsAboutToBeInserted,
                this, &QAbstractItemModel::columnsAboutToBeInserted);
        connect(_m, &QAbstractItemModel::columnsAboutToBeMoved,
                this, &QAbstractItemModel::columnsAboutToBeMoved);
        connect(_m, &QAbstractItemModel::columnsInserted,
                this, &QAbstractItemModel::columnsInserted);
        connect(_m, &QAbstractItemModel::columnsMoved,
                this, &QAbstractItemModel::columnsMoved);
        connect(_m, &QAbstractItemModel::columnsRemoved,
                this, &QAbstractItemModel::columnsRemoved);
        connect(_m, &QAbstractItemModel::columnsAboutToBeInserted,
                this, &QAbstractItemModel::columnsAboutToBeInserted);
    } else if (_or == Qt::Vertical) {
        connect(_m, &QAbstractItemModel::rowsAboutToBeInserted,
                this, &QAbstractItemModel::rowsAboutToBeInserted);
        connect(_m, &QAbstractItemModel::rowsAboutToBeMoved,
                this, &QAbstractItemModel::rowsAboutToBeMoved);
        connect(_m, &QAbstractItemModel::rowsInserted,
                this, &QAbstractItemModel::rowsInserted);
        connect(_m, &QAbstractItemModel::rowsMoved,
                this, &QAbstractItemModel::rowsMoved);
        connect(_m, &QAbstractItemModel::rowsRemoved,
                this, &QAbstractItemModel::rowsRemoved);
        connect(_m, &QAbstractItemModel::rowsAboutToBeInserted,
                this, &QAbstractItemModel::rowsAboutToBeInserted);
    } else {
        Q_ASSERT(false);
    }
    connect(_m, &QAbstractItemModel::headerDataChanged,
            this, [this] (Qt::Orientation orientation, int first, int last) {
        if (orientation != _or) { return; }
        emit dataChanged(index(first), index(last));
    });
    connect(_m, &QAbstractItemModel::modelAboutToBeReset,
            this, &QAbstractItemModel::modelAboutToBeReset);
    connect(_m, &QAbstractItemModel::modelReset,
            this, &QAbstractItemModel::modelReset);
}

int HeaderModel::rowCount(const QModelIndex &parent) const {
    return _or == Qt::Horizontal
            ? _m->columnCount(parent)
            : _m->rowCount(parent);
}

QVariant HeaderModel::data(const QModelIndex &index, int role) const {
    return _m->headerData(index.row(), _or, role);
}

bool HeaderModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    return _m->setHeaderData(index.row(), _or, value, role);
}

