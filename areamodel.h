#pragma once

#include <QAbstractTableModel>
#include <QPoint>
#include <QSize>

#include "headermodel.h"

class AreaModel : public QAbstractTableModel {
    Q_OBJECT
    Q_PROPERTY(QAbstractTableModel* sourceModel READ sourceModel WRITE setSourceModel NOTIFY sourceModelChanged)
    Q_PROPERTY(QSize areaSize READ areaSize WRITE setAreaSize RESET resetAreaSize NOTIFY areaSizeChanged)
    Q_PROPERTY(int areaRowsSize READ areaRowsSize WRITE setAreaRowsSize RESET resetAreaRowsSize NOTIFY areaRowsSizeChanged)
    Q_PROPERTY(int areaColumnsSize READ areaColumnsSize WRITE setAreaColumnsSize RESET resetAreaColumnsSize NOTIFY areaColumnsSizeChanged)
    Q_PROPERTY(int splitOrientation READ splitOrientation WRITE setSplitOrientation NOTIFY splitOrientationChanged)
    Q_PROPERTY(QPoint first READ first WRITE setFirst NOTIFY firstChanged)
    Q_PROPERTY(int firstRow READ firstRow WRITE setFirstRow NOTIFY firstRowChanged)
    Q_PROPERTY(int firstColumn READ firstColumn WRITE setFirstColumn NOTIFY firstColumnChanged)
    Q_PROPERTY(QAbstractItemModel* headerHModel READ headerHModel NOTIFY headerHModelChanged)
    Q_PROPERTY(QAbstractItemModel* headerVModel READ headerVModel NOTIFY headerVModelChanged)

public:
    static const QString ITEM_NAME;     // AreaModel
    static const bool IS_QML_REG;
    static const int SPLIT_ORIENTATION_DEFAULT = Qt::Vertical;
    static const int SIZE_DEFAULT = -1;

    AreaModel(QAbstractTableModel *model = nullptr, QObject *parent = nullptr);
    AreaModel(QAbstractTableModel *model, int splitOrientation, QObject *parent);
    QAbstractTableModel *sourceModel() const;
    void setSourceModel(QAbstractTableModel *model);
    const QSize &areaSize() const;
    void setAreaSize(const QSize &size);
    void resetAreaSize();
    int areaRowsSize() const;
    void setAreaRowsSize(int rowsSize);
    void resetAreaRowsSize();
    int areaColumnsSize() const;
    void setAreaColumnsSize(int columnsSize);
    void resetAreaColumnsSize();
    int splitOrientation() const;
    void setSplitOrientation(int splitOrientation);
    QPoint first() const;
    void setFirst(QPoint point);
    int firstRow() const;
    void setFirstRow(int firstRow);
    int firstColumn() const;
    void setFirstColumn(int firstColumn);

    HeaderModel *headerHModel() const;
    HeaderModel *headerVModel() const;
signals:
    void sourceModelChanged();
    void areaSizeChanged();
    void areaRowsSizeChanged();
    void areaColumnsSizeChanged();
    void splitOrientationChanged();
    void firstChanged();
    void firstRowChanged();
    void firstColumnChanged();

    void headerHModelChanged();
    void headerVModelChanged();

private:
    int rowMapToSource(int areaRow) const;
    int columnMapToSource(int areaRow) const;
    QModelIndex indexMapToSource(const QModelIndex &indexArea) const;

    QAbstractTableModel *_sourceModel = nullptr;
    QSize _areaSize = QSize(SIZE_DEFAULT, SIZE_DEFAULT);
    int _splitOrientation = SPLIT_ORIENTATION_DEFAULT;
    QPoint _first;
    bool __fromParent = false;

    HeaderModel *_headerHModel = nullptr;
    HeaderModel *_headerVModel = nullptr;

public:
    Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_INVOKABLE int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_INVOKABLE QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Q_INVOKABLE bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Q_INVOKABLE QModelIndex sibling(int row, int column, const QModelIndex &idx) const override;
    Q_INVOKABLE QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Q_INVOKABLE bool setHeaderData(int section, Qt::Orientation orientation,
                                   const QVariant &value, int role = Qt::EditRole) override;
    Q_INVOKABLE void fetchMore(const QModelIndex &parent = QModelIndex()) override;
    Q_INVOKABLE bool canFetchMore(const QModelIndex &parent) const override;
    Q_INVOKABLE QMap<int, QVariant> itemData(const QModelIndex &index) const override;
    Q_INVOKABLE bool setItemData(const QModelIndex &index, const QMap<int, QVariant> &roles) override;
    Q_INVOKABLE QStringList mimeTypes() const override;
    Q_INVOKABLE QMimeData *mimeData(const QModelIndexList &indexes) const override;
    Q_INVOKABLE bool canDropMimeData(const QMimeData *data, Qt::DropAction action,
                                     int row, int column, const QModelIndex &parent) const override;
    Q_INVOKABLE bool dropMimeData(const QMimeData *data, Qt::DropAction action,
                                  int row, int column, const QModelIndex &parent) override;
    Q_INVOKABLE Qt::DropActions supportedDropActions() const override;
    Q_INVOKABLE Qt::DropActions supportedDragActions() const override;
    Q_INVOKABLE bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    Q_INVOKABLE bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;
    Q_INVOKABLE bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    Q_INVOKABLE bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;
    Q_INVOKABLE bool moveRows(const QModelIndex &sourceParent, int sourceRow,
                              int count, const QModelIndex &destinationParent, int destinationChild) override;
    Q_INVOKABLE bool moveColumns(const QModelIndex &sourceParent, int sourceColumn,
                                 int count, const QModelIndex &destinationParent, int destinationChild) override;
    Q_INVOKABLE Qt::ItemFlags flags(const QModelIndex &index) const override;
    Q_INVOKABLE void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;
    Q_INVOKABLE QModelIndex buddy(const QModelIndex &index) const override;
    Q_INVOKABLE QModelIndexList match(const QModelIndex &start, int role,
                                      const QVariant &value, int hits = 1,
                                      Qt::MatchFlags flags =
                                      Qt::MatchFlags(Qt::MatchStartsWith|Qt::MatchWrap)) const override;
    Q_INVOKABLE QSize span(const QModelIndex &index) const override;
    Q_INVOKABLE QHash<int, QByteArray> roleNames() const override;

public slots:
    bool submit() override;
    void revert() override;
};
