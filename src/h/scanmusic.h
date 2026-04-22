//
// Created by edward on 2026/4/16.
//

#ifndef ECHONOTE_SCANMUSIC_H
#define ECHONOTE_SCANMUSIC_H

#include <QDir>
#include <QFileInfo>
#include <QFutureWatcher>
#include <QHash>
#include <QMutex>
#include <QObject>
#include <QStandardPaths>
#include <QVector>

#include <atomic>

#include "commonstruct.h"

namespace ScanMusic {
    // 扫描选项：把“策略”集中到一个结构体里，便于后续扩展
    struct ScanOptions {
        QString rootPath; // 传空则自动用系统音乐目录
        bool recursive = true; // 是否递归子目录
        bool includeHidden = false; // 是否包含隐藏文件
        int batchSize = 100; // 每批回传给 UI 的数量
        bool useCache = true; // 是否启用内存缓存
    };

    // 统计信息：用于日志、调试、UI提示
    struct ScanStats {
        int filesVisited = 0; // 遍历到的文件总数（包含非音频）
        int audioMatched = 0; // 命中音频扩展名的数量
        int metadataOk = 0; // 成功读取元数据数量
        int metadataFallback = 0; // 使用兜底（文件名等）的数量
        qint64 elapsedMs = 0; // 总耗时
    };

    // 路径工具：把 "~/Music" 这类路径解析为真实路径
    QString resolveMusicPath(QString path);

    // 兼容当前调用方式：同步扫描（会阻塞当前线程）
    QVector<SongInfo> scanMusic(const QString &path);

    // 同步扫描：支持统计信息与取消标记
    QVector<SongInfo> scanMusicSync(
        const ScanOptions &options,
        ScanStats *statsOut = nullptr,
        std::atomic_bool *cancelFlag = nullptr
    );

    // 异步扫描服务：后台扫描 + 分批回传 + 可取消
    class ScannerService final : public QObject {
        Q_OBJECT

    public:
        explicit ScannerService(QObject *parent = nullptr);

        ~ScannerService() override;

        // 启动异步扫描。若已有任务在跑，先取消再重启。
        void startScan(const ScanOptions &options = {});

        // 请求取消（不会瞬停，会在下一个检查点停）
        void cancel();

        // 当前是否正在扫描
        bool isRunning() const;

    signals:
        // 每处理一个文件触发一次（用于进度条）
        void progress(int processed, int totalHint);

        // 分批返回数据（用于增量刷新 UI）
        void batchReady(const QVector<SongInfo> &batch);

        // 最终完成（包含全量数据、统计、是否被取消）
        void finished(const QVector<SongInfo> &allSongs, const ScanStats &stats, bool canceled);

    private:
        // 缓存键：文件路径 + 签名（大小/修改时间）
        struct FileSignature {
            qint64 size = 0;
            QDateTime modifiedUtc;

            bool operator==(const FileSignature &other) const {
                return size == other.size && modifiedUtc == other.modifiedUtc;
            }
        };

        struct CachedEntry {
            FileSignature sig;
            SongInfo song;
        };

        // 线程函数：真正执行扫描
        void runWorker(ScanOptions options);

        // 内部元数据读取（单文件）
        static SongInfo readOneFile(const QString &filePath, bool &metadataOk);

        // 构造文件签名（用于判断缓存是否可复用）
        static FileSignature makeSignature(const QFileInfo &fi);

    private:
        QFutureWatcher<void> m_watcher;
        std::atomic_bool m_cancelRequested{false};

        mutable QMutex m_cacheMutex;
        QHash<QString, CachedEntry> m_cache; // key: 绝对路径
    };
}

#endif // ECHONOTE_SCANMUSIC_H
