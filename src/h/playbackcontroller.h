//
// Created by edward on 2026/4/22.
//

#ifndef ECHONOTE_PLAYBACKCONTROLLER_H
#define ECHONOTE_PLAYBACKCONTROLLER_H

#include <QMediaPlayer>
#include <QVector>

#include "commonstruct.h"

class PlaybackController {
public:
    PlaybackController();

    ~PlaybackController();

    void playNow(const SongInfo &song, int index, const QVector<SongInfo> &queue);

    void togglePlayPause();

    void playNext();

    void playPrevious();

private:
    QMediaPlayer *m_player;
    QAudioOutput *m_audio;
};


#endif //ECHONOTE_PLAYBACKCONTROLLER_H
