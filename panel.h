#ifndef PANEL_H
#define PANEL_H

#include <QFrame>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QVBoxLayout>

#include "operation.h"

class Panel : public QFrame
{
    Q_OBJECT

public:
    Panel();
    ~Panel();

    void setPanel(Operation op);

signals:
    void valueChanged(Operation op);

private:
    Operation op = opNone;
    QVBoxLayout *panelLayout = nullptr;
};

#endif // PANEL_H
