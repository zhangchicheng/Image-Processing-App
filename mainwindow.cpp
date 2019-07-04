#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *widget = new QWidget;
    widget->setStyleSheet("Panel {background: #36393e;}");
    setCentralWidget(widget);

    createActions();
    createMenu();
    createTool();
    createPanel();
    createView();

    mainlayout = new QVBoxLayout;
    mainlayout->setMargin(0);
    mainlayout->setMenuBar(menu);
    mainlayout->addWidget(tool);

    sublayout = new QHBoxLayout;
    sublayout->setSpacing(0);
    sublayout->addWidget(view);
    sublayout->addWidget(panel);

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
    if (newImage.isNull())
    {
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

    if (firstDialog)
    {
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

void MainWindow::process(Operation op)
{

    if (originalImage.isNull())
    {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Image not found. Please open an exsiting image file."));
        return;
    }
    switch(op)
    {
    case opEqualizeHist:
    {
        qDebug("equalizeHist"); break;
    }
    case opBlur:
    {
        QComboBox *filter = this->panel->findChild<QComboBox*>("Filter");
        qDebug()<<(filter->currentText());
        break;
    }
    default: break;
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
    connect(equalizeHist, &QAction::triggered, [=]() { panel->setPanel(opEqualizeHist); });
    threshold = new QAction(tr("Threshold"));
    connect(threshold, &QAction::triggered, [=]() { panel->setPanel(opThreshold); });
    blur = new QAction(tr("Blur"));
    connect(blur, &QAction::triggered, [=]() { panel->setPanel(opBlur); });
}

void MainWindow::createMenu()
{
    menu = new QMenuBar;

    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = new QMenu(tr("&Edit"), this);
    editMenu->addAction(delAct);

    toolsMenu = new QMenu(tr("&Tools"), this);
    toolsMenu->addAction(threshold);
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
    panel = new Panel;
    connect(panel, &Panel::valueChanged, [=](Operation op) { process(op); });
    panel->setFixedWidth(256);
}

void MainWindow::createView()
{
    view = new QGraphicsView;
    view->setMinimumSize(512, 512);
    view->setScene(&scene);
    view->setBackgroundBrush(QBrush(QColor("#2c2e33")));
    view->setStyleSheet("border: 0px");
}
