#include "BitmapBinarization.h"

BitmapBinarization::BitmapBinarization(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    QImage backgroundImage;
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    backgroundImage.load("Resources\\greenGradientBackground.jpg");
    backgroundImage = backgroundImage.scaledToWidth(ui.lbl_background->width(), Qt::SmoothTransformation);
    backgroundImage = backgroundImage.scaledToHeight(ui.lbl_background->width(), Qt::SmoothTransformation);
    ui.lbl_background->setPixmap(QPixmap::fromImage(backgroundImage));

    ui.threadsSlider->setValue(std::thread::hardware_concurrency());
    ui.thresholdSlider->setValue(127);
}

BitmapBinarization::~BitmapBinarization()
{}

void BitmapBinarization::inputButtonClicked()
{
    inputFilepath = QFileDialog::getOpenFileName(this, tr("Choose"), "", tr("Images (*.bmp)"));

    if (QString::compare(inputFilepath, QString()) != 0)
    {
        QImage image;
        bool valid = image.load(inputFilepath);

        if (valid)
        {
            image = image.scaledToWidth(ui.inputImage->width(), Qt::SmoothTransformation);
            ui.inputImage->setPixmap(QPixmap::fromImage(image));
            inputImageFlag = true;
        }
        else
        {
            qDebug() << "ERROR";
        }
    }
}

void BitmapBinarization::thresholdSliderValueChanged()
{
    threshold = ui.thresholdSlider->value();

    ui.lbl_threshold->setText(QString::number(ui.thresholdSlider->value()));

    qDebug() << threshold;

}

void BitmapBinarization::threadsSliderValueChanged()
{
    threads = ui.threadsSlider->value();

    ui.lbl_threads->setText(QString::number(ui.threadsSlider->value()));

    qDebug() << threads;
}

void BitmapBinarization::startButtonClicked()
{
    if(!inputImageFlag)
    {
        QMessageBox::warning(this, "Incorrect settings", "Please choose bmp file to binarize");
    }
    else
    {
        if (!ui.cppRadioButton->isChecked() && !ui.asmRadioButton->isChecked())
        {
            QMessageBox::warning(this, "Incorrect settings", "Please check C++ or ASM button");
        }
        else
        {
            if (ui.cppRadioButton->isChecked())
            {
                Image image(inputFilepath.toStdString(), threads, threshold, true);
                blacks = image.amountBlack;
                whites = image.amountWhite;
                allpixels = image.arrLen;
                blackPercent = (blacks * 100 / allpixels);
                whitePercent = 100 - blackPercent;
                ui.lbl_exeTime->setText(QString::number(image.elapsedTime) + " ms");
                isBinarized = true;
            }
            else if (ui.asmRadioButton->isChecked())
            {
                Image image(inputFilepath.toStdString(), threads, threshold, false);
                blacks = image.amountBlack;
                whites = image.amountWhite;
                allpixels = image.arrLen;
                blackPercent = (blacks * 100 / allpixels);
                whitePercent = 100 - blackPercent;
                ui.lbl_exeTime->setText(QString::number(image.elapsedTime) + " ms");
                isBinarized = true;
            }

            imageAfter.load("binarized.bmp");

            imageAfter = imageAfter.scaledToWidth(ui.outputImage->width(), Qt::SmoothTransformation);
            ui.outputImage->setPixmap(QPixmap::fromImage(imageAfter));
        }
    }
}

void BitmapBinarization::detailsButtonClicked()
{
    if (isBinarized)
    {
        QMessageBox::information(this, "Details", "Your image has " + QString::number(blackPercent) + "% of black pixels and "
            + QString::number(whitePercent) + "% of white pixels");
    }
    else
    {
        QMessageBox::warning(this, "Incorrect settings", "First binarize bmp file to show details!");
    }
}

void BitmapBinarization::histogramButtonClicked()
{
    if (isBinarized)
    {
        histogram = new Histogram(true, this);
        histogram->show();
    }
    else
    {
        QMessageBox::warning(this, "Incorrect settings", "First binarize bmp file to show input histogram!");
    }
}

void BitmapBinarization::pieChartButtonClicked()
{
    if (isBinarized)
    {
        histogram = new Histogram(false, this);
        histogram->show();
    }
    else
    {
        QMessageBox::warning(this, "Incorrect settings", "First binarize bmp file to show output histogram!");
    }
    
}
