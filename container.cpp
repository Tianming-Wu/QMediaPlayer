#include "container.h"
#include "./ui_container.h"

Container::Container(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Container)
    , titleHeader("QMediaPlayer")
    , settings(QApplication::applicationDirPath() + "/QMediaPlayer.ini", QSettings::IniFormat)
{
    ui->setupUi(this);
    this->setWindowTitle(titleHeader);
    this->setAttribute(Qt::WA_AlwaysStackOnTop);
    this->setWindowFlag(Qt::WindowStaysOnTopHint);

    player = new QMediaPlayer(this);
    videoWidget = new QVideoWidget(this);
    audioOutput = new QAudioOutput(this);

    player->setVideoOutput(videoWidget);
    player->setAudioOutput(audioOutput);

    videoWidget->show();
    videoWidget->setGeometry(10,80,771,441);
    this->setGeometry(0,0,800,534);

    updateSettings();

    connect(player, &QMediaPlayer::durationChanged, this, [=](qint64 duration) { this->duration = duration; ref_t = static_cast<double>(duration) / 10000.0; });
    connect(player, &QMediaPlayer::positionChanged, this, &Container::updateProgress);

    // connect(ui->progressBar, &QSlider::sliderPressed, this, [=]() { progct->start(100); });
    // connect(ui->progressBar, &QSlider::sliderReleased, this, [=]() { progct->stop(); });
    // connect(progct, &QTimer::timeout, this, [=]() { player->setPosition(ui->progressBar->value()*progressCoe); });

    connect(player, &QMediaPlayer::errorOccurred, this, [=](QMediaPlayer::Error error, const QString &errorString) {
        QMessageBox::warning(this, "Error", errorString);
    });
    connect(player, &QMediaPlayer::mediaStatusChanged, this, [this](QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::LoadedMedia) {
            if(settings.value("Player/AutoBegin").toBool()) player->play();
            else player->pause();
        }
    });

    connect(ui->buttonPlay, &QPushButton::clicked, this, [=]() {
        if(player->playbackState() == QMediaPlayer::PlayingState) {
            player->pause();
        } else {
            player->play();
        }
    });

    connect(ui->buttonQuit, &QPushButton::toggled, this, [=](){ this->close(); });
}

Container::~Container()
{
    delete ui;
}

void Container::openVideo(const QUrl &videourl)
{
    source = videourl;
    player->setSource(source);
}

void Container::updateProgress(qint64 position)
{
    ui->progressBar->setValue(position/ref_t);
    ui->progressLabel->setText(formatDuration(position) + "/" + formatDuration(duration));
}

void Container::updateSettings()
{
    settings.beginGroup("Player");

    if(!settings.contains("PlaybackRate")) settings.setValue("PlaybackRate", qreal(1.0));
    player->setPlaybackRate(settings.value("PlaybackRate").toReal());

    if(!settings.contains("Volume")) settings.setValue("Volume", float(1.0));
    audioOutput->setVolume(settings.value("Volume").toFloat());

    if(!settings.contains("AutoBegin")) settings.setValue("AutoBegin", true);

    settings.endGroup();
}


QString Container::formatDuration(qint64 progress, bool usems) {
    const qint64 hours = progress / 3600000;
    qint64 remaining = progress % 3600000;

    const qint64 minutes = remaining / 60000;
    remaining %= 60000;

    const qint64 seconds = remaining / 1000;
    const qint64 milliseconds = remaining % 1000;

    QString timeString;

    if (hours > 0) {
        timeString = QString("%1:%2:%3")
            .arg(hours, 2, 10, QLatin1Char('0'))
            .arg(minutes, 2, 10, QLatin1Char('0'))
            .arg(seconds, 2, 10, QLatin1Char('0'));
    } else {
        timeString = QString("%1:%2")
            .arg(minutes, 2, 10, QLatin1Char('0'))
            .arg(seconds, 2, 10, QLatin1Char('0'));
    }

    if (usems) {
        timeString.append(QString(".%1").arg(milliseconds, 3, 10, QLatin1Char('0')));
    }

    return timeString;
}

void Container::run()
{
    // if(settings.value("AutoBegin").toBool() && player->mediaStatus() == QMediaPlayer::LoadedMedia) player->play();
    // videoWidget->setFullScreen(true);
    // videoWidget->showMaximized();
}
