//
// Created by edward on 2026/4/22.
//

#include "../h/playbackcontroller.h"
#include <QUrl>
#include <QWidget>

PlaybackController::PlaybackController(QWidget *parent) : QObject(parent) {
    qDebug() << "PlaybackController构造完成";
    m_player = new QMediaPlayer(this);
    m_audio = new QAudioOutput(this);
    m_player->setAudioOutput(m_audio);

    // Forward useful signals from QMediaPlayer to UI via our own signals
    connect(m_player, &QMediaPlayer::positionChanged, this, [this](qint64 pos) {
        emit positionChanged(pos);
    });

    connect(m_player, &QMediaPlayer::durationChanged, this, [this](qint64 dur) {
        emit durationChanged(dur);
    });

    connect(m_player, &QMediaPlayer::playbackStateChanged, this, [this](QMediaPlayer::PlaybackState state) {
        emit playStateChanged(state == QMediaPlayer::PlayingState);
    });

    connect(m_player, &QMediaPlayer::mediaStatusChanged, this, [this](QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::EndOfMedia) {
            // automatically go to next when current finished
            playNext();
        }
    });

    connect(m_player, &QMediaPlayer::errorOccurred, this, [this](QMediaPlayer::Error /*err*/, const QString &msg) {
        emit errorOccurred(msg);
    });
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

    emit currentSongChanged(song, index);
    emit playStateChanged(true);
}

void PlaybackController::togglePlayPause() const {
    if (m_player->playbackState() == QMediaPlayer::PlayingState) {
        m_player->pause();
    } else {
        m_player->play();
    }
}

void PlaybackController::playNext() {
    if (m_songs_queue.isEmpty()) {
        qDebug() << "Queue is empty, cannot play next.";
        return;
    }

    m_current_song_index = (m_current_song_index + 1) % m_songs_queue.size();
    const SongInfo &next_song = m_songs_queue[m_current_song_index];
    playNow(next_song, m_current_song_index, m_songs_queue);
}

void PlaybackController::playPrevious() {
    if (m_songs_queue.isEmpty()) {
        qDebug() << "Queue is empty, cannot play previous.";
        return;
    }

    m_current_song_index = (m_current_song_index - 1 + m_songs_queue.size()) % m_songs_queue.size();
    const SongInfo &prev_song = m_songs_queue[m_current_song_index];
    playNow(prev_song, m_current_song_index, m_songs_queue);
}
