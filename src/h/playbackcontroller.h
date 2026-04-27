//
// Created by edward on 2026/4/22.
//

#ifndef ECHONOTE_PLAYBACKCONTROLLER_H
#define ECHONOTE_PLAYBACKCONTROLLER_H

#include <QMediaPlayer>
#include <QVector>
#include <QAudioOutput>
#include <QDebug>

#include "commonstruct.h"

class PlaybackController : public QObject {
    Q_OBJECT

public:
    PlaybackController();

    ~PlaybackController() override;

    void playNow(const SongInfo &song, int index, const QVector<SongInfo> &queue);

    void togglePlayPause();

    void playNext();

    void playPrevious();

signals :

private:
    QMediaPlayer *m_player;
    QAudioOutput *m_audio;
    QVector<SongInfo> m_songs_queue;
    int m_current_song_index;
};


#endif //ECHONOTE_PLAYBACKCONTROLLER_H
