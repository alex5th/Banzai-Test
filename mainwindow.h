#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QFile>
#include <QWidget>
#include <QLayout>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QDebug>

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QFile* file = new QFile;

    QWidget* w = new QWidget;
    QVBoxLayout* mLayout = new QVBoxLayout;
    QHBoxLayout* hLayout = new QHBoxLayout;
    QPushButton* bInFile1 = new QPushButton("Добавить файл с интерполяцией");
    QPushButton* bInFile2 = new QPushButton("Добавить файл с точками");
    QLabel* text = new QLabel("Методы интерполяции:");
    QComboBox* cb = new QComboBox;
    QStringList listMethods;
    QPushButton* bOutFile = new QPushButton("Вывод в файл");

    QMap<float, float> interBase;
    QVector<float> interSearch;
public:
    MainWindow(QWidget *parent = 0);
    bool checkRangeArg(QStringList temp);
    ~MainWindow();
public slots:
    void inFile1();
    void inFile2();
    void interpolation();
};

#endif // MAINWINDOW_H
