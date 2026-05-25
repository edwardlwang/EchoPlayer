//
// Created by edward on 2026/4/22.
//

#include <QUrl>

#include "../h/playercontrolbar.h"
#include "../h/playercontroller.h"

PlayerController::PlayerController(QWidget *parent) : QObject(parent) {
    qDebug() << "PlayerController构造完成";
    m_player = new QMediaPlayer(this);
    m_audio = new QAudioOutput(this);
    m_player->setAudioOutput(m_audio);

    m_audio->setVolume(0.66); // 默认音量为66%

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

PlayerController::~PlayerController() {
    m_player->stop();
    qDebug() << "Playerontroller析构完成";
}

void PlayerController::toggleOrPlaySong(const SongInfo &song, int row, const QVector<SongInfo> &queue) {
    // 防止空指针崩溃
    if (!m_player) {
        return;
    }
    // 同一首：直接切换播放/暂停
    if (song.file_path == m_player->source().toLocalFile()) {
        this->togglePlayPause();
    } else {
        // 不同歌曲：统一交给 playNow 处理（包含更新队列、索引和 current song）
        playNow(song, row, queue);
    }
}

void PlayerController::setVolume(int volume) {
    qDebug() << "Setting volume to:" << volume;
    m_audio->setVolume(static_cast<float>(volume) / 100);
}

void PlayerController::playNow(const SongInfo &song, int index, const QVector<SongInfo> &queue) {
    m_songs_queue = queue;

    if (m_songs_queue.isEmpty()) {
        m_current_song_index = -1;
        qWarning() << "Play queue is empty; ignoring playNow.";
        return;
    }

    if (index < 0 || index >= m_songs_queue.size()) {
        qWarning() << "Invalid song index:" << index << ", clamping to 0.";
        m_current_song_index = 0;
    } else {
        m_current_song_index = index;
    }

    const SongInfo &current_song = m_songs_queue[m_current_song_index];
    qDebug() << "Playing now:" << current_song.file_path;
    m_player->setSource(QUrl::fromLocalFile(current_song.file_path));
    m_player->play();

    emit currentSongChanged(current_song, m_current_song_index);
}

void PlayerController::togglePlayPause() {
    if (m_player->playbackState() == QMediaPlayer::PlayingState) {
        m_player->pause();
    } else {
        m_player->play();
    }
}

void PlayerController::playNext() {
    if (m_songs_queue.isEmpty()) {
        qDebug() << "Queue is empty, cannot play next.";
        return;
    }

    m_current_song_index = (m_current_song_index + 1) % m_songs_queue.size();
    const SongInfo &next_song = m_songs_queue[m_current_song_index];
    playNow(next_song, m_current_song_index, m_songs_queue);
}

void PlayerController::playPrevious() {
    if (m_songs_queue.isEmpty()) {
        qDebug() << "Queue is empty, cannot play previous.";
        return;
    }

    m_current_song_index = (m_current_song_index - 1 + m_songs_queue.size()) % m_songs_queue.size();
    const SongInfo &prev_song = m_songs_queue[m_current_song_index];
    playNow(prev_song, m_current_song_index, m_songs_queue);
}
