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

    histAndPanelLayout = new QVBoxLayout;

    sublayout->addLayout(histAndPanelLayout);
    mainlayout->addLayout(sublayout);
    widget->setLayout(mainlayout);

    statusBar()->showMessage(tr("Ready"));
    connect(&thread, SIGNAL(transactionStarted(const QString &)), statusBar(), SLOT(showMessage(const QString &)));
    connect(&thread, SIGNAL(allTransactionDone()), this, SLOT(allTransactionDone()));
}

void MainWindow::createActions()
{
    openAct = new QAction(tr("&Open..."));
    openAct->setIcon(QIcon(":/resources/icons/open.png"));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_O), this, SLOT(open()));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);

    flipHorizontallyAct = new QAction(tr("Flip &Horizontally"));
    flipHorizontallyAct->setIcon(QIcon(":/resources/icons/flip-horizontally.png"));
    connect(flipHorizontallyAct, &QAction::triggered, this, &MainWindow::flipHorizontally);

    flipVerticallyAct = new QAction(tr("Flip &Vertically"));
    flipVerticallyAct->setIcon(QIcon(":/resources/icons/flip-vertically.png"));
    connect(flipVerticallyAct, &QAction::triggered, this, &MainWindow::flipVertically);

    zoomOutAct = new QAction(tr("Zoom Out"));
    zoomOutAct->setIcon(QIcon(":/resources/icons/zoom-out.png"));
    connect(zoomOutAct, &QAction::triggered, this, &MainWindow::zoomOut);

    zoomInAct = new QAction(tr("Zoom In"));
    zoomInAct->setIcon(QIcon(":/resources/icons/zoom-in.png"));
    connect(zoomInAct, &QAction::triggered, this, &MainWindow::zoomIn);

    delAct = new QAction(tr("&Delete"));
    delAct->setIcon(QIcon(":/resources/icons/trash-can.png"));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_D), this, SLOT(del()));
    connect(delAct, &QAction::triggered, this, &MainWindow::del);

    saveAct = new QAction(tr("&Save"));
    saveAct->setIcon(QIcon(":/resources/icons/save.png"));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_S), this, SLOT(save()));
    connect(saveAct, &QAction::triggered, this, &MainWindow::save);

    exitAct = new QAction(tr("Exit"));
    exitAct->setIcon(QIcon(":/resources/icons/exit.png"));
    connect(exitAct, &QAction::triggered, this, &QWidget::close);

    for (int i = 0; i < MaxRecentFiles; ++i)
    {
        recentFileAct[i] = new QAction;
        recentFileAct[i]->setVisible(false);
        connect(recentFileAct[i], &QAction::triggered, [=]() { loadFile(recentFileAct[i]->data().toString()); });
    }

    equalizeHist = new QAction(tr("Euqalize Histogram"));
    connect(equalizeHist, &QAction::triggered, [=]() { panel->setPanel(opEqualizeHist); histAndPanelLayout->addWidget(panel); panel->show(); });

    threshold = new QAction(tr("Threshold"));
    connect(threshold, &QAction::triggered, [=]() { panel->setPanel(opThreshold); histAndPanelLayout->addWidget(panel); panel->show(); });

    blur = new QAction(tr("Blur"));
    connect(blur, &QAction::triggered, [=]() { panel->setPanel(opBlur); histAndPanelLayout->addWidget(panel); panel->show(); });

    GaussianBlur = new QAction(tr("Gaussian Blur"));
    connect(GaussianBlur, &QAction::triggered, [=]() { panel->setPanel(opGaussianBlur); histAndPanelLayout->addWidget(panel); panel->show(); });

    medianBlur = new QAction(tr("Meidan Blur"));
    connect(medianBlur, &QAction::triggered, [=]() { panel->setPanel(opMedianBlur); histAndPanelLayout->addWidget(panel); panel->show(); });

    bilateralFilter = new QAction(tr("Bilateral Filter"));
    connect(bilateralFilter, &QAction::triggered, [=]() { panel->setPanel(opBilateralFilter); histAndPanelLayout->addWidget(panel); panel->show(); });

    erode = new QAction(tr("Erode"));
    connect(erode, &QAction::triggered, [=]() { panel->setPanel(opErode); histAndPanelLayout->addWidget(panel); panel->show(); });

    dilate = new QAction(tr("Dilate"));
    connect(dilate, &QAction::triggered, [=]() { panel->setPanel(opDilate); histAndPanelLayout->addWidget(panel); panel->show(); });

    closePanel = new QAction(tr("Close Panel"));
    connect(closePanel, &QAction::triggered, [=]() { histAndPanelLayout->removeWidget(panel); panel->hide(); });
}

void MainWindow::createMenu()
{
    menu = new QMenuBar;

    fileMenu = new QMenu(tr("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    separatorAct = fileMenu->addSeparator();
    for (int i = 0; i < MaxRecentFiles; ++i)
        fileMenu->addAction(recentFileAct[i]);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = new QMenu(tr("&Edit"));
    editMenu->addAction(flipHorizontallyAct);
    editMenu->addAction(flipVerticallyAct);
    editMenu->addAction(zoomInAct);
    editMenu->addAction(zoomOutAct);
    editMenu->addAction(delAct);

    toolsMenu = new QMenu(tr("&Tools"));
    blurMenu = new QMenu(tr("Blur"));
    toolsMenu->addMenu(blurMenu);
    blurMenu->addAction(blur);
    blurMenu->addAction(GaussianBlur);
    blurMenu->addAction(medianBlur);
    blurMenu->addAction(bilateralFilter);
    toolsMenu->addAction(equalizeHist);
    toolsMenu->addAction(threshold);
    toolsMenu->addAction(erode);
    toolsMenu->addAction(dilate);

    toolsMenu->addSeparator();
    toolsMenu->addAction(closePanel);

    menu->addMenu(fileMenu);
    menu->addMenu(editMenu);
    menu->addMenu(toolsMenu);
}

void MainWindow::createTool()
{
    tool = new QToolBar;

    tool->addAction(openAct);
    tool->addAction(saveAct);
    tool->addSeparator();
    tool->addAction(zoomInAct);
    tool->addAction(zoomOutAct);
    tool->addSeparator();
    tool->addAction(delAct);
    tool->addAction(exitAct);
}

void MainWindow::createView()
{
    view = new QGraphicsView;
    view->setMinimumSize(400, 350);
    view->setScene(&scene);
    view->setBackgroundBrush(QBrush(QColor("#2c2e33")));
    view->setStyleSheet("border: 0px");
}

void MainWindow::createPanel()
{
    panel = new Panel;
    connect(panel, &Panel::valueChanged, [=](Operation op) { process(op); });
    panel->setFixedWidth(200);
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

    setImage(newImage, true);

    setWindowFilePath(fileName);

    recentFiles.removeAll(fileName);
    recentFiles.prepend(fileName);
    updateRecentFile();

    const QString message = tr("Opened \"%1\", %2x%3, Depth: %4")
            .arg(QDir::toNativeSeparators(fileName)).arg(oriImage.width()).arg(oriImage.height()).arg(oriImage.depth());
    statusBar()->showMessage(message);
    return true;
}

bool MainWindow::saveFile(const QString &fileName)
{
    QImageWriter writer(fileName);
    if (!procImage.isNull() && !writer.write(procImage)) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot write %1: %2")
                                 .arg(QDir::toNativeSeparators(fileName)), writer.errorString());
        return false;
    }
    const QString message = tr("Wrote \"%1\"").arg(QDir::toNativeSeparators(fileName));
    statusBar()->showMessage(message);
    return true;
}

void MainWindow::setImage(const QImage &newImage, bool init)
{
    scene.clear();
    if (init)
    {
        oriImage = newImage;
        procImage = QImage();
    }
    else
        procImage = newImage;
    scene.addPixmap(QPixmap::fromImage(newImage));
    view->setSceneRect(0, 0, newImage.width(), newImage.height());

    if (hist)
    {
        histAndPanelLayout->removeWidget(hist);
    }
    hist = new Histogram(newImage);
    histAndPanelLayout->addWidget(hist);
    histAndPanelLayout->addWidget(panel);
    hist->show();
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
    scene.clear();
    oriImage = QImage();
    procImage = QImage();
}

void MainWindow::save()
{
    QFileDialog dialog(this, tr("Save File As"));
    initializeImageFileDialog(dialog, QFileDialog::AcceptSave);

    while (dialog.exec() == QDialog::Accepted && !saveFile(dialog.selectedFiles().first())) {}
}

void MainWindow::updateRecentFile()
{
    QMutableStringListIterator i(recentFiles);

    while (i.hasNext())
    {
        if (!QFile::exists(i.next()))
            i.remove();
    }

    for (int j = 0; j < MaxRecentFiles; ++j)
    {
        if (j < recentFiles.count())
        {
            QString text = tr("&%1 %2").arg(j+1).arg(strippedName(recentFiles[j]));
            recentFileAct[j]->setText(text);
            recentFileAct[j]->setData(recentFiles[j]);
            recentFileAct[j]->setVisible(true);
        }
        else
        {
            recentFileAct[j]->setVisible(false);
        }
    }
    separatorAct->setVisible(!recentFiles.isEmpty());
}

void MainWindow::flipHorizontally()
{
    procImage = oriImage.mirrored(true, false);
    oriImage = procImage;
    setImage(procImage, false);
}

void MainWindow::flipVertically()
{
    procImage = oriImage.mirrored(false, true);
    oriImage = procImage;
    setImage(procImage, false);
}

void MainWindow::zoomOut()
{
    view->scale(0.8, 0.8);
}

void MainWindow::zoomIn()
{
    view->scale(1.2, 1.2);
}

void MainWindow::process(Operation op)
{

    if (oriImage.isNull())
    {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Image not found. Please open an exsiting image file."));
        return;
    }
    if (!procImage.isNull() && op != this->op)
    {
        oriImage = procImage;
    }
    switch(op)
    {
    case opThreshold:
    {
        auto *type = this->panel->findChild<QComboBox*>("ThreshType");
        auto *thresh = this->panel->findChild<QSlider*>("ThreshValue");
        auto *maxBinVal = this->panel->findChild<QSlider*>("MaxBinValue");
        addTransaction(new ThresholdTransaction(oriImage, thresh->value(), maxBinVal->value(), type->currentText()));
        break;
    }
    case opEqualizeHist:
    {
        addTransaction(new EqualizeHistTransaction(oriImage));
        break;
    }
    case opBlur:
    {
        auto *width = this->panel->findChild<QSpinBox*>("Width");
        auto *height = this->panel->findChild<QSpinBox*>("Height");
        addTransaction(new BlurTransaction(oriImage, width->value(), height->value()));
        break;
    }
    case opMedianBlur:
    {
        auto *size = this->panel->findChild<QSpinBox*>("Size");
        addTransaction(new MedianBlurTransaction(oriImage, size->value()));
        break;
    }
    case opGaussianBlur:
    {
        auto *width = this->panel->findChild<QSpinBox*>("Width");
        auto *height = this->panel->findChild<QSpinBox*>("Height");
        auto *sigmaX = this->panel->findChild<QDoubleSpinBox*>("SigmaX");
        auto *sigmaY = this->panel->findChild<QDoubleSpinBox*>("SigmaY");
        if (width->value() % 2 != 1 || height->value() % 2 != 1)
            QMessageBox::information(this, QGuiApplication::applicationDisplayName(), tr("Width and height must be odd."));
        else
            addTransaction(new GaussianBlurTransaction(oriImage, width->value(), height->value(), sigmaX->value(), sigmaY->value()));
        break;
    }
    case opBilateralFilter:
    {
        auto *diam = this->panel->findChild<QSpinBox*>("Diameter");
        auto *color = this->panel->findChild<QDoubleSpinBox*>("Color");
        auto *space = this->panel->findChild<QDoubleSpinBox*>("Space");
        addTransaction(new BilateralFilterTransaction(oriImage, diam->value(), color->value(), space->value()));
        break;
    }
    case opErode:
    {
        auto *kernel = this->panel->findChild<QComboBox*>("Kernel");
        auto *size = this->panel->findChild<QSpinBox*>("Size");
        addTransaction(new ErodeTransaction(oriImage, kernel->currentText(), size->value()));
        break;
    }
    case opDilate:
    {
        auto *kernel = this->panel->findChild<QComboBox*>("Kernel");
        auto *size = this->panel->findChild<QSpinBox*>("Size");
        addTransaction(new DilateTransaction(oriImage, kernel->currentText(), size->value()));
        break;
    }
    default:
        break;
    }
    this->op = op;
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void MainWindow::addTransaction(Transaction *transact)
{
    thread.addTransaction(transact);
    openAct->setEnabled(false);
    saveAct->setEnabled(false);
}

void MainWindow::allTransactionDone()
{
    openAct->setEnabled(true);
    saveAct->setEnabled(true);
    procImage = thread.image();
    setImage(procImage, false);
    statusBar()->showMessage(tr("Ready"));
}
