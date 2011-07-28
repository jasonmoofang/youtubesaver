#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDir>
#include <KProcess>
#include <KLineEdit>
#include <KMessageBox>

#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent) : QWidget(parent) {
    fileList = new QListWidget();
    status = new QLabel();
    status->setWordWrap(true);
    fileName = new QLabel();
    connect(fileList, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(selectItem(QListWidgetItem*)));

    previewButton = new QPushButton("Preview this file");
    connect(previewButton, SIGNAL(clicked()), this, SLOT(doPreview()));
    refreshButton = new QPushButton("Refresh");
    connect(refreshButton, SIGNAL(clicked()), this, SLOT(refreshFileList()));

    QVBoxLayout *videoLayout = new QVBoxLayout();
    videoUrl = new KUrlRequester(this);
    videoUrl->setFilter(QString("*.flv"));
    videoUrl->setMode(KFile::File | KFile::LocalOnly);
    videoUrl->lineEdit()->setReadOnly(true);
    connect(videoUrl, SIGNAL(urlSelected(const KUrl&)), this, SLOT(appendFLV(const KUrl&)));
    exportVideo = new QPushButton("Save as flv (video and audio)");
    connect(exportVideo, SIGNAL(clicked()), this, SLOT(doExportVideo()));
    videoLayout->addWidget(new QLabel("Choose a location:"));
    videoLayout->addWidget(videoUrl);
    videoLayout->addWidget(exportVideo);
    QGroupBox *videoGroup = new QGroupBox("Save Video", this);
    videoGroup->setLayout(videoLayout);

    QVBoxLayout *mp3Layout = new QVBoxLayout();
    mp3Title = new KLineEdit(this);
    mp3Artist = new KLineEdit(this);
    mp3Album = new KLineEdit(this);
    mp3Url = new KUrlRequester(this);
    mp3Url->setFilter(QString("*.mp3"));
    mp3Url->setMode(KFile::File | KFile::LocalOnly);
    mp3Url->lineEdit()->setReadOnly(true);
    connect(mp3Url, SIGNAL(urlSelected(const KUrl&)), this, SLOT(appendMp3(const KUrl&)));
    exportmp3 = new QPushButton("Save as mp3 (audio only)");
    connect(exportmp3, SIGNAL(clicked()), this, SLOT(doExportMp3()));
    mp3Layout->addWidget(new QLabel("Enter audio title:"));
    mp3Layout->addWidget(mp3Title);
    mp3Layout->addWidget(new QLabel("Enter artist (optional):"));
    mp3Layout->addWidget(mp3Artist);
    mp3Layout->addWidget(new QLabel("Enter album (optional):"));
    mp3Layout->addWidget(mp3Album);
    mp3Layout->addWidget(new QLabel("Choose a location:"));
    mp3Layout->addWidget(mp3Url);
    mp3Layout->addWidget(exportmp3);
    QGroupBox *mp3Group = new QGroupBox("Save Audio", this);
    mp3Group->setLayout(mp3Layout);

    QHBoxLayout* outermost = new QHBoxLayout();
    QVBoxLayout* rightPanel = new QVBoxLayout();
    QVBoxLayout* leftPanel = new QVBoxLayout();
    leftPanel->addWidget(new QLabel("Youtube Video cache files found:"));
    leftPanel->addWidget(fileList);
    leftPanel->addWidget(refreshButton);
    rightPanel->addWidget(fileName);
    rightPanel->addWidget(previewButton);
    rightPanel->addWidget(videoGroup);
    rightPanel->addWidget(mp3Group);
    rightPanel->addWidget(status);
    outermost->addLayout(leftPanel);
    outermost->addLayout(rightPanel);
    setLayout(outermost);

    refreshFileList();
}

void MainWindow::doPreview() {
    if (fileNameString.isEmpty() || !QFile::exists(fileNameString)) {
        setStatusError("No file to preview! (None is selected, or maybe it has been deleted)");
        return;
    }
    QStringList previewCommand;
    previewCommand << "smplayer";
    previewCommand << fileNameString;
    KProcess::execute(previewCommand);
}

void MainWindow::refreshFileList() {
	QProcess p(this);
	p.start("showflashcache");
	p.waitForFinished();
    fileList->clear();
	QStringList l = QString(p.readAllStandardOutput()).trimmed().split(QRegExp("\\s+"));
    foreach (QString s, l) {
        fileList->insertItem(0, s);
    }
    if (fileList->count() > 0) {
        selectItem(fileList->item(0));
        setStatusInfo("");
    } else {
        setStatusInfo("No file was found. Load a Youtube video then hit refresh");
    }
}

void MainWindow::selectItem(QListWidgetItem* item) {
    clearFields();
    fileName->setText("Current File : " + item->text());
    fileNameString = item->text();
}

void MainWindow::doExportVideo() {
    if (fileNameString.isEmpty() || !QFile::exists(fileNameString)) {
        setStatusError("No file to export! (None is selected, or maybe it has been deleted)");
        return;
    } else if (!videoUrl->url().isValid()) {
        setStatusError("Invalid destination! Please select a destination");
        return;
    }
    setStatusInfo("");
    QStringList exportCommand;
    exportCommand << "cp";
    exportCommand << fileNameString;
    exportCommand << videoUrl->url().path().replace(" ", "");
    KProcess::execute(exportCommand);
    KMessageBox::information(this, "Done!");
    setStatusInfo("");
}

void MainWindow::appendFLV(const KUrl& url) {
    if (!url.path().endsWith(".flv")) {
        videoUrl->setText(url.path().replace(" ","") + ".flv");
    } else {
        videoUrl->setText(url.path().replace(" ",""));
    }
}

void MainWindow::appendMp3(const KUrl& url) {
    if (!url.path().endsWith(".mp3")) {
        mp3Url->setText(url.path().replace(" ","") + ".mp3");
    } else {
        mp3Url->setText(url.path().replace(" ",""));
    }
}

void MainWindow::doExportMp3() {
    if (mp3Title->text().isEmpty()) {
        setStatusError("Please enter a title for the audio!");
        return;
    } else if (fileNameString.isEmpty() || !QFile::exists(fileNameString)) {
        setStatusError("No file to export! (None is selected, or maybe it has been deleted)");
        return;
    } else if (!mp3Url->url().isValid()) {
        setStatusError("Invalid destination! Please select a destination");
        return;
    }
    exportMp3To(mp3Url->url().path());
}

void MainWindow::exportMp3To(const QString& path, const QString& failureMessage) {
    QStringList exportCommand;
    QString pathArg = path;
    pathArg.truncate(path.length()-4);
    exportCommand << "flv-to-mp3";
    exportCommand << fileNameString;
    exportCommand << pathArg;
    exportCommand << mp3Title->text();
    exportCommand << mp3Artist->text();
    exportCommand << mp3Album->text();
    KProcess::execute(exportCommand);
    KMessageBox::information(this, "Done!");
    setStatusInfo("");
}

void MainWindow::clearFields() {
    fileNameString = "";
    fileName->clear();
    videoUrl->clear();
    mp3Url->clear();
    mp3Title->clear();
    mp3Album->clear();
    mp3Artist->clear();
    setStatusInfo("");
}

void MainWindow::setStatusInfo(QString msg) {
    status->setText("<font color='#080'>" + msg + "</font>");
}

void MainWindow::setStatusError(QString msg) {
    status->setText("<font color='#f00'>" + msg + "</font>");
}