#ifndef BLURPANEL_H
#define BLURPANEL_H

#include <QToolBox>
#include <QComboBox>

class BlurPanel : public QToolBox
{
public:
    BlurPanel();

private:
    void valueChanged();

public:
    QComboBox* filter;
};

#endif // BLURPANEL_H
