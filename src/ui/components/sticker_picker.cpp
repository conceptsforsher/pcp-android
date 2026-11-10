#include "sticker_picker.hpp"

#include <QPainter>
#include <QMenu>
#include <QApplication>
#include <QScrollBar>

StickerPicker::StickerPicker(QWidget* parent)
    : QWidget(parent, Qt::Popup)
    , m_settings(QStringLiteral("PcpMatrix"), QStringLiteral("StickerPicker"))
{
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);

    loadFavorites();
    setupUi();
    applyStyle();
    populateTabs();
}

void StickerPicker::setupUi()
{
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(12, 12, 12, 12);
    mainLayout->setSpacing(8);

    m_searchBar = new QLineEdit(this);
    m_searchBar->setPlaceholderText(QStringLiteral("Search stickers..."));
    m_searchBar->setClearButtonEnabled(true);
    m_searchBar->setFixedHeight(32);
    connect(m_searchBar, &QLineEdit::textChanged, this, &StickerPicker::onSearchTextChanged);

    m_tabWidget = new QTabWidget(this);
    m_tabWidget->setDocumentMode(true);

    m_addPackButton = new QPushButton(QStringLiteral("+ Add Sticker Pack"), this);
    m_addPackButton->setFixedHeight(32);
    m_addPackButton->setCursor(Qt::PointingHandCursor);
    m_addPackButton->setFlat(true);
    connect(m_addPackButton, &QPushButton::clicked, this, &StickerPicker::onAddStickerPack);

    mainLayout->addWidget(m_searchBar);
    mainLayout->addWidget(m_tabWidget, 1);
    mainLayout->addWidget(m_addPackButton);
}

void StickerPicker::applyStyle()
{
    setStyleSheet(QStringLiteral(R"(
        StickerPicker {
            background-color: #2d2d2d;
            border: 1px solid #444444;
            border-radius: 12px;
        }
        QTabWidget::pane {
            border: none;
            background-color: #2d2d2d;
        }
        QTabBar::tab {
            background-color: #3a3a3a;
            color: #cccccc;
            padding: 6px 12px;
            border: none;
            border-radius: 4px;
            margin-right: 2px;
            font-size: 12px;
        }
        QTabBar::tab:selected {
            background-color: #505050;
            color: #ffffff;
        }
        QTabBar::tab:hover:!selected {
            background-color: #454545;
        }
        QLineEdit {
            background-color: #3a3a3a;
            color: #e0e0e0;
            border: 1px solid #555555;
            border-radius: 6px;
            padding: 4px 8px;
            font-size: 13px;
        }
        QLineEdit:focus {
            border-color: #777777;
        }
        QPushButton#addPackButton {
            color: #aaaaaa;
            font-size: 12px;
            border: 1px dashed #555555;
            border-radius: 6px;
            padding: 4px;
        }
        QPushButton#addPackButton:hover {
            color: #ffffff;
            border-color: #888888;
            background-color: #3a3a3a;
        }
        QScrollArea {
            border: none;
            background-color: transparent;
        }
        QScrollBar:vertical {
            background: #2d2d2d;
            width: 6px;
            margin: 0;
        }
        QScrollBar::handle:vertical {
            background: #555555;
            border-radius: 3px;
            min-height: 20px;
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0;
        }
    )"));

    m_addPackButton->setObjectName(QStringLiteral("addPackButton"));
}

void StickerPicker::loadFavorites()
{
    int size = m_settings.beginReadArray(QStringLiteral("favorites"));
    for (int i = 0; i < size; ++i) {
        m_settings.setArrayIndex(i);
        m_favorites.append(m_settings.value(QStringLiteral("emoji")).toString());
    }
    m_settings.endArray();
}

void StickerPicker::saveFavorites()
{
    m_settings.beginWriteArray(QStringLiteral("favorites"));
    for (int i = 0; i < m_favorites.size(); ++i) {
        m_settings.setArrayIndex(i);
        m_settings.setValue(QStringLiteral("emoji"), m_favorites[i]);
    }
    m_settings.endArray();
    m_settings.sync();
}

QPixmap StickerPicker::renderEmojiPixmap(const QString& emoji, int size)
{
    QPixmap pixmap(size, size);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    QFont font;
    font.setPointSize(size / 2);
    painter.setFont(font);
    painter.drawText(QRect(0, 0, size, size), Qt::AlignCenter, emoji);
    painter.end();
    return pixmap;
}

QPushButton* StickerPicker::createStickerButton(const QString& emoji, const QString& name)
{
    auto* button = new QPushButton(this);
    button->setIcon(QIcon(renderEmojiPixmap(emoji, 36)));
    button->setIconSize(QSize(36, 36));
    button->setText(name.isEmpty() ? emoji : name);
    button->setFixedSize(kStickerButtonSize, kStickerButtonSize);
    button->setFlat(true);
    button->setCursor(Qt::PointingHandCursor);
    button->setToolTip(name.isEmpty() ? emoji : name);
    button->setProperty("stickerEmoji", emoji);
    button->setStyleSheet(QStringLiteral(R"(
        QPushButton {
            background-color: transparent;
            color: #cccccc;
            border: 1px solid transparent;
            border-radius: 8px;
            font-size: 10px;
            padding: 4px;
        }
        QPushButton:hover {
            background-color: #404040;
            border-color: #555555;
        }
        QPushButton:pressed {
            background-color: #505050;
        }
    )"));
    button->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(button, &QPushButton::clicked, this, &StickerPicker::onStickerClicked);
    connect(button, &QPushButton::customContextMenuRequested, this, &StickerPicker::onStickerContextMenu);
    return button;
}

void StickerPicker::populateTabs()
{
    m_packs.clear();
    m_packs.append({QStringLiteral("Favorites"), {}});
    m_packs.append({QStringLiteral("Recent"), {
        {QStringLiteral("\xF0\x9F\x98\x82"), QStringLiteral("Joy")},
        {QStringLiteral("\xF0\x9F\x98\x8D"), QStringLiteral("Heart Eyes")},
        {QStringLiteral("\xF0\x9F\x91\x8D"), QStringLiteral("Thumbs Up")},
        {QStringLiteral("\xF0\x9F\x98\xA2"), QStringLiteral("Crying")},
        {QStringLiteral("\xF0\x9F\x98\xAE"), QStringLiteral("Wow")},
        {QStringLiteral("\xF0\x9F\x91\x8E"), QStringLiteral("Thumbs Down")},
    }});
    m_packs.append({QStringLiteral("Memes"), {
        {QStringLiteral("\xF0\x9F\x98\x82"), QStringLiteral("Joy")},
        {QStringLiteral("\xF0\x9F\x98\x82"), QStringLiteral("Laughing")},
        {QStringLiteral("\xF0\x9F\xA4\xA3"), QStringLiteral("ROFL")},
        {QStringLiteral("\xF0\x9F\x98\x85"), QStringLiteral("Grin")},
        {QStringLiteral("\xF0\x9F\x98\x86"), QStringLiteral("XD")},
        {QStringLiteral("\xF0\x9F\x98\x9C"), QStringLiteral("Wink Tongue")},
        {QStringLiteral("\xF0\x9F\x98\x9D"), QStringLiteral("Tongue")},
        {QStringLiteral("\xF0\x9F\xA4\xAA"), QStringLiteral("Star-Struck")},
        {QStringLiteral("\xF0\x9F\x98\xB3"), QStringLiteral("Flushed")},
        {QStringLiteral("\xF0\x9F\x98\xB1"), QStringLiteral("Scream")},
        {QStringLiteral("\xF0\x9F\xA4\x94"), QStringLiteral("Thinking")},
        {QStringLiteral("\xF0\x9F\x98\x8F"), QStringLiteral("Smirk")},
    }});
    m_packs.append({QStringLiteral("Cats"), {
        {QStringLiteral("\xF0\x9F\x90\xB1"), QStringLiteral("Cat Face")},
        {QStringLiteral("\xF0\x9F\x98\xBA"), QStringLiteral("Smile Cat")},
        {QStringLiteral("\xF0\x9F\x98\xB9"), QStringLiteral("Tear Cat")},
        {QStringLiteral("\xF0\x9F\x98\xBB"), QStringLiteral("Heart Cat")},
        {QStringLiteral("\xF0\x9F\x98\xBC"), QStringLiteral("Grin Cat")},
        {QStringLiteral("\xF0\x9F\x98\xBD"), QStringLiteral("Kiss Cat")},
        {QStringLiteral("\xF0\x9F\x90\xB1\xE2\x80\x8D\xE2\xAC\x9B"), QStringLiteral("Black Cat")},
        {QStringLiteral("\xF0\x9F\x90\x88"), QStringLiteral("Cat 2")},
        {QStringLiteral("\xF0\x9F\x90\x88\xE2\x80\x8D\xE2\xAC\x9B"), QStringLiteral("Black Cat V2")},
        {QStringLiteral("\xF0\x9F\x99\x80"), QStringLiteral("Weary Cat")},
        {QStringLiteral("\xF0\x9F\x98\xBF"), QStringLiteral("Crying Cat")},
        {QStringLiteral("\xF0\x9F\x98\xBE"), QStringLiteral("Pouting Cat")},
    }});
    m_packs.append({QStringLiteral("Gestures"), {
        {QStringLiteral("\xF0\x9F\x91\x8D"), QStringLiteral("Thumbs Up")},
        {QStringLiteral("\xF0\x9F\x91\x8E"), QStringLiteral("Thumbs Down")},
        {QStringLiteral("\xE2\x9C\x8C\xEF\xB8\x8F"), QStringLiteral("Peace")},
        {QStringLiteral("\xF0\x9F\x91\x8B"), QStringLiteral("Wave")},
        {QStringLiteral("\xF0\x9F\x92\xAA"), QStringLiteral("Biceps")},
        {QStringLiteral("\xF0\x9F\x99\x8F"), QStringLiteral("Pray")},
        {QStringLiteral("\xF0\x9F\x91\x8C"), QStringLiteral("OK")},
        {QStringLiteral("\xF0\x9F\xA4\x9E"), QStringLiteral("Fingers Crossed")},
        {QStringLiteral("\xF0\x9F\xA4\x99"), QStringLiteral("Call Me")},
        {QStringLiteral("\xE2\x9C\x8A"), QStringLiteral("Fist")},
        {QStringLiteral("\xF0\x9F\x91\x8F"), QStringLiteral("Clap")},
        {QStringLiteral("\xF0\x9F\x99\x8C"), QStringLiteral("Raised Hands")},
    }});

    for (int i = 0; i < m_packs.size(); ++i) {
        if (i == 0) {
            updateFavoriteTab();
        } else {
            populateTab(i, m_packs[i].stickers);
        }
    }
}

void StickerPicker::populateTab(int index, const QVector<QPair<QString, QString>>& stickers)
{
    if (m_tabContents.contains(index)) {
        auto& content = m_tabContents[index];
        qDeleteAll(content.buttons);
        content.buttons.clear();
        delete content.container;
    }

    auto* scrollArea = new QScrollArea(m_tabWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setFrameShape(QFrame::NoFrame);

    auto* container = new QWidget(scrollArea);
    auto* gridLayout = new QGridLayout(container);
    gridLayout->setContentsMargins(8, 8, 8, 8);
    gridLayout->setSpacing(6);

    QVector<QPushButton*> buttons;
    for (int i = 0; i < stickers.size(); ++i) {
        const auto& [emoji, name] = stickers[i];
        auto* button = createStickerButton(emoji, name);
        int row = i / kGridColumns;
        int col = i % kGridColumns;
        gridLayout->addWidget(button, row, col);
        buttons.append(button);
    }

    scrollArea->setWidget(container);

    if (index < m_tabWidget->count()) {
        m_tabWidget->removeTab(index);
    }
    m_tabWidget->insertTab(index, scrollArea, m_packs[index].name);

    TabContent content;
    content.gridLayout = gridLayout;
    content.container = container;
    content.buttons = buttons;
    m_tabContents[index] = content;
}

void StickerPicker::updateFavoriteTab()
{
    QVector<QPair<QString, QString>> favStickers;
    for (const auto& pack : m_packs) {
        if (pack.name == QStringLiteral("Favorites"))
            continue;
        for (const auto& [emoji, name] : pack.stickers) {
            if (isFavorite(emoji)) {
                favStickers.append({emoji, name});
            }
        }
    }
    populateTab(0, favStickers);
}

bool StickerPicker::isFavorite(const QString& emoji) const
{
    return m_favorites.contains(emoji);
}

void StickerPicker::toggleFavorite(const QString& emoji)
{
    if (m_favorites.contains(emoji)) {
        m_favorites.removeAll(emoji);
    } else {
        m_favorites.append(emoji);
    }
    saveFavorites();
    updateFavoriteTab();
}

void StickerPicker::onStickerClicked()
{
    auto* button = qobject_cast<QPushButton*>(sender());
    if (!button)
        return;

    QString emoji = button->property("stickerEmoji").toString();
    if (!emoji.isEmpty()) {
        emit stickerSelected(emoji);
        close();
    }
}

void StickerPicker::onStickerContextMenu(const QPoint& pos)
{
    auto* button = qobject_cast<QPushButton*>(sender());
    if (!button)
        return;

    QString emoji = button->property("stickerEmoji").toString();
    if (emoji.isEmpty())
        return;

    QMenu menu(this);
    bool fav = isFavorite(emoji);
    QString actionText = fav ? QStringLiteral("Remove from Favorites") : QStringLiteral("Add to Favorites");

    QAction* action = menu.addAction(actionText);
    action->setData(emoji);
    QAction* result = menu.exec(button->mapToGlobal(pos));

    if (result == action) {
        toggleFavorite(emoji);
    }
}

void StickerPicker::onSearchTextChanged(const QString& text)
{
    filterStickers(text);
}

void StickerPicker::filterStickers(const QString& filter)
{
    if (filter.isEmpty()) {
        for (auto it = m_tabContents.begin(); it != m_tabContents.end(); ++it) {
            for (auto* button : it->buttons) {
                button->setVisible(true);
            }
        }
        return;
    }

    const QString lowerFilter = filter.toLower();
    for (auto it = m_tabContents.begin(); it != m_tabContents.end(); ++it) {
        int visibleCount = 0;
        for (auto* button : it->buttons) {
            bool matches = button->toolTip().toLower().contains(lowerFilter) ||
                           button->text().toLower().contains(lowerFilter);
            button->setVisible(matches);
            if (matches)
                ++visibleCount;
        }

        if (visibleCount > 0 && !filter.isEmpty()) {
            m_tabWidget->setCurrentIndex(it.key());
        }
    }
}

void StickerPicker::onAddStickerPack()
{
    // Placeholder: would open a dialog to add/import sticker packs
    // For now, just a visual acknowledgment
    m_addPackButton->setText(QStringLiteral("Feature coming soon..."));
    m_addPackButton->setEnabled(false);
}
