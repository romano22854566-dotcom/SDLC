#ifndef INPUTDIALOG_H
#define INPUTDIALOG_H

#include <QDialog>

class QLineEdit;

class InputDialog : public QDialog {
    Q_OBJECT
public:
    explicit InputDialog(QWidget *parent = nullptr);

    void setInitialValue(double value, bool hasPreviousValue);
    QString getInputText() const;

private:
    QLineEdit *m_lineEdit;
};

#endif 