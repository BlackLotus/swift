/*
 * Copyright (c) 2010-2018 Isode Limited.
 * All rights reserved.
 * See the COPYING file for more information.
 */

#include <Swift/QtUI/QtSingleWindow.h>

#include <Swiften/Base/Platform.h>

#include <Swift/QtUI/QtChatTabs.h>
#include <Swift/QtUI/QtSettingsProvider.h>

namespace Swift {

static const QString SINGLE_WINDOW_GEOMETRY = QString("SINGLE_WINDOW_GEOMETRY");
static const QString SINGLE_WINDOW_SPLITS = QString("SINGLE_WINDOW_SPLITS");

QtSingleWindow::QtSingleWindow(QtSettingsProvider* settings) : QSplitter() {
    settings_ = settings;
    auto geometryVariant = settings_->getQSettings()->value(SINGLE_WINDOW_GEOMETRY);
    if (geometryVariant.isValid()) {
        restoreGeometry(geometryVariant.toByteArray());
    }
    connect(this, SIGNAL(splitterMoved(int, int)), this, SLOT(handleSplitterMoved(/*int, int*/)));
    setChildrenCollapsible(false);
#ifdef SWIFTEN_PLATFORM_MACOSX
    setHandleWidth(0);
#endif
}

QtSingleWindow::~QtSingleWindow() {

}

void QtSingleWindow::addWidget(QWidget* widget) {
    QtChatTabs* tabs = dynamic_cast<QtChatTabs*>(widget);
    if (tabs) {
        connect(tabs, SIGNAL(onTitleChanged(const QString&)), this, SLOT(handleTabsTitleChanged(const QString&)));
    }
    QSplitter::addWidget(widget);
}

void QtSingleWindow::handleTabsTitleChanged(const QString& title) {
    setWindowTitle(title);
}

void QtSingleWindow::handleSplitterMoved() {
    QList<QVariant> variantValues;
    QList<int> intValues = sizes();
    for (const auto& value : intValues) {
        variantValues.append(QVariant(value));
    }
    settings_->getQSettings()->setValue(SINGLE_WINDOW_SPLITS, QVariant(variantValues));
}

void QtSingleWindow::restoreSplitters() {
    QList<QVariant> variantValues = settings_->getQSettings()->value(SINGLE_WINDOW_SPLITS).toList();
    QList<int> intValues;
    for (auto&& value : variantValues) {
        intValues.append(value.toInt());
    }
    setSizes(intValues);
}

void QtSingleWindow::insertAtFront(QWidget* widget) {
    insertWidget(0, widget);
    auto splitsVariant = settings_->getQSettings()->value(SINGLE_WINDOW_SPLITS);
    if (splitsVariant.isValid()) {
        restoreSplitters();
    }
    else {
        handleSplitterMoved();
    }
    setStretchFactor(0, 0);
    setStretchFactor(1, 1);
}

void QtSingleWindow::handleGeometryChanged() {
    settings_->getQSettings()->setValue(SINGLE_WINDOW_GEOMETRY, saveGeometry());
}

void QtSingleWindow::resizeEvent(QResizeEvent* event) {
    handleGeometryChanged();
    QSplitter::resizeEvent(event);
}

void QtSingleWindow::moveEvent(QMoveEvent*) {
    handleGeometryChanged();
}

}
