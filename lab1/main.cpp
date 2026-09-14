#include <QApplication>
#include "model.h"
#include "mainwindow.h"
#include "controller.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    TimeConverterModel model;
    MainWindow view;

    QObject::connect(&model, &TimeConverterModel::dataChanged,
                     &view, &MainWindow::onModelDataChanged);

    AppController controller(&model, &view);

    view.show();
    return app.exec();
}