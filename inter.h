#ifndef INTER_H
#define INTER_H

#include <QMap>
#include <QVector>
#include <QString>
#include <cmath>

class Inter
{
private:
    QMap<float, float> interBase;
    QVector<float> interSearch;
    QVector<float> interRes;
public:
    Inter();
    Inter(QMap<float, float> iB, QVector<float> iS);
    QVector<float> line();
    QVector<float> square();
    QVector<float> cube();
};

#endif // INTER_H
