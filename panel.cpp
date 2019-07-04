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
            while ((item = this->layout()->takeAt(0)) != nullptr)
            {
                delete item->widget();
                delete item;
            }
            delete panelLayout;
            panelLayout = nullptr;
        }
        switch(op)
        {
        case opThreshold:
        {
            panelLayout = new QGridLayout;
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
            threshValueLabel->setBuddy(thresh);
            connect(thresh, QOverload<int>::of(&QSlider::valueChanged), [=]() { this->valueChanged(opThreshold); });

            panelLayout->addWidget(threshTypeLabel, 0, 0);
            panelLayout->addWidget(type, 0, 1);
            panelLayout->addWidget(threshValueLabel, 1, 0);
            panelLayout->addWidget(thresh, 1, 1);
            panelLayout->setAlignment(Qt::AlignTop);
            setLayout(panelLayout);
            break;
        }
        case opEqualizeHist :
        {
            break;
        }
        case opBlur :
        {
            panelLayout = new QGridLayout;

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

            panelLayout->addWidget(filterLabel, 0, 0);
            panelLayout->addWidget(filter, 0, 1);
            panelLayout->addWidget(widthLabel, 1, 0);
            panelLayout->addWidget(width, 1, 1);
            panelLayout->addWidget(heightLabel, 2, 0);
            panelLayout->addWidget(height, 2, 1);
            panelLayout->setAlignment(Qt::AlignTop);
            setLayout(panelLayout);
            break;
        }
        default: break;
        }
        this->op = op;
    }
}
