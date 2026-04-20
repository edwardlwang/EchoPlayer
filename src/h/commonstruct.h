//
// Created by edward on 2026/4/16.
//

#ifndef ECHONOTE_COMMONSTRUCT_H
#define ECHONOTE_COMMONSTRUCT_H

#include <QString>

// 歌曲信息
struct SongInfo {
    QString file_path;
    QString title;
    QString artist;
    QString album;
    int duration_sec;
};

// 按钮信息
struct BtnInfo {
    QString key;
    QString text;
    QString iconPath;
};

#endif //ECHONOTE_COMMONSTRUCT_H
