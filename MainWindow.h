#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QGroupBox>
#include <KUrlRequester>
#include <KLineEdit>

class MainWindow : public QWidget {
    Q_OBJECT;

    QListWidget* fileList;
    QLabel* fileName;
    QLabel* status;
    QString fileNameString;
    QPushButton* previewButton;
    QPushButton* refreshButton;
    QPushButton* exportVideo;
    QPushButton* exportmp3;
    KUrlRequester* videoUrl;
    KUrlRequester* mp3Url;
    KLineEdit* mp3Title;
    KLineEdit* mp3Artist;
    KLineEdit* mp3Album;
    void exportMp3To(const QString& path, const QString& failureMessage = "");
    void clearFields();

public:
    MainWindow(QWidget* parent = 0);
    void setStatusInfo(QString msg);
    void setStatusError(QString msg);

public slots:
    void doPreview();
    void refreshFileList();
    void selectItem(QListWidgetItem* item);
    void appendFLV(const KUrl&);
    void appendMp3(const KUrl&);
    void doExportVideo();
    void doExportMp3();
};

#endif // MAINWINDOW_H