#pragma once

#include <QAbstractListModel>


class HeaderModel : public QAbstractListModel {
    Q_OBJECT
public:
    static const QString ITEM_NAME;     // HeaderModel
    static const bool IS_QML_REG;
    HeaderModel(QAbstractTableModel *model,
                Qt::Orientation orientation);
    virtual ~HeaderModel() override = default;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

private:
    Qt::Orientation _or;
    QAbstractTableModel *_m;
};

