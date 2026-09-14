#include "inputdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

InputDialog::InputDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Ввод данных");
    setModal(true);
    setFixedSize(320, 140);

    auto *layout = new QVBoxLayout(this);
    auto *label = new QLabel("Введите время ожидания (в минутах):", this);
    m_lineEdit = new QLineEdit(this);
    m_lineEdit->setPlaceholderText("Например: 45.5");

    auto *btnLayout = new QHBoxLayout();
    auto *btnOk = new QPushButton("ОК", this);
    auto *btnCancel = new QPushButton("Отмена", this);

    btnLayout->addWidget(btnOk);
    btnLayout->addWidget(btnCancel);

    layout->addWidget(label);
    layout->addWidget(m_lineEdit);
    layout->addLayout(btnLayout);

    connect(btnOk, &QPushButton::clicked, this, &QDialog::accept);
    connect(btnCancel, &QPushButton::clicked, this, &QDialog::reject);
}

void InputDialog::setInitialValue(double value, bool hasPreviousValue) {
    if (hasPreviousValue) {
        m_lineEdit->setText(QString::number(value, 'f', 1));
        m_lineEdit->selectAll();
    } else {
        m_lineEdit->clear();
    }
}

QString InputDialog::getInputText() const {
    return m_lineEdit->text().trimmed();
}