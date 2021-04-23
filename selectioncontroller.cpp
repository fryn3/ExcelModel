#include "selectioncontroller.h"
#include <QQmlEngine>
#include "privateselectioncontroller.h"

static bool registerMe() {
    qmlRegisterType<SelectionController>(
        QString("cpp.%1").arg(SelectionController::ITEM_NAME).toUtf8(), 12, 34,
                SelectionController::ITEM_NAME.toUtf8());
    return true;
}

const QString SelectionController::ITEM_NAME = "SelectionController";
const bool SelectionController::IS_QML_REG = registerMe();

SelectionController::SelectionController(QObject *parent) : SelectionController(nullptr, parent) { }

SelectionController::SelectionController(SubtableModel *model, QObject *parent)
        : QObject(parent), _model(model),
          _private(new PrivateSelectionController(this)) { }

void SelectionController::normalizeBounds() {
    if (_startRow > _endRow) {
        std::swap(_startRow, _endRow);
        _private->emitStartRowChanged();
        _private->emitEndRowChanged();
    }
    if (_startColumn > _endColumn) {
        std::swap(_startColumn, _endColumn);
        _private->emitStartRowChanged();
        _private->emitEndRowChanged();
    }
    setStartColumn(std::max(0, _startColumn));
    setStartRow(std::max(0, _startRow));
    setEndColumn(std::min(_endColumn, _model->totalColumnCount() - 1));
    setEndRow(std::min(_endRow, _model->totalRowCount() - 1));
    setActiveRow(std::min(std::max(0, _activeRow), _model->totalRowCount() - 1));
    setActiveColumn(std::min(std::max(0, _activeColumn), _model->totalColumnCount() - 1));
}

void SelectionController::collapseToActive() {
    setStartRow(activeRow());
    setEndRow(activeRow());
    setStartColumn(activeColumn());
    setEndColumn(activeColumn());
}

int SelectionController::startRow() const {
    return _startRow;
}

void SelectionController::setStartRow(int startRow) {
    if (_startRow == startRow) { return; }
    if (_disable && startRow != activeRow()) { return; }
    _startRow = startRow;
    _private->emitStartRowChanged();
}

int SelectionController::startColumn() const {
    return _startColumn;
}

void SelectionController::setStartColumn(int startColumn) {
    if (_startColumn == startColumn) { return; }
    if (_disable && startColumn != activeColumn()) { return; }
    _startColumn = startColumn;
    _private->emitStartColumnChanged();
}

int SelectionController::endRow() const {
    return _endRow;
}

void SelectionController::setEndRow(int endRow) {
    if (_endRow == endRow) { return; }
    if (_disable && endRow != activeRow()) { return; }
    _endRow = endRow;
    _private->emitEndRowChanged();
}

int SelectionController::endColumn() const {
    return _endColumn;
}

void SelectionController::setEndColumn(int endColumn) {
    if (_endColumn == endColumn) { return; }
    if (_disable && endColumn != activeColumn()) { return; }
    _endColumn = endColumn;
    _private->emitEndColumnChanged();
}

int SelectionController::activeRow() const {
    return _activeRow;
}

void SelectionController::setActiveRow(int activeRow) {
    if (_activeRow == activeRow) { return; }
    _activeRow = activeRow;
    _private->emitActiveRowChanged();
    if (_disable) {
        collapseToActive();
    }
}

int SelectionController::activeColumn() const {
    return _activeColumn;
}

void SelectionController::setActiveColumn(int activeColumn) {
    if (_activeColumn == activeColumn) { return; }
    _activeColumn = activeColumn;
    _private->emitActiveColumnChanged();
    if (_disable) {
        collapseToActive();
    }
}

bool SelectionController::mouseSelection() const {
    return _mouseSelection;
}

void SelectionController::setMouseSelection(bool mouseSelection) {
    if (_mouseSelection == mouseSelection) { return; }
    _mouseSelection = mouseSelection;
    _private->emitMouseSelectionChanged();
}

QPoint SelectionController::currentItem() const {
    return QPoint(_activeRow, _activeColumn);
}

void SelectionController::setCurrentItem(const QPoint &currentItem) {
    if (SelectionController::currentItem() == currentItem) { return; }
    _private->disconnectCurrentItem();
    setActiveRow(currentItem.x());
    setActiveColumn(currentItem.y());
    if (_disable) {
        collapseToActive();
    }
    _private->emitCurrentItemChanged();
    _private->connectCurrentItem();
}

QRect SelectionController::selectedArea() const {
    return QRect(QPoint(_startRow, _startColumn), QPoint(_endRow, _endColumn));
}

void SelectionController::setSelectedArea(const QRect &selectedArea) {
    if (SelectionController::selectedArea() == selectedArea) { return; }
    if (_disable) { return; }
    _private->disconnectSelectedArea();
    const auto &topLeft = selectedArea.topLeft();
    setStartRow(topLeft.x());
    setStartColumn(topLeft.y());
    const auto &bottomRight = selectedArea.bottomRight();
    setEndRow(bottomRight.x());
    setEndColumn(bottomRight.y());
    _private->emitSelectedAreaChanged();
    _private->connectSelectedArea();
}

SubtableModel *SelectionController::model() const {
    return _model;
}

void SelectionController::setModel(SubtableModel *model) {
    if (_model == model) { return; }
    _model = model;
    _private->emitModelChanged();
}

bool SelectionController::disable() const {
    return _disable;
}

void SelectionController::setDisable(bool disable) {
    if (_disable == disable) { return; }
    _disable = disable;
    if (_disable) {
        collapseToActive();
    }
    emit disableChanged();
}
