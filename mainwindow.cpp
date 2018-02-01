#include "mainwindow.h"

bool MainWindow::checkRangeArg(QStringList temp)
{
    for (QString i: temp)
        if ((i.toFloat() < interBase.firstKey()) || (i.toFloat() > interBase.lastKey()))
            return false;
    return true;
}

void MainWindow::inFile1()
{
    QString fn;
    fn = QFileDialog::getOpenFileName(0,QString("Выберите файл с интерполяцией"),"input1.txt",QString("Text file (*.txt)"));
    file->setFileName(fn);
    file->open(QIODevice::ReadOnly);
    QStringList temp = QString(file->readAll()).split(QRegExp("[\\s]"),QString::SkipEmptyParts);
    file->close();
    if (fn!="")
        if (temp.size() != temp[0].toInt() * 2 + 1)
        {
            bInFile2->setDisabled(true);
            bOutFile->setDisabled(true);
            inFile1();
            return;
        }
        else
        {
            bInFile2->setDisabled(false);
            interBase.clear();
            for (int i = 1; i < temp.size(); i = i + 2)
                interBase.insert(temp[i].toFloat(), temp[i + 1].toFloat());
        }
    qDebug() << interBase;

}

void MainWindow::inFile2()
{
    QString fn;
    fn = QFileDialog::getOpenFileName(0,QString("Выберите файл с аргументами"),"input2.txt",QString("Text file (*.txt)"));
    file->setFileName(fn);
    file->open(QIODevice::ReadOnly);
    QStringList temp = QString(file->readAll()).split(QRegExp("[\\s]"),QString::SkipEmptyParts);
    if (fn!="")
        if ((temp.size() != temp[0].toInt() + 1) || (!checkRangeArg(temp)))
        {
            bOutFile->setDisabled(true);
            inFile2();
            return;
        }
        else
        {
            bOutFile->setDisabled(false);
            interSearch.clear();
            for (int i = 1; i < temp.size(); ++i)
                interSearch.push_back(temp[i].toFloat());
            qDebug() << interSearch;
        }

    file->close();
}

void MainWindow::interpolation()
{
    for (float x: interSearch) //linear
    {
        auto m = interBase.lowerBound(x);
        qDebug() << (m - 1).value() +  (m.value() - (m - 1).value()) / (m.key() - (m - 1).key()) * (x - (m - 1).key());
    }

    for (float x: interSearch) //kvadrat
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
        qDebug() << a[0] + a[1] * x + a[2] * x * x;
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    listMethods << "Линейная" << "Квадратичная" << "Кубическая";
    cb->addItems(listMethods);
    bInFile2->setDisabled(true);
    bOutFile->setDisabled(true);


    this->setCentralWidget(w);
    this->setWindowTitle("Толчинский, интерполяция");
    w->setLayout(mLayout);
    mLayout->addWidget(bInFile1);
    mLayout->addWidget(bInFile2);
    mLayout->addLayout(hLayout);
    hLayout->addWidget(text);
    hLayout->addWidget(cb);
    mLayout->addWidget(bOutFile);



    QObject::connect(bInFile1, &QPushButton::clicked, this, &MainWindow::inFile1);
    QObject::connect(bInFile2, &QPushButton::clicked, this, &MainWindow::inFile2);
    QObject::connect(bOutFile, &QPushButton::clicked, this, &MainWindow::interpolation);
}

MainWindow::~MainWindow()
{

}
