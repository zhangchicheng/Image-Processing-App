#ifndef BLURPANEL_H
#define BLURPANEL_H

#include <QComboBox>
#include <QSpinBox>
#include <QVBoxLayout>

#include "panel.h"

class BlurPanel : public Panel
{
    Q_OBJECT

public:
    BlurPanel();
    ~BlurPanel();

    virtual void setPanel();

    QString getFilter() { return f; }
    int getWidth() { return w; }
    int getHeight() { return h; }

private slots:
    void setFilter(const QString &f);
    void setWidth(int w);
    void setHeight(int h);

private:
    QComboBox *filter;
    QSpinBox *width;
    QSpinBox *height;

    QString f;
    int w;
    int h;
};

#endif // BLURPANEL_H
