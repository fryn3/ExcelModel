#pragma once

#include "selectioncontroller.h"

class PrivateSelectionController : public QObject {
    Q_OBJECT
    friend SelectionController;
    static const QList<QPair<void (PrivateSelectionController::*)(),
                             void (SelectionController::*)()>> _signalsSelection;
    static const QList<QPair<void (PrivateSelectionController::*)(),
                             void (SelectionController::*)()>> _signalsActive;
    PrivateSelectionController(SelectionController *parent = nullptr);
public:
    virtual ~PrivateSelectionController() = default;
    void emitStartRowChanged()          { emit signalStartRowChanged(); }
    void emitStartColumnChanged()       { emit signalStartColumnChanged(); }
    void emitEndRowChanged()            { emit signalEndRowChanged(); }
    void emitEndColumnChanged()         { emit signalEndColumnChanged(); }
    void emitActiveRowChanged()         { emit signalActiveRowChanged(); }
    void emitActiveColumnChanged()      { emit signalActiveColumnChanged(); }
    void emitMouseSelectionChanged()    { emit signalMouseSelectionChanged(); }
    void emitCurrentItemChanged()       { emit signalCurrentItemChanged(); }
    void emitSelectedAreaChanged()      { emit signalSelectedAreaChanged(); }
    void emitModelChanged()             { emit signalModelChanged(); }

    void connectCurrentItem();
    void disconnectCurrentItem();
    void connectSelectedArea();
    void disconnectSelectedArea();

signals:
    void signalStartRowChanged();
    void signalStartColumnChanged();
    void signalEndRowChanged();
    void signalEndColumnChanged();
    void signalActiveRowChanged();
    void signalActiveColumnChanged();
    void signalMouseSelectionChanged();
    void signalCurrentItemChanged();
    void signalSelectedAreaChanged();
    void signalModelChanged();
private:
    SelectionController *_parent;
};
