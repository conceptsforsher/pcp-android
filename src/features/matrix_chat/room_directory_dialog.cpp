#include "features/matrix_chat/room_directory_dialog.hpp"

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

#include "app/application.hpp"
#include "matrix/matrix_session.hpp"
#include "protocol/protocol_room.hpp"
#include "protocol/protocol_type.hpp"

namespace {

class RoomResultWidget : public QWidget
{
public:
    RoomResultWidget(const ProtocolRoom &room, QWidget *parent = nullptr)
        : QWidget(parent)
    {
        auto *rowLayout = new QHBoxLayout(this);
        rowLayout->setContentsMargins(4, 4, 4, 4);

        auto *infoLayout = new QVBoxLayout;
        auto *nameLabel = new QLabel(QStringLiteral("<b>%1</b>").arg(room.name.toHtmlEscaped()), this);
        infoLayout->addWidget(nameLabel);

        QString details;
        if (!room.topic.isEmpty())
            details += tr("Topic: %1").arg(room.topic);
        if (room.memberCount > 0) {
            if (!details.isEmpty())
                details += QStringLiteral("  |  ");
            details += tr("Members: %1").arg(room.memberCount);
        }
        if (!details.isEmpty()) {
            auto *detailLabel = new QLabel(details, this);
            detailLabel->setStyleSheet(QStringLiteral("color: gray;"));
            infoLayout->addWidget(detailLabel);
        }

        rowLayout->addLayout(infoLayout, 1);

        auto *joinButton = new QPushButton(tr("Join"), this);
        joinButton->setFixedWidth(60);
        rowLayout->addWidget(joinButton);

        connect(joinButton, &QPushButton::clicked, this, [roomId = room.id, name = room.name, this]() {
            auto *session = pcp::Application::instance().matrixSession();
            if (!session) {
                QMessageBox::warning(this->window(), tr("Error"), tr("No Matrix session available."));
                return;
            }
            auto reply = QMessageBox::question(this->window(), tr("Join Room"),
                                               tr("Do you want to join \"%1\"?").arg(name),
                                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
            if (reply == QMessageBox::Yes) {
                session->joinRoom(roomId);
                QMessageBox::information(this->window(), tr("Joined"),
                                         tr("Successfully joined \"%1\".").arg(name));
                if (auto *dlg = qobject_cast<QDialog *>(this->window()))
                    dlg->accept();
            }
        });
    }
};

} // anonymous namespace

RoomDirectoryDialog::RoomDirectoryDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUi();
}

void RoomDirectoryDialog::setupUi()
{
    setWindowTitle(tr("Search Matrix Rooms"));
    setMinimumSize(500, 400);
    setModal(true);

    auto *mainLayout = new QVBoxLayout(this);

    auto *searchLayout = new QHBoxLayout;
    m_searchEdit = new QLineEdit(this);
    m_searchEdit->setPlaceholderText(tr("Enter room name or topic..."));
    m_searchButton = new QPushButton(tr("Search"), this);
    searchLayout->addWidget(m_searchEdit);
    searchLayout->addWidget(m_searchButton);
    mainLayout->addLayout(searchLayout);

    m_resultsList = new QListWidget(this);
    m_resultsList->setSelectionMode(QAbstractItemView::SingleSelection);
    mainLayout->addWidget(m_resultsList);

    auto *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    auto *closeButton = new QPushButton(tr("Close"), this);
    buttonLayout->addWidget(closeButton);
    mainLayout->addLayout(buttonLayout);

    connect(m_searchButton, &QPushButton::clicked, this, &RoomDirectoryDialog::onSearchClicked);
    connect(m_searchEdit, &QLineEdit::returnPressed, this, &RoomDirectoryDialog::onSearchClicked);
    connect(m_resultsList, &QListWidget::itemDoubleClicked, this, &RoomDirectoryDialog::onItemDoubleClicked);
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);
}

void RoomDirectoryDialog::onSearchClicked()
{
    const QString query = m_searchEdit->text().trimmed();
    if (query.isEmpty())
        return;

    m_resultsList->clear();
    m_searchButton->setEnabled(false);

    auto *session = pcp::Application::instance().matrixSession();
    if (!session) {
        QMessageBox::warning(this, tr("Error"), tr("No Matrix session available."));
        m_searchButton->setEnabled(true);
        return;
    }

    const QVector<ProtocolRoom> results = session->searchRooms(query);

    for (const auto &room : results) {
        auto *item = new QListWidgetItem();
        item->setData(Qt::UserRole, room.id);
        item->setSizeHint(QSize(0, 52));
        m_resultsList->addItem(item);

        auto *widget = new RoomResultWidget(room, m_resultsList);
        m_resultsList->setItemWidget(item, widget);
    }

    m_searchButton->setEnabled(true);

    if (results.isEmpty()) {
        QMessageBox::information(this, tr("No Results"), tr("No rooms found matching your query."));
    }
}

void RoomDirectoryDialog::onItemDoubleClicked(QListWidgetItem *item)
{
    if (!item)
        return;

    const QString roomId = item->data(Qt::UserRole).toString();
    auto *session = pcp::Application::instance().matrixSession();
    if (!session) {
        QMessageBox::warning(this, tr("Error"), tr("No Matrix session available."));
        return;
    }

    auto *widget = m_resultsList->itemWidget(item);
    QString roomName = tr("this room");
    if (widget) {
        roomName = widget->findChild<QLabel *>()->text();
        roomName.remove(QRegularExpression(QStringLiteral("<[^>]*>")));
    }

    auto reply = QMessageBox::question(this, tr("Join Room"),
                                       tr("Do you want to join \"%1\"?").arg(roomName),
                                       QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        session->joinRoom(roomId);
        QMessageBox::information(this, tr("Joined"), tr("Successfully joined \"%1\".").arg(roomName));
        accept();
    }
}
