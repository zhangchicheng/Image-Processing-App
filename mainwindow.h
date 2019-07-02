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

#include <memory>

#include "panel.h"

enum Operation { opEqualizeHist, opBlur };

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
    void setPanel(Operation);

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
    QGraphicsView *view;
    QGraphicsScene scene;
    QImage originalImage, processedImage;

    std::unique_ptr<Panel> panel;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *toolsMenu;
    QAction *openAct;
    QAction *delAct;
    QAction *saveAct;
    QAction *exitAct;

    Operation op;
    QAction *equalizeHist;
    QAction *blur;
};

#endif // MAINWINDOW_H
