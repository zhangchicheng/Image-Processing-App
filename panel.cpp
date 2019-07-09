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

            auto title = new QLabel(tr("Threshold"));
            title->setStyleSheet("QLabel { font-weight: bold; color : white; }");

            QFrame* line = new QFrame;
            line->setFrameShape(QFrame::HLine);
            line->setStyleSheet(QString("color: #9fa1a3;"));

            auto threshTypeLabel = new QLabel(tr("Threshold Type"));
            threshTypeLabel->setStyleSheet("QLabel { color : #9fa1a3; }");
            auto type = new QComboBox;
            type->setObjectName("ThreshType");
            type->addItem("Binary");
            type->addItem("Binary Inverted");
            type->addItem("Truncate");
            type->addItem("To Zero");
            type->addItem("To Zero Inverted");
            threshTypeLabel->setBuddy(type);
            connect(type, QOverload<const QString &>::of(&QComboBox::currentTextChanged), [=]() { this->valueChanged(opThreshold); });

            auto threshValueLabel = new QLabel(tr("Threshold Value"));
            threshValueLabel->setStyleSheet("QLabel { color : #9fa1a3; }");
            auto thresh = new QSlider(Qt::Horizontal);
            thresh->setObjectName("ThreshValue");
            thresh->setRange(0,255);
            thresh->setValue(127);
            threshValueLabel->setBuddy(thresh);
            connect(thresh, QOverload<int>::of(&QSlider::valueChanged), [=]() { this->valueChanged(opThreshold); });

            auto maxBinValueLabel = new QLabel(tr("Max Binary Value"));
            maxBinValueLabel->setStyleSheet("QLabel { color : #9fa1a3; }");
            auto maxVal = new QSlider(Qt::Horizontal);
            maxVal->setObjectName("MaxBinValue");
            maxVal->setRange(0,255);
            maxVal->setValue(127);
            maxBinValueLabel->setBuddy(maxVal);
            connect(maxVal, QOverload<int>::of(&QSlider::valueChanged), [=]() { this->valueChanged(opThreshold); });

            auto widgetLayout = new QGridLayout;
            widgetLayout->addWidget(threshTypeLabel, 0, 0);
            widgetLayout->addWidget(type, 0, 1);
            widgetLayout->addWidget(threshValueLabel, 1, 0);
            widgetLayout->addWidget(thresh, 1, 1);
            widgetLayout->addWidget(maxBinValueLabel, 2, 0);
            widgetLayout->addWidget(maxVal, 2, 1);
            widgetLayout->setAlignment(Qt::AlignTop);
            panelLayout->addWidget(title);
            panelLayout->addWidget(line);
            panelLayout->addLayout(widgetLayout);
            panelLayout->setAlignment(Qt::AlignTop);
            panelLayout->setMargin(10);
            setLayout(panelLayout);
            break;
        }
        case opEqualizeHist :
        {
            break;
        }
        case opBlur :
        {
            panelLayout = new QVBoxLayout;

            auto title = new QLabel(tr("Blur"));
            title->setStyleSheet("QLabel { font-weight: bold; color : white; }");


            QFrame* line = new QFrame;
            line->setFrameShape(QFrame::HLine);
            line->setStyleSheet(QString("color: #9fa1a3;"));

            auto filterLabel = new QLabel(tr("Filter:"));
            filterLabel->setStyleSheet("QLabel { color : #9fa1a3; }");
            auto filter = new QComboBox;
            filter->setObjectName("Filter");
            filter->addItem("Normalized Box Filter");
            filter->addItem("Guassian Filter");
            filter->addItem("Median Filter");
            filter->addItem("Bilateral Filter");
            filterLabel->setBuddy(filter);
            connect(filter, QOverload<const QString &>::of(&QComboBox::currentTextChanged), [=]() { this->valueChanged(opBlur); });

            auto widthLabel = new QLabel(tr("Kernel Width:"));
            widthLabel->setStyleSheet("QLabel { color : #9fa1a3; }");
            auto width = new QSpinBox;
            width->setObjectName("Width");
            width->setMinimum(1);
            widthLabel->setBuddy(width);
            connect(width, QOverload<int>::of(&QSpinBox::valueChanged), [=]() { this->valueChanged(opBlur); });

            auto heightLabel = new QLabel(tr("Kernel Height:"));
            heightLabel->setStyleSheet("QLabel { color : #9fa1a3; }");
            auto height = new QSpinBox;
            height->setObjectName("Height");
            height->setMinimum(1);
            heightLabel->setBuddy(height);
            connect(height, QOverload<int>::of(&QSpinBox::valueChanged), [=]() { this->valueChanged(opBlur); });

            auto widgetLayout = new QGridLayout;
            widgetLayout->addWidget(filterLabel, 0, 0);
            widgetLayout->addWidget(filter, 0, 1);
            widgetLayout->addWidget(widthLabel, 1, 0);
            widgetLayout->addWidget(width, 1, 1);
            widgetLayout->addWidget(heightLabel, 2, 0);
            widgetLayout->addWidget(height, 2, 1);
            widgetLayout->setAlignment(Qt::AlignTop);
            panelLayout->addWidget(title);
            panelLayout->addWidget(line);
            panelLayout->addLayout(widgetLayout);
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
