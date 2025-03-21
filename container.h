#ifndef CONTAINER_H
#define CONTAINER_H

#include <QMainWindow>
#include <QMenu>
#include <QSettings>
#include <QScreen>
#include <QTimer>
#include <QPropertyAnimation>
#include <QMessageBox>

#include <QFile>
#include <QFileDialog>

#include <QMediaPlayer>
#include <QVideoWidget>
#include <QAudioOutput>
// #include <QVideoSink>
#include <QMediaMetaData>
// #include <QGraphicsVideoItem>

#include "main.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Container;
}
QT_END_NAMESPACE

class Container : public QMainWindow
{
    Q_OBJECT

public:
    Container(QWidget *parent = nullptr);
    ~Container();

    QString formatDuration(qint64 progress, bool usems = false);
    void run();

public slots:
    void openVideo(const QUrl &videourl);
    void updateProgress(qint64 position);
    void updateSettings();

private:
    Ui::Container *ui;
    const QString titleHeader;
    QSettings settings;

    QMediaPlayer *player;
    QVideoWidget *videoWidget;
    QAudioOutput *audioOutput;

    qint64 duration;
    double ref_t;
    QUrl source;
};
#endif // CONTAINER_H
