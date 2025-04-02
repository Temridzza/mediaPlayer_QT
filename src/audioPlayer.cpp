#include "audioPlayer.h"
#include <QDirIterator>

#include <QMediaPlayer>
#include <QAudioOutput>
#include <QAudioFormat>

AudioPlayer::AudioPlayer(QObject *parent) : QObject(parent), m_player(nullptr)
{
    m_player = new QMediaPlayer(this);

    connect(m_player, &QMediaPlayer::mediaStatusChanged, this, &AudioPlayer::onMediaStatusChanged);
    connect(m_player, &QMediaPlayer::positionChanged, this, &AudioPlayer::onPositionChanged);

    setupAudioOutput();
}

void AudioPlayer::setupAudioOutput()
{
    QAudioFormat format;
    format.setSampleRate(44100);
    format.setChannelCount(2);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleSize(16);

    m_audioOutput = new QAudioOutput(format, this);
    m_audioOutput->setVolume(100);

    m_player->setProperty("bufferSize", 8192);
    m_player->setProperty("audioOutput", QVariant::fromValue(m_audioOutput));
}

AudioPlayer::~AudioPlayer()
{
    delete m_player;
    delete m_audioOutput;
    delete mediaList;
}

void AudioPlayer::setMediaList(std::vector<QUrl>* list)
{
    mediaList = list;
    currentIndex = 0;
    m_player->setMedia((*mediaList)[currentIndex]);
    m_player->play();
}

void AudioPlayer::playNext()
{
    qDebug() << "The following audio is running";

    m_player->stop();
    if (!mediaList->empty() && (currentIndex+1) < mediaList->size()) {
        currentIndex++;

        m_player->setMedia((*mediaList)[currentIndex]);

        connect(m_player, &QMediaPlayer::mediaStatusChanged,
                [this](QMediaPlayer::MediaStatus status) {
                    if (status == QMediaPlayer::LoadedMedia) {
                        m_player->play();
                        emit playbackStarted(currentIndex);
                        disconnect(this, nullptr, m_player, nullptr); // Отключаем сигнал после успешной загрузки
                    }
                });
    } else {
        emit playbackFinished();
    }
}

void AudioPlayer::playBack(){
    qDebug() << "Запущено предыдущее аудио";
    m_player->stop();
    if (!mediaList->empty() && (currentIndex-1) >= 0) {
        currentIndex--;
        m_player->setMedia((*mediaList)[currentIndex]);
        m_player->play();
        emit playbackStarted(currentIndex);
    } else {
        emit playbackFinished();
    }
}

void AudioPlayer::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    switch (status) {
    case QMediaPlayer::EndOfMedia:
        qDebug() << "Playback has ended";
        emit playbackFinished();
        playNext();
        break;
    default:
        ;
    }
}

