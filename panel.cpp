#include "panel.h"

Panel::Panel()
{

}

Panel::~Panel()
{

}

void Panel::setPanel(Operation op)
{
    if (op != this->op)
    {
        if (panelLayout)
        {
            QLayoutItem *item;
            while ((item = panelLayout->takeAt(0)) != nullptr)
            {
                if (item->widget())
                {
                    delete item->widget();
                    delete item;
                }
                else if (item->layout())
                {
                    QLayoutItem *subItem;
                    while ((subItem = item->layout()->takeAt(0)) != nullptr)
                    {
                        delete subItem->widget();
                        delete subItem;
                    }
                    delete item;
                }
            }
            delete panelLayout;
            panelLayout = nullptr;
        }
        switch(op)
        {
        case opThreshold:
        {
            panelLayout = new QVBoxLayout;

            title = new QLabel(tr("Threshold"));
            title->setStyleSheet("QLabel { font-weight: bold; color : white; }");

            QFrame* line = new QFrame;
            line->setFrameShape(QFrame::HLine);
            line->setStyleSheet(QString("color: #9fa1a3;"));

            auto threshType= new QGroupBox(tr("Type"));
            threshType->setStyleSheet("QGroupBox { color : #9fa1a3; }");
            auto type = new QComboBox;
            type->setObjectName("ThreshType");
            type->addItem("Binary");
            type->addItem("Binary Inverted");
            type->addItem("Truncate");
            type->addItem("To Zero");
            type->addItem("To Zero Inverted");
            connect(type, QOverload<const QString &>::of(&QComboBox::currentTextChanged), [=]() { this->valueChanged(opThreshold); });

            auto typeLayout = new QVBoxLayout;
            typeLayout->addWidget(type);
            threshType->setLayout(typeLayout);

            auto threshValue = new QGroupBox(tr("Value"));
            threshValue->setStyleSheet("QGroupBox { color : #9fa1a3; }");
            auto thresh = new QSlider(Qt::Horizontal);
            thresh->setObjectName("ThreshValue");
            thresh->setRange(0,255);
            thresh->setValue(127);
            connect(thresh, QOverload<int>::of(&QSlider::valueChanged), [=]() { this->valueChanged(opThreshold); });

            auto valueLayout = new QVBoxLayout;
            valueLayout->addWidget(thresh);
            threshValue->setLayout(valueLayout);

            auto maxBinValue = new QGroupBox(tr("Max Binary Value"));
            maxBinValue->setStyleSheet("QGroupBox { color : #9fa1a3; }");
            auto maxVal = new QSlider(Qt::Horizontal);
            maxVal->setObjectName("MaxBinValue");
            maxVal->setRange(0,255);
            maxVal->setValue(127);
            connect(maxVal, QOverload<int>::of(&QSlider::valueChanged), [=]() { this->valueChanged(opThreshold); });

            auto maxBinLayout = new QVBoxLayout;
            maxBinLayout->addWidget(maxVal);
            maxBinValue->setLayout(maxBinLayout);

            panelLayout->addWidget(title);
            panelLayout->addWidget(line);
            panelLayout->addWidget(threshType);
            panelLayout->addWidget(threshValue);
            panelLayout->addWidget(maxBinValue);
            panelLayout->setAlignment(Qt::AlignTop);
            panelLayout->setMargin(10);
            setLayout(panelLayout);
            break;
        }
        case opEqualizeHist :
        {
            panelLayout = new QVBoxLayout;

            title = new QLabel(tr("Equalize Histogram"));
            title->setStyleSheet("QLabel { font-weight: bold; color : white; }");

            QFrame* line = new QFrame;
            line->setFrameShape(QFrame::HLine);
            line->setStyleSheet(QString("color: #9fa1a3;"));

            auto histLabel = new QLabel();
            histLabel->setStyleSheet("QLabel { color : #9fa1a3; }");

            auto widgetLayout = new QGridLayout;
            widgetLayout->addWidget(histLabel, 0, 0);
            widgetLayout->setAlignment(Qt::AlignTop);
            panelLayout->addWidget(title);
            panelLayout->addWidget(line);
            panelLayout->addLayout(widgetLayout);
            panelLayout->setAlignment(Qt::AlignTop);
            panelLayout->setMargin(10);
            setLayout(panelLayout);
            emit valueChanged(opEqualizeHist);

            break;
        }
        case opBlur :
        {
            panelLayout = new QVBoxLayout;

            title = new QLabel(tr("Blur"));
            title->setStyleSheet("QLabel { font-weight: bold; color : white; }");


            QFrame* line = new QFrame;
            line->setFrameShape(QFrame::HLine);
            line->setStyleSheet(QString("color: #9fa1a3;"));

            auto kernel = new QGroupBox(tr("Kernel"));
            kernel->setStyleSheet("QGroupBox { color : #9fa1a3; }");
            auto widthLabel = new QLabel(tr("Width :"));
            widthLabel->setStyleSheet("QLabel { color : #9fa1a3; }");
            auto width = new QSpinBox;
            width->setObjectName("Width");
            width->setMinimum(1);
            connect(width, QOverload<int>::of(&QSpinBox::valueChanged), [=]() { this->valueChanged(opBlur); });

            auto heightLabel = new QLabel(tr("Height :"));
            heightLabel->setStyleSheet("QLabel { color : #9fa1a3; }");
            auto height = new QSpinBox;
            height->setObjectName("Height");
            height->setMinimum(1);
            connect(height, QOverload<int>::of(&QSpinBox::valueChanged), [=]() { this->valueChanged(opBlur); });

            auto kernelLayout = new QGridLayout;
            kernelLayout->addWidget(widthLabel, 0, 0);
            kernelLayout->addWidget(width, 0, 1);
            kernelLayout->addWidget(heightLabel, 1, 0);
            kernelLayout->addWidget(height, 1, 1);
            kernel->setLayout(kernelLayout);

            panelLayout->addWidget(title);
            panelLayout->addWidget(line);
            panelLayout->addWidget(kernel);
            panelLayout->setAlignment(Qt::AlignTop);
            panelLayout->setMargin(10);
            setLayout(panelLayout);
            break;
        }
        case opMedianBlur:
        {
            panelLayout = new QVBoxLayout;

            title = new QLabel(tr("Median Blur"));
            title->setStyleSheet("QLabel { font-weight: bold; color : white; }");


            QFrame* line = new QFrame;
            line->setFrameShape(QFrame::HLine);
            line->setStyleSheet(QString("color: #9fa1a3;"));

            auto kernel = new QGroupBox(tr("Kernel"));
            kernel->setStyleSheet("QGroupBox { color : #9fa1a3; }");
            auto sizeLabel = new QLabel(tr("Size :"));
            sizeLabel->setStyleSheet("QLabel { color : #9fa1a3; }");
            auto size = new QSpinBox;
            size->setObjectName("Size");
            size->setMinimum(1);
            size->setSingleStep(2);
            connect(size, QOverload<int>::of(&QSpinBox::valueChanged), [=]() { this->valueChanged(opMedianBlur); });

            auto kernelLayout = new QGridLayout;
            kernelLayout->addWidget(sizeLabel, 0, 0);
            kernelLayout->addWidget(size, 0, 1);
            kernel->setLayout(kernelLayout);

            panelLayout->addWidget(title);
            panelLayout->addWidget(line);
            panelLayout->addWidget(kernel);
            panelLayout->setAlignment(Qt::AlignTop);
            panelLayout->setMargin(10);
            setLayout(panelLayout);
            break;
        }
        case opGaussianBlur :
        {
            panelLayout = new QVBoxLayout;

            title = new QLabel(tr("Gaussian Blur"));
            title->setStyleSheet("QLabel { font-weight: bold; color : white; }");


            QFrame* line = new QFrame;
            line->setFrameShape(QFrame::HLine);
            line->setStyleSheet(QString("color: #9fa1a3;"));

            auto kernel = new QGroupBox(tr("Kernel"));
            kernel->setStyleSheet("QGroupBox { color : #9fa1a3; }");

            auto widthLabel = new QLabel(tr("Width :"));
            widthLabel->setStyleSheet("QLabel { color : #9fa1a3; }");
            auto width = new QSpinBox;
            width->setObjectName("Width");
            width->setMinimum(1);
            width->setSingleStep(2);
            widthLabel->setBuddy(width);
            connect(width, QOverload<int>::of(&QSpinBox::valueChanged), [=]() { this->valueChanged(opGaussianBlur); });

            auto heightLabel = new QLabel(tr("Height :"));
            heightLabel->setStyleSheet("QLabel { color : #9fa1a3; }");
            auto height = new QSpinBox;
            height->setObjectName("Height");
            height->setMinimum(1);
            height->setSingleStep(2);
            heightLabel->setBuddy(height);
            connect(height, QOverload<int>::of(&QSpinBox::valueChanged), [=]() { this->valueChanged(opGaussianBlur); });

            auto kernelLayout = new QGridLayout;
            kernelLayout->addWidget(widthLabel, 0, 0);
            kernelLayout->addWidget(width, 0, 1);
            kernelLayout->addWidget(heightLabel, 1, 0);
            kernelLayout->addWidget(height, 1, 1);
            kernel->setLayout(kernelLayout);

            auto sigma = new QGroupBox(tr("Sigma"));
            sigma->setStyleSheet("QGroupBox { color : #9fa1a3; }");
            auto sigmaXLabel = new QLabel(tr("X:"));
            sigmaXLabel->setStyleSheet("QLabel { color : #9fa1a3; }");
            auto sigmaX = new QDoubleSpinBox;
            sigmaX->setObjectName("SigmaX");
            sigmaX->setMinimum(0.00);
            sigmaXLabel->setBuddy(sigmaX);
            connect(sigmaX, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [=]() { this->valueChanged(opGaussianBlur); });

            auto sigmaYLabel = new QLabel(tr("Y :"));
            sigmaYLabel->setStyleSheet("QLabel { color : #9fa1a3; }");
            auto sigmaY = new QDoubleSpinBox;
            sigmaY->setObjectName("SigmaY");
            sigmaY->setMinimum(0.00);
            sigmaYLabel->setBuddy(sigmaY);
            connect(sigmaY, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [=]() { this->valueChanged(opGaussianBlur); });

            auto sigmaLayout = new QGridLayout;
            sigmaLayout->addWidget(sigmaXLabel, 0, 0);
            sigmaLayout->addWidget(sigmaX, 0, 1);
            sigmaLayout->addWidget(sigmaYLabel, 1, 0);
            sigmaLayout->addWidget(sigmaY, 1, 1);
            sigma->setLayout(sigmaLayout);

            panelLayout->addWidget(title);
            panelLayout->addWidget(line);
            panelLayout->addWidget(kernel);
            panelLayout->addWidget(sigma);
            panelLayout->setAlignment(Qt::AlignTop);
            panelLayout->setMargin(10);
            setLayout(panelLayout);
            break;
        }
        case opBilateralFilter:
        {
            panelLayout = new QVBoxLayout;

            title = new QLabel(tr("Bilateral Filter"));
            title->setStyleSheet("QLabel { font-weight: bold; color : white; }");


            QFrame* line = new QFrame;
            line->setFrameShape(QFrame::HLine);
            line->setStyleSheet(QString("color: #9fa1a3;"));

            auto d = new QGroupBox(tr("Diameter"));
            d->setStyleSheet("QGroupBox { color : #9fa1a3; }");

            auto diam = new QSpinBox;
            diam->setObjectName("Diameter");
            connect(diam, QOverload<int>::of(&QSpinBox::valueChanged), [=]() { this->valueChanged(opBilateralFilter); });

            auto diamLayout = new QVBoxLayout;
            diamLayout->addWidget(diam);
            d->setLayout(diamLayout);

            auto sigma = new QGroupBox(tr("Sigma"));
            sigma->setStyleSheet("QGroupBox { color : #9fa1a3; }");
            auto colorLabel = new QLabel(tr("Color :"));
            colorLabel->setStyleSheet("QLabel { color : #9fa1a3; }");
            auto color = new QDoubleSpinBox;
            color->setMaximum(199.99);
            color->setObjectName("Color");
            connect(color, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [=]() { this->valueChanged(opBilateralFilter); });

            auto spaceLabel = new  QLabel(tr("Space :"));
            spaceLabel->setStyleSheet("QLabel { color : #9fa1a3; }");
            auto space = new QDoubleSpinBox;
            space->setMaximum(199.99);
            space->setObjectName("Space");
            connect(space, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [=]() { this->valueChanged(opBilateralFilter); });

            auto sigmaLayout = new QGridLayout;
            sigmaLayout->addWidget(colorLabel, 0, 0);
            sigmaLayout->addWidget(color, 0, 1);
            sigmaLayout->addWidget(spaceLabel, 1, 0);
            sigmaLayout->addWidget(space, 1, 1);
            sigma->setLayout(sigmaLayout);

            panelLayout->addWidget(title);
            panelLayout->addWidget(line);
            panelLayout->addWidget(d);
            panelLayout->addWidget(sigma);
            panelLayout->setAlignment(Qt::AlignTop);
            panelLayout->setMargin(10);
            setLayout(panelLayout);
            break;
        }
        case opErode:
        case opDilate:
        {
            panelLayout = new QVBoxLayout;

            title = op==opErode ? new QLabel(tr("Erode")) : new QLabel(tr("Dilate"));
            title->setStyleSheet("QLabel { font-weight: bold; color : white; }");


            QFrame* line = new QFrame;
            line->setFrameShape(QFrame::HLine);
            line->setStyleSheet(QString("color: #9fa1a3;"));

            auto kernel= new QGroupBox(tr("Kernel"));
            kernel->setStyleSheet("QGroupBox { color : #9fa1a3; }");

            auto typeLabel = new QLabel(tr("Type: "));
            typeLabel->setStyleSheet("QLabel { color : #9fa1a3; }");
            auto type = new QComboBox;
            type->setObjectName("Kernel");
            type->addItem("Rectangular Box");
            type->addItem("Cross");
            type->addItem("Ellipse");
            connect(type, QOverload<const QString &>::of(&QComboBox::currentTextChanged), [=]() { op==opErode ? this->valueChanged(opErode) : this->valueChanged(opDilate); });

            auto sizeLabel = new QLabel(tr("Size :"));
            sizeLabel->setStyleSheet("QLabel { color : #9fa1a3; }");
            auto size = new QSpinBox;
            size->setObjectName("Size");
            size->setMinimum(0);
            connect(size, QOverload<int>::of(&QSpinBox::valueChanged), [=]() { op==opErode ? this->valueChanged(opErode) : this->valueChanged(opDilate); });

            auto kernelLayout = new QGridLayout;
            kernelLayout->addWidget(typeLabel, 0, 0);
            kernelLayout->addWidget(type, 0, 1);
            kernelLayout->addWidget(sizeLabel, 1, 0);
            kernelLayout->addWidget(size, 1, 1);
            kernel->setLayout(kernelLayout);

            panelLayout->addWidget(title);
            panelLayout->addWidget(line);
            panelLayout->addWidget(kernel);
            panelLayout->setAlignment(Qt::AlignTop);
            panelLayout->setMargin(10);
            setLayout(panelLayout);
            break;
        }
        default: break;
        }
        this->op = op;
    }
}
