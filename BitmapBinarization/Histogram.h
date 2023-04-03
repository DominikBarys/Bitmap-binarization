#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_histogram.h"
#include <QtCharts>
#include <QChartView>
#include <QLineSeries>
#include "Image.h"

class Histogram : public QDialog
{
    Q_OBJECT

public:

    Histogram(bool lineChart,QWidget* parent = nullptr);
    ~Histogram();

private:

    bool lineChart;

    Image image;

    Ui::HistogramClass ui;

    QChartView* chartView;

    QLineSeries* red;
    QLineSeries* green;
    QLineSeries* blue;

    QChart* chart;

    QPieSeries* series;

    QPieSlice* slice;

    QChart* pieChart;

    QChartView* pieChartView;
};