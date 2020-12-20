#pragma once

#include <QObject>
#include <QPoint>
#include <QRect>
#include <subtablemodel.h>

class SelectionController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int startRow READ startRow WRITE setStartRow NOTIFY startRowChanged)
    Q_PROPERTY(QPoint currentItem READ currentItem WRITE setCurrentItem NOTIFY currentItemChanged)
    Q_PROPERTY(QRect selectedArea READ selectedArea WRITE setSelectedArea NOTIFY selectedAreaChanged)
    Q_PROPERTY(SubtableModel* model READ model WRITE setModel NOTIFY modelChanged)
    static const QList<void (SelectionController::*)()> _signalsSelection;
    static const QList<void (SelectionController::*)()> _signalsActive;
public:
    static const QString ITEM_NAME;     // SelectionController
    static const bool IS_QML_REG;
    SelectionController(QObject *parent = nullptr);
    SelectionController(SubtableModel *model, QObject *parent = nullptr);
    virtual ~SelectionController() = default;
    Q_INVOKABLE void collapseToActive();
    int startRow() const;
    void setStartRow(int startRow);
    int startColumn() const;
    void setStartColumn(int startColumn);
    int endRow() const;
    void setEndRow(int endRow);
    int endColumn() const;
    void setEndColumn(int endColumn);
    int activeRow() const;
    void setActiveRow(int activeRow);
    int activeColumn() const;
    void setActiveColumn(int activeColumn);
    bool mouseSelection() const;
    void setMouseSelection(bool mouseSelection);
    QPoint currentItem() const;
    void setCurrentItem(const QPoint &currentItem);
    QRect selectedArea() const;
    void setSelectedArea(const QRect &selectedArea);
    SubtableModel *model() const;
    void setModel(SubtableModel *model);

signals:
    void startRowChanged();
    void startColumnChanged();
    void endRowChanged();
    void endColumnChanged();
    void activeRowChanged();
    void activeColumnChanged();
    void mouseSelectionChanged();
    void currentItemChanged();
    void selectedAreaChanged();
    void modelChanged();
private slots:
    void normalizeBounds();
private:
    void connectCurrentItem();
    void disconnectCurrentItem();
    void connectSelectedArea();
    void disconnectSelectedArea();
    int _startRow = -1;
    int _startColumn = -1;
    int _endRow = -1;
    int _endColumn = -1;
    int _activeRow = -1;
    int _activeColumn = -1;
    bool _mouseSelection = false;
    SubtableModel *_model = nullptr;
};

