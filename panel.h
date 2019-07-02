#ifndef PANEL_H
#define PANEL_H

#include <QFrame>

class Panel : public QFrame
{
    Q_OBJECT

public:
    Panel();
    virtual ~Panel();

    virtual void setPanel();

signals:
    void valueChanged();
};

#endif // PANEL_H
