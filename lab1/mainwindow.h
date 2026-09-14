#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "model.h"

class QLabel;
class QPushButton;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

    double getLastInput() const;
    bool hasLastInput() const;
    void setLastInput(double val);

signals:
    void openInputRequested();

public slots:
    void onModelDataChanged(const ConversionResult &result);

private:
    double m_lastInputValue;
    bool m_hasLastInput;

    QPushButton *m_btnInput;
    QLabel *m_lblEnteredTime;
    QLabel *m_lblClinic;
    QLabel *m_lblWindows;
    QLabel *m_lblFood;
    QLabel *m_lblMicrowave;

    void setupUi();
};

#endif 