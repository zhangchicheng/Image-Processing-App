#include "mainwindow.h"
#include "blurpanel.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *widget = new QWidget;
    widget->setStyleSheet("QWidget {background: 'white';}");
    setCentralWidget(widget);

    createActions();
    createMenu();
    createTool();
    createPanel();
    createView();

    QVBoxLayout *mainlayout = new QVBoxLayout;
    mainlayout->setMenuBar(menu);
    mainlayout->addWidget(tool);

    QHBoxLayout *sublayout = new QHBoxLayout;
    sublayout->addWidget(panel, 1);
    sublayout->addWidget(view, 3);

    mainlayout->addLayout(sublayout);
    widget->setLayout(mainlayout);
}

MainWindow::~MainWindow()
{

}

bool MainWindow::loadFile(const QString &fileName)
{
    QImageReader reader(fileName);
    reader.setAutoTransform(true);
    const QImage newImage = reader.read();
    if (newImage.isNull()) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1: %2")
                                 .arg(QDir::toNativeSeparators(fileName), reader.errorString()));
        return false;
    }

    setImage(newImage);

    setWindowFilePath(fileName);

    const QString message = tr("Opened \"%1\", %2x%3, Depth: %4")
        .arg(QDir::toNativeSeparators(fileName)).arg(originalImage.width()).arg(originalImage.height()).arg(originalImage.depth());
    statusBar()->showMessage(message);
    return true;
}

void MainWindow::setImage(const QImage &newImage)
{
    originalImage = newImage;
    scene.addPixmap(QPixmap::fromImage(originalImage));
}

static void initializeImageFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode)
{
    static bool firstDialog = true;

    if (firstDialog) {
        firstDialog = false;
        const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
        dialog.setDirectory(picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last());
    }

    QStringList mimeTypeFilters;
    const QByteArrayList supportedMimeTypes = acceptMode == QFileDialog::AcceptOpen
        ? QImageReader::supportedMimeTypes() : QImageWriter::supportedMimeTypes();
    foreach (const QByteArray &mimeTypeName, supportedMimeTypes)
        mimeTypeFilters.append(mimeTypeName);
    mimeTypeFilters.sort();
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("image/jpeg");
    if (acceptMode == QFileDialog::AcceptSave)
        dialog.setDefaultSuffix("jpg");
}

void MainWindow::open()
{
    QFileDialog dialog(this, tr("Open File"));
    initializeImageFileDialog(dialog, QFileDialog::AcceptOpen);
    while (dialog.exec() == QDialog::Accepted && !loadFile(dialog.selectedFiles().first())) {}
}

void MainWindow::del()
{

}

void MainWindow::save()
{

}

void MainWindow::setPanel(Operation op)
{
    switch(op)
    {
    case opEqualizeHist : qDebug("EqualizeHist"); break;
    case opBlur :
    {
        auto filter = new QComboBox;
        filter->setStyleSheet("background:white");
        filter->addItem("Gaussian Filter");
        filter->addItem("Median Filter");
        filter->addItem("Bilateral Filter");
        panel->add(filter, "Filter"); break;
    }
    }
}

void MainWindow::createActions()
{
    openAct = new QAction(tr("&Open..."));
    openAct->setIcon(QIcon(":/resources/icons/open.png"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);

    delAct = new QAction(tr("&Delete"));
    delAct->setIcon(QIcon(":/resources/icons/trash-can.png"));
    connect(delAct, &QAction::triggered, this, &MainWindow::del);

    saveAct = new QAction(tr("&Save"));
    saveAct->setIcon(QIcon(":/resources/icons/save.png"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::save);

    exitAct = new QAction(tr("&Exit"));
    exitAct->setIcon(QIcon(":/resources/icons/exit.png"));
    connect(exitAct, &QAction::triggered, this, &QWidget::close);

    equalizeHist = new QAction(tr("Euqalize Histogram"));
    connect(equalizeHist, &QAction::triggered, this, [=]() { setPanel(opEqualizeHist); });
    blur = new QAction(tr("Blur"));
    connect(blur, &QAction::triggered, this, [=]() { setPanel(opBlur); });
}

void MainWindow::createMenu()
{
    menu = new QMenuBar;

    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(exitAct);

    editMenu = new QMenu(tr("&Edit"), this);
    editMenu->addAction(delAct);

    toolsMenu = new QMenu(tr("&Tools"), this);
    toolsMenu->addAction(equalizeHist);
    toolsMenu->addAction(blur);

    menu->addMenu(fileMenu);
    menu->addMenu(editMenu);
    menu->addMenu(toolsMenu);
}

void MainWindow::createTool()
{
    tool = new QToolBar;

    tool->addAction(openAct);
    tool->addAction(saveAct);
    tool->addAction(delAct);
    tool->addAction(exitAct);
}

void MainWindow::createPanel()
{
    panel = new QFrame;
    //qDebug(static_cast<BlurPanel*>(panel)->filter->currentText().toLatin1());
    panel->setStyleSheet("background:#36393e");
    panel->setMinimumSize(200, 400);

    QVBoxLayout *layout = new QVBoxLayout;
    BlurPanel *p = new BlurPanel;
    layout->addWidget(p);
    panel->setLayout(layout);
}

void MainWindow::createView()
{
    view = new QGraphicsView;
    view->setMinimumSize(600, 400);
    view->setStyleSheet("background:gray");
    view->setScene(&scene);
}
