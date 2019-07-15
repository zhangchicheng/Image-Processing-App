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
#include <QShortcut>
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
#include "histogram.h"
#include "transactionthread.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() {}

private slots:
    void open();
    void del();
    void save();
    void process(Operation);
    void allTransactionDone();

public:
    QVBoxLayout *mainlayout;
    QHBoxLayout *sublayout;
    QVBoxLayout *histAndPanelLayout;

private:
    bool loadFile(const QString &);
    bool saveFile(const QString &);
    void setImage(const QImage &, bool);
    QString strippedName(const QString &);
    void openRecentFile();
    void updateRecentFile();
    void flipHorizontally();
    void flipVertically();
    void zoomIn();
    void zoomOut();
    void addTransaction(Transaction *);

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
    QImage oriImage, procImage;
    Histogram *hist;
    QStringList recentFiles;
    TransactionThread thread;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *toolsMenu;
    QMenu *blurMenu;
    QAction *openAct;
    QAction *flipHorizontallyAct;
    QAction *flipVerticallyAct;
    QAction *zoomOutAct;
    QAction *zoomInAct;
    QAction *delAct;
    QAction *saveAct;
    QAction *exitAct;
    enum {MaxRecentFiles = 5};
    QAction *recentFileAct[MaxRecentFiles];
    QAction *separatorAct;

    Operation op = opNone;
    QAction *threshold;
    QAction *equalizeHist;
    QAction *blur;
    QAction *GaussianBlur;
    QAction *medianBlur;
    QAction *bilateralFilter;
    QAction *erode;
    QAction *dilate;
    QAction *closePanel;
};

#endif // MAINWINDOW_H
