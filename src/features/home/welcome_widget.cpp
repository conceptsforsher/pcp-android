#include "welcome_widget.hpp"

#include <QApplication>
#include <QGraphicsDropShadowEffect>
#include <QHBoxLayout>
#include <QPainter>
#include <QVBoxLayout>

WelcomeWidget::WelcomeWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
}

QPixmap WelcomeWidget::generateAppIcon(int size) const
{
    QPixmap pixmap(size, size);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect circleRect(0, 0, size, size);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor("#1976D2"));
    painter.drawEllipse(circleRect);

    QFont font;
    font.setPixelSize(size / 2);
    font.setBold(true);
    painter.setFont(font);
    painter.setPen(Qt::white);
    painter.drawText(circleRect, Qt::AlignCenter, QStringLiteral("PC"));

    painter.end();
    return pixmap;
}

QFrame *WelcomeWidget::createActionCard(const QString &accentColor,
                                         const QString &iconText,
                                         const QString &iconBgColor,
                                         const QString &title,
                                         const QString &description,
                                         const QString &buttonText,
                                         QObject *receiver,
                                         const char *signal)
{
    auto *card = new QFrame(this);
    card->setFixedSize(200, 260);
    card->setObjectName(QStringLiteral("actionCard"));
    card->setStyleSheet(QStringLiteral(
        "#actionCard {"
        "  background-color: white;"
        "  border: 2px solid %1;"
        "  border-radius: 12px;"
        "}").arg(accentColor));

    auto *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(20);
    shadow->setOffset(0, 4);
    shadow->setColor(QColor(0, 0, 0, 40));
    card->setGraphicsEffect(shadow);

    auto *layout = new QVBoxLayout(card);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(8);
    layout->setContentsMargins(16, 16, 16, 16);

    // Icon circle
    auto *iconLabel = new QLabel(card);
    QPixmap icon(48, 48);
    icon.fill(Qt::transparent);
    {
        QPainter p(&icon);
        p.setRenderHint(QPainter::Antialiasing);
        p.setPen(Qt::NoPen);
        p.setBrush(QColor(iconBgColor));
        p.drawEllipse(0, 0, 48, 48);
        p.setPen(Qt::white);
        QFont f;
        f.setPixelSize(24);
        f.setBold(true);
        p.setFont(f);
        p.drawText(QRect(0, 0, 48, 48), Qt::AlignCenter, iconText);
    }
    iconLabel->setPixmap(icon);
    iconLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(iconLabel);

    // Title
    auto *titleLabel = new QLabel(title, card);
    QFont titleFont;
    titleFont.setPointSize(12);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(QStringLiteral("border: none; background: transparent;"));
    layout->addWidget(titleLabel);

    // Description
    auto *descLabel = new QLabel(description, card);
    descLabel->setAlignment(Qt::AlignCenter);
    descLabel->setWordWrap(true);
    descLabel->setStyleSheet(QStringLiteral("color: #666; border: none; background: transparent;"));
    layout->addWidget(descLabel);

    layout->addStretch();

    // Action button
    auto *button = new QPushButton(buttonText, card);
    button->setCursor(Qt::PointingHandCursor);
    button->setStyleSheet(QStringLiteral(
        "QPushButton {"
        "  background-color: %1;"
        "  color: white;"
        "  border: none;"
        "  border-radius: 8px;"
        "  padding: 10px 20px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "  background-color: %2;"
        "}").arg(accentColor, accentColor));
    layout->addWidget(button);

    QObject::connect(button, SIGNAL(clicked()), receiver, signal);

    return card;
}

void WelcomeWidget::setupUi()
{
    auto *rootLayout = new QVBoxLayout(this);
    rootLayout->setAlignment(Qt::AlignCenter);
    rootLayout->setSpacing(16);

    // App icon
    auto *iconLabel = new QLabel(this);
    iconLabel->setPixmap(generateAppIcon(128));
    iconLabel->setAlignment(Qt::AlignCenter);
    iconLabel->setFixedSize(128, 128);
    rootLayout->addWidget(iconLabel, 0, Qt::AlignCenter);

    rootLayout->addSpacing(8);

    // Heading
    m_headingLabel = new QLabel(tr("Welcome to PCP"), this);
    QFont headingFont;
    headingFont.setPointSize(22);
    headingFont.setBold(true);
    m_headingLabel->setFont(headingFont);
    m_headingLabel->setAlignment(Qt::AlignCenter);
    rootLayout->addWidget(m_headingLabel);

    // Subtitle
    m_subtitleLabel = new QLabel(tr("Connect to your chat networks to get started"), this);
    m_subtitleLabel->setAlignment(Qt::AlignCenter);
    m_subtitleLabel->setStyleSheet(QStringLiteral("color: #666; font-size: 14px;"));
    rootLayout->addWidget(m_subtitleLabel);

    rootLayout->addSpacing(12);

    // Action cards
    auto *cardsLayout = new QHBoxLayout();
    cardsLayout->setSpacing(20);
    cardsLayout->setAlignment(Qt::AlignCenter);

    m_matrixCard = createActionCard(
        QStringLiteral("#0D47A1"),
        QStringLiteral("M"),
        QStringLiteral("#1976D2"),
        tr("Matrix"),
        tr("Connect to the Matrix network"),
        tr("Add Account"),
        this,
        SIGNAL(addMatrixRequested()));

    m_ircCard = createActionCard(
        QStringLiteral("#1B5E20"),
        QStringLiteral("#"),
        QStringLiteral("#43A047"),
        tr("IRC"),
        tr("Connect to IRC servers"),
        tr("Add Server"),
        this,
        SIGNAL(addIrcRequested()));

    m_lemmyCard = createActionCard(
        QStringLiteral("#E65100"),
        QStringLiteral("L"),
        QStringLiteral("#FB8C00"),
        tr("Lemmy"),
        tr("Browse Lemmy communities"),
        tr("Add Instance"),
        this,
        SIGNAL(addLemmyRequested()));

    cardsLayout->addWidget(m_matrixCard);
    cardsLayout->addWidget(m_ircCard);
    cardsLayout->addWidget(m_lemmyCard);

    rootLayout->addLayout(cardsLayout);

    rootLayout->addSpacing(16);

    // Learn More link
    m_learnMoreLabel = new QLabel(this);
    m_learnMoreLabel->setText(QStringLiteral(
        "<a href=\"https://github.com/pcp-chat/pcp-android-concept\" "
        "style=\"color: #1976D2; text-decoration: none;\">Learn More</a>"));
    m_learnMoreLabel->setAlignment(Qt::AlignCenter);
    m_learnMoreLabel->setOpenExternalLinks(true);
    m_learnMoreLabel->setTextFormat(Qt::RichText);
    m_learnMoreLabel->setCursor(Qt::PointingHandCursor);
    rootLayout->addWidget(m_learnMoreLabel);

    rootLayout->addStretch();
}
