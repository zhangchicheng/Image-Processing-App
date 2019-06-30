#include "blurpanel.h"

BlurPanel::BlurPanel() : QToolBox (new QToolBox)
{
    filter = new QComboBox;
    filter->addItem("Gaussian Filter");
    filter->addItem("Median Filter");
    filter->addItem("Bilateral Filter");
    filter->show();

   // connect(filter, &QComboBox::currentIndexChanged, this, valueChanged());
}

void BlurPanel::valueChanged()
{
    //get combox value
    //connect(this, combox value, process func);
}

