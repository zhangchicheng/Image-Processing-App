#ifndef PANEL_H
#define PANEL_H

#include <QFrame>
#include <QLabel>
#include <QGroupBox>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QTextEdit>
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

public:
    Operation op = opNone;
    QLabel *title = nullptr;
    QVBoxLayout *panelLayout = nullptr;
};

#endif // PANEL_H
