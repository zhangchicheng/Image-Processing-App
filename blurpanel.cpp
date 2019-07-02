#include "blurpanel.h"

BlurPanel::BlurPanel()
{
    setPanel();
}

BlurPanel::~BlurPanel()
{

}

void BlurPanel::setFilter(const QString &f)
{
    if (f != this->f)
    {
        this->f = f;
        emit valueChanged();
    }
}

void BlurPanel::setWidth(int w)
{
    if (w != this->w)
    {
        this->w = w;
        emit valueChanged();
    }
}

void BlurPanel::setHeight(int h)
{
    if (h != this->h)
    {
        this->h = h;
        emit valueChanged();
    }
}

void BlurPanel::setPanel()
{
    QVBoxLayout *layout = new QVBoxLayout;

    filter = new QComboBox;
    filter->addItem("Normalized Box Filter");
    filter->addItem("Guassian Filter");
    filter->addItem("Median Filter");
    filter->addItem("Bilateral Filter");
    connect(filter, QOverload<const QString &>::of(&QComboBox::activated),
            [=](const QString &f){ setFilter(f); });

    width = new QSpinBox;
    width->setMinimum(1);
    connect(width, QOverload<int>::of(&QSpinBox::valueChanged),
            [=](int w){ setWidth(w); });

    height = new QSpinBox;
    height->setMinimum(1);
    connect(height, QOverload<int>::of(&QSpinBox::valueChanged),
            [=](int h){ setHeight(h); });

    layout->addWidget(filter, 0);
    layout->addWidget(width, 1);
    layout->addWidget(height, 2);
    setLayout(layout);
}
