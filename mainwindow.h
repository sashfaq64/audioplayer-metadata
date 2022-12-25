#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QMainWindow>
#include <QtMultimedia>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QMediaPlayer* player;
    QAudioOutput* output;



private slots:
    void setProgressBar();
    void updateProgressBar(qint64 change);
    void playpause();
    void setUpPlayback(QString audio);
    void on_actionOpen_triggered();
    void audioStatusChanged(QMediaPlayer::MediaStatus);
    void setImage(QMediaPlayer::PlaybackState state);
    void forward();
    void backward();

};
#endif // MAINWINDOW_H
