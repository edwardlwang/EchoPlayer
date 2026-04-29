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
    PlaybackController(QWidget *parent = nullptr);

    ~PlaybackController() override;

    void playNow(const SongInfo &song, int index, const QVector<SongInfo> &queue);

    void togglePlayPause() const;

    void playNext();

    void playPrevious();

signals :
    void currentSongChanged(const SongInfo &, int index);

    void playStateChanged(bool isPlaying);

    void positionChanged(qint64);

    void durationChanged(qint64);

    void errorOccurred(const QString &);

private:
    QMediaPlayer *m_player;
    QAudioOutput *m_audio;
    QVector<SongInfo> m_songs_queue;
    int m_current_song_index = -1;
};


#endif //ECHONOTE_PLAYBACKCONTROLLER_H
