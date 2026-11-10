#include "features/lemmy/lemmy_home_widget.hpp"
#include "app/application.hpp"
#include "lemmy/lemmy_session.hpp"
#include "lemmy/lemmy_session_holder.hpp"
#include "lemmy/lemmy_client.hpp"
#include "lemmy/lemmy_room.hpp"
#include "protocol/protocol_room.hpp"
#include "protocol/protocol_message.hpp"
#include "protocol/protocol_type.hpp"

#include <QListWidget>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QTabBar>
#include <QScrollArea>
#include <QStackedWidget>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QPixmap>
#include <QDateTime>
#include <QFont>
#include <QUrl>

static const char *kLemmyHomeStyleSheet = R"(
    LemmyHomeWidget {
        background-color: #1e1e2e;
    }
    LemmyHomeWidget QComboBox {
        background-color: #313244;
        color: #cdd6f4;
        border: 1px solid #45475a;
        border-radius: 6px;
        padding: 4px 10px;
        font-size: 12px;
        min-width: 80px;
    }
    LemmyHomeWidget QComboBox::drop-down {
        border: none;
        width: 20px;
    }
    LemmyHomeWidget QComboBox QAbstractItemView {
        background-color: #313244;
        color: #cdd6f4;
        selection-background-color: #45475a;
        border: 1px solid #45475a;
    }
    LemmyHomeWidget QPushButton {
        background-color: #313244;
        color: #cdd6f4;
        border: 1px solid #45475a;
        border-radius: 6px;
        padding: 4px 12px;
        font-size: 12px;
    }
    LemmyHomeWidget QPushButton:hover {
        background-color: #45475a;
        border-color: #89b4fa;
    }
    LemmyHomeWidget QPushButton:checked {
        background-color: #cba6f7;
        color: #1e1e2e;
        border-color: #cba6f7;
    }
    LemmyHomeWidget QPushButton#upvoteBtn {
        color: #a6e3a1;
        border: none;
        background: transparent;
        font-weight: bold;
        font-size: 14px;
        padding: 2px 4px;
    }
    LemmyHomeWidget QPushButton#downvoteBtn {
        color: #f38ba8;
        border: none;
        background: transparent;
        font-weight: bold;
        font-size: 14px;
        padding: 2px 4px;
    }
    LemmyHomeWidget QPushButton#communityBtn {
        color: #89b4fa;
        border: none;
        background: transparent;
        font-size: 12px;
        text-align: left;
        padding: 0px;
    }
    LemmyHomeWidget QPushButton#communityBtn:hover {
        color: #b4d9ff;
        text-decoration: underline;
    }
    LemmyHomeWidget QTabBar {
        background-color: #181825;
        border: none;
    }
    LemmyHomeWidget QTabBar::tab {
        background-color: #181825;
        color: #6c7086;
        border: none;
        border-bottom: 2px solid transparent;
        padding: 8px 20px;
        font-size: 13px;
        font-weight: bold;
        min-width: 80px;
    }
    LemmyHomeWidget QTabBar::tab:selected {
        color: #cba6f7;
        border-bottom: 2px solid #cba6f7;
    }
    LemmyHomeWidget QTabBar::tab:hover {
        color: #cdd6f4;
    }
    LemmyHomeWidget QListWidget {
        background-color: #1e1e2e;
        color: #cdd6f4;
        border: none;
        outline: none;
    }
    LemmyHomeWidget QListWidget::item {
        background-color: #1e1e2e;
        border-bottom: 1px solid #313244;
        padding: 0px;
    }
    LemmyHomeWidget QListWidget::item:hover {
        background-color: #2a2a3c;
    }
    LemmyHomeWidget QListWidget::item:selected {
        background-color: #2a2a3c;
    }
    LemmyHomeWidget QScrollArea {
        background-color: #1e1e2e;
        border: none;
    }
    LemmyHomeWidget QProgressBar {
        background-color: #313244;
        border: none;
        border-radius: 4px;
        height: 4px;
        text-align: center;
        font-size: 11px;
        color: #6c7086;
    }
    LemmyHomeWidget QProgressBar::chunk {
        background-color: #cba6f7;
        border-radius: 4px;
    }
)";

// ---------------------------------------------------------------------------
// Static helpers
// ---------------------------------------------------------------------------

static QString relativeTime(qint64 unixSec)
{
    if (unixSec <= 0)
        return QStringLiteral("just now");

    qint64 now = QDateTime::currentSecsSinceEpoch();
    qint64 diff = now - unixSec;

    if (diff < 0) diff = 0;
    if (diff < 60)       return QStringLiteral("just now");
    if (diff < 3600)     return QStringLiteral("%1m").arg(diff / 60);
    if (diff < 86400)    return QStringLiteral("%1h").arg(diff / 3600);
    if (diff < 2592000)  return QStringLiteral("%1d").arg(diff / 86400);
    if (diff < 31536000) return QStringLiteral("%1mo").arg(diff / 2592000);

    return QStringLiteral("%1y").arg(diff / 31536000);
}

static qint64 parseTimestampSec(const QString &publishedStr)
{
    if (publishedStr.isEmpty())
        return 0;

    QDateTime dt = QDateTime::fromString(publishedStr, Qt::ISODate);
    if (!dt.isValid())
        dt = QDateTime::fromString(publishedStr, Qt::ISODateWithMs);

    return dt.isValid() ? dt.toSecsSinceEpoch() : 0;
}

static QString extractDomain(const QString &url)
{
    if (url.isEmpty())
        return QString();
    QUrl parsed(url);
    QString host = parsed.host();
    if (host.startsWith(QStringLiteral("www.")))
        host = host.mid(4);
    return host;
}

static QString elideLines(const QString &text, int maxLines)
{
    QStringList lines = text.split(QChar('\n'));
    if (lines.size() <= maxLines)
        return text;

    QString result;
    for (int i = 0; i < maxLines; ++i)
        result += lines[i] + QChar('\n');
    result.chop(1);
    return result + QStringLiteral("…");
}

static QPixmap makeCommunityAvatar(const QString &name, int size)
{
    QPixmap pix(size, size);
    pix.fill(Qt::transparent);

    QPainter painter(&pix);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect r(0, 0, size, size);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0xcba6f7));
    painter.drawRoundedRect(r, size / 4, size / 4);

    QString letter;
    if (!name.isEmpty())
        letter = name.at(0).toUpper();
    else
        letter = QStringLiteral("?");

    QFont font;
    font.setPixelSize(size / 2 + 2);
    font.setBold(true);
    painter.setFont(font);
    painter.setPen(Qt::white);
    painter.drawText(r, Qt::AlignCenter, letter);

    painter.end();
    return pix;
}

// ---------------------------------------------------------------------------
// Constructor / Destructor
// ---------------------------------------------------------------------------

LemmyHomeWidget::LemmyHomeWidget(QWidget *parent)
    : QWidget(parent)
    , m_instanceSelector(nullptr)
    , m_sortSelector(nullptr)
    , m_refreshButton(nullptr)
    , m_tabBar(nullptr)
    , m_stack(nullptr)
    , m_postsList(nullptr)
    , m_postsLoading(nullptr)
    , m_postsEmptyLabel(nullptr)
    , m_communitiesList(nullptr)
    , m_communitiesLoading(nullptr)
    , m_communitiesEmptyLabel(nullptr)
    , m_filterCommunityId(0)
    , m_votePostId(0)
    , m_currentSession(nullptr)
{
    m_currentSort = QStringLiteral("Active");
    setStyleSheet(QString::fromLatin1(kLemmyHomeStyleSheet));
    setupUi();
}

LemmyHomeWidget::~LemmyHomeWidget() = default;

// ---------------------------------------------------------------------------
// UI Setup
// ---------------------------------------------------------------------------

void LemmyHomeWidget::setupUi()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // --- Top bar ---
    auto *topBar = new QHBoxLayout();
    topBar->setContentsMargins(8, 8, 8, 4);
    topBar->setSpacing(6);

    m_instanceSelector = new QComboBox(this);
    m_instanceSelector->setMinimumWidth(180);
    m_instanceSelector->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    topBar->addWidget(m_instanceSelector, 1);

    m_sortSelector = new QComboBox(this);
    m_sortSelector->addItem(QStringLiteral("Active"));
    m_sortSelector->addItem(QStringLiteral("Hot"));
    m_sortSelector->addItem(QStringLiteral("New"));
    m_sortSelector->addItem(QStringLiteral("Top Day"));
    m_sortSelector->addItem(QStringLiteral("Top Week"));
    m_sortSelector->addItem(QStringLiteral("Top Month"));
    m_sortSelector->addItem(QStringLiteral("Top Year"));
    m_sortSelector->setCurrentIndex(0);
    topBar->addWidget(m_sortSelector);

    m_refreshButton = new QPushButton(tr("Refresh"), this);
    topBar->addWidget(m_refreshButton);

    mainLayout->addLayout(topBar);

    // --- Tab bar ---
    m_tabBar = new QTabBar(this);
    m_tabBar->addTab(tr("Posts"));
    m_tabBar->addTab(tr("Communities"));
    mainLayout->addWidget(m_tabBar);

    // --- Stacked widget ---
    m_stack = new QStackedWidget(this);

    // Posts page
    auto *postsPage = new QWidget();
    auto *postsLayout = new QVBoxLayout(postsPage);
    postsLayout->setContentsMargins(0, 0, 0, 0);
    postsLayout->setSpacing(0);

    m_postsLoading = new QProgressBar(this);
    m_postsLoading->setRange(0, 0);
    m_postsLoading->setFixedHeight(4);
    m_postsLoading->setVisible(false);
    postsLayout->addWidget(m_postsLoading);

    m_postsEmptyLabel = new QLabel(tr("No posts to show"), this);
    m_postsEmptyLabel->setAlignment(Qt::AlignCenter);
    m_postsEmptyLabel->setStyleSheet(QStringLiteral(
        "color: #6c7086; font-size: 14px; padding: 40px;"));
    m_postsEmptyLabel->setVisible(false);
    postsLayout->addWidget(m_postsEmptyLabel);

    m_postsList = new QListWidget(this);
    m_postsList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_postsList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_postsList->setSelectionMode(QAbstractItemView::SingleSelection);
    m_postsList->setSpacing(4);
    postsLayout->addWidget(m_postsList, 1);

    m_stack->addWidget(postsPage);

    // Communities page
    auto *communitiesPage = new QWidget();
    auto *communitiesLayout = new QVBoxLayout(communitiesPage);
    communitiesLayout->setContentsMargins(0, 0, 0, 0);
    communitiesLayout->setSpacing(0);

    m_communitiesLoading = new QProgressBar(this);
    m_communitiesLoading->setRange(0, 0);
    m_communitiesLoading->setFixedHeight(4);
    m_communitiesLoading->setVisible(false);
    communitiesLayout->addWidget(m_communitiesLoading);

    m_communitiesEmptyLabel = new QLabel(tr("No communities to show"), this);
    m_communitiesEmptyLabel->setAlignment(Qt::AlignCenter);
    m_communitiesEmptyLabel->setStyleSheet(QStringLiteral(
        "color: #6c7086; font-size: 14px; padding: 40px;"));
    m_communitiesEmptyLabel->setVisible(false);
    communitiesLayout->addWidget(m_communitiesEmptyLabel);

    m_communitiesList = new QListWidget(this);
    m_communitiesList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_communitiesList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_communitiesList->setSelectionMode(QAbstractItemView::SingleSelection);
    m_communitiesList->setSpacing(4);
    communitiesLayout->addWidget(m_communitiesList, 1);

    m_stack->addWidget(communitiesPage);

    mainLayout->addWidget(m_stack, 1);

    // --- Connections ---
    connect(m_instanceSelector, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &LemmyHomeWidget::onInstanceChanged);
    connect(m_sortSelector, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &LemmyHomeWidget::onSortChanged);
    connect(m_tabBar, &QTabBar::currentChanged,
            this, &LemmyHomeWidget::onTabChanged);
    connect(m_refreshButton, &QPushButton::clicked,
            this, &LemmyHomeWidget::onRefreshClicked);
    connect(m_postsList, &QListWidget::itemActivated,
            this, &LemmyHomeWidget::onPostItemActivated);
    connect(m_communitiesList, &QListWidget::itemActivated,
            this, &LemmyHomeWidget::onCommunityItemActivated);

    // Populate instances
    populateInstanceSelector();

    // Default to posts tab
    m_tabBar->setCurrentIndex(0);
    m_stack->setCurrentIndex(0);
}

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------

void LemmyHomeWidget::setActiveInstance(const QString &instanceUrl)
{
    if (m_activeInstanceUrl == instanceUrl)
        return;

    m_activeInstanceUrl = instanceUrl;

    int idx = m_instanceSelector->findData(instanceUrl);
    if (idx >= 0)
        m_instanceSelector->setCurrentIndex(idx);

    m_currentSession = pcp::Application::instance()
                           .lemmySessionHolder()
                           ->getSession(instanceUrl);

    refreshPosts();
}

void LemmyHomeWidget::refreshPosts()
{
    showLoading(true, QString(), true);

    if (!m_currentSession) {
        m_posts.clear();
        populatePostsList();
        showLoading(false, QString(), true);
        return;
    }

    // Determine sort mapping for LemmyClient
    static const QMap<QString, QString> sortMap = {
        {QStringLiteral("Active"),    QStringLiteral("Active")},
        {QStringLiteral("Hot"),       QStringLiteral("Hot")},
        {QStringLiteral("New"),       QStringLiteral("New")},
        {QStringLiteral("Top Day"),   QStringLiteral("TopDay")},
        {QStringLiteral("Top Week"),  QStringLiteral("TopWeek")},
        {QStringLiteral("Top Month"), QStringLiteral("TopMonth")},
        {QStringLiteral("Top Year"),  QStringLiteral("TopYear")},
    };
    QString lemmySort = sortMap.value(m_currentSort, QStringLiteral("Active"));

    LemmySession *session = m_currentSession;

    QVector<ProtocolMessage> msgs;
    if (m_filterCommunityId > 0) {
        msgs = session->getMessages(QString::number(m_filterCommunityId));
    } else {
        msgs = session->getMessages(QStringLiteral("0"));
    }

    // Convert ProtocolMessages back to LemmyPosts via our stored metadata
    m_posts.clear();
    for (const auto &msg : msgs) {
        LemmyPost post;
        post.id = msg.id;
        post.name = msg.senderName;
        post.body = msg.text;
        post.communityName = msg.metadata.value(QStringLiteral("communityName")).toString();
        post.communityId = msg.metadata.value(QStringLiteral("communityId")).toInteger();
        post.creatorName = msg.senderId;
        post.score = msg.metadata.value(QStringLiteral("score")).toInteger();
        post.upvotes = msg.metadata.value(QStringLiteral("upvotes")).toInteger();
        post.downvotes = msg.metadata.value(QStringLiteral("downvotes")).toInteger();
        post.comments = msg.metadata.value(QStringLiteral("comments")).toInteger();
        post.published = msg.metadata.value(QStringLiteral("published")).toString();
        post.nsfw = msg.metadata.value(QStringLiteral("nsfw")).toBool();
        post.locked = msg.metadata.value(QStringLiteral("locked")).toBool();
        post.featuredCommunity = msg.metadata.value(QStringLiteral("featuredCommunity")).toBool();
        post.url = msg.metadata.value(QStringLiteral("url")).toString();
        post.embedTitle = msg.metadata.value(QStringLiteral("embedTitle")).toString();
        m_posts.append(post);
    }

    populatePostsList();
    showLoading(false, QString(), true);
}

void LemmyHomeWidget::refreshCommunities()
{
    showLoading(false, QString(), false);

    if (!m_currentSession) {
        m_communities.clear();
        populateCommunitiesList();
        showLoading(false, QString(), false);
        return;
    }

    m_communities = m_currentSession->getRooms();

    showLoading(false, QString(), false);
    populateCommunitiesList();
}

// ---------------------------------------------------------------------------
// Private slots
// ---------------------------------------------------------------------------

void LemmyHomeWidget::onInstanceChanged(int index)
{
    QString url = m_instanceSelector->itemData(index).toString();
    if (url.isEmpty())
        return;

    m_activeInstanceUrl = url;
    m_currentSession = pcp::Application::instance()
                           .lemmySessionHolder()
                           ->getSession(url);
    m_filterCommunityId = 0;

    if (m_tabBar->currentIndex() == 0)
        refreshPosts();
    else
        refreshCommunities();
}

void LemmyHomeWidget::onSortChanged(int index)
{
    static const QStringList sorts = {
        QStringLiteral("Active"),
        QStringLiteral("Hot"),
        QStringLiteral("New"),
        QStringLiteral("Top Day"),
        QStringLiteral("Top Week"),
        QStringLiteral("Top Month"),
        QStringLiteral("Top Year"),
    };

    if (index >= 0 && index < sorts.size())
        m_currentSort = sorts[index];

    if (m_tabBar->currentIndex() == 0)
        refreshPosts();
}

void LemmyHomeWidget::onTabChanged(int index)
{
    m_stack->setCurrentIndex(index);

    if (index == 0) {
        refreshPosts();
    } else {
        refreshCommunities();
    }
}

void LemmyHomeWidget::onRefreshClicked()
{
    if (m_tabBar->currentIndex() == 0)
        refreshPosts();
    else
        refreshCommunities();
}

void LemmyHomeWidget::onPostItemActivated(QListWidgetItem *item)
{
    if (!item)
        return;

    bool ok = false;
    int idx = item->data(static_cast<int>(PostIndexRole)).toInt(&ok);
    if (ok && idx >= 0 && idx < m_posts.size()) {
        const LemmyPost &post = m_posts[idx];
        emit postSelected(post.id);
    }
}

void LemmyHomeWidget::onCommunityItemActivated(QListWidgetItem *item)
{
    if (!item)
        return;

    bool ok = false;
    int idx = item->data(static_cast<int>(CommunityIndexRole)).toInt(&ok);
    if (ok && idx >= 0 && idx < m_communities.size()) {
        const ProtocolRoom &room = m_communities[idx];
        qint64 communityId = room.metadata.value(
            QStringLiteral("communityId")).toInteger();
        m_filterCommunityId = communityId;

        // Switch to posts tab showing filtered community
        m_tabBar->setCurrentIndex(0);
        refreshPosts();

        emit communitySelected(communityId);
    }
}

void LemmyHomeWidget::onUpvoteClicked()
{
    if (!m_currentSession || m_votePostId <= 0)
        return;

    LemmySession *session = m_currentSession;
    session->sendMessage(
        QStringLiteral("like:%1").arg(m_votePostId),
        QStringLiteral("1"),
        ProtocolContentType::ACTION);
}

void LemmyHomeWidget::onDownvoteClicked()
{
    if (!m_currentSession || m_votePostId <= 0)
        return;

    LemmySession *session = m_currentSession;
    session->sendMessage(
        QStringLiteral("like:%1").arg(m_votePostId),
        QStringLiteral("-1"),
        ProtocolContentType::ACTION);
}

void LemmyHomeWidget::onCommunityClicked()
{
    QPushButton *btn = qobject_cast<QPushButton *>(sender());
    if (!btn)
        return;

    bool ok = false;
    qint64 communityId = btn->property("communityId").toLongLong(&ok);
    if (ok && communityId > 0) {
        m_filterCommunityId = communityId;
        m_tabBar->setCurrentIndex(0);
        refreshPosts();
        emit communitySelected(communityId);
    }
}

// ---------------------------------------------------------------------------
// Populate lists
// ---------------------------------------------------------------------------

void LemmyHomeWidget::populateInstanceSelector()
{
    m_instanceSelector->blockSignals(true);
    m_instanceSelector->clear();

    auto *holder = pcp::Application::instance().lemmySessionHolder();
    if (!holder) {
        m_instanceSelector->blockSignals(false);
        return;
    }

    QVector<LemmySession *> sessions = holder->getAllSessions();
    for (LemmySession *s : sessions) {
        if (s && s->connectionState() == ConnectionState::CONNECTED) {
            QString url = s->userId();
            m_instanceSelector->addItem(url, url);
        }
    }

    if (m_instanceSelector->count() == 0) {
        m_instanceSelector->addItem(tr("No instances connected"));
    }

    if (!m_activeInstanceUrl.isEmpty()) {
        int idx = m_instanceSelector->findData(m_activeInstanceUrl);
        if (idx >= 0)
            m_instanceSelector->setCurrentIndex(idx);
    } else if (m_instanceSelector->count() > 0 &&
               m_instanceSelector->itemData(0).isValid()) {
        m_activeInstanceUrl = m_instanceSelector->itemData(0).toString();
        m_currentSession = holder->getSession(m_activeInstanceUrl);
    }

    m_instanceSelector->blockSignals(false);
}

void LemmyHomeWidget::populatePostsList()
{
    m_postsList->clear();

    if (m_posts.isEmpty()) {
        m_postsEmptyLabel->setVisible(true);
        m_postsList->setVisible(false);
        return;
    }

    m_postsEmptyLabel->setVisible(false);
    m_postsList->setVisible(true);

    for (int i = 0; i < m_posts.size(); ++i) {
        auto *item = new QListWidgetItem();
        item->setData(static_cast<int>(PostIndexRole), i);
        item->setSizeHint(QSize(0, 140));

        QWidget *card = createPostCardWidget(m_posts[i], i);
        m_postsList->addItem(item);
        m_postsList->setItemWidget(item, card);
    }
}

void LemmyHomeWidget::populateCommunitiesList()
{
    m_communitiesList->clear();

    if (m_communities.isEmpty()) {
        m_communitiesEmptyLabel->setVisible(true);
        m_communitiesList->setVisible(false);
        return;
    }

    m_communitiesEmptyLabel->setVisible(false);
    m_communitiesList->setVisible(true);

    for (int i = 0; i < m_communities.size(); ++i) {
        auto *item = new QListWidgetItem();
        item->setData(static_cast<int>(CommunityIndexRole), i);
        item->setSizeHint(QSize(0, 60));

        QWidget *card = createCommunityCardWidget(m_communities[i], i);
        m_communitiesList->addItem(item);
        m_communitiesList->setItemWidget(item, card);
    }
}

// ---------------------------------------------------------------------------
// Card factories
// ---------------------------------------------------------------------------

QWidget *LemmyHomeWidget::createPostCardWidget(const LemmyPost &post, int dataIndex)
{
    Q_UNUSED(dataIndex)

    auto *container = new QWidget();
    container->setStyleSheet(QStringLiteral("background: transparent;"));

    auto *card = new QVBoxLayout(container);
    card->setContentsMargins(12, 10, 12, 10);
    card->setSpacing(4);

    // --- Row 1: Community name + creator ---
    auto *headerRow = new QHBoxLayout();
    headerRow->setSpacing(6);

    auto *communityBtn = new QPushButton(
        QStringLiteral("c/") + post.communityName, container);
    communityBtn->setObjectName(QStringLiteral("communityBtn"));
    communityBtn->setCursor(Qt::PointingHandCursor);
    communityBtn->setProperty("communityId", post.communityId);
    communityBtn->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
    connect(communityBtn, &QPushButton::clicked,
            this, &LemmyHomeWidget::onCommunityClicked);
    headerRow->addWidget(communityBtn);

    auto *byLabel = new QLabel(
        tr("by ") + post.creatorName, container);
    byLabel->setStyleSheet(QStringLiteral(
        "color: #6c7086; font-size: 11px; background: transparent;"));
    headerRow->addWidget(byLabel);

    // Badges
    if (post.nsfw) {
        auto *nsfwBadge = new QLabel(tr("NSFW"), container);
        nsfwBadge->setStyleSheet(QStringLiteral(
            "background-color: #f38ba8; color: #1e1e2e; "
            "border-radius: 4px; padding: 1px 6px; font-size: 10px; font-weight: bold;"));
        nsfwBadge->setFixedHeight(18);
        headerRow->addWidget(nsfwBadge);
    }
    if (post.locked) {
        auto *lockedBadge = new QLabel(tr("LOCKED"), container);
        lockedBadge->setStyleSheet(QStringLiteral(
            "background-color: #f9e2af; color: #1e1e2e; "
            "border-radius: 4px; padding: 1px 6px; font-size: 10px; font-weight: bold;"));
        lockedBadge->setFixedHeight(18);
        headerRow->addWidget(lockedBadge);
    }
    if (post.featuredCommunity) {
        auto *pinnedBadge = new QLabel(tr("PINNED"), container);
        pinnedBadge->setStyleSheet(QStringLiteral(
            "background-color: #a6e3a1; color: #1e1e2e; "
            "border-radius: 4px; padding: 1px 6px; font-size: 10px; font-weight: bold;"));
        pinnedBadge->setFixedHeight(18);
        headerRow->addWidget(pinnedBadge);
    }

    headerRow->addStretch();
    card->addLayout(headerRow);

    // --- Row 2: Post title (bold, 2 lines max) ---
    QString titleText = post.name;
    if (titleText.isEmpty())
        titleText = tr("(untitled)");

    auto *titleLabel = new QLabel(titleText, container);
    titleLabel->setStyleSheet(QStringLiteral(
        "color: #cdd6f4; font-weight: bold; font-size: 14px; background: transparent;"));
    titleLabel->setWordWrap(true);
    titleLabel->setMaximumHeight(titleLabel->fontMetrics().lineSpacing() * 2 + 4);
    titleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    card->addWidget(titleLabel);

    // --- Row 3: Body preview or URL preview ---
    if (!post.url.isEmpty()) {
        auto *urlRow = new QHBoxLayout();
        urlRow->setSpacing(4);

        auto *linkIcon = new QLabel(QStringLiteral("\xF0\x9F\x94\x97"), container);
        linkIcon->setStyleSheet(QStringLiteral(
            "font-size: 12px; background: transparent;"));
        urlRow->addWidget(linkIcon);

        QString domain = extractDomain(post.url);
        auto *domainLabel = new QLabel(domain, container);
        domainLabel->setStyleSheet(QStringLiteral(
            "color: #89b4fa; font-size: 12px; background: transparent;"));
        urlRow->addWidget(domainLabel);

        if (!post.embedTitle.isEmpty()) {
            auto *embedLabel = new QLabel(post.embedTitle, container);
            embedLabel->setStyleSheet(QStringLiteral(
                "color: #6c7086; font-size: 12px; background: transparent;"));
            embedLabel->setWordWrap(true);
            urlRow->addWidget(embedLabel, 1);
        } else {
            urlRow->addStretch();
        }
        card->addLayout(urlRow);
    } else if (!post.body.isEmpty()) {
        QString preview = elideLines(post.body, 3);
        auto *bodyLabel = new QLabel(preview, container);
        bodyLabel->setStyleSheet(QStringLiteral(
            "color: #9399b2; font-size: 12px; background: transparent;"));
        bodyLabel->setWordWrap(true);
        bodyLabel->setMaximumHeight(
            bodyLabel->fontMetrics().lineSpacing() * 3 + 4);
        bodyLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        card->addWidget(bodyLabel);
    }

    // --- Row 4: Score, comments, time ---
    auto *footerRow = new QHBoxLayout();
    footerRow->setSpacing(8);

    // Upvote button
    auto *upvoteBtn = new QPushButton(QStringLiteral("\xE2\x96\xB2"), container);
    upvoteBtn->setObjectName(QStringLiteral("upvoteBtn"));
    upvoteBtn->setToolTip(tr("Upvote"));
    upvoteBtn->setCursor(Qt::PointingHandCursor);
    upvoteBtn->setFixedSize(24, 24);
    connect(upvoteBtn, &QPushButton::clicked, this, [this, post]() {
        m_votePostId = post.id;
        onUpvoteClicked();
    });
    footerRow->addWidget(upvoteBtn);

    // Score
    auto *scoreLabel = new QLabel(
        QString::number(post.score), container);
    scoreLabel->setStyleSheet(QStringLiteral(
        "color: #cdd6f4; font-size: 12px; font-weight: bold; background: transparent;"));
    scoreLabel->setAlignment(Qt::AlignCenter);
    scoreLabel->setFixedWidth(30);
    footerRow->addWidget(scoreLabel);

    // Downvote button
    auto *downvoteBtn = new QPushButton(QStringLiteral("\xE2\x96\xBC"), container);
    downvoteBtn->setObjectName(QStringLiteral("downvoteBtn"));
    downvoteBtn->setToolTip(tr("Downvote"));
    downvoteBtn->setCursor(Qt::PointingHandCursor);
    downvoteBtn->setFixedSize(24, 24);
    connect(downvoteBtn, &QPushButton::clicked, this, [this, post]() {
        m_votePostId = post.id;
        onDownvoteClicked();
    });
    footerRow->addWidget(downvoteBtn);

    footerRow->addSpacing(8);

    // Comment icon + count
    auto *commentLabel = new QLabel(
        QStringLiteral("\xF0\x9F\x92\xAC ") + QString::number(post.comments),
        container);
    commentLabel->setStyleSheet(QStringLiteral(
        "color: #6c7086; font-size: 12px; background: transparent;"));
    footerRow->addWidget(commentLabel);

    footerRow->addStretch();

    // Relative time
    qint64 sec = parseTimestampSec(post.published);
    QString timeStr = relativeTime(sec);
    auto *timeLabel = new QLabel(timeStr, container);
    timeLabel->setStyleSheet(QStringLiteral(
        "color: #6c7086; font-size: 11px; background: transparent;"));
    footerRow->addWidget(timeLabel);

    card->addLayout(footerRow);

    return container;
}

QWidget *LemmyHomeWidget::createCommunityCardWidget(const ProtocolRoom &room, int dataIndex)
{
    Q_UNUSED(dataIndex)

    auto *container = new QWidget();
    container->setStyleSheet(QStringLiteral("background: transparent;"));

    auto *row = new QHBoxLayout(container);
    row->setContentsMargins(12, 8, 12, 8);
    row->setSpacing(10);

    // Icon
    QPixmap avatar = makeCommunityAvatar(room.name, 40);
    auto *avatarLabel = new QLabel(container);
    avatarLabel->setPixmap(avatar);
    avatarLabel->setFixedSize(40, 40);
    row->addWidget(avatarLabel);

    // Name + title
    auto *textCol = new QVBoxLayout();
    textCol->setSpacing(2);

    auto *nameLabel = new QLabel(room.name, container);
    nameLabel->setStyleSheet(QStringLiteral(
        "color: #cdd6f4; font-weight: bold; font-size: 14px; background: transparent;"));
    textCol->addWidget(nameLabel);

    QString title = room.topic.isEmpty() ? room.name : room.topic;
    auto *titleLabel = new QLabel(title, container);
    titleLabel->setStyleSheet(QStringLiteral(
        "color: #6c7086; font-size: 12px; background: transparent;"));
    titleLabel->setWordWrap(false);
    textCol->addWidget(titleLabel);

    row->addLayout(textCol, 1);

    // Subscriber count
    QString subText = QString::number(room.memberCount) + tr(" subscribers");
    auto *subLabel = new QLabel(subText, container);
    subLabel->setStyleSheet(QStringLiteral(
        "color: #9399b2; font-size: 11px; background: transparent;"));
    subLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    row->addWidget(subLabel);

    return container;
}

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

void LemmyHomeWidget::showLoading(bool loading, const QString &message, bool isPosts)
{
    Q_UNUSED(message)

    if (isPosts) {
        m_postsLoading->setVisible(loading);
    } else {
        m_communitiesLoading->setVisible(loading);
    }
}

void LemmyHomeWidget::updateVoteButtons(const LemmyPost &post)
{
    Q_UNUSED(post)
    // Re-rendered on refresh; no persistent toggle state in this implementation
}
