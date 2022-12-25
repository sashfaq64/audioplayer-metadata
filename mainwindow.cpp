#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QtMultimedia>
#include <string>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>


bool init = false;

/*
MainWindow:

    1) setup blank GUI
    2) connect buttons on GUI to respective functions

*/

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    // setup blank GUI

    setImage(QMediaPlayer::PausedState);
    ui->progressBar->setValue(0);
    ui->progressBar->setMinimum(0);


    // connect button to playback functions
    connect(ui->img, SIGNAL(released()),this,SLOT(playpause()));
    connect(ui->backward, SIGNAL(released()),this,SLOT(backward()));
    connect(ui->forward, SIGNAL(released()),this,SLOT(forward()));



}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
setProgressBar:

    1) connect the playback position of the song to the progressbar on the GUI
*/

void MainWindow::setProgressBar(){

    //connect the playback position of the song to the progressbar on the GUI
    connect(player,SIGNAL(positionChanged(qint64)), this, SLOT(updateProgressBar(qint64)));


}

/*
updateProgressBar:

    1) obtain the playback position of song and set the progressbar

*/

void MainWindow::updateProgressBar(qint64 change){


    ui->progressBar->setValue(change);

}

/*
playpause:

    1) boolean init blocks this function if a song has not been selected
    2) set the progressbar length to the length of the song
    3) depending on the playback state of the song (pause/play/stopped), the
       play/pause button will change the state of the playback
*/
void MainWindow::playpause(){

    if(init == true){


        ui->progressBar->setMaximum(player->duration());

        //if playbackState() is 1, then the song is playing
        if(player->playbackState() == 1)
            //switch song to pause on button click
            player->pause();

        else
            //switch song to play on button click
            player->play();
    }

}

/*
on_actionOpen_triggered:

    - function is triggered when user clicks on "Open" in file menu.
    - song is selcted and can be mp3, m4a, or .wav

    1) Prompt user with QFileDialog to select file
    2) Once file is selected, send filepath to another function to
       complete setup
    3) If file is not selected, return with error message and
       do nothing

*/

void MainWindow::on_actionOpen_triggered()
{

    //filepath of song is stored here
    QString fileName =
            //file can only be one of 3 file types: mp3, m4a, or .wav
            QFileDialog::getOpenFileName(this,
                                         "Open Audio File",
                                         ".",
                                         "Audio Files (*.mp3 *.m4a *.wav)");

    //if fileName is not empty
    if(fileName.length() != 0){

        //send to this function to complete setup
        setUpPlayback(fileName);

    }
    //no file selected
    else
        QMessageBox::warning(this,"error","No file selected");



}

/*
setUpPlayback:

    -Once given filename (audio), setup playback
    1) If a song has been previously selected, clear it
    2) Set up media player and output
    3) If song ends,restart song


*/

void MainWindow::setUpPlayback(QString audio){


    if(init == true){

        setImage(QMediaPlayer::PausedState);
        player->stop();


    }

    //setup qmediaplayer and output device

    player = new QMediaPlayer();
    output = new QAudioOutput();

    init = true;

    player->setAudioOutput(output);

    output->setVolume(50);

    //feed file to player
    player->setSource(QUrl::fromLocalFile(audio));


    //set icon to pause
    setImage(QMediaPlayer::PausedState);


    //sync progressbar to media playback
    setProgressBar();
    ui->progressBar->setValue(0);
    ui->progressBar->setMinimum(0);

    //if song finishes, reset everything
    connect(player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),this,SLOT(audioStatusChanged(QMediaPlayer::MediaStatus)));

    //change image based on playback state
    connect(player, SIGNAL(playbackStateChanged(QMediaPlayer::PlaybackState)),this,SLOT(setImage(QMediaPlayer::PlaybackState)));


}

/*
audioStatusChanged:
    1) If song is finished, reset song
    2) If song is loaded, obtain song metadata and display

*/

void MainWindow::audioStatusChanged(QMediaPlayer::MediaStatus status){

    //end of media

    if(status == 6){

        //reset progress bar
        ui->progressBar->setValue(0);
        player->stop();

        setImage(QMediaPlayer::PausedState);

    }

    //obtain metadata of artist and song and display on GUI

    if(status == 2){

        QMediaMetaData data = player->metaData();

        //artist name
        ui->artist->setText(data.stringValue(data.ContributingArtist));
        //title
        ui->song->setText(data.stringValue(data.Title));

        ui->song->setStyleSheet("font-weight: bold; color: red; font-size: 22px");
    }



}

/*
setImage:
    1) Controls the image displayed on the play/pause button
    2) If song is playing, set image to pause
    3) If song is paused, set image to play

*/

void MainWindow::setImage(QMediaPlayer::PlaybackState state){


    //if in paused state
    if(state == 2){

        ui->img->setIcon(QIcon(":/image/play.png"));

    }

    //if in playing state
    if(state == 1){

        ui->img->setIcon(QIcon(":/image/pause.png"));
    }


}

/*
backward:
    1) Only execute function if song is loaded
    2) set song back 5000 miliseconds (5 seconds)


*/

void MainWindow::backward(){

    if(init == true){

        player->setPosition(player->position()-5000);

    }


}

/*
forward:
    1) Only execute function if song is loaded
    2) set song forward 5000 miliseconds (5 seconds)


*/

void MainWindow::forward(){

    if(init == true){

        player->setPosition(player->position()+5000);

    }

}

