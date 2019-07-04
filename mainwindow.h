#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGridLayout>
#include <QFileDialog>
#include <QToolBar>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBox>
#include <QMenu>
#include <QAction>
#include <QStandardPaths>
#include <QImage>
#include <QImageReader>
#include <QImageWriter>
#include <QSignalMapper>
#include <QGuiApplication>
#include <QMessageBox>
#include <QDebug>

#include "operation.h"
#include "panel.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void open();
    void del();
    void save();
    void process(Operation);

private:
    bool loadFile(const QString &);
    void setImage(const QImage &);

    void createActions();
    void createMenu();
    void createTool();
    void createPanel();
    void createView();

    QMenuBar *menu;
    QToolBar *tool;
    Panel *panel;
    QGraphicsView *view;
    QGraphicsScene scene;
    QImage originalImage, processedImage;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *toolsMenu;
    QAction *openAct;
    QAction *delAct;
    QAction *saveAct;
    QAction *exitAct;

    QVBoxLayout *mainlayout;
    QHBoxLayout *sublayout;
    Operation op = opNone;
    QAction *threshold;
    QAction *equalizeHist;
    QAction *blur;
};

#endif // MAINWINDOW_H
