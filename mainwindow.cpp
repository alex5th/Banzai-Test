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
    fn = QFileDialog::getOpenFileName(0,QString("Выберите файл с интерполяцией"),"",QString("Text file (*.txt)"));
    file->setFileName(fn);
    file->open(QIODevice::ReadOnly);
    QStringList temp = QString(file->readAll()).split(QRegExp("[\\s]"),QString::SkipEmptyParts);
    file->close();
    if (fn!="")
        if ((temp.size() != temp[0].toInt() * 2 + 1) || (temp[0].toInt() < 2))
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
    fn = QFileDialog::getOpenFileName(0,QString("Выберите файл с аргументами"),"",QString("Text file (*.txt)"));
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

void MainWindow::outFile()
{
    Inter inter1(interBase, interSearch);
    QVector<float> outData;

    switch (cb->currentIndex()) {
    case 0:
        outData = inter1.line();
        break;
    case 1:
        outData = inter1.square();
        break;
    case 2:
        outData = inter1.cube();
        break;
    default:
        break;
    }

    if (outData.isEmpty())
    {
        QErrorMessage errorMessage;
        errorMessage.showMessage("Нет возможности посчитать!");
        errorMessage.exec();
        return;
    }

    int fileNumber = 0;
    do
    {
        file->setFileName("output" + QString::number(fileNumber) + ".txt");
        ++fileNumber;
    }
    while (file->exists());


    if(file->open(QIODevice::WriteOnly))
    {
        QTextStream out(file);
        out << outData.size() << endl;
        for (int i = 0; i < outData.size(); ++i)
        {
            out << interSearch[i] << "  " <<outData[i] << endl;
        }
    }
    file->close();
    qDebug() << outData;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    listMethods << "Линейная" << "Квадратичная" << "Кубическая";
    cb->addItems(listMethods);
    bInFile2->setDisabled(true);
    bOutFile->setDisabled(true);


    this->setCentralWidget(w);
    this->setWindowTitle("Интерполяция");
    w->setLayout(mLayout);
    mLayout->addWidget(bInFile1);
    mLayout->addWidget(bInFile2);
    mLayout->addLayout(hLayout);
    hLayout->addWidget(text);
    hLayout->addWidget(cb);
    mLayout->addWidget(bOutFile);



    QObject::connect(bInFile1, &QPushButton::clicked, this, &MainWindow::inFile1);
    QObject::connect(bInFile2, &QPushButton::clicked, this, &MainWindow::inFile2);
    QObject::connect(bOutFile, &QPushButton::clicked, this, &MainWindow::outFile);
}


MainWindow::~MainWindow()
{

}
