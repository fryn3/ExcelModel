#include "selectioncontroller.h"
#include "myfuncs.h"
#include <QQmlEngine>

static bool registerMe() {
    qmlRegisterType<SelectionController>(
        QString("cpp.%1").arg(SelectionController::ITEM_NAME).toUtf8(), 12, 34,
                SelectionController::ITEM_NAME.toUtf8());
    return true;
}

const QString SelectionController::ITEM_NAME = "SelectionController";
const bool SelectionController::IS_QML_REG = registerMe();


const QList<void (SelectionController::*)()> SelectionController::_signalsSelection {
    &SelectionController::startRowChanged,
    &SelectionController::startColumnChanged,
    &SelectionController::endColumnChanged,
    &SelectionController::endColumnChanged,
    &SelectionController::mouseSelectionChanged,
};
const QList<void (SelectionController::*)()> SelectionController::_signalsActive {
    &SelectionController::activeRowChanged,
    &SelectionController::activeColumnChanged,
};

SelectionController::SelectionController(QObject *parent) : SelectionController(nullptr, parent) { }

SelectionController::SelectionController(SubtableModel *model, QObject *parent)
        : QObject(parent), _model(model) {
    connectSelectedArea();
    connectCurrentItem();
}

void SelectionController::collapseToActive() {
    _startRow = _endRow = _activeRow;
    _startColumn = _endColumn = _activeColumn;
}

int SelectionController::startRow() const {
    return _startRow;
}

void SelectionController::setStartRow(int startRow) {
    if (_startRow == startRow) { return; }
    _startRow = startRow;
    emit startRowChanged();
}

int SelectionController::startColumn() const {
    return _startColumn;
}

void SelectionController::setStartColumn(int startColumn) {
    if (_startColumn == startColumn) { return; }
    _startColumn = startColumn;
    emit startColumnChanged();
}

int SelectionController::endRow() const {
    return _endRow;
}

void SelectionController::setEndRow(int endRow) {
    if (_endRow == endRow) { return; }
    _endRow = endRow;
    emit endRowChanged();
}

int SelectionController::endColumn() const {
    return _endColumn;
}

void SelectionController::setEndColumn(int endColumn) {
    if (_endColumn == endColumn) { return; }
    _endColumn = endColumn;
    emit endColumnChanged();
}

int SelectionController::activeRow() const {
    return _activeRow;
}

void SelectionController::setActiveRow(int activeRow) {
    if (_activeRow == activeRow) { return; }
    _activeRow = activeRow;
    emit activeRowChanged();
}

int SelectionController::activeColumn() const {
    return _activeColumn;
}

void SelectionController::setActiveColumn(int activeColumn) {
    if (_activeColumn == activeColumn) { return; }
    _activeColumn = activeColumn;
    emit activeColumnChanged();
}

bool SelectionController::mouseSelection() const {
    return _mouseSelection;
}

void SelectionController::setMouseSelection(bool mouseSelection) {
    if (_mouseSelection == mouseSelection) { return; }
    _mouseSelection = mouseSelection;
    emit mouseSelectionChanged();
}

QPoint SelectionController::currentItem() const {
    return QPoint(_activeRow, _activeColumn);
}

void SelectionController::setCurrentItem(const QPoint &currentItem) {
    if (SelectionController::currentItem() == currentItem) { return; }
    disconnectCurrentItem();
    setActiveRow(currentItem.x());
    setActiveColumn(currentItem.y());
//    normalizeBounds();
    emit currentItemChanged();
    connectCurrentItem();
}

QRect SelectionController::selectedArea() const {
    return QRect(QPoint(_startRow, _startColumn), QPoint(_endRow, _endColumn));
}

void SelectionController::setSelectedArea(const QRect &selectedArea) {
    if (SelectionController::selectedArea() == selectedArea) { return; }
    disconnectSelectedArea();
    const auto &topLeft = selectedArea.topLeft();
    setStartRow(topLeft.x());
    setStartColumn(topLeft.y());
    const auto &bottomRight = selectedArea.bottomRight();
    setEndRow(bottomRight.x());
    setEndColumn(bottomRight.y());
//    normalizeBounds();
    emit selectedAreaChanged();
    connectSelectedArea();
}

SubtableModel *SelectionController::model() const {
    return _model;
}

void SelectionController::setModel(SubtableModel *model) {
    if (_model == model) { return; }
    _model = model;
    emit modelChanged();
}

void SelectionController::normalizeBounds() {
    if (_startRow > _endRow) {
        std::swap(_startRow, _endRow);
    }
    if (_startColumn > _endColumn) {
        std::swap(_startColumn, _endColumn);
    }
    _startColumn = std::max(0, _startColumn);
    _startRow = std::max(0, _startRow);
    _endColumn = std::min(_endColumn, _model->totalColumnCount() - 1);
    _endRow = std::min(_endRow, _model->totalRowCount() - 1);
    _activeRow = std::min(std::max(0, _activeRow), _model->totalRowCount() - 1);
    _activeColumn = std::min(std::max(0, _activeColumn), _model->totalColumnCount() - 1);
}

void SelectionController::connectCurrentItem() {
//    for (const auto &sig: _signalsActive) {
//        connect(this, sig, this, &SelectionController::normalizeBounds);
//        connect(this, sig, this, &SelectionController::currentItemChanged);
//    }
}

void SelectionController::disconnectCurrentItem() {
//    for (const auto &sig: _signalsActive) {
//        disconnect(this, sig, this, &SelectionController::normalizeBounds);
//        disconnect(this, sig, this, &SelectionController::currentItemChanged);
//    }
}

void SelectionController::connectSelectedArea() {
//    for (const auto &sig: _signalsSelection) {
//        connect(this, sig, this, &SelectionController::normalizeBounds);
//        connect(this, sig, this, &SelectionController::selectedAreaChanged);
//    }
}

void SelectionController::disconnectSelectedArea() {
//    for (const auto &sig: _signalsSelection) {
//        disconnect(this, sig, this, &SelectionController::normalizeBounds);
//        disconnect(this, sig, this, &SelectionController::selectedAreaChanged);
//    }
}
