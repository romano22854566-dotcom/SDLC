#include "model.h"

TimeConverterModel::TimeConverterModel(QObject *parent)
    : QObject(parent), m_minutes(0.0) {}

void TimeConverterModel::setMinutes(double minutes) {
    m_minutes = minutes;
    recalculate();
    emit dataChanged(m_result); 
}

double TimeConverterModel::getMinutes() const {
    return m_minutes;
}

ConversionResult TimeConverterModel::getResult() const {
    return m_result;
}

void TimeConverterModel::recalculate() {
    m_result.originalMinutes = m_minutes;
    m_result.clinicQueue = m_minutes / 45.0;
    m_result.windowsBoot = m_minutes / 1.0;
    m_result.foodDelivery = m_minutes / 40.0;
    m_result.microwaveWarmup = m_minutes / 3.0;
}