#include "features/lemmy/lemmy_post_detail_widget.hpp"

#include "app/application.hpp"
#include "lemmy/lemmy_client.hpp"
#include "lemmy/lemmy_session.hpp"
#include "lemmy/lemmy_session_holder.hpp"

#include <QApplication>
#include <QDateTime>
#include <QFont>
#include <QFrame>
#include <QHBoxLayout>
#include <QJsonArray>
#include <QJsonObject>
#include <QLabel>
#include <QPushButton>
#include <QRegularExpression>
#include <QScrollArea>
#include <QScrollBar>
#include <QSizePolicy>
#include <QTextEdit>
#include <QVBoxLayout>

#include <utility>

const QStringList LemmyPostDetailWidget::kDepthColors = {
    QStringLiteral("#4CAF50"),
    QStringLiteral("#2196F3"),
    QStringLiteral("#FF9800"),
    QStringLiteral("#9C27B0"),
    QStringLiteral("#F44336"),
    QStringLiteral("#00BCD4"),
};

LemmyPostDetailWidget::LemmyPostDetailWidget(QWidget *parent)
    : QWidget(parent)
    , m_headerWidget(nullptr)
    , m_headerLayout(nullptr)
    , m_commentScrollArea(nullptr)
    , m_commentContainer(nullptr)
    , m_commentLayout(nullptr)
    , m_replyBar(nullptr)
    , m_replyInput(nullptr)
    , m_submitButton(nullptr)
    , m_backButton(nullptr)
    , m_postId(0)
{
    setupUi();
}

LemmyPostDetailWidget::~LemmyPostDetailWidget() = default;

void LemmyPostDetailWidget::setupUi()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    auto *topBar = new QHBoxLayout();
    topBar->setContentsMargins(8, 6, 8, 6);
    m_backButton = new QPushButton(QStringLiteral("\xE2\x86\x90  ") + tr("Back"), this);
    m_backButton->setFlat(true);
    m_backButton->setCursor(Qt::PointingHandCursor);
    m_backButton->setStyleSheet(QStringLiteral(
        "QPushButton { color: #2b579a; font-weight: bold; border: none; padding: 4px 8px; }"
        "QPushButton:hover { color: #1e4278; text-decoration: underline; }"));
    topBar->addWidget(m_backButton);
    topBar->addStretch();
    mainLayout->addLayout(topBar);

    m_headerWidget = new QWidget(this);
    m_headerLayout = new QVBoxLayout(m_headerWidget);
    m_headerLayout->setContentsMargins(12, 4, 12, 8);
    m_headerLayout->setSpacing(4);

    mainLayout->addWidget(m_headerWidget);

    auto *separator = new QFrame(this);
    separator->setFrameShape(QFrame::HLine);
    separator->setStyleSheet(QStringLiteral("QFrame { color: #ccc; }"));
    separator->setMinimumHeight(2);
    mainLayout->addWidget(separator);

    m_commentScrollArea = new QScrollArea(this);
    m_commentScrollArea->setWidgetResizable(true);
    m_commentScrollArea->setFrameShape(QFrame::NoFrame);
    m_commentScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_commentScrollArea->setStyleSheet(QStringLiteral("QScrollArea { background-color: #fafafa; }"));

    m_commentContainer = new QWidget();
    m_commentContainer->setStyleSheet(QStringLiteral("background-color: #fafafa;"));
    m_commentLayout = new QVBoxLayout(m_commentContainer);
    m_commentLayout->setContentsMargins(8, 8, 8, 8);
    m_commentLayout->setSpacing(0);
    m_commentLayout->addStretch(1);

    m_commentScrollArea->setWidget(m_commentContainer);
    mainLayout->addWidget(m_commentScrollArea, 1);

    m_replyBar = new QWidget(this);
    auto *replyLayout = new QHBoxLayout(m_replyBar);
    replyLayout->setContentsMargins(8, 6, 8, 8);
    replyLayout->setSpacing(6);

    m_replyInput = new QTextEdit(this);
    m_replyInput->setPlaceholderText(tr("Write a comment..."));
    m_replyInput->setMaximumHeight(100);
    m_replyInput->setMinimumHeight(36);
    m_replyInput->setAcceptRichText(false);
    m_replyInput->setStyleSheet(QStringLiteral(
        "QTextEdit { border: 1px solid #ccc; border-radius: 8px; "
        "padding: 6px 10px; font-size: 13px; }"
        "QTextEdit:focus { border-color: #2b579a; }"));
    replyLayout->addWidget(m_replyInput, 1);

    m_submitButton = new QPushButton(tr("Submit"), this);
    m_submitButton->setMinimumWidth(72);
    m_submitButton->setMinimumHeight(36);
    m_submitButton->setStyleSheet(QStringLiteral(
        "QPushButton { background-color: #2b579a; color: white; border: none; "
        "border-radius: 8px; padding: 6px 16px; font-weight: bold; font-size: 13px; }"
        "QPushButton:hover { background-color: #3a6bb5; }"
        "QPushButton:pressed { background-color: #1e4278; }"
        "QPushButton:disabled { background-color: #ccc; }"));
    replyLayout->addWidget(m_submitButton);

    m_replyBar->hide();
    mainLayout->addWidget(m_replyBar);

    connect(m_backButton, &QPushButton::clicked, this, &LemmyPostDetailWidget::backRequested);
    connect(m_submitButton, &QPushButton::clicked, this, [this]() {
        const QString text = m_replyInput->toPlainText().trimmed();
        if (text.isEmpty()) {
            return;
        }
        submitComment(0, text);
    });
}

void LemmyPostDetailWidget::loadPost(qint64 postId, QString instanceUrl)
{
    m_postId = postId;
    m_instanceUrl = std::move(instanceUrl);
    m_postVote = 0;
    m_commentVotes.clear();
    m_collapsed.clear();
    m_commentContainers.clear();
    m_commentPage = 1;

    auto *session = pcp::Application::instance().lemmySessionHolder()->getSession(m_instanceUrl);
    if (!session) {
        return;
    }

    auto *client = session->findChild<LemmyClient *>(QString(), Qt::FindDirectChildrenOnly);
    if (!client) {
        return;
    }

    QJsonObject postResp = client->getPost(static_cast<int>(m_postId));
    QJsonObject postView = postResp.value(QStringLiteral("post_view")).toObject();
    m_currentPost = LemmyPost::fromJson(postView);

    buildPostHeader();

    QJsonObject commentsResp = client->getComments(static_cast<int>(m_postId),
                                                    QStringLiteral("Hot"), m_commentPage, 50);
    QJsonArray commentsArray = commentsResp.value(QStringLiteral("comments")).toArray();

    m_rootComments.clear();
    m_children.clear();

    for (const QJsonValue &val : commentsArray) {
        LemmyComment comment = LemmyComment::fromJson(val.toObject());
        if (comment.removed || comment.deleted) {
            comment.content = comment.removed ? tr("[removed]") : tr("[deleted]");
        }
        m_children[comment.parentId].append(std::move(comment));
    }

    m_rootComments = m_children.value(0);

    buildCommentTree();
    m_replyBar->show();
}

void LemmyPostDetailWidget::buildPostHeader()
{
    while (m_headerLayout->count() > 0) {
        QLayoutItem *item = m_headerLayout->takeAt(0);
        if (auto *w = item->widget()) {
            w->setParent(nullptr);
            w->deleteLater();
        }
        delete item;
    }

    auto *separator = new QFrame(m_headerWidget);
    separator->setFrameShape(QFrame::HLine);
    separator->setStyleSheet(QStringLiteral("QFrame { color: #ddd; }"));
    m_headerLayout->addWidget(separator);

    auto *titleLabel = new QLabel(m_currentPost.name, m_headerWidget);
    titleLabel->setWordWrap(true);
    titleLabel->setStyleSheet(QStringLiteral(
        "QLabel { font-size: 18px; font-weight: bold; color: #222; padding-bottom: 4px; }"));
    m_headerLayout->addWidget(titleLabel);

    auto *metaContainer = new QWidget(m_headerWidget);
    auto *metaRow = new QHBoxLayout(metaContainer);
    metaRow->setContentsMargins(0, 0, 0, 0);
    metaRow->setSpacing(8);

    QString initials = initialsFromName(m_currentPost.creatorName);
    QString color = avatarColorForName(m_currentPost.creatorName);

    auto *avatarLabel = new QLabel(initials, metaContainer);
    avatarLabel->setFixedSize(24, 24);
    avatarLabel->setAlignment(Qt::AlignCenter);
    avatarLabel->setStyleSheet(QStringLiteral(
        "QLabel { background-color: %1; color: white; border-radius: 12px; "
        "font-weight: bold; font-size: 11px; }").arg(color));
    metaRow->addWidget(avatarLabel);

    auto *communityLabel = new QLabel(
        QStringLiteral("c/%1").arg(m_currentPost.communityName), metaContainer);
    communityLabel->setStyleSheet(QStringLiteral(
        "QLabel { color: #2b579a; font-weight: bold; font-size: 13px; }"));
    metaRow->addWidget(communityLabel);

    auto *byLabel = new QLabel(
        tr("by %1").arg(m_currentPost.creatorName), metaContainer);
    byLabel->setStyleSheet(QStringLiteral(
        "QLabel { color: #888; font-size: 12px; }"));
    metaRow->addWidget(byLabel);
    metaRow->addStretch();
    m_headerLayout->addWidget(metaContainer);

    bool hasFlags = m_currentPost.nsfw || m_currentPost.locked ||
                    m_currentPost.featuredLocal || m_currentPost.featuredCommunity;
    if (hasFlags) {
        auto *flagContainer = new QWidget(m_headerWidget);
        auto *flagLayout = new QHBoxLayout(flagContainer);
        flagLayout->setContentsMargins(0, 0, 0, 0);
        flagLayout->setSpacing(8);

        if (m_currentPost.nsfw) {
            auto *nsfwLabel = new QLabel(tr("NSFW"), flagContainer);
            nsfwLabel->setStyleSheet(QStringLiteral(
                "QLabel { background-color: #d32f2f; color: white; font-weight: bold; "
                "font-size: 11px; padding: 2px 8px; border-radius: 4px; }"));
            flagLayout->addWidget(nsfwLabel);
        }
        if (m_currentPost.locked) {
            auto *lockedLabel = new QLabel(tr("\xF0\x9F\x94\x92 Locked"), flagContainer);
            lockedLabel->setStyleSheet(QStringLiteral(
                "QLabel { color: #f57c00; font-weight: bold; font-size: 11px; "
                "padding: 2px 8px; border: 1px solid #f57c00; border-radius: 4px; }"));
            flagLayout->addWidget(lockedLabel);
        }
        if (m_currentPost.featuredLocal || m_currentPost.featuredCommunity) {
            auto *featuredLabel = new QLabel(
                m_currentPost.featuredLocal ? tr("Featured (Local)") : tr("Featured"),
                flagContainer);
            featuredLabel->setStyleSheet(QStringLiteral(
                "QLabel { background-color: #388e3c; color: white; font-weight: bold; "
                "font-size: 11px; padding: 2px 8px; border-radius: 4px; }"));
            flagLayout->addWidget(featuredLabel);
        }
        flagLayout->addStretch();
        m_headerLayout->addWidget(flagContainer);
    }

    auto *bodyWidget = createPostBodyWidget();
    if (bodyWidget) {
        m_headerLayout->addWidget(bodyWidget);
    }

    auto *metaWidget = createPostMetaWidget();
    m_headerLayout->addWidget(metaWidget);

    auto *bottomSep = new QFrame(m_headerWidget);
    bottomSep->setFrameShape(QFrame::HLine);
    bottomSep->setStyleSheet(QStringLiteral("QFrame { color: #ddd; }"));
    m_headerLayout->addWidget(bottomSep);
}

QWidget *LemmyPostDetailWidget::createPostBodyWidget()
{
    if (!m_currentPost.body.isEmpty()) {
        auto *bodyLabel = new QLabel(
            simpleMarkdownToHtml(m_currentPost.body), m_headerWidget);
        bodyLabel->setWordWrap(true);
        bodyLabel->setTextFormat(Qt::RichText);
        bodyLabel->setOpenExternalLinks(true);
        bodyLabel->setTextInteractionFlags(
            Qt::TextBrowserInteraction | Qt::LinksAccessibleByMouse);
        bodyLabel->setStyleSheet(QStringLiteral(
            "QLabel { font-size: 14px; color: #333; padding: 8px 0px; "
            "line-height: 1.5; }"));
        return bodyLabel;
    }

    if (!m_currentPost.url.isEmpty()) {
        return createUrlEmbedWidget();
    }

    return new QLabel(QString(), m_headerWidget);
}

QWidget *LemmyPostDetailWidget::createUrlEmbedWidget()
{
    auto *embed = new QWidget(m_headerWidget);
    auto *embedLayout = new QHBoxLayout(embed);
    embedLayout->setContentsMargins(0, 8, 0, 8);
    embedLayout->setSpacing(12);

    auto *textCol = new QVBoxLayout();
    textCol->setSpacing(4);

    auto *urlTitle = new QLabel(m_currentPost.embedTitle, embed);
    urlTitle->setWordWrap(true);
    urlTitle->setStyleSheet(QStringLiteral(
        "QLabel { font-size: 13px; font-weight: bold; color: #2b579a; }"));

    if (!m_currentPost.embedDescription.isEmpty()) {
        auto *urlDesc = new QLabel(m_currentPost.embedDescription, embed);
        urlDesc->setWordWrap(true);
        urlDesc->setMaximumHeight(60);
        urlDesc->setStyleSheet(QStringLiteral(
            "QLabel { font-size: 12px; color: #666; }"));
        textCol->addWidget(urlTitle);
        textCol->addWidget(urlDesc);
    } else {
        textCol->addWidget(urlTitle);
    }

    auto *urlLink = new QLabel(
        QStringLiteral("<a href=\"%1\" style=\"color:#2b579a;\">%2</a>")
            .arg(m_currentPost.url.toHtmlEscaped(),
                 m_currentPost.url.toHtmlEscaped()), embed);
    urlLink->setTextFormat(Qt::RichText);
    urlLink->setOpenExternalLinks(true);
    urlLink->setWordWrap(true);
    urlLink->setStyleSheet(QStringLiteral("QLabel { font-size: 11px; }"));
    textCol->addWidget(urlLink);

    embedLayout->addLayout(textCol, 1);
    embedLayout->addStretch();

    embed->setStyleSheet(QStringLiteral(
        "background-color: #f0f4f8; border: 1px solid #dde; "
        "border-radius: 8px; padding: 8px;"));

    return embed;
}

QWidget *LemmyPostDetailWidget::createPostMetaWidget()
{
    auto *meta = new QWidget(m_headerWidget);
    auto *metaLayout = new QHBoxLayout(meta);
    metaLayout->setContentsMargins(0, 4, 0, 4);
    metaLayout->setSpacing(16);

    auto *voteWidget = createVoteWidget(m_currentPost.score, true, m_currentPost.id);
    metaLayout->addWidget(voteWidget);

    auto *commentCount = new QLabel(
        tr("%n comment(s)", nullptr, static_cast<int>(m_currentPost.comments)), meta);
    commentCount->setStyleSheet(QStringLiteral(
        "QLabel { color: #666; font-size: 12px; padding: 4px 8px; "
        "background-color: #eee; border-radius: 4px; }"));
    metaLayout->addWidget(commentCount);

    if (!m_currentPost.published.isEmpty()) {
        QDateTime dt = QDateTime::fromString(m_currentPost.published, Qt::ISODate);
        auto *timeLabel = new QLabel(timeAgo(dt), meta);
        timeLabel->setStyleSheet(QStringLiteral("QLabel { color: #999; font-size: 12px; }"));
        metaLayout->addWidget(timeLabel);
    }

    metaLayout->addStretch();
    return meta;
}

QWidget *LemmyPostDetailWidget::createVoteWidget(qint64 score, bool isPost, qint64 id)
{
    auto *container = new QWidget();
    auto *layout = new QHBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(2);

    auto *upBtn = new QPushButton(QStringLiteral("\xE2\x96\xB2"), container);
    upBtn->setFixedSize(28, 28);
    upBtn->setCursor(Qt::PointingHandCursor);
    upBtn->setStyleSheet(QStringLiteral(
        "QPushButton { border: none; font-size: 14px; color: #aaa; border-radius: 4px; }"
        "QPushButton:hover { background-color: #e8f5e9; color: #4caf50; }"));

    auto *scoreLabel = new QLabel(QString::number(score), container);
    scoreLabel->setAlignment(Qt::AlignCenter);
    scoreLabel->setFixedWidth(32);
    scoreLabel->setStyleSheet(QStringLiteral(
        "QLabel { font-weight: bold; font-size: 13px; color: #555; }"));

    auto *downBtn = new QPushButton(QStringLiteral("\xE2\x96\xBC"), container);
    downBtn->setFixedSize(28, 28);
    downBtn->setCursor(Qt::PointingHandCursor);
    downBtn->setStyleSheet(QStringLiteral(
        "QPushButton { border: none; font-size: 14px; color: #aaa; border-radius: 4px; }"
        "QPushButton:hover { background-color: #ffebee; color: #f44336; }"));

    layout->addWidget(upBtn);
    layout->addWidget(scoreLabel);
    layout->addWidget(downBtn);

    if (isPost) {
        m_postScoreLabel = scoreLabel;
        m_postUpBtn = upBtn;
        m_postDownBtn = downBtn;
        connect(upBtn, &QPushButton::clicked, this, &LemmyPostDetailWidget::onUpvotePost);
        connect(downBtn, &QPushButton::clicked, this, &LemmyPostDetailWidget::onDownvotePost);
    } else {
        connect(upBtn, &QPushButton::clicked, this, [this, id]() { onUpvoteComment(id); });
        connect(downBtn, &QPushButton::clicked, this, [this, id]() { onDownvoteComment(id); });
    }

    return container;
}

void LemmyPostDetailWidget::updateVoteDisplay(QLabel *scoreLabel, QPushButton *upBtn,
                                               QPushButton *downBtn, int vote, qint64 score)
{
    if (!scoreLabel || !upBtn || !downBtn) {
        return;
    }
    scoreLabel->setText(QString::number(score));

    if (vote > 0) {
        upBtn->setStyleSheet(QStringLiteral(
            "QPushButton { border: none; font-size: 14px; color: #4caf50; border-radius: 4px; "
            "background-color: #e8f5e9; }"
            "QPushButton:hover { background-color: #c8e6c9; }"));
        downBtn->setStyleSheet(QStringLiteral(
            "QPushButton { border: none; font-size: 14px; color: #aaa; border-radius: 4px; }"
            "QPushButton:hover { background-color: #ffebee; color: #f44336; }"));
    } else if (vote < 0) {
        downBtn->setStyleSheet(QStringLiteral(
            "QPushButton { border: none; font-size: 14px; color: #f44336; border-radius: 4px; "
            "background-color: #ffebee; }"
            "QPushButton:hover { background-color: #ffcdd2; }"));
        upBtn->setStyleSheet(QStringLiteral(
            "QPushButton { border: none; font-size: 14px; color: #aaa; border-radius: 4px; }"
            "QPushButton:hover { background-color: #e8f5e9; color: #4caf50; }"));
    } else {
        upBtn->setStyleSheet(QStringLiteral(
            "QPushButton { border: none; font-size: 14px; color: #aaa; border-radius: 4px; }"
            "QPushButton:hover { background-color: #e8f5e9; color: #4caf50; }"));
        downBtn->setStyleSheet(QStringLiteral(
            "QPushButton { border: none; font-size: 14px; color: #aaa; border-radius: 4px; }"
            "QPushButton:hover { background-color: #ffebee; color: #f44336; }"));
    }
}

void LemmyPostDetailWidget::onUpvotePost()
{
    auto *session = pcp::Application::instance().lemmySessionHolder()->getSession(m_instanceUrl);
    if (!session) return;
    auto *client = session->findChild<LemmyClient *>(QString(), Qt::FindDirectChildrenOnly);
    if (!client) return;

    m_postVote = (m_postVote > 0) ? 0 : 1;
    client->likePost(static_cast<int>(m_currentPost.id), m_postVote);

    qint64 newScore = m_currentPost.score;
    if (m_postVote > 0) {
        newScore = m_currentPost.score + 1;
    } else if (m_currentPost.score > 0 && m_postVote == 0) {
        newScore = m_currentPost.score - 1;
    }
    updateVoteDisplay(m_postScoreLabel, m_postUpBtn, m_postDownBtn, m_postVote, newScore);
}

void LemmyPostDetailWidget::onDownvotePost()
{
    auto *session = pcp::Application::instance().lemmySessionHolder()->getSession(m_instanceUrl);
    if (!session) return;
    auto *client = session->findChild<LemmyClient *>(QString(), Qt::FindDirectChildrenOnly);
    if (!client) return;

    m_postVote = (m_postVote < 0) ? 0 : -1;
    client->likePost(static_cast<int>(m_currentPost.id), m_postVote);

    qint64 newScore = m_currentPost.score;
    if (m_postVote < 0) {
        newScore = m_currentPost.score - 1;
    } else if (m_currentPost.score < 0 && m_postVote == 0) {
        newScore = m_currentPost.score + 1;
    }
    updateVoteDisplay(m_postScoreLabel, m_postUpBtn, m_postDownBtn, m_postVote, newScore);
}

void LemmyPostDetailWidget::onUpvoteComment(qint64 commentId)
{
    auto *session = pcp::Application::instance().lemmySessionHolder()->getSession(m_instanceUrl);
    if (!session) return;
    auto *client = session->findChild<LemmyClient *>(QString(), Qt::FindDirectChildrenOnly);
    if (!client) return;

    int cur = m_commentVotes.value(commentId, 0);
    int newVote = (cur > 0) ? 0 : 1;
    m_commentVotes[commentId] = newVote;
    client->likeComment(static_cast<int>(commentId), newVote);

    for (auto &list : m_children) {
        for (auto &c : list) {
            if (c.id == commentId) {
                c.score += (newVote > 0 ? 1 : (cur > 0 ? -1 : 0));
                break;
            }
        }
    }
    for (auto &c : m_rootComments) {
        if (c.id == commentId) {
            c.score += (newVote > 0 ? 1 : (cur > 0 ? -1 : 0));
            break;
        }
    }
    buildCommentTree();
}

void LemmyPostDetailWidget::onDownvoteComment(qint64 commentId)
{
    auto *session = pcp::Application::instance().lemmySessionHolder()->getSession(m_instanceUrl);
    if (!session) return;
    auto *client = session->findChild<LemmyClient *>(QString(), Qt::FindDirectChildrenOnly);
    if (!client) return;

    int cur = m_commentVotes.value(commentId, 0);
    int newVote = (cur < 0) ? 0 : -1;
    m_commentVotes[commentId] = newVote;
    client->likeComment(static_cast<int>(commentId), newVote);

    for (auto &list : m_children) {
        for (auto &c : list) {
            if (c.id == commentId) {
                c.score += (newVote < 0 ? -1 : (cur < 0 ? 1 : 0));
                break;
            }
        }
    }
    for (auto &c : m_rootComments) {
        if (c.id == commentId) {
            c.score += (newVote < 0 ? -1 : (cur < 0 ? 1 : 0));
            break;
        }
    }
    buildCommentTree();
}

void LemmyPostDetailWidget::submitComment(qint64 parentId, QString content)
{
    auto *session = pcp::Application::instance().lemmySessionHolder()->getSession(m_instanceUrl);
    if (!session) return;
    auto *client = session->findChild<LemmyClient *>(QString(), Qt::FindDirectChildrenOnly);
    if (!client) return;

    QJsonObject resp = client->createComment(
        static_cast<int>(m_postId), static_cast<int>(parentId), content);
    QJsonObject commentView = resp.value(QStringLiteral("comment_view")).toObject();
    if (commentView.isEmpty()) {
        return;
    }

    LemmyComment newComment = LemmyComment::fromJson(commentView);
    m_children[parentId].append(newComment);

    if (parentId == 0) {
        m_replyInput->clear();
    }

    buildCommentTree();
}

void LemmyPostDetailWidget::buildCommentTree()
{
    clearComments();

    if (m_rootComments.isEmpty()) {
        auto *emptyLabel = new QLabel(tr("No comments yet."), m_commentContainer);
        emptyLabel->setAlignment(Qt::AlignCenter);
        emptyLabel->setStyleSheet(QStringLiteral(
            "QLabel { color: #999; font-style: italic; padding: 24px; font-size: 14px; }"));
        m_commentLayout->insertWidget(m_commentLayout->count() - 1, emptyLabel);
        return;
    }

    for (const auto &comment : m_rootComments) {
        addCommentsRecursive(comment.id, 0, m_commentContainer, m_commentLayout);
    }

    if (m_commentLayout->count() > 1) {
        auto *moreBtn = new QPushButton(tr("Load more comments"), m_commentContainer);
        moreBtn->setFlat(true);
        moreBtn->setCursor(Qt::PointingHandCursor);
        moreBtn->setStyleSheet(QStringLiteral(
            "QPushButton { color: #2b579a; font-size: 12px; border: none; padding: 8px; }"
            "QPushButton:hover { text-decoration: underline; }"));
        const int page = m_commentPage;
        connect(moreBtn, &QPushButton::clicked, this, [this, page]() {
            onLoadMoreReplies(0, page + 1);
        });
        m_commentLayout->insertWidget(m_commentLayout->count() - 1, moreBtn);
    }
}

void LemmyPostDetailWidget::clearComments()
{
    while (m_commentLayout->count() > 1) {
        QLayoutItem *item = m_commentLayout->takeAt(0);
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }
    m_commentContainers.clear();
}

void LemmyPostDetailWidget::addCommentsRecursive(qint64 parentId, int depth,
                                                  QWidget *container, QVBoxLayout *layout)
{
    const auto &children = m_children.value(parentId);
    for (const auto &comment : children) {
        QWidget *commentWidget = createCommentWidget(comment);
        if (!commentWidget) {
            continue;
        }

        int marginLeft = depth * kIndentPerLevel;
        commentWidget->setContentsMargins(marginLeft, 0, 0, 0);

        int insertPos = layout->count() - 1;
        layout->insertWidget(insertPos, commentWidget);

        auto *innerContainer = new QWidget(container);
        auto *innerLayout = new QVBoxLayout(innerContainer);
        innerLayout->setContentsMargins(0, 0, 0, 0);
        innerLayout->setSpacing(0);
        layout->insertWidget(layout->count() - 1, innerContainer);

        m_commentContainers.insert(comment.id, innerContainer);

        if (m_collapsed.value(comment.id, false)) {
            innerContainer->setVisible(false);
        }

        addCommentsRecursive(comment.id, depth + 1, innerContainer, innerLayout);
    }
}

QWidget *LemmyPostDetailWidget::createCommentWidget(const LemmyComment &comment)
{
    auto *wrapper = new QWidget();
    auto *outerLayout = new QHBoxLayout(wrapper);
    outerLayout->setContentsMargins(0, 0, 0, 0);
    outerLayout->setSpacing(0);

    int colorIndex = comment.depth % kDepthColors.size();
    auto *borderLine = new QFrame(wrapper);
    borderLine->setFixedWidth(3);
    borderLine->setStyleSheet(QStringLiteral(
        "QFrame { background-color: %1; border: none; border-radius: 2px; }")
        .arg(kDepthColors.at(colorIndex)));
    outerLayout->addWidget(borderLine);

    auto *card = new QWidget(wrapper);
    card->setStyleSheet(QStringLiteral(
        "background-color: white; border-bottom: 1px solid #eee;"));
    outerLayout->addWidget(card, 1);

    auto *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(8, 6, 8, 6);
    cardLayout->setSpacing(2);

    auto *topRow = new QHBoxLayout();
    topRow->setSpacing(6);

    auto *toggleBtn = new QPushButton(
        m_collapsed.value(comment.id, false) ? QStringLiteral("\xE2\x96\xB6") : QStringLiteral("\xE2\x96\xBC"),
        card);
    toggleBtn->setFixedSize(18, 18);
    toggleBtn->setFlat(true);
    toggleBtn->setCursor(Qt::PointingHandCursor);
    toggleBtn->setStyleSheet(QStringLiteral(
        "QPushButton { border: none; font-size: 10px; color: #aaa; }"
        "QPushButton:hover { color: #333; }"));
    toggleBtn->setVisible(m_children.contains(comment.id) && !m_children.value(comment.id).isEmpty());
    connect(toggleBtn, &QPushButton::clicked, this, [this, id = comment.id]() {
        onToggleCollapse(id);
    });
    topRow->addWidget(toggleBtn);

    QString initials = initialsFromName(comment.creatorName);
    QString avatarColor = avatarColorForName(comment.creatorName);
    auto *avatarLabel = new QLabel(initials, card);
    avatarLabel->setFixedSize(20, 20);
    avatarLabel->setAlignment(Qt::AlignCenter);
    avatarLabel->setStyleSheet(QStringLiteral(
        "QLabel { background-color: %1; color: white; border-radius: 10px; "
        "font-weight: bold; font-size: 10px; }").arg(avatarColor));
    topRow->addWidget(avatarLabel);

    auto *nameLabel = new QLabel(comment.creatorName, card);
    nameLabel->setStyleSheet(QStringLiteral(
        "QLabel { font-weight: bold; font-size: 12px; color: #333; }"));
    topRow->addWidget(nameLabel);

    if (!comment.published.isEmpty()) {
        QDateTime dt = QDateTime::fromString(comment.published, Qt::ISODate);
        auto *timeLabel = new QLabel(timeAgo(dt), card);
        timeLabel->setStyleSheet(QStringLiteral("QLabel { font-size: 11px; color: #aaa; }"));
        topRow->addWidget(timeLabel);
    }

    topRow->addStretch();
    cardLayout->addLayout(topRow);

    auto *bodyLabel = new QLabel(simpleMarkdownToHtml(comment.content), card);
    bodyLabel->setWordWrap(true);
    bodyLabel->setTextFormat(Qt::RichText);
    bodyLabel->setOpenExternalLinks(true);
    bodyLabel->setTextInteractionFlags(
        Qt::TextBrowserInteraction | Qt::LinksAccessibleByMouse);
    bodyLabel->setStyleSheet(QStringLiteral(
        "QLabel { font-size: 13px; color: #444; padding: 4px 0px 2px 0px; }"));
    cardLayout->addWidget(bodyLabel);

    auto *actionRow = new QHBoxLayout();
    actionRow->setSpacing(12);

    auto *voteWidget = createVoteWidget(comment.score, false, comment.id);
    actionRow->addWidget(voteWidget);

    int childCount = m_children.value(comment.id).size();
    if (childCount > 0) {
        auto *replyCountLabel = new QLabel(
            tr("%n reply/replies", nullptr, childCount), card);
        replyCountLabel->setStyleSheet(QStringLiteral(
            "QLabel { font-size: 11px; color: #999; }"));
        actionRow->addWidget(replyCountLabel);
    }

    auto *replyBtn = new QPushButton(tr("Reply"), card);
    replyBtn->setFlat(true);
    replyBtn->setCursor(Qt::PointingHandCursor);
    replyBtn->setStyleSheet(QStringLiteral(
        "QPushButton { color: #2b579a; font-size: 11px; border: none; padding: 2px 6px; }"
        "QPushButton:hover { background-color: #e8e8f0; border-radius: 4px; }"));
    connect(replyBtn, &QPushButton::clicked, this, [this, id = comment.id]() {
        onReplyClicked(id);
    });
    actionRow->addWidget(replyBtn);

    actionRow->addStretch();
    cardLayout->addLayout(actionRow);

    return wrapper;
}

void LemmyPostDetailWidget::onToggleCollapse(qint64 commentId)
{
    auto it = m_commentContainers.find(commentId);
    if (it == m_commentContainers.end()) {
        return;
    }

    bool currentlyCollapsed = m_collapsed.value(commentId, false);
    m_collapsed[commentId] = !currentlyCollapsed;

    buildCommentTree();
}

void LemmyPostDetailWidget::onLoadMoreReplies(qint64 parentId, int page)
{
    auto *session = pcp::Application::instance().lemmySessionHolder()->getSession(m_instanceUrl);
    if (!session) return;
    auto *client = session->findChild<LemmyClient *>(QString(), Qt::FindDirectChildrenOnly);
    if (!client) return;

    m_commentPage = page;

    QJsonObject commentsResp = client->getComments(
        static_cast<int>(m_postId), QStringLiteral("Hot"), page, 50);
    QJsonArray commentsArray = commentsResp.value(QStringLiteral("comments")).toArray();

    if (commentsArray.isEmpty()) {
        return;
    }

    for (const QJsonValue &val : commentsArray) {
        LemmyComment comment = LemmyComment::fromJson(val.toObject());
        if (comment.removed || comment.deleted) {
            comment.content = comment.removed ? tr("[removed]") : tr("[deleted]");
        }
        auto &siblings = m_children[comment.parentId];
        bool found = false;
        for (const auto &c : siblings) {
            if (c.id == comment.id) {
                found = true;
                break;
            }
        }
        if (!found) {
            siblings.append(std::move(comment));
        }
    }

    m_rootComments = m_children.value(0);
    buildCommentTree();
}

void LemmyPostDetailWidget::onReplyClicked(qint64 parentId)
{
    for (int i = 0; i < m_commentLayout->count(); ++i) {
        QLayoutItem *item = m_commentLayout->itemAt(i);
        if (item && item->widget()
            && item->widget()->property("replyParentId").toLongLong() == parentId) {
            delete item->widget();
            return;
        }
    }

    QWidget *replyInput = createReplyInputWidget(parentId, m_commentContainer);
    if (!replyInput) {
        return;
    }

    auto containerIt = m_commentContainers.find(parentId);
    if (containerIt != m_commentContainers.end() && containerIt.value()) {
        auto *containerLayout = containerIt.value()->layout();
        if (containerLayout) {
            containerLayout->addWidget(replyInput);
            return;
        }
    }

    m_commentLayout->insertWidget(m_commentLayout->count() - 1, replyInput);
}

QWidget *LemmyPostDetailWidget::createReplyInputWidget(qint64 parentId, QWidget *parentContainer)
{
    auto *wrapper = new QWidget(parentContainer);
    wrapper->setProperty("replyParentId", parentId);
    auto *layout = new QVBoxLayout(wrapper);
    layout->setContentsMargins(8 + (1 * kIndentPerLevel), 4, 8, 8);
    layout->setSpacing(4);

    auto *inputRow = new QHBoxLayout();
    inputRow->setSpacing(6);

    auto *input = new QTextEdit(wrapper);
    input->setPlaceholderText(tr("Write a reply..."));
    input->setMaximumHeight(80);
    input->setMinimumHeight(32);
    input->setAcceptRichText(false);
    input->setStyleSheet(QStringLiteral(
        "QTextEdit { border: 1px solid #ccc; border-radius: 6px; "
        "padding: 4px 8px; font-size: 12px; }"
        "QTextEdit:focus { border-color: #2b579a; }"));
    inputRow->addWidget(input, 1);

    auto *submitBtn = new QPushButton(tr("Reply"), wrapper);
    submitBtn->setMinimumWidth(56);
    submitBtn->setMinimumHeight(30);
    submitBtn->setStyleSheet(QStringLiteral(
        "QPushButton { background-color: #2b579a; color: white; border: none; "
        "border-radius: 6px; padding: 4px 12px; font-size: 12px; font-weight: bold; }"
        "QPushButton:hover { background-color: #3a6bb5; }"));

    auto *cancelBtn = new QPushButton(tr("Cancel"), wrapper);
    cancelBtn->setMinimumWidth(56);
    cancelBtn->setMinimumHeight(30);
    cancelBtn->setStyleSheet(QStringLiteral(
        "QPushButton { background-color: #eee; color: #555; border: none; "
        "border-radius: 6px; padding: 4px 12px; font-size: 12px; }"
        "QPushButton:hover { background-color: #ddd; }"));

    inputRow->addWidget(submitBtn);
    inputRow->addWidget(cancelBtn);
    layout->addLayout(inputRow);

    connect(submitBtn, &QPushButton::clicked, this, [this, parentId, input, wrapper]() {
        onSubmitInlineReply(parentId, input, wrapper);
    });
    connect(cancelBtn, &QPushButton::clicked, this, [wrapper]() {
        delete wrapper;
    });

    return wrapper;
}

void LemmyPostDetailWidget::onSubmitInlineReply(qint64 parentId, QTextEdit *input,
                                                 QWidget *replyContainer)
{
    const QString text = input->toPlainText().trimmed();
    if (text.isEmpty()) {
        return;
    }

    submitComment(parentId, text);
}

QString LemmyPostDetailWidget::simpleMarkdownToHtml(const QString &text) const
{
    QString html = text.toHtmlEscaped();

    QRegularExpression boldRe(QStringLiteral(R"(\*\*(.+?)\*\*)"));
    html.replace(boldRe, QStringLiteral(R"(<b>\1</b>)"));

    QRegularExpression italicRe(QStringLiteral(R"(\*([^*]+)\*)"));
    html.replace(italicRe, QStringLiteral(R"(<i>\1</i>)"));

    QRegularExpression strikeRe(QStringLiteral(R"(~~(.+?)~~)"));
    html.replace(strikeRe, QStringLiteral(R"(<s>\1</s>)"));

    QRegularExpression codeRe(QStringLiteral(R"(`([^`]+)`)"));
    html.replace(codeRe,
        QStringLiteral(R"(<code style='background-color:#f0f0f0; padding:1px 4px; "
                       "border-radius:3px; font-family:monospace; font-size:12px;'>\1</code>)"));

    QRegularExpression urlRe(QStringLiteral(R"((https?://[^\s<>"]+))"));
    html.replace(urlRe, QStringLiteral(R"(<a href="\1" style="color:#2b579a;">\1</a>)"));

    QRegularExpression headerRe(QStringLiteral(R"(^### (.+)$)"), QRegularExpression::MultilineOption);
    html.replace(headerRe, QStringLiteral(R"(<h3>\1</h3>)"));

    QRegularExpression header2Re(QStringLiteral(R"(^## (.+)$)"), QRegularExpression::MultilineOption);
    html.replace(header2Re, QStringLiteral(R"(<h2>\1</h2>)"));

    QRegularExpression header1Re(QStringLiteral(R"(^# (.+)$)"), QRegularExpression::MultilineOption);
    html.replace(header1Re, QStringLiteral(R"(<h1>\1</h1>)"));

    QRegularExpression quoteRe(QStringLiteral(R"(^&gt; (.+)$)"), QRegularExpression::MultilineOption);
    html.replace(quoteRe,
        QStringLiteral(R"(<blockquote style='border-left:3px solid #ccc; margin:4px 0; "
                       "padding:2px 8px; color:#666;'>\1</blockquote>)"));

    html.replace(QStringLiteral("\n"), QStringLiteral("<br>"));

    return html;
}

QString LemmyPostDetailWidget::formatTimestamp(const QString &published) const
{
    if (published.isEmpty()) {
        return {};
    }
    QDateTime dt = QDateTime::fromString(published, Qt::ISODate);
    if (!dt.isValid()) {
        return published;
    }
    return dt.toString(QStringLiteral("MMM d, yyyy HH:mm"));
}

QString LemmyPostDetailWidget::timeAgo(const QDateTime &dt) const
{
    if (!dt.isValid()) {
        return {};
    }

    qint64 secs = dt.secsTo(QDateTime::currentDateTimeUtc());
    if (secs < 0) {
        secs = 0;
    }

    if (secs < 60) {
        return tr("just now");
    }
    if (secs < 3600) {
        return tr("%n minute(s) ago", nullptr, static_cast<int>(secs / 60));
    }
    if (secs < 86400) {
        return tr("%n hour(s) ago", nullptr, static_cast<int>(secs / 3600));
    }
    if (secs < 604800) {
        return tr("%n day(s) ago", nullptr, static_cast<int>(secs / 86400));
    }
    if (secs < 2592000) {
        return tr("%n week(s) ago", nullptr, static_cast<int>(secs / 604800));
    }
    if (secs < 31536000) {
        return tr("%n month(s) ago", nullptr, static_cast<int>(secs / 2592000));
    }
    return tr("%n year(s) ago", nullptr, static_cast<int>(secs / 31536000));
}

QString LemmyPostDetailWidget::avatarColorForName(const QString &name) const
{
    static const QStringList palette = {
        QStringLiteral("#E53935"), QStringLiteral("#D81B60"), QStringLiteral("#8E24AA"),
        QStringLiteral("#5E35B1"), QStringLiteral("#3949AB"), QStringLiteral("#1E88E5"),
        QStringLiteral("#039BE5"), QStringLiteral("#00ACC1"), QStringLiteral("#00897B"),
        QStringLiteral("#43A047"), QStringLiteral("#7CB342"), QStringLiteral("#C0CA33"),
        QStringLiteral("#FDD835"), QStringLiteral("#FFB300"), QStringLiteral("#FB8C00"),
        QStringLiteral("#F4511E")
    };
    uint hash = qHash(name);
    return palette.at(static_cast<int>(hash) % palette.size());
}

QString LemmyPostDetailWidget::initialsFromName(const QString &name) const
{
    if (name.isEmpty()) {
        return QStringLiteral("?");
    }
    QStringList parts = name.split(QChar(' '), Qt::SkipEmptyParts);
    if (parts.isEmpty()) {
        return name.left(1).toUpper();
    }
    QString result;
    result += parts.first().at(0).toUpper();
    if (parts.size() >= 2) {
        result += parts.last().at(0).toUpper();
    }
    return result;
}
