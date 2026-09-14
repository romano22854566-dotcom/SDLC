#include "controller.h"
#include "model.h"
#include "mainwindow.h"
#include "inputdialog.h"
#include <QMessageBox>
#include <cmath>

AppController::AppController(TimeConverterModel *model, MainWindow *view, QObject *parent)
    : QObject(parent), m_model(model), m_view(view) {
    connect(m_view, &MainWindow::openInputRequested, this, &AppController::handleOpenInputDialog);
}

void AppController::handleOpenInputDialog() {
    InputDialog dialog(m_view);

    dialog.setInitialValue(m_view->getLastInput(), m_view->hasLastInput());

    if (dialog.exec() == QDialog::Accepted) {
        QString text = dialog.getInputText();
        bool ok = false;
        double minutes = text.toDouble(&ok);
        if (!ok || minutes <= 0.0 || !std::isfinite(minutes)) {
            QMessageBox::critical(m_view, "Ошибка ввода",
                                  "Введены некорректные данные! Пожалуйста, укажите положительное вещественное число минут.");
            return;
        }

        m_view->setLastInput(minutes);

        m_model->setMinutes(minutes);
    }
}