//
// Created by edward on 2026/4/22.
//

#include "../h/playbackcontroller.h"

PlaybackController::PlaybackController() {
    qDebug() << "PlaybackController构造完成";
    m_player = new QMediaPlayer(this);
    m_audio = new QAudioOutput(this);
    m_player->setAudioOutput(m_audio);
}

PlaybackController::~PlaybackController() {
    m_player->stop();
    qDebug() << " PlaybackController析构完成";
}

void PlaybackController::playNow(const SongInfo &song, int index, const QVector<SongInfo> &queue) {
    m_songs_queue = queue;
    m_current_song_index = index;

    qDebug() << "Playing now:" << song.file_path;
    m_player->setSource(QUrl::fromLocalFile(song.file_path));
    m_player->play();
}

void PlaybackController::togglePlayPause() {
    if (m_player->playbackState() == QMediaPlayer::PlayingState) {
        m_player->pause();
    } else {
        m_player->play();
    }
}

