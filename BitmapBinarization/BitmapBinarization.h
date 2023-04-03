#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_BitmapBinarization.h"
#include <qfiledialog.h>
#include <qdebug.h>
#include <qmessagebox.h>
#include "Image.h"
#include <Windows.h>
#include "Histogram.h"

class BitmapBinarization : public QMainWindow
{
    Q_OBJECT

public:
    BitmapBinarization(QWidget *parent = nullptr);
    ~BitmapBinarization();

private:
    Histogram* histogram;

    QImage imageAfter;

    QString inputFilepath;

    qlonglong blacks = 0;
    qlonglong whites = 0;
    qlonglong allpixels = 0;

    int threshold = 0;
    int threads = 1;

    Ui::BitmapBinarizationClass ui;

    bool inputImageFlag = false;
    bool isBinarized = false;

    int blackPercent;
    int whitePercent;

private slots:

    void inputButtonClicked();
    void thresholdSliderValueChanged();
    void threadsSliderValueChanged();
    void startButtonClicked();
    void detailsButtonClicked();
    void histogramButtonClicked();
    void pieChartButtonClicked();
};
