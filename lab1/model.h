#ifndef MODEL_H
#define MODEL_H

#include <QObject>

struct ConversionResult {
    double originalMinutes = 0.0;
    double clinicQueue = 0.0;       
    double windowsBoot = 0.0;       
    double foodDelivery = 0.0;      
    double microwaveWarmup = 0.0;   
};

class TimeConverterModel : public QObject {
    Q_OBJECT
public:
    explicit TimeConverterModel(QObject *parent = nullptr);

    void setMinutes(double minutes);
    double getMinutes() const;
    ConversionResult getResult() const;

signals:
    void dataChanged(const ConversionResult &result);

private:
    double m_minutes;
    ConversionResult m_result;
    void recalculate();
};

#endif 