#include "mainwindow.h"
#include <QVBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_lastInputValue(0.0), m_hasLastInput(false) {
    setupUi();
}

void MainWindow::setupUi() {
    setWindowTitle("Конвертер времени ожидания (MVC Active Model)");
    resize(420, 320);

    auto *centralWidget = new QWidget(this);
    auto *mainLayout = new QVBoxLayout(centralWidget);

    m_btnInput = new QPushButton("Ввести данные", this);
    m_btnInput->setFixedHeight(38);
    mainLayout->addWidget(m_btnInput);

    auto *groupBox = new QGroupBox("Результаты конвертации", this);
    auto *groupLayout = new QVBoxLayout(groupBox);

    m_lblEnteredTime = new QLabel("Введенное время: —", this);
    m_lblClinic = new QLabel("Очередь в поликлинике: —", this);
    m_lblWindows = new QLabel("Загрузка Windows: —", this);
    m_lblFood = new QLabel("Ожидание доставки еды: —", this);
    m_lblMicrowave = new QLabel("Прогрев микроволновки: —", this);

    groupLayout->addWidget(m_lblEnteredTime);
    groupLayout->addWidget(m_lblClinic);
    groupLayout->addWidget(m_lblWindows);
    groupLayout->addWidget(m_lblFood);
    groupLayout->addWidget(m_lblMicrowave);

    mainLayout->addWidget(groupBox);
    setCentralWidget(centralWidget);

    connect(m_btnInput, &QPushButton::clicked, this, &MainWindow::openInputRequested);
}

double MainWindow::getLastInput() const {
    return m_lastInputValue;
}

bool MainWindow::hasLastInput() const {
    return m_hasLastInput;
}

void MainWindow::setLastInput(double val) {
    m_lastInputValue = val;
    m_hasLastInput = true;
}

void MainWindow::onModelDataChanged(const ConversionResult &result) {
    m_lblEnteredTime->setText(QString("Введенное время: <b>%1 мин</b>").arg(result.originalMinutes, 0, 'f', 1));
    m_lblClinic->setText(QString("Очередь в поликлинике: <b>%1</b> шт.").arg(result.clinicQueue, 0, 'f', 2));
    m_lblWindows->setText(QString("Загрузка Windows: <b>%1</b> раз").arg(result.windowsBoot, 0, 'f', 2));
    m_lblFood->setText(QString("Ожидание доставки еды: <b>%1</b> заказов").arg(result.foodDelivery, 0, 'f', 2));
    m_lblMicrowave->setText(QString("Прогрев микроволновки: <b>%1</b> раз").arg(result.microwaveWarmup, 0, 'f', 2));
}