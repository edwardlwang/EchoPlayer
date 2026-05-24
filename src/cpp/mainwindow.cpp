//
// Created by edward on 2026/3/25.
//

#include "../h/mainwindow.h"
#include "../h/contentarea.h"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent) {
        qDebug() << "MainWindow构造完成";

        // 设置主窗口属性
        this->resize(960, 540);
        this->setWindowTitle("EchoPlayer");
        this->setWindowIcon(QIcon(":/icons/music.svg"));

        // 为主窗口添加中心控件，所有子控件都添加在中心控件里
        m_central_widget = new QWidget(this);
        this->setCentralWidget(m_central_widget);

        // 为中心控件添加垂直布局：上方内容区，下方底部状态栏
        m_central_widget_layout = new QVBoxLayout(m_central_widget);
        m_central_widget_layout->setContentsMargins(0, 0, 0, 0);
        m_central_widget_layout->setSpacing(0);

        // 上方内容区容器
        m_content_widget = new QWidget(m_central_widget);
        m_content_layout = new QHBoxLayout(m_content_widget);
        m_content_layout->setContentsMargins(0, 0, 0, 0);
        m_content_layout->setSpacing(0);

        // 左侧导航栏
        m_navigation_bar = new NavigationBar(m_content_widget);
        m_content_layout->addWidget(m_navigation_bar);

        // 右侧内容区域
        m_content = new ContentArea(m_content_widget);
        m_content_layout->addWidget(m_content, 1);

        connect(m_navigation_bar, &NavigationBar::navigationButtonClicked,
                m_content, &ContentArea::showPage);

        // 上方内容区占据剩余空间
        m_central_widget_layout->addWidget(m_content_widget, 1);

        m_player_controller = new PlayerController(this);
        // m_player_control_bar = new PlayerControlBar(m_player_controller, m_central_widget);
        m_player_control_bar = new PlayerControlBar(m_central_widget);

        // 底部状态栏固定在窗口底部
        m_central_widget_layout->addWidget(m_player_control_bar);

        m_song_page = m_content->findChild<SongPage *>("songpage");
        if (!m_song_page) {
                qFatal("Failed to find SongPage named songpage");
        }

        connect(m_song_page, &SongPage::songActivated, this, [this](const SongInfo &song, int row) {
                if (!m_player_controller) {
                        return;
                }
                m_player_controller->toggleOrPlaySong(song, row, m_song_page->getSongs());
        });

        connect(m_player_control_bar, &PlayerControlBar::playPauseClicked,
                m_player_controller, &PlayerController::togglePlayPause);

        connect(m_player_controller, &PlayerController::playStateChanged,
                m_player_control_bar, &PlayerControlBar::updatePlayerStatusIcon);

        connect(m_player_control_bar, &PlayerControlBar::nextClicked,
                m_player_controller, &PlayerController::playNext);

        connect(m_player_control_bar, &PlayerControlBar::prevClicked,
                m_player_controller, &PlayerController::playPrevious);

        connect(m_song_page, &SongPage::songActivated,
                m_player_control_bar, &PlayerControlBar::updateSongInfo);

        connect(m_player_controller, &PlayerController::currentSongChanged,
                m_player_control_bar, &PlayerControlBar::updateSongInfo);

        connect(m_player_controller, &PlayerController::currentSongChanged,
                m_song_page, &SongPage::highlightCurrentSong);

        connect(m_player_control_bar->getVolumeSlider(), &QSlider::valueChanged,
                m_player_controller, &PlayerController::setVolume);
}

MainWindow::~MainWindow() {
        qDebug() << "MainWindow析构完成";
}
