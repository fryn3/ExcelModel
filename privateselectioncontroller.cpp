#include "privateselectioncontroller.h"

const QList<QPair<void (PrivateSelectionController::*)(),
                  void (SelectionController::*)()>> PrivateSelectionController::_signalsSelection {
    { &PrivateSelectionController::signalStartRowChanged, &SelectionController::startRowChanged },
    { &PrivateSelectionController::signalStartColumnChanged, &SelectionController::startColumnChanged },
    { &PrivateSelectionController::signalEndRowChanged, &SelectionController::endRowChanged },
    { &PrivateSelectionController::signalEndColumnChanged, &SelectionController::endColumnChanged },
    { &PrivateSelectionController::signalMouseSelectionChanged, &SelectionController::mouseSelectionChanged },
};
const QList<QPair<void (PrivateSelectionController::*)(),
                  void (SelectionController::*)()>> PrivateSelectionController::_signalsActive {
    { &PrivateSelectionController::signalActiveRowChanged, &SelectionController::activeRowChanged },
    { &PrivateSelectionController::signalActiveColumnChanged, &SelectionController::activeColumnChanged },
};

PrivateSelectionController::PrivateSelectionController(SelectionController *parent)
        : QObject(parent), _parent(parent) {
    for (const auto &sig: _signalsActive) {
        connect(this, sig.first, _parent, sig.second);
    }
    for (const auto &sig: _signalsSelection) {
        connect(this, sig.first, _parent, sig.second);
    }
    connect(this, &PrivateSelectionController::signalCurrentItemChanged,
            parent, &SelectionController::currentItemChanged);
    connect(this, &PrivateSelectionController::signalSelectedAreaChanged,
            parent, &SelectionController::selectedAreaChanged);
    connect(this, &PrivateSelectionController::signalModelChanged,
            parent, &SelectionController::modelChanged);
    connectCurrentItem();
    connectSelectedArea();
}

void PrivateSelectionController::connectCurrentItem() {
    for (const auto &sig: _signalsActive) {
        connect(this, sig.first, _parent, &SelectionController::currentItemChanged);
    }
}

void PrivateSelectionController::disconnectCurrentItem() {
    for (const auto &sig: _signalsActive) {
        disconnect(this, sig.first, _parent, &SelectionController::currentItemChanged);
    }
}

void PrivateSelectionController::connectSelectedArea() {
    for (const auto &sig: _signalsSelection) {
        connect(this, sig.first, _parent, &SelectionController::selectedAreaChanged);
    }
}

void PrivateSelectionController::disconnectSelectedArea() {
    for (const auto &sig: _signalsSelection) {
        disconnect(this, sig.first, _parent, &SelectionController::selectedAreaChanged);
    }
}
