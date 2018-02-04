#include "inter.h"

Inter::Inter()
{

}

Inter::Inter(QMap<float, float> iB, QVector<float> iS): interBase(iB), interSearch(iS)
{

}

QVector<float> Inter::line()
{
    for (float x: interSearch)
    {
        auto m = interBase.lowerBound(x);
        interRes.push_back((m - 1).value() +  (m.value() - (m - 1).value()) / (m.key() - (m - 1).key()) * (x - (m - 1).key()));
    }
    return interRes;
}

QVector<float> Inter::square()
{
    if (interBase.size() < 3) return interRes;
    for (float x: interSearch)
    {
        auto m = interBase.lowerBound(x);
        QMap<float, float>::iterator it[3];
        if ((m - 1).key() == interBase.firstKey())
        {
            it[0] = m - 1;
            it[1] = m;
            it[2] = m + 1;
        }
        else if (m.key() == interBase.lastKey())
        {
            it[0] = m - 2;
            it[1] = m - 1;
            it[2] = m;
        }
        else if ((x - (m - 2).key()) <= ((m + 1).key() - x))
        {
            it[0] = m - 2;
            it[1] = m - 1;
            it[2] = m;
        }
        else
        {
            it[0] = m - 1;
            it[1] = m;
            it[2] = m + 1;
        }
        float a[3];
        a[2] = (it[2].value() - it[0].value()) / ((it[2].key() - it[0].key()) * (it[2].key() - it[1].key())) -
                (it[1].value() - it[0].value()) / ((it[1].key() - it[0].key()) * (it[2].key() - it[1].key()));
        a[1] = (it[1].value() - it[0].value()) / (it[1].key() - it[0].key()) - a[2] * (it[1].key() + it[0].key());
        a[0] = it[0].value() - a[1] * it[0].key() - a[2] * it[0].key() * it[0].key();
        interRes.push_back(a[0] + a[1] * x + a[2] * x * x);
    }
    return interRes;
}

QVector<float> Inter::cube()
{
    if (interBase.size() < 3) return interRes;
    QVector<float> h, l, interBaseKeys;
    for (QMap<float, float>::iterator it = interBase.begin() + 1; it != interBase.end(); it++)
    {
        interBaseKeys.push_back((it - 1).key());
        h.push_back(it.key() - (it - 1).key());
        l.push_back((it.value() - (it - 1).value())/h.last());
    }
    QVector<float> delta, lambda;
    delta.push_back(- h[1] / (2 * (h[0] + h[1])));
    lambda.push_back(1.5 * (l[1] - l[0]) / (h[0] + h[1]));
    for (int k = 1; k < interBase.size() - 2; k++)
    {
        delta.push_back(-h[k + 1]/(2 * h[k] + 2 * h[k + 1] + h[k] * delta[k - 1]));
        lambda.push_back((3 * l[k + 1] - 3 * l[k] - h[k] * lambda[k-1]) /
                (2 * h[k] + 2*h[k + 1] + h[k] * delta[k - 1]));
    }
    QVector<float> c(interBase.size());
    c[0] = 0;
    c[interBase.size() - 1] = 0;
    for (int k = interBase.size() - 1; k > 1; k--)
    {
        c[k-1] = delta[k-2] * c[k] + lambda[k-2];
    }
    QVector<float> b, d;
    for(int k = 0; k < interBase.size() - 1; k++)
    {
        d.push_back((c[k + 1] - c[k]) / (3 * h[k]));
        b.push_back(l[k] + (2 * c[k + 1] * h[k] + h[k] * c[k]) / 3);
    }
    for (float x: interSearch)
    {
        auto m = interBase.lowerBound(x) - 1;
        int indX0 = interBaseKeys.indexOf(m.key());
        interRes.push_back(m.value() + b[indX0] * (x - m.key()) + c[indX0] * pow((x - m.key()), 2)
                    + d[indX0] * pow((x - m.key()), 3));
    }
    return interRes;
}
