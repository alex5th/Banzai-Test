#ifndef INTER_H
#define INTER_H

#include <QObject>

class Inter : public QObject
{
    Q_OBJECT
private:
public:
    explicit Inter(QObject *parent = nullptr);
    Inter(int );
signals:

public slots:
};

#endif // INTER_H
