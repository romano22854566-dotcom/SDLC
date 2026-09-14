#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>

class TimeConverterModel;
class MainWindow;

class AppController : public QObject {
    Q_OBJECT
public:
    AppController(TimeConverterModel *model, MainWindow *view, QObject *parent = nullptr);

public slots:
    void handleOpenInputDialog();

private:
    TimeConverterModel *m_model;
    MainWindow *m_view;
};

#endif 