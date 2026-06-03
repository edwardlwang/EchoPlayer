//
// Created by edward on 2026/4/22.
//

#ifndef ECHONOTE_PLAYERCONTROLLER_H
#define ECHONOTE_PLAYERCONTROLLER_H

#include <QMediaPlayer>
#include <QVector>
#include <QAudioOutput>

#include "commonstruct.h"

class PlayerController : public QObject {
    Q_OBJECT

public:
    explicit PlayerController(QWidget *parent = nullptr);

    ~PlayerController() override;

    void playNow(const SongInfo &song, int index, const QVector<SongInfo> &queue);

    void togglePlayPause();

    void playNext();

    void playPrevious();

    void setVolume(int volume);

    void toggleOrPlaySong(const SongInfo &song, int row, const QVector<SongInfo> &queue);


signals :
    void currentSongChanged(const SongInfo &, int index);
    void playlistChanged(const QVector<SongInfo> &queue, int currentIndex);

    void playStateChanged(bool is_playing);

    void positionChanged(qint64);

    void durationChanged(qint64);

    void errorOccurred(const QString &);

private:
    QMediaPlayer *m_player;
    QAudioOutput *m_audio;
    QVector<SongInfo> m_songs_queue;
    int m_current_song_index = -1;
};


#endif //ECHONOTE_PLAYERCONTROLLER_H
