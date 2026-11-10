#include "features/home/media_gallery_widget.hpp"

#include <QApplication>
#include <QClipboard>
#include <QFileDialog>
#include <QFileInfo>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QListWidget>
#include <QMenu>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPixmap>
#include <QPushButton>
#include <QScrollArea>
#include <QScrollBar>
#include <QStackedWidget>
#include <QStandardPaths>
#include <QStyle>
#include <QTabBar>
#include <QVBoxLayout>

#include "app/application.hpp"
#include "matrix/matrix_session.hpp"
#include "ui/components/voice_audio_player.hpp"
#include "util/json_util.hpp"

MediaGalleryWidget::MediaGalleryWidget(const QString &roomId, QWidget *parent)
    : QWidget(parent)
    , m_roomId(roomId)
    , m_nam(new QNetworkAccessManager(this))
{
    auto *session = pcp::Application::instance().matrixSession();
    if (session) {
        m_homeserverUrl = session->homeserverUrl();
        m_accessToken = session->accessToken();
    }

    setupUi();
    loadMedia();
}

void MediaGalleryWidget::setupUi()
{
    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    auto *header = new QLabel(tr("Shared Media"), this);
    QFont hFont;
    hFont.setPointSize(16);
    hFont.setBold(true);
    header->setFont(hFont);
    header->setContentsMargins(16, 12, 16, 4);
    root->addWidget(header);

    m_tabBar = new QTabBar(this);
    m_tabBar->setExpanding(false);
    m_tabBar->setDrawBase(false);
    m_tabBar->addTab(tr("Images"));
    m_tabBar->addTab(tr("Videos"));
    m_tabBar->addTab(tr("Files"));
    m_tabBar->addTab(tr("Links"));
    m_tabBar->addTab(tr("Audio"));
    m_tabBar->setStyleSheet(QStringLiteral(
        "QTabBar::tab {"
        "  padding: 8px 16px;"
        "  border: none;"
        "  border-bottom: 2px solid transparent;"
        "  color: #757575;"
        "  font-size: 13px;"
        "}"
        "QTabBar::tab:selected {"
        "  color: #1976D2;"
        "  border-bottom: 2px solid #1976D2;"
        "  font-weight: bold;"
        "}"
        "QTabBar::tab:hover {"
        "  color: #1565C0;"
        "}"));
    root->addWidget(m_tabBar);

    m_stack = new QStackedWidget(this);

    // ── Images tab ──
    m_imagesPage = new QWidget(this);
    auto *imagesLayout = new QVBoxLayout(m_imagesPage);
    imagesLayout->setContentsMargins(8, 8, 8, 8);
    imagesLayout->setSpacing(8);

    m_imagesScroll = new QScrollArea(this);
    m_imagesScroll->setWidgetResizable(true);
    m_imagesScroll->setFrameShape(QFrame::NoFrame);
    m_imagesScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    auto *imagesInner = new QWidget(this);
    imagesInner->setObjectName(QStringLiteral("imagesInner"));
    auto *imagesGrid = new QGridLayout(imagesInner);
    imagesGrid->setContentsMargins(0, 0, 0, 0);
    imagesGrid->setSpacing(6);
    imagesGrid->setObjectName(QStringLiteral("imagesGrid"));
    m_imagesScroll->setWidget(imagesInner);

    m_imagesEmptyLabel = new QLabel(tr("No images shared yet"), this);
    m_imagesEmptyLabel->setAlignment(Qt::AlignCenter);
    m_imagesEmptyLabel->setStyleSheet(QStringLiteral("color: #999; font-size: 14px; padding: 40px;"));
    m_imagesEmptyLabel->setVisible(false);

    imagesLayout->addWidget(m_imagesEmptyLabel);
    imagesLayout->addWidget(m_imagesScroll);

    m_loadMoreImages = new QPushButton(tr("Load More"), this);
    m_loadMoreImages->setVisible(false);
    m_loadMoreImages->setCursor(Qt::PointingHandCursor);
    m_loadMoreImages->setStyleSheet(QStringLiteral(
        "QPushButton { background-color: #ECEFF1; border: 1px solid #CFD8DC; border-radius: 6px;"
        " color: #1976D2; font-size: 13px; padding: 8px 20px; }"
        "QPushButton:hover { background-color: #CFD8DC; }"));
    imagesLayout->addWidget(m_loadMoreImages, 0, Qt::AlignCenter);

    m_stack->addWidget(m_imagesPage);

    // ── Videos tab ──
    m_videosPage = new QWidget(this);
    auto *videosLayout = new QVBoxLayout(m_videosPage);
    videosLayout->setContentsMargins(8, 8, 8, 8);
    videosLayout->setSpacing(8);

    m_videosScroll = new QScrollArea(this);
    m_videosScroll->setWidgetResizable(true);
    m_videosScroll->setFrameShape(QFrame::NoFrame);
    m_videosScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    auto *videosInner = new QWidget(this);
    videosInner->setObjectName(QStringLiteral("videosInner"));
    auto *videosGrid = new QGridLayout(videosInner);
    videosGrid->setContentsMargins(0, 0, 0, 0);
    videosGrid->setSpacing(6);
    videosGrid->setObjectName(QStringLiteral("videosGrid"));
    m_videosScroll->setWidget(videosInner);

    m_videosEmptyLabel = new QLabel(tr("No videos shared yet"), this);
    m_videosEmptyLabel->setAlignment(Qt::AlignCenter);
    m_videosEmptyLabel->setStyleSheet(QStringLiteral("color: #999; font-size: 14px; padding: 40px;"));
    m_videosEmptyLabel->setVisible(false);

    videosLayout->addWidget(m_videosEmptyLabel);
    videosLayout->addWidget(m_videosScroll);

    m_loadMoreVideos = new QPushButton(tr("Load More"), this);
    m_loadMoreVideos->setVisible(false);
    m_loadMoreVideos->setCursor(Qt::PointingHandCursor);
    m_loadMoreVideos->setStyleSheet(QStringLiteral(
        "QPushButton { background-color: #ECEFF1; border: 1px solid #CFD8DC; border-radius: 6px;"
        " color: #1976D2; font-size: 13px; padding: 8px 20px; }"
        "QPushButton:hover { background-color: #CFD8DC; }"));
    videosLayout->addWidget(m_loadMoreVideos, 0, Qt::AlignCenter);

    m_stack->addWidget(m_videosPage);

    // ── Files tab ──
    m_filesPage = new QWidget(this);
    auto *filesLayout = new QVBoxLayout(m_filesPage);
    filesLayout->setContentsMargins(8, 8, 8, 8);
    filesLayout->setSpacing(8);

    m_filesList = new QListWidget(this);
    m_filesList->setFrameShape(QFrame::NoFrame);
    m_filesList->setSelectionMode(QAbstractItemView::SingleSelection);
    m_filesList->setContextMenuPolicy(Qt::CustomContextMenu);
    m_filesList->setStyleSheet(QStringLiteral(
        "QListWidget { background: transparent; }"
        "QListWidget::item { padding: 8px; border-bottom: 1px solid #F0F0F0; }"
        "QListWidget::item:hover { background-color: #F5F5F5; }"));
    filesLayout->addWidget(m_filesList);

    m_filesEmptyLabel = new QLabel(tr("No files shared yet"), this);
    m_filesEmptyLabel->setAlignment(Qt::AlignCenter);
    m_filesEmptyLabel->setStyleSheet(QStringLiteral("color: #999; font-size: 14px; padding: 40px;"));
    m_filesEmptyLabel->setVisible(false);

    filesLayout->addWidget(m_filesEmptyLabel);

    m_loadMoreFiles = new QPushButton(tr("Load More"), this);
    m_loadMoreFiles->setVisible(false);
    m_loadMoreFiles->setCursor(Qt::PointingHandCursor);
    m_loadMoreFiles->setStyleSheet(QStringLiteral(
        "QPushButton { background-color: #ECEFF1; border: 1px solid #CFD8DC; border-radius: 6px;"
        " color: #1976D2; font-size: 13px; padding: 8px 20px; }"
        "QPushButton:hover { background-color: #CFD8DC; }"));
    filesLayout->addWidget(m_loadMoreFiles, 0, Qt::AlignCenter);

    m_stack->addWidget(m_filesPage);

    // ── Links tab ──
    m_linksPage = new QWidget(this);
    auto *linksLayout = new QVBoxLayout(m_linksPage);
    linksLayout->setContentsMargins(8, 8, 8, 8);
    linksLayout->setSpacing(8);

    m_linksList = new QListWidget(this);
    m_linksList->setFrameShape(QFrame::NoFrame);
    m_linksList->setSelectionMode(QAbstractItemView::SingleSelection);
    m_linksList->setContextMenuPolicy(Qt::CustomContextMenu);
    m_linksList->setStyleSheet(QStringLiteral(
        "QListWidget { background: transparent; }"
        "QListWidget::item { padding: 8px; border-bottom: 1px solid #F0F0F0; }"
        "QListWidget::item:hover { background-color: #F5F5F5; }"));
    linksLayout->addWidget(m_linksList);

    m_linksEmptyLabel = new QLabel(tr("No links shared yet"), this);
    m_linksEmptyLabel->setAlignment(Qt::AlignCenter);
    m_linksEmptyLabel->setStyleSheet(QStringLiteral("color: #999; font-size: 14px; padding: 40px;"));
    m_linksEmptyLabel->setVisible(false);

    linksLayout->addWidget(m_linksEmptyLabel);

    m_loadMoreLinks = new QPushButton(tr("Load More"), this);
    m_loadMoreLinks->setVisible(false);
    m_loadMoreLinks->setCursor(Qt::PointingHandCursor);
    m_loadMoreLinks->setStyleSheet(QStringLiteral(
        "QPushButton { background-color: #ECEFF1; border: 1px solid #CFD8DC; border-radius: 6px;"
        " color: #1976D2; font-size: 13px; padding: 8px 20px; }"
        "QPushButton:hover { background-color: #CFD8DC; }"));
    linksLayout->addWidget(m_loadMoreLinks, 0, Qt::AlignCenter);

    m_stack->addWidget(m_linksPage);

    // ── Audio tab ──
    m_audioPage = new QWidget(this);
    auto *audioLayout = new QVBoxLayout(m_audioPage);
    audioLayout->setContentsMargins(8, 8, 8, 8);
    audioLayout->setSpacing(8);

    m_audioList = new QListWidget(this);
    m_audioList->setFrameShape(QFrame::NoFrame);
    m_audioList->setSelectionMode(QAbstractItemView::SingleSelection);
    m_audioList->setStyleSheet(QStringLiteral(
        "QListWidget { background: transparent; }"
        "QListWidget::item { padding: 4px; border-bottom: 1px solid #F0F0F0; }"
        "QListWidget::item:hover { background-color: #F5F5F5; }"));
    audioLayout->addWidget(m_audioList);

    m_audioEmptyLabel = new QLabel(tr("No audio files shared yet"), this);
    m_audioEmptyLabel->setAlignment(Qt::AlignCenter);
    m_audioEmptyLabel->setStyleSheet(QStringLiteral("color: #999; font-size: 14px; padding: 40px;"));
    m_audioEmptyLabel->setVisible(false);

    audioLayout->addWidget(m_audioEmptyLabel);

    m_loadMoreAudio = new QPushButton(tr("Load More"), this);
    m_loadMoreAudio->setVisible(false);
    m_loadMoreAudio->setCursor(Qt::PointingHandCursor);
    m_loadMoreAudio->setStyleSheet(QStringLiteral(
        "QPushButton { background-color: #ECEFF1; border: 1px solid #CFD8DC; border-radius: 6px;"
        " color: #1976D2; font-size: 13px; padding: 8px 20px; }"
        "QPushButton:hover { background-color: #CFD8DC; }"));
    audioLayout->addWidget(m_loadMoreAudio, 0, Qt::AlignCenter);

    m_stack->addWidget(m_audioPage);

    root->addWidget(m_stack);

    // Connections
    connect(m_tabBar, &QTabBar::currentChanged, this, &MediaGalleryWidget::onTabChanged);
    connect(m_loadMoreImages, &QPushButton::clicked, this, &MediaGalleryWidget::onLoadMore);
    connect(m_loadMoreVideos, &QPushButton::clicked, this, &MediaGalleryWidget::onLoadMore);
    connect(m_loadMoreFiles, &QPushButton::clicked, this, &MediaGalleryWidget::onLoadMore);
    connect(m_loadMoreLinks, &QPushButton::clicked, this, &MediaGalleryWidget::onLoadMore);
    connect(m_loadMoreAudio, &QPushButton::clicked, this, &MediaGalleryWidget::onLoadMore);

    connect(m_filesList, &QListWidget::customContextMenuRequested, this, [this](const QPoint &pos) {
        int row = m_filesList->currentRow();
        if (row >= 0 && row < m_files.size())
            onContextMenuRequested(pos, row, TAB_FILES);
    });

    connect(m_linksList, &QListWidget::customContextMenuRequested, this,
            &MediaGalleryWidget::onLinksContextMenu);
}

void MediaGalleryWidget::loadMedia()
{
    fetchMediaBatch();
}

void MediaGalleryWidget::fetchMediaBatch(const QString &from)
{
    if (m_homeserverUrl.isEmpty())
        return;

    QString path = QStringLiteral("/_matrix/client/r0/rooms/%1/messages?dir=b&limit=%2&filter=")
                       .arg(m_roomId)
                       .arg(kBatchSize);

    QJsonObject filter;
    QJsonObject roomFilter;
    QJsonObject timelineFilter;
    QJsonArray types;
    types.append(QStringLiteral("m.room.message"));
    QStringList notTypes;
    notTypes << QStringLiteral("m.text") << QStringLiteral("m.notice") << QStringLiteral("m.emote");
    QJsonArray notTypesArr;
    for (const auto &nt : notTypes)
        notTypesArr.append(nt);
    timelineFilter[QStringLiteral("not_types")] = notTypesArr;
    timelineFilter[QStringLiteral("types")] = types;
    roomFilter[QStringLiteral("timeline")] = timelineFilter;
    filter[QStringLiteral("room")] = roomFilter;

    path += QString::fromUtf8(QUrl::toPercentEncoding(
        QString::fromUtf8(QJsonDocument(filter).toJson(QJsonDocument::Compact))));

    if (!from.isEmpty())
        path += QStringLiteral("&from=") + from;

    QUrl url(m_homeserverUrl + path);
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    if (!m_accessToken.isEmpty())
        req.setRawHeader("Authorization", ("Bearer " + m_accessToken).toUtf8());

    QNetworkReply *reply = m_nam->get(req);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();

        if (reply->error() != QNetworkReply::NoError)
            return;

        QJsonObject json = JsonUtil::parseObject(QString::fromUtf8(reply->readAll()));
        QJsonArray chunk = JsonUtil::optArray(json, QStringLiteral("chunk"));

        processMediaEvents(chunk);
        m_loadedCount += chunk.size();

        QString end = JsonUtil::optString(json, QStringLiteral("end"));
        bool hasMore = !end.isEmpty() && chunk.size() == kBatchSize;

        m_imagesNextBatch = hasMore ? end : QString();
        m_videosNextBatch = hasMore ? end : QString();
        m_filesNextBatch = hasMore ? end : QString();
        m_linksNextBatch = hasMore ? end : QString();
        m_audioNextBatch = hasMore ? end : QString();

        m_imagesLoaded = true;
        m_videosLoaded = true;
        m_filesLoaded = true;
        m_linksLoaded = true;
        m_audioLoaded = true;

        populateImagesTab();
        populateVideosTab();
        populateFilesTab();
        populateLinksTab();
        populateAudioTab();

        m_loadMoreImages->setVisible(hasMore && !m_images.isEmpty());
        m_loadMoreVideos->setVisible(hasMore && !m_videos.isEmpty());
        m_loadMoreFiles->setVisible(hasMore && !m_files.isEmpty());
        m_loadMoreLinks->setVisible(hasMore && !m_links.isEmpty());
        m_loadMoreAudio->setVisible(hasMore && !m_audio.isEmpty());
    });
}

void MediaGalleryWidget::processMediaEvents(const QJsonArray &events)
{
    for (const auto &ev : events) {
        QJsonObject evObj = ev.toObject();
        QString type = JsonUtil::optString(evObj, QStringLiteral("type"));
        if (type != QStringLiteral("m.room.message"))
            continue;

        QJsonObject content = JsonUtil::optObject(evObj, QStringLiteral("content"));
        QString msgType = JsonUtil::optString(content, QStringLiteral("msgtype"));

        MediaItem item;
        item.mxcUri = JsonUtil::optString(content, QStringLiteral("url"));
        item.fileName = JsonUtil::optString(content, QStringLiteral("body"));
        item.senderId = JsonUtil::optString(evObj, QStringLiteral("sender"));
        item.timestamp = JsonUtil::optLong(evObj, QStringLiteral("origin_server_ts"));
        item.mimeType = JsonUtil::optString(content, QStringLiteral("mimetype"));

        QJsonObject info = JsonUtil::optObject(content, QStringLiteral("info"));
        item.fileSize = JsonUtil::optLong(info, QStringLiteral("size"));
        item.metadata = info;

        if (msgType == QStringLiteral("m.image")) {
            if (!item.mxcUri.isEmpty()) m_images.append(item);
        } else if (msgType == QStringLiteral("m.video")) {
            if (!item.mxcUri.isEmpty()) m_videos.append(item);
        } else if (msgType == QStringLiteral("m.file")) {
            if (!item.mxcUri.isEmpty()) m_files.append(item);

            if (item.mimeType.startsWith(QStringLiteral("audio/")))
                m_audio.append(item);
        } else if (msgType == QStringLiteral("m.audio")) {
            if (!item.mxcUri.isEmpty()) m_audio.append(item);
        }

        // Extract links from m.text messages (in the main body text)
        QString body = JsonUtil::optString(content, QStringLiteral("body"));
        static const QRegularExpression urlRe(
            QStringLiteral(R"((https?://[^\s<>"{}|\\^`\[\]]+))"),
            QRegularExpression::CaseInsensitiveOption);
        QRegularExpressionMatchIterator it = urlRe.globalMatch(body);
        while (it.hasNext()) {
            QRegularExpressionMatch match = it.next();
            MediaItem linkItem;
            linkItem.fileName = match.captured(1);
            linkItem.senderId = JsonUtil::optString(evObj, QStringLiteral("sender"));
            linkItem.timestamp = JsonUtil::optLong(evObj, QStringLiteral("origin_server_ts"));
            linkItem.mimeType = QStringLiteral("text/url");
            m_links.append(linkItem);
        }
    }
}

void MediaGalleryWidget::populateImagesTab()
{
    auto *inner = m_imagesPage->findChild<QWidget *>(QStringLiteral("imagesInner"));
    auto *grid = inner ? inner->findChild<QGridLayout *>(QStringLiteral("imagesGrid")) : nullptr;
    if (!grid)
        return;

    while (QLayoutItem *child = grid->takeAt(0)) {
        if (child->widget())
            child->widget()->deleteLater();
        delete child;
    }

    if (m_images.isEmpty()) {
        m_imagesEmptyLabel->setVisible(true);
        m_imagesScroll->setVisible(false);
        return;
    }

    m_imagesEmptyLabel->setVisible(false);
    m_imagesScroll->setVisible(true);

    constexpr int kCols = 4;
    for (int i = 0; i < m_images.size(); ++i) {
        QWidget *thumb = createImageThumbnail(m_images[i], i);
        grid->addWidget(thumb, i / kCols, i % kCols);
    }
}

QWidget *MediaGalleryWidget::createImageThumbnail(const MediaItem &item, int index)
{
    auto *container = new QWidget(this);
    container->setFixedSize(140, 140);
    container->setCursor(Qt::PointingHandCursor);
    container->setStyleSheet(QStringLiteral(
        "QWidget { border: 1px solid #E0E0E0; border-radius: 8px; background-color: #F5F5F5; }"
        "QWidget:hover { border-color: #1976D2; background-color: #E3F2FD; }"));

    auto *layout = new QVBoxLayout(container);
    layout->setContentsMargins(4, 4, 4, 4);
    layout->setSpacing(2);

    auto *iconLabel = new QLabel(this);
    iconLabel->setAlignment(Qt::AlignCenter);
    iconLabel->setFixedSize(100, 100);

    QString mxcHttp = resolveMxcToHttp(item.mxcUri);
    QUrl thumbUrl(mxcHttp);
    QNetworkRequest thumbReq(thumbUrl);
    if (!m_accessToken.isEmpty())
        thumbReq.setRawHeader("Authorization", ("Bearer " + m_accessToken).toUtf8());

    QNetworkReply *thumbReply = m_nam->get(thumbReq);
    connect(thumbReply, &QNetworkReply::finished, container, [iconLabel, thumbReply]() {
        thumbReply->deleteLater();
        if (thumbReply->error() == QNetworkReply::NoError) {
            QPixmap pix;
            pix.loadFromData(thumbReply->readAll());
            if (!pix.isNull()) {
                iconLabel->setPixmap(pix.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
                iconLabel->setStyleSheet(QStringLiteral("border: none; background: transparent;"));
            }
        }
    });

    layout->addWidget(iconLabel, 0, Qt::AlignCenter);

    auto *nameLabel = new QLabel(senderDisplayName(item), this);
    nameLabel->setAlignment(Qt::AlignCenter);
    nameLabel->setStyleSheet(QStringLiteral(
        "border: none; background: transparent; color: #666; font-size: 10px;"));
    nameLabel->setMaximumHeight(16);
    nameLabel->setWordWrap(false);
    layout->addWidget(nameLabel);

    container->installEventFilter(this);
    container->setProperty("mediaIndex", index);

    return container;
}

QWidget *MediaGalleryWidget::createVideoThumbnail(const MediaItem &item, int index)
{
    auto *container = new QWidget(this);
    container->setFixedSize(160, 140);
    container->setCursor(Qt::PointingHandCursor);
    container->setStyleSheet(QStringLiteral(
        "QWidget { border: 1px solid #E0E0E0; border-radius: 8px; background-color: #F5F5F5; }"
        "QWidget:hover { border-color: #D32F2F; background-color: #FFEBEE; }"));

    auto *layout = new QVBoxLayout(container);
    layout->setContentsMargins(4, 4, 4, 4);
    layout->setSpacing(2);

    auto *thumbWidget = new QWidget(this);
    thumbWidget->setFixedSize(140, 80);
    thumbWidget->setStyleSheet(QStringLiteral(
        "border: none; background-color: #212121; border-radius: 6px;"));

    auto *thumbLayout = new QVBoxLayout(thumbWidget);
    thumbLayout->setAlignment(Qt::AlignCenter);

    auto *playLabel = new QLabel(QStringLiteral("\u25B6"), this);
    playLabel->setAlignment(Qt::AlignCenter);
    QFont playFont;
    playFont.setPointSize(28);
    playLabel->setFont(playFont);
    playLabel->setStyleSheet(QStringLiteral(
        "border: none; background: transparent; color: rgba(255,255,255,0.85);"));
    thumbLayout->addWidget(playLabel);

    layout->addWidget(thumbWidget, 0, Qt::AlignCenter);

    auto *nameLabel = new QLabel(senderDisplayName(item), this);
    nameLabel->setAlignment(Qt::AlignCenter);
    nameLabel->setStyleSheet(QStringLiteral(
        "border: none; background: transparent; color: #666; font-size: 10px;"));
    nameLabel->setMaximumHeight(16);
    layout->addWidget(nameLabel);

    auto *sizeLabel = new QLabel(formatFileSize(item.fileSize), this);
    sizeLabel->setAlignment(Qt::AlignCenter);
    sizeLabel->setStyleSheet(QStringLiteral(
        "border: none; background: transparent; color: #999; font-size: 10px;"));
    sizeLabel->setMaximumHeight(14);
    layout->addWidget(sizeLabel);

    container->setProperty("mediaIndex", index);

    return container;
}

void MediaGalleryWidget::populateVideosTab()
{
    auto *inner = m_videosPage->findChild<QWidget *>(QStringLiteral("videosInner"));
    auto *grid = inner ? inner->findChild<QGridLayout *>(QStringLiteral("videosGrid")) : nullptr;
    if (!grid)
        return;

    while (QLayoutItem *child = grid->takeAt(0)) {
        if (child->widget())
            child->widget()->deleteLater();
        delete child;
    }

    if (m_videos.isEmpty()) {
        m_videosEmptyLabel->setVisible(true);
        m_videosScroll->setVisible(false);
        return;
    }

    m_videosEmptyLabel->setVisible(false);
    m_videosScroll->setVisible(true);

    constexpr int kCols = 3;
    for (int i = 0; i < m_videos.size(); ++i) {
        QWidget *thumb = createVideoThumbnail(m_videos[i], i);
        grid->addWidget(thumb, i / kCols, i % kCols);
    }
}

void MediaGalleryWidget::populateFilesTab()
{
    m_filesList->clear();

    if (m_files.isEmpty()) {
        m_filesEmptyLabel->setVisible(true);
        m_filesList->setVisible(false);
        return;
    }

    m_filesEmptyLabel->setVisible(false);
    m_filesList->setVisible(true);

    for (int i = 0; i < m_files.size(); ++i) {
        const auto &item = m_files[i];

        auto *rowWidget = new QWidget(this);
        auto *rowLayout = new QHBoxLayout(rowWidget);
        rowLayout->setContentsMargins(8, 6, 8, 6);
        rowLayout->setSpacing(10);

        auto *iconLabel = new QLabel(iconForMimeType(item.mimeType), this);
        QFont iconFont;
        iconFont.setPointSize(18);
        iconLabel->setFont(iconFont);
        iconLabel->setFixedWidth(36);
        iconLabel->setAlignment(Qt::AlignCenter);
        iconLabel->setStyleSheet(QStringLiteral("border: none; background: transparent;"));
        rowLayout->addWidget(iconLabel);

        auto *infoLayout = new QVBoxLayout;
        infoLayout->setSpacing(2);

        auto *nameLabel = new QLabel(item.fileName, this);
        QFont nameFont;
        nameFont.setPointSize(12);
        nameLabel->setFont(nameFont);
        nameLabel->setStyleSheet(QStringLiteral("border: none; background: transparent;"));
        infoLayout->addWidget(nameLabel);

        auto *metaLabel = new QLabel(
            QStringLiteral("%1 \u00B7 %2 \u00B7 %3")
                .arg(formatFileSize(item.fileSize),
                     formatDate(item.timestamp),
                     senderDisplayName(item)),
            this);
        metaLabel->setStyleSheet(QStringLiteral(
            "border: none; background: transparent; color: #999; font-size: 11px;"));
        infoLayout->addWidget(metaLabel);

        rowLayout->addLayout(infoLayout, 1);
        rowLayout->addStretch();

        auto *downloadBtn = new QPushButton(QStringLiteral("\u2B07"), this);
        downloadBtn->setFixedSize(32, 32);
        downloadBtn->setCursor(Qt::PointingHandCursor);
        downloadBtn->setToolTip(tr("Download"));
        downloadBtn->setStyleSheet(QStringLiteral(
            "QPushButton { border: 1px solid #CFD8DC; border-radius: 4px;"
            " background: transparent; font-size: 14px; }"
            "QPushButton:hover { background-color: #E3F2FD; border-color: #1976D2; }"));

        connect(downloadBtn, &QPushButton::clicked, this, [this, i]() {
            onFileDownloadClicked(i);
        });

        rowLayout->addWidget(downloadBtn);

        auto *listItem = new QListWidgetItem(m_filesList);
        listItem->setSizeHint(rowWidget->sizeHint());
        listItem->setData(Qt::UserRole, i);
        m_filesList->setItemWidget(listItem, rowWidget);
    }
}

void MediaGalleryWidget::populateLinksTab()
{
    m_linksList->clear();

    if (m_links.isEmpty()) {
        m_linksEmptyLabel->setVisible(true);
        m_linksList->setVisible(false);
        return;
    }

    m_linksEmptyLabel->setVisible(false);
    m_linksList->setVisible(true);

    for (int i = 0; i < m_links.size(); ++i) {
        const auto &item = m_links[i];

        auto *rowWidget = new QWidget(this);
        auto *rowLayout = new QHBoxLayout(rowWidget);
        rowLayout->setContentsMargins(8, 8, 8, 8);
        rowLayout->setSpacing(10);

        auto *linkIcon = new QLabel(QStringLiteral("\U0001F517"), this);
        linkIcon->setFixedWidth(28);
        linkIcon->setAlignment(Qt::AlignCenter);
        linkIcon->setStyleSheet(QStringLiteral("border: none; background: transparent; font-size: 16px;"));
        rowLayout->addWidget(linkIcon);

        auto *infoLayout = new QVBoxLayout;
        infoLayout->setSpacing(2);

        auto *urlLabel = new QLabel(item.fileName, this);
        urlLabel->setWordWrap(true);
        urlLabel->setStyleSheet(QStringLiteral(
            "border: none; background: transparent; color: #1976D2; font-size: 12px;"));
        urlLabel->setCursor(Qt::PointingHandCursor);
        urlLabel->setTextFormat(Qt::PlainText);
        infoLayout->addWidget(urlLabel);

        auto *metaLabel = new QLabel(
            tr("Shared by %1 on %2")
                .arg(senderDisplayName(item), formatDate(item.timestamp)),
            this);
        metaLabel->setStyleSheet(QStringLiteral(
            "border: none; background: transparent; color: #999; font-size: 11px;"));
        infoLayout->addWidget(metaLabel);

        rowLayout->addLayout(infoLayout, 1);

        auto *listItem = new QListWidgetItem(m_linksList);
        listItem->setSizeHint(rowWidget->sizeHint());
        listItem->setData(Qt::UserRole, i);
        m_linksList->setItemWidget(listItem, rowWidget);
    }
}

void MediaGalleryWidget::populateAudioTab()
{
    m_audioList->clear();

    if (m_audio.isEmpty()) {
        m_audioEmptyLabel->setVisible(true);
        m_audioList->setVisible(false);
        return;
    }

    m_audioEmptyLabel->setVisible(false);
    m_audioList->setVisible(true);

    for (int i = 0; i < m_audio.size(); ++i) {
        const auto &item = m_audio[i];

        auto *rowWidget = new QWidget(this);
        auto *rowLayout = new QVBoxLayout(rowWidget);
        rowLayout->setContentsMargins(8, 6, 8, 6);
        rowLayout->setSpacing(4);

        auto *headerLayout = new QHBoxLayout;
        auto *nameLabel = new QLabel(item.fileName, this);
        QFont nameFont;
        nameFont.setPointSize(11);
        nameLabel->setFont(nameFont);
        nameLabel->setStyleSheet(QStringLiteral("border: none; background: transparent;"));
        headerLayout->addWidget(nameLabel);

        auto *sizeLabel = new QLabel(formatFileSize(item.fileSize), this);
        sizeLabel->setStyleSheet(QStringLiteral(
            "border: none; background: transparent; color: #999; font-size: 11px;"));
        headerLayout->addWidget(sizeLabel);
        headerLayout->addStretch();
        rowLayout->addLayout(headerLayout);

        auto *metaLabel = new QLabel(
            tr("Sent by %1 on %2")
                .arg(senderDisplayName(item), formatDate(item.timestamp)),
            this);
        metaLabel->setStyleSheet(QStringLiteral(
            "border: none; background: transparent; color: #999; font-size: 11px;"));
        rowLayout->addWidget(metaLabel);

        auto *player = new VoiceAudioPlayer(this);
        player->setMiniMode(true);
        player->setProperty("mediaIndex", i);

        QString mxcHttp = resolveMxcToHttp(item.mxcUri);
        player->loadAudio(mxcHttp);
        rowLayout->addWidget(player);

        auto *listItem = new QListWidgetItem(m_audioList);
        listItem->setSizeHint(rowWidget->sizeHint());
        listItem->setData(Qt::UserRole, i);
        m_audioList->setItemWidget(listItem, rowWidget);
    }
}

void MediaGalleryWidget::onTabChanged(int index)
{
    m_stack->setCurrentIndex(index);

    switch (index) {
    case TAB_IMAGES:
        if (!m_imagesLoaded)
            loadMedia();
        else
            populateImagesTab();
        break;
    case TAB_VIDEOS:
        if (!m_videosLoaded)
            loadMedia();
        else
            populateVideosTab();
        break;
    case TAB_FILES:
        if (!m_filesLoaded)
            loadMedia();
        else
            populateFilesTab();
        break;
    case TAB_LINKS:
        if (!m_linksLoaded)
            loadMedia();
        else
            populateLinksTab();
        break;
    case TAB_AUDIO:
        if (!m_audioLoaded)
            loadMedia();
        else
            populateAudioTab();
        break;
    }
}

void MediaGalleryWidget::onLoadMore()
{
    int tab = m_tabBar->currentIndex();
    QString from;
    switch (tab) {
    case TAB_IMAGES: from = m_imagesNextBatch; break;
    case TAB_VIDEOS: from = m_videosNextBatch; break;
    case TAB_FILES:  from = m_filesNextBatch; break;
    case TAB_LINKS:  from = m_linksNextBatch; break;
    case TAB_AUDIO:  from = m_audioNextBatch; break;
    }
    fetchMediaBatch(from);
}

void MediaGalleryWidget::onImageThumbnailClicked(int mediaIndex)
{
    Q_UNUSED(mediaIndex)
}

void MediaGalleryWidget::onVideoThumbnailClicked(int mediaIndex)
{
    Q_UNUSED(mediaIndex)
}

void MediaGalleryWidget::onFileDownloadClicked(int mediaIndex)
{
    if (mediaIndex < 0 || mediaIndex >= m_files.size())
        return;
    downloadMedia(m_files[mediaIndex].mxcUri, mediaIndex, TAB_FILES);
}

void MediaGalleryWidget::downloadMedia(const QString &mxcUri, int mediaIndex, int tabIndex)
{
    if (mxcUri.isEmpty())
        return;

    QString httpUrl = resolveMxcToHttp(mxcUri);
    QUrl url(httpUrl);
    QNetworkRequest req(url);
    if (!m_accessToken.isEmpty())
        req.setRawHeader("Authorization", ("Bearer " + m_accessToken).toUtf8());

    QNetworkReply *reply = m_nam->get(req);
    connect(reply, &QNetworkReply::finished, this, [this, reply, mediaIndex, tabIndex]() {
        reply->deleteLater();

        if (reply->error() != QNetworkReply::NoError)
            return;

        QByteArray data = reply->readAll();
        QByteArray contentDisposition = reply->rawHeader("Content-Disposition");
        QString fileName;
        if (!contentDisposition.isEmpty()) {
            static const QRegularExpression fnRe(QStringLiteral("filename=\"?([^\";\n]+)\"?"));
            QRegularExpressionMatch m = fnRe.match(QString::fromUtf8(contentDisposition));
            if (m.hasMatch())
                fileName = m.captured(1);
        }

        if (fileName.isEmpty()) {
            if (tabIndex == TAB_FILES && mediaIndex < m_files.size())
                fileName = m_files[mediaIndex].fileName;
            else if (tabIndex == TAB_IMAGES && mediaIndex < m_images.size())
                fileName = m_images[mediaIndex].fileName;
            else if (tabIndex == TAB_VIDEOS && mediaIndex < m_videos.size())
                fileName = m_videos[mediaIndex].fileName;
            else if (tabIndex == TAB_AUDIO && mediaIndex < m_audio.size())
                fileName = m_audio[mediaIndex].fileName;
            else
                fileName = QStringLiteral("download");
        }

        QString saveDir = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
        QString savePath = QFileDialog::getSaveFileName(
            this, tr("Save File"), saveDir + QLatin1Char('/') + fileName);

        if (savePath.isEmpty())
            return;

        QFile file(savePath);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(data);
            file.close();
            emit fileOpened(savePath);
        }
    });
}

void MediaGalleryWidget::downloadAndShow(int mediaIndex, int tabIndex)
{
    Q_UNUSED(mediaIndex)
    Q_UNUSED(tabIndex)
}

void MediaGalleryWidget::onContextMenuRequested(const QPoint &pos, int mediaIndex, int tabIndex)
{
    QMenu menu(this);

    if (tabIndex == TAB_IMAGES || tabIndex == TAB_VIDEOS) {
        QAction *viewAction = menu.addAction(tr("View"));
        QAction *downloadAction = menu.addAction(tr("Download"));
        QAction *copyLinkAction = menu.addAction(tr("Copy Link"));

        connect(viewAction, &QAction::triggered, this, [this, mediaIndex, tabIndex]() {
            downloadAndShow(mediaIndex, tabIndex);
        });
        connect(downloadAction, &QAction::triggered, this, [this, mediaIndex, tabIndex]() {
            QString uri;
            if (tabIndex == TAB_IMAGES && mediaIndex < m_images.size())
                uri = m_images[mediaIndex].mxcUri;
            else if (tabIndex == TAB_VIDEOS && mediaIndex < m_videos.size())
                uri = m_videos[mediaIndex].mxcUri;
            downloadMedia(uri, mediaIndex, tabIndex);
        });
        connect(copyLinkAction, &QAction::triggered, this, [this, mediaIndex, tabIndex]() {
            QString uri;
            if (tabIndex == TAB_IMAGES && mediaIndex < m_images.size())
                uri = resolveMxcToHttp(m_images[mediaIndex].mxcUri);
            else if (tabIndex == TAB_VIDEOS && mediaIndex < m_videos.size())
                uri = resolveMxcToHttp(m_videos[mediaIndex].mxcUri);
            QApplication::clipboard()->setText(uri);
        });
    } else if (tabIndex == TAB_FILES) {
        QAction *downloadAction = menu.addAction(tr("Download"));
        QAction *copyLinkAction = menu.addAction(tr("Copy Link"));

        connect(downloadAction, &QAction::triggered, this, [this, mediaIndex]() {
            onFileDownloadClicked(mediaIndex);
        });
        connect(copyLinkAction, &QAction::triggered, this, [this, mediaIndex]() {
            if (mediaIndex < m_files.size())
                QApplication::clipboard()->setText(
                    resolveMxcToHttp(m_files[mediaIndex].mxcUri));
        });
    }

    menu.exec(m_filesList->viewport()->mapToGlobal(pos));
}

void MediaGalleryWidget::onLinksContextMenu(const QPoint &pos)
{
    int row = m_linksList->currentRow();
    if (row < 0 || row >= m_links.size())
        return;

    QMenu menu(this);
    QAction *copyAction = menu.addAction(tr("Copy Link"));
    QAction *openAction = menu.addAction(tr("Open in Browser"));

    connect(copyAction, &QAction::triggered, this, [this, row]() {
        if (row < m_links.size())
            QApplication::clipboard()->setText(m_links[row].fileName);
    });

    menu.exec(m_linksList->viewport()->mapToGlobal(pos));
}

QString MediaGalleryWidget::resolveMxcToHttp(const QString &mxcUri) const
{
    if (!mxcUri.startsWith(QStringLiteral("mxc://")))
        return mxcUri;

    QString serverAndId = mxcUri.mid(6);
    return QStringLiteral("%1/_matrix/media/r0/download/%2").arg(m_homeserverUrl, serverAndId);
}

void MediaGalleryWidget::showEmptyState(QLayout *layout, QLabel *&label)
{
    Q_UNUSED(layout)
    if (label) {
        label->setVisible(true);
    }
}

QString MediaGalleryWidget::formatFileSize(qint64 bytes) const
{
    if (bytes <= 0)
        return {};

    if (bytes < 1024)
        return QStringLiteral("%1 B").arg(bytes);
    if (bytes < 1024 * 1024)
        return QStringLiteral("%1 KB").arg(bytes / 1024.0, 0, 'f', 1);
    if (bytes < 1024LL * 1024 * 1024)
        return QStringLiteral("%1 MB").arg(bytes / (1024.0 * 1024.0), 0, 'f', 1);
    return QStringLiteral("%1 GB").arg(bytes / (1024.0 * 1024.0 * 1024.0), 0, 'f', 2);
}

QString MediaGalleryWidget::formatDate(qint64 ts) const
{
    if (ts <= 0)
        return {};

    QDateTime dt = QDateTime::fromMSecsSinceEpoch(ts);
    QDateTime now = QDateTime::currentDateTime();

    if (dt.date() == now.date())
        return dt.toString(QStringLiteral("HH:mm"));
    if (dt.date().year() == now.date().year())
        return dt.toString(QStringLiteral("MMM d"));
    return dt.toString(QStringLiteral("yyyy-MM-dd"));
}

QString MediaGalleryWidget::senderDisplayName(const MediaItem &item) const
{
    if (!item.senderName.isEmpty())
        return item.senderName;

    QString sid = item.senderId;
    int colonIdx = sid.indexOf(QLatin1Char(':'));
    if (colonIdx > 1 && sid.startsWith(QLatin1Char('@')))
        return sid.mid(1, colonIdx - 1);

    return sid;
}

QString MediaGalleryWidget::iconForMimeType(const QString &mimeType) const
{
    if (mimeType.startsWith(QStringLiteral("image/")))
        return QStringLiteral("\U0001F5BC");
    if (mimeType.startsWith(QStringLiteral("video/")))
        return QStringLiteral("\U0001F3AC");
    if (mimeType.startsWith(QStringLiteral("audio/")))
        return QStringLiteral("\U0001F3B5");
    if (mimeType.contains(QStringLiteral("pdf")))
        return QStringLiteral("\U0001F4C4");
    if (mimeType.contains(QStringLiteral("zip")) || mimeType.contains(QStringLiteral("tar")) ||
        mimeType.contains(QStringLiteral("rar")) || mimeType.contains(QStringLiteral("gz")))
        return QStringLiteral("\U0001F4E6");
    if (mimeType.contains(QStringLiteral("text")) || mimeType.contains(QStringLiteral("json")) ||
        mimeType.contains(QStringLiteral("xml")))
        return QStringLiteral("\U0001F4C3");
    if (mimeType.contains(QStringLiteral("spreadsheet")) || mimeType.contains(QStringLiteral("excel")) ||
        mimeType.contains(QStringLiteral("csv")))
        return QStringLiteral("\U0001F4CA");
    if (mimeType.contains(QStringLiteral("presentation")) || mimeType.contains(QStringLiteral("powerpoint")))
        return QStringLiteral("\U0001F4CA");
    return QStringLiteral("\U0001F4CE");
}
