#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "audioPlayer.h"
#include "converterJSON.h"

#include <QMainWindow>

#include <QMediaPlayer>
#include <QMediaContent>
#include <QString>
#include <QDirIterator>
#include <QUrl>
#include <QFile>
#include <QMediaContent>
#include <QImage>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onPositionChanged(qint64 position);
    static void onPlaybackStarted(int& index) { qDebug() << "Аудио запущено"; }
    void onPlaybackFinished() const;

    //volume slider
    void updateVolume(int value) const;

    //positions slider
    void onPositionSliderValueChanged(int position) const;
    void SliderPressed() const { audioPlayer->blockSignals(true); }
    void SliderReleased() const { audioPlayer->blockSignals(false); }

    //button
    void openFolder_button();
    void play_button() const;
    void pause_button() const;
    void next_button();
    void back_button();

private:
    Ui::MainWindow *ui;
    AudioPlayer* audioPlayer;
    ConverterJSON* converter;

    static std::vector<QUrl> processFiles(const QString& directoryPath);
    QString getName() const;
    bool extractAlbumCover();
    void setBackgroundImage();

    QString selectedPath;
    QString nameAudio;
    QString elapsedTimeStr;
};
#endif // MAINWINDOW_H
