#include "about_dialog.hpp"

#include <QApplication>
#include <QDialogButtonBox>
#include <QFont>
#include <QPainter>
#include <QVBoxLayout>

AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUi();
}

QPixmap AboutDialog::generateAppIcon(int size) const
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

void AboutDialog::setupUi()
{
    setWindowTitle(tr("About PCP"));
    setFixedSize(420, 560);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    auto *rootLayout = new QVBoxLayout(this);
    rootLayout->setAlignment(Qt::AlignCenter);
    rootLayout->setSpacing(10);
    rootLayout->setContentsMargins(30, 20, 30, 20);

    auto addSpacing = [&](int pixels) {
        rootLayout->addSpacing(pixels);
    };

    // App icon
    m_appIconLabel = new QLabel(this);
    m_appIconLabel->setPixmap(generateAppIcon(96));
    m_appIconLabel->setAlignment(Qt::AlignCenter);
    m_appIconLabel->setFixedSize(96, 96);
    rootLayout->addWidget(m_appIconLabel, 0, Qt::AlignCenter);

    addSpacing(4);

    // Title
    m_titleLabel = new QLabel(tr("PCP Concept"), this);
    QFont titleFont;
    titleFont.setPointSize(18);
    titleFont.setBold(true);
    m_titleLabel->setFont(titleFont);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    rootLayout->addWidget(m_titleLabel);

    addSpacing(2);

    // Version
    m_versionLabel = new QLabel(tr("Version 1.0.0"), this);
    m_versionLabel->setAlignment(Qt::AlignCenter);
    m_versionLabel->setStyleSheet(QStringLiteral("color: #666;"));
    rootLayout->addWidget(m_versionLabel);

    addSpacing(6);

    // Description
    m_descriptionLabel = new QLabel(
        tr("100% C++ Matrix/IRC/Lemmy client -\n"
           "A concept rewrite of pcp-android\n"
           "in native C++"),
        this);
    m_descriptionLabel->setAlignment(Qt::AlignCenter);
    m_descriptionLabel->setWordWrap(true);
    rootLayout->addWidget(m_descriptionLabel);

    addSpacing(6);

    // Built with Qt
    m_builtWithLabel = new QLabel(
        tr("Built with Qt %1").arg(QString::fromLatin1(qVersion())),
        this);
    m_builtWithLabel->setAlignment(Qt::AlignCenter);
    m_builtWithLabel->setStyleSheet(QStringLiteral("color: #1976D2; font-weight: bold;"));
    rootLayout->addWidget(m_builtWithLabel);

    addSpacing(4);

    // C++20 badge
    m_standardBadge = new QLabel(tr("C++20 Standard"), this);
    m_standardBadge->setAlignment(Qt::AlignCenter);
    m_standardBadge->setFixedWidth(160);
    m_standardBadge->setFixedHeight(28);
    m_standardBadge->setStyleSheet(QStringLiteral(
        "background-color: #E8F5E9; color: #2E7D32;"
        "border-radius: 14px; font-weight: bold;"
        "padding: 4px;"));
    rootLayout->addWidget(m_standardBadge, 0, Qt::AlignCenter);

    addSpacing(6);

    // Authors
    m_authorsLabel = new QLabel(tr("PCP Team"), this);
    m_authorsLabel->setAlignment(Qt::AlignCenter);
    rootLayout->addWidget(m_authorsLabel);

    addSpacing(4);

    // GitHub link
    m_linkLabel = new QLabel(this);
    m_linkLabel->setText(QStringLiteral(
        "<a href=\"https://github.com/pcp-chat/pcp-android-concept\" "
        "style=\"color: #1976D2;\">github.com/pcp-chat/pcp-android-concept</a>"));
    m_linkLabel->setAlignment(Qt::AlignCenter);
    m_linkLabel->setOpenExternalLinks(true);
    m_linkLabel->setTextFormat(Qt::RichText);
    rootLayout->addWidget(m_linkLabel);

    addSpacing(4);

    // License
    m_licenseLabel = new QLabel(tr("Licensed under Apache 2.0"), this);
    m_licenseLabel->setAlignment(Qt::AlignCenter);
    m_licenseLabel->setStyleSheet(QStringLiteral("color: #666;"));
    rootLayout->addWidget(m_licenseLabel);

    addSpacing(10);

    // Third Party Licenses button
    m_thirdPartyButton = new QPushButton(tr("Third Party Licenses"), this);
    m_thirdPartyButton->setCursor(Qt::PointingHandCursor);
    m_thirdPartyButton->setStyleSheet(QStringLiteral(
        "QPushButton { border: 1px solid #ccc; border-radius: 6px; padding: 6px 16px; }"
        "QPushButton:hover { background-color: #f0f0f0; }"));
    rootLayout->addWidget(m_thirdPartyButton, 0, Qt::AlignCenter);

    QObject::connect(m_thirdPartyButton, &QPushButton::clicked, this, [this]() {
        QDialog creditsDialog(this);
        creditsDialog.setWindowTitle(tr("Third Party Licenses"));
        creditsDialog.setFixedSize(400, 300);

        auto *layout = new QVBoxLayout(&creditsDialog);
        auto *label = new QLabel(
            tr("This application uses the following open source libraries:\n\n"
               "Qt %1 - LGPL v3 / GPL v3 / Commercial\n"
               "Copyright The Qt Company Ltd.\n\n"
               "All trademarks and registered trademarks are the\n"
               "property of their respective owners.").arg(QString::fromLatin1(qVersion())),
            &creditsDialog);
        label->setWordWrap(true);
        layout->addWidget(label);

        auto *closeBtn = new QPushButton(tr("Close"), &creditsDialog);
        layout->addWidget(closeBtn, 0, Qt::AlignCenter);
        QObject::connect(closeBtn, &QPushButton::clicked, &creditsDialog, &QDialog::accept);

        creditsDialog.exec();
    });

    addSpacing(10);

    // OK button
    m_okButton = new QPushButton(tr("OK"), this);
    m_okButton->setFixedWidth(100);
    m_okButton->setStyleSheet(QStringLiteral(
        "QPushButton { background-color: #1976D2; color: white; border-radius: 6px;"
        "padding: 8px 24px; font-weight: bold; }"
        "QPushButton:hover { background-color: #1565C0; }"));
    rootLayout->addWidget(m_okButton, 0, Qt::AlignCenter);

    QObject::connect(m_okButton, &QPushButton::clicked, this, &QDialog::accept);
}
