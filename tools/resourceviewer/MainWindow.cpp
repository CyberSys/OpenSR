#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QSettings>
#include <QDebug>
#include <QMessageBox>
#include <fstream>
#include <libRanger.h>
#include <squish.h>

using namespace Rangers;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(&scene);
    ui->graphicsView->setInteractive(true);

    scene.setBackgroundBrush(QBrush(Qt::black));
    scene.addItem(&item);

    ui->fileTreeView->setModel(&model);

    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(&animationTimer, SIGNAL(timeout()), this, SLOT(nextFrame()));
    connect(ui->framerateSpinBox, SIGNAL(valueChanged(double)), this, SLOT(framerateChanged(double)));
    connect(ui->durationSpinBox, SIGNAL(valueChanged(double)), this, SLOT(durationChanged(double)));
    connect(ui->fileTreeView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(treeDoubleClicked(const QModelIndex&)));

    QSettings settings("OpenSR", "ResourceViewer");
    restoreGeometry(settings.value("mainWindow/geometry").toByteArray());
    restoreState(settings.value("mainWindow/state").toByteArray());
    ui->vertSplitter->restoreGeometry(settings.value("mainWindow/vertSplitterGeometry").toByteArray());
    ui->vertSplitter->restoreState(settings.value("mainWindow/vertSplitterState").toByteArray());
    ui->horizSplitter->restoreGeometry(settings.value("mainWindow/horizSplitterGeometry").toByteArray());
    ui->horizSplitter->restoreState(settings.value("mainWindow/horizSplitterState").toByteArray());

    if (qApp->arguments().count() > 1)
        loadFile(qApp->arguments().at(1));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void extractPKG(Rangers::PKGItem *root, QString path, std::ifstream& f)
{
    if (!root)
        root = Rangers::loadPKG(f);

    if (root->dataType == 3)
    {
        if (qstrlen(root->name) > 0)
        {
            path += QString("/") + QString::fromAscii(root->name);
            QDir().mkpath(path);
        }

        for (int i = 0; i < root->childCount; i++)
            extractPKG(&root->childs[i], path, f);
    }
    else
    {
        QFile out(path + "/" + QString::fromAscii(root->name));
        qDebug("Extracting file %s ...", (path + "/" + QString::fromAscii(root->name)).toLocal8Bit().data());
        out.open(QIODevice::WriteOnly);
        char *data = (char*)Rangers::extractFile(*root, f);
        out.write(data, root->size);
        out.close();
        delete data;
    }
}

void MainWindow::nextFrame()
{
    currentFrame = (currentFrame + 1) % frames.count();
    item.setPixmap(frames.at(currentFrame));
    ui->frameLabel->setText(QString::number(currentFrame) + "/" + QString::number(frames.count()));
}

void MainWindow::loadFile(const QString& fileName)
{
    QFileInfo fileInfo(fileName);
    if (!fileInfo.exists())
        return;

    if (fileInfo.suffix().toLower() == "pkg")
    {
        model.addPKG(fileInfo);
    }
    else
    {
        loadResource(model.addFile(fileInfo));
    }
}

void MainWindow::loadHAI(HAIAnimation anim)
{
    for (int i = 0; i < anim.frameCount; i++)
    {
        frames.append(QPixmap::fromImage(QImage(anim.frames + i * anim.width * anim.height * 4, anim.width, anim.height, QImage::Format_ARGB32)));
    }
    delete anim.frames;
    currentFrame = 0;
    item.setPixmap(frames.at(currentFrame));
    animationTimer.setInterval(1000 / 15);
    animationTimer.setSingleShot(false);
    animationTimer.start();

    ui->seekLabel->setText("");
    ui->waitLabel->setText("");
    ui->framerateSpinBox->setValue(15);
    ui->durationSpinBox->setValue(1000.0 / 15);
    ui->paramGroupBox->setEnabled(true);

    scene.setSceneRect(0, 0, anim.width, anim.height);
    scene.invalidate();
}

void MainWindow::loadGI(GIFrame frame)
{
    item.setPixmap(QPixmap::fromImage(QImage(frame.data, frame.width, frame.height, QImage::Format_ARGB32)));
    delete frame.data;
    scene.setSceneRect(0, 0, frame.width, frame.height);
    scene.invalidate();
}

void MainWindow::loadGAI(GAIAnimation anim)
{
    for (int i = 0; i < anim.frameCount; i++)
    {
        frames.append(QPixmap::fromImage(QImage(anim.frames[i].data, anim.frames[i].width, anim.frames[i].height, QImage::Format_ARGB32)));
        delete[] anim.frames[i].data;
    }
    delete[] anim.frames;
    currentFrame = 0;
    item.setPixmap(frames.at(currentFrame));
    animationTimer.setInterval(1000 / 15);
    animationTimer.setSingleShot(false);
    animationTimer.start();
    scene.setSceneRect(0, 0, anim.width, anim.height);
    scene.invalidate();

    ui->seekLabel->setText(QString::number(anim.waitSeek));
    ui->waitLabel->setText(QString::number(anim.waitSize));
    ui->framerateSpinBox->setValue(15);
    ui->durationSpinBox->setValue(1000.0 / 15);
    ui->paramGroupBox->setEnabled(true);
}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open image file"), QString(),
                       tr("All suported files (*.gai *.hai *.gi *.pkg *.dds)"));
    if (fileName.isNull())
        return;

    loadFile(fileName);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings("OpenSR", "ResourceViewer");
    settings.setValue("mainWindow/geometry", saveGeometry());
    settings.setValue("mainWindow/state", saveState());
    settings.setValue("mainWindow/vertSplitterGeometry", ui->vertSplitter->saveGeometry());
    settings.setValue("mainWindow/vertSplitterState", ui->vertSplitter->saveState());
    settings.setValue("mainWindow/horizSplitterGeometry", ui->horizSplitter->saveGeometry());
    settings.setValue("mainWindow/horizSplitterState", ui->horizSplitter->saveState());
    QMainWindow::closeEvent(event);
}

void MainWindow::framerateChanged(double value)
{
    animationTimer.setInterval(int(1000 / value));
    ui->durationSpinBox->setValue(1000.0 / value);
}

void MainWindow::durationChanged(double value)
{
    animationTimer.setInterval(int(value));
    ui->framerateSpinBox->setValue(1000.0 / value);
}

void MainWindow::loadResource(FileNode *node)
{
    ui->paramGroupBox->setEnabled(false);
    if (!frames.isEmpty())
    {
        frames.clear();
        animationTimer.stop();
    }

    QString fileName = node->name;
    QFileInfo fileInfo(fileName);
    if (fileInfo.suffix() == "gi")
    {
        Rangers::GIFrame frame = Rangers::loadGIFile(model.getData(node).data());
        loadGI(frame);
    }
    else if (fileInfo.suffix().toLower() == "gai")
    {
        Rangers::GAIHeader h = Rangers::loadGAIHeader(model.getData(node).data());
        Rangers::GIFrame *bg = 0;
        if (h.haveBackground)
        {
            FileNode *bgNode = model.getSiblingNode(node, fileInfo.completeBaseName() + ".gi");
            if (!bgNode)
                bgNode = model.getSiblingNode(node, fileInfo.completeBaseName() + ".GI");

            if (!bgNode)
                qCritical() << "Could not found background frame!";
            else
            {
                bg = new Rangers::GIFrame;
                *bg = Rangers::loadGIFile(model.getData(bgNode).data());
            }
        }
        Rangers::GAIAnimation anim = Rangers::loadGAIAnimation(model.getData(node).data(), bg);
        loadGAI(anim);
        /*if (bg)
            delete bg->data;
        delete bg;*/
    }
    else if (fileInfo.suffix().toLower() == "hai")
    {
        Rangers::HAIAnimation anim = Rangers::loadHAI(model.getData(node).data());
        loadHAI(anim);
    }
    else if (fileInfo.suffix().toLower() == "dds")
    {
        loadDDS(model.getData(node).data());
    }
}

void convertRGBAToBGRA(unsigned char *rgba, int width, int height)
{
    for (int i = 0; i < width * height; i++)
    {
        unsigned char tmp = rgba[i * 4];
        rgba[i * 4] = rgba[i * 4 + 2];
        rgba[i * 4 + 2] = tmp;
    }
}

void MainWindow::loadDDS(const char *data)
{
    if (*(uint32_t*)(data) != 0x20534444)
    {
        QMessageBox::warning(this, tr("Unsupported file"), tr("Not a DDS file."));
        return;
    }

    DDSHeader header = *((DDSHeader*)(data + 4));

    switch (header.ddspf.fourCC)
    {
    case 0x31545844:
    case 0x33545844:
    case 0x35545844:
        break;
    default:
        QMessageBox::warning(this, tr("Unsupported file"), tr("Unsupported DDS compression."));
        return;

    }

    if ((header.caps & DDSCAPS_COMPLEX) && (header.caps2 & DDSCAPS2_VOLUME)
            && (header.flags & DDSD_LINEARSIZE) && (header.flags & DDSD_LINEARSIZE) && (header.ddspf.flags & DDPF_FOURCC))
    {
        unsigned char *rgbaData = new unsigned char[header.height * header.width * 4];
        for (int i = 0; i < header.depth; i++)
        {
            unsigned char *dxt = ((unsigned char *)data) + 4 + sizeof(DDSHeader) + header.pitchOrLinearSize * header.height * i;
            int squishFlags;

            switch (header.ddspf.fourCC)
            {
            case 0x31545844:
                squishFlags = squish::kDxt1;
                break;
            case 0x33545844:
                squishFlags = squish::kDxt3;
                break;
            case 0x35545844:
                squishFlags = squish::kDxt5;
                break;
            }
            squish::DecompressImage(rgbaData, header.width, header.height, dxt, squishFlags);
            convertRGBAToBGRA(rgbaData, header.width, header.height);
            frames.append(QPixmap::fromImage(QImage(rgbaData, header.width, header.height, QImage::Format_ARGB32)));
        }
        delete rgbaData;
        currentFrame = 0;
        item.setPixmap(frames.at(currentFrame));
        animationTimer.setInterval(1000 / 15);
        animationTimer.setSingleShot(false);
        animationTimer.start();
        scene.setSceneRect(0, 0, header.width, header.height);
        scene.invalidate();

        ui->seekLabel->setText(QString::number(header.reserved1[0]));
        ui->waitLabel->setText(QString::number(header.reserved1[1]));
        ui->framerateSpinBox->setValue(15);
        ui->durationSpinBox->setValue(1000.0 / 15);
        ui->paramGroupBox->setEnabled(true);
    }
    else if (header.ddspf.flags & DDPF_FOURCC)
    {
        unsigned char *rgbaData = new unsigned char[header.height * header.width * 4];
        unsigned char *dxt = ((unsigned char *)data) + 4 + sizeof(DDSHeader);
        int squishFlags;

        switch (header.ddspf.fourCC)
        {
        case 0x31545844:
            squishFlags = squish::kDxt1;
            break;
        case 0x33545844:
            squishFlags = squish::kDxt3;
            break;
        case 0x35545844:
            squishFlags = squish::kDxt5;
            break;
        }
        squish::DecompressImage(rgbaData, header.width, header.height, dxt, squishFlags);
        convertRGBAToBGRA(rgbaData, header.width, header.height);
        item.setPixmap(QPixmap::fromImage(QImage(rgbaData, header.width, header.height, QImage::Format_ARGB32)));
        delete rgbaData;
        scene.setSceneRect(0, 0, header.width, header.height);
        scene.invalidate();
    }
    else
    {
        QMessageBox::warning(this, tr("Unsupported file"), tr("Unsupported DDS format."));
        return;
    }
}

void MainWindow::treeDoubleClicked(const QModelIndex& index)
{
    FileNode *node = static_cast<FileNode *>(index.internalPointer());
    loadResource(node);
}
