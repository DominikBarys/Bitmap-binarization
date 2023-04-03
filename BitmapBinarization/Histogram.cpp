#include "Histogram.h"

Histogram::Histogram(bool lineChart, QWidget* parent)
{
    ui.setupUi(this);

    if (lineChart)
    {
         red = new QLineSeries();
         green = new QLineSeries();
         blue = new QLineSeries();

        red->setColor("red");
        green->setColor("green");
        blue->setColor("blue");
        red->clear();
        green->clear();
        blue->clear();

        for (int i = 0; i < 256; i++) {
            red->append(i, image.redValues[i]);
            green->append(i, image.greenValues[i]);
            blue->append(i, image.blueValues[i]);
        }

        chart = new QChart();
        chart->addSeries(red);
        chart->addSeries(green);
        chart->addSeries(blue);

        chart->createDefaultAxes();
        chart->setTitle("Color histogram");
        chart->legend()->hide();

        chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);
        chartView->setParent(ui.horizontalFrame);
    }
    else
    {
        series = new QPieSeries();
        series->append("White", image.amountWhite);
        series->append("Black", image.amountBlack);

        slice = series->slices().at(0);
        slice->setLabelVisible();
        slice->setPen(QPen(Qt::lightGray, 2));
        slice->setBrush(Qt::white);
        slice = series->slices().at(1);
        slice->setLabelVisible();
        slice->setPen(QPen(Qt::lightGray, 2)); 
        slice->setBrush(Qt::black); 

        pieChart = new QChart();
        pieChart->addSeries(series);
        pieChart->setTitle("Pie chart of white and black pixels");

        pieChartView = new QChartView(pieChart);
        pieChartView->setRenderHint(QPainter::Antialiasing);
        pieChartView->setParent(ui.horizontalFrame);
    }
}

Histogram::~Histogram()
{
    if(red != nullptr)
        delete red;
    if (green != nullptr)
        delete green;
    if (blue != nullptr)
        delete blue;
    if (chart != nullptr)
        delete chart;
    if (chartView != nullptr)
        delete chartView;
    if (series != nullptr)
        delete series;
    if (pieChart != nullptr)
        delete pieChart;
    if (pieChartView != nullptr)
        delete pieChartView;
}