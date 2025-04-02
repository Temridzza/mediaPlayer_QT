#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QMediaPlayer>
#include <QString>
#include <QUrl>
#include <QThread>
#include <QObject>
#include <QBuffer>

#include <QAudioOutput>
#include <QMediaMetaData>

class AudioPlayer : public QObject
{
    Q_OBJECT

public:
    explicit AudioPlayer(QObject *parent = nullptr);
    ~AudioPlayer() override;

    void setMediaList(std::vector<QUrl>* list);
    void setCurrentIndex(int index) { currentIndex = index; }
    void setVolume(const int& volume) const { m_player->setVolume(volume); }
    void setPosition(const int& position) const { m_player->setPosition(position); }

    bool hasNext() const { return currentIndex < mediaList->size(); }

    QString getNextFilePath() const;
    int getDuration() const { return m_player->duration(); }
    QString getFullName() const { return ((*mediaList)[currentIndex].toString()); }
    QVariant getMetaData() const { return m_player->metaData(QMediaMetaData::CoverArtImage); }

    void playNext();
    void playBack();
    void play() const { m_player->play(); }
    void stop() const { m_player->stop(); }
    void pause() const { m_player->pause(); }

    void blockSignals(const bool& flag) const { m_player->blockSignals(flag);  }

signals:
    void playbackStarted(int& currentIndex);
    void playbackFinished();
    void positionChanged(qint64 position);
protected:
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void onPositionChanged(qint64 position) { emit positionChanged(position); }

private:    
    std::vector<QUrl>* mediaList;
    int currentIndex = 0;
    QMediaPlayer* m_player;
    QAudioOutput* m_audioOutput;
    QIODevice* m_device;
    QBuffer* m_buffer;

    void volumeChanged();
    void typeConnection();
    void setupAudioOutput();
};

#endif // AUDIOPLAYER_H
