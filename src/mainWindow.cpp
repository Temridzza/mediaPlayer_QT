#include "mainWindow.h"
#include "./ui_mainwindow.h"
#include "mainWindow.h"
#include <QFileDialog>
#include <QTime>
#include <QImage>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow())
{
    ui->setupUi(this);

    audioPlayer = new AudioPlayer(this);
    converter = new ConverterJSON();

    ui->volumeSlider->setValue(100);

    connect(audioPlayer, &AudioPlayer::playbackStarted, this, &MainWindow::onPlaybackStarted);
    connect(audioPlayer, &AudioPlayer::playbackFinished, this, &MainWindow::onPlaybackFinished);

    //volume slider
    connect(ui->volumeSlider, &QSlider::valueChanged, this, &MainWindow::updateVolume);


    //positions slider
    connect(audioPlayer, &AudioPlayer::positionChanged, this, &MainWindow::onPositionChanged);
    connect(ui->positions_slider, &QSlider::sliderMoved, this, &MainWindow::onPositionSliderValueChanged);
    connect(ui->positions_slider, &QSlider::sliderPressed, this, &MainWindow::SliderPressed);
    connect(ui->positions_slider, &QSlider::sliderReleased, this, &MainWindow::SliderReleased);
}

MainWindow::~MainWindow()
{
    if(ui->saveBox->isChecked() == true)
        converter->expFile(getName().toStdString(), nameAudio.toStdString(), elapsedTimeStr.toStdString());

    delete ui;
    delete audioPlayer;
    delete converter;
}

std::vector<QUrl> MainWindow::processFiles(const QString& directoryPath)
{
    QDirIterator it(directoryPath, QStringList() << "*.mp3",
                  QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot,
                  QDirIterator::Subdirectories);

   std::vector<QUrl> filePaths;
   while (it.hasNext()) {
       filePaths.push_back(QUrl::fromLocalFile(it.next()));
   }

   std::sort(filePaths.begin(), filePaths.end(),
             [](const QUrl& a, const QUrl& b) {
                 return a.toLocalFile().compare(b.toLocalFile(),
                                               Qt::CaseInsensitive) < 0;
             });
   return filePaths;
}

void MainWindow::openFolder_button()
{
    //selectedPath = "C:\\Users\\kupri\\OneDrive\\desktop\\test";
    selectedPath = QFileDialog::getExistingDirectory(nullptr, "Выбор папки");
    if (!selectedPath.isEmpty()) {
        audioPlayer->setMediaList(new std::vector<QUrl>(processFiles(selectedPath)));
        nameAudio = getName();
    }
}

void MainWindow::play_button() const {
    audioPlayer->play();
}

void MainWindow::pause_button() const {
    audioPlayer->pause();
}

void MainWindow::next_button() {
    audioPlayer->playNext();
    nameAudio = getName();
}

void MainWindow::back_button() {
    audioPlayer->playBack();
    nameAudio = getName();
}

void MainWindow::updateVolume(int value) const {
    int currentVolume = ui->volumeSlider->value();
    audioPlayer->setVolume(currentVolume);
}

void MainWindow::onPositionChanged(const qint64 position)
{
    const int duration = audioPlayer->getDuration();
    const int percentage = static_cast<int>(100.0 * position / duration);
    ui->positions_slider->setValue(percentage);

    const int elapsedTime = static_cast<int>(position / 1000);
    const QTime timeElapsed(QTime::fromMSecsSinceStartOfDay(elapsedTime * 1000));
    elapsedTimeStr = timeElapsed.toString("hh:mm:ss");

    const QString displayText = QString("Name: %1 \nTime: %2").arg(nameAudio).arg(elapsedTimeStr);
    ui->label->setText(displayText);
}

void MainWindow::onPositionSliderValueChanged(const int position) const {
    if (const int duration = audioPlayer->getDuration(); duration > 0)
    {
        audioPlayer->stop();
        const int seekPosition = static_cast<int>(duration * position / 100);
        audioPlayer->setPosition(seekPosition);
        audioPlayer->play();
    }
}

QString MainWindow::getName() const {
    QString str = audioPlayer->getFullName();
    for(int i = str.length()-1; i > 0; i--) {
        if(str[i] == '/' || str[i] == '\\') {
            QString result = "";
            for(int j = i+1; j < str.length(); j++)
                result += str[j];
            return result;
        }
    }
    return "";
}

void MainWindow::onPlaybackFinished() const {
    if(ui->saveBox->isChecked() == true)
        converter->expFile(getName().toStdString(), nameAudio.toStdString(), "finish");
}

