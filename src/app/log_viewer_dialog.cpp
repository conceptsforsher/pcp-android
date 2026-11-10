#include "log_viewer_dialog.hpp"
#include "log_collector.hpp"
#include <QFileDialog>
#include <QDateTime>
#include <QMessageBox>
#include <QSettings>

namespace pcp_chat {

LogViewerDialog::LogViewerDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Log Viewer");
    setMinimumSize(800, 500);

    auto *layout = new QVBoxLayout(this);

    // Filter bar
    auto *filterBar = new QHBoxLayout();
    filterBar->addWidget(new QLabel("Level:"));
    m_levelFilter = new QComboBox();
    m_levelFilter->addItems({"All", "Debug", "Info", "Warning", "Critical", "Fatal"});
    filterBar->addWidget(m_levelFilter);

    filterBar->addWidget(new QLabel("Source:"));
    m_sourceFilter = new QComboBox();
    m_sourceFilter->addItems({"All", "Network", "Crypto", "Database", "Sync", "UI"});
    filterBar->addWidget(m_sourceFilter);

    filterBar->addStretch();

    m_refreshBtn = new QPushButton("Refresh");
    m_exportBtn = new QPushButton("Export...");
    m_clearBtn = new QPushButton("Clear");

    filterBar->addWidget(m_refreshBtn);
    filterBar->addWidget(m_exportBtn);
    filterBar->addWidget(m_clearBtn);
    layout->addLayout(filterBar);

    // Log view
    m_logView = new QTextEdit();
    m_logView->setReadOnly(true);
    m_logView->setFont(QFont("monospace", 10));
    layout->addWidget(m_logView);

    // Close button
    auto *btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    auto *closeBtn = new QPushButton("Close");
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);
    btnLayout->addWidget(closeBtn);
    layout->addLayout(btnLayout);

    // Connections
    connect(m_refreshBtn, &QPushButton::clicked, this, &LogViewerDialog::refreshLogs);
    connect(m_exportBtn, &QPushButton::clicked, this, &LogViewerDialog::exportLogs);
    connect(m_clearBtn, &QPushButton::clicked, this, &LogViewerDialog::clearLogs);

    connect(m_levelFilter, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this](int) {
        filterLogs(m_levelFilter->currentText());
    });

    refreshLogs();
}

void LogViewerDialog::refreshLogs()
{
    QStringList logs = LogCollector::collectRecentLogs(2000);
    m_logView->setPlainText(logs.join("\n"));

    // Scroll to bottom
    QTextCursor cursor = m_logView->textCursor();
    cursor.movePosition(QTextCursor::End);
    m_logView->setTextCursor(cursor);
}

void LogViewerDialog::filterLogs(const QString &filter)
{
    Q_UNUSED(filter);
    refreshLogs();
}

void LogViewerDialog::exportLogs()
{
    QString path = QFileDialog::getSaveFileName(this, "Export Logs",
        "pcp_logs_" + QDate::currentDate().toString("yyyyMMdd") + ".txt",
        "Text files (*.txt);;All files (*)");
    if (path.isEmpty()) return;

    QStringList logs = LogCollector::collectRecentLogs(10000);
    LogCollector::exportLogs(path, logs);
    QMessageBox::information(this, "Export", "Logs exported to " + path);
}

void LogViewerDialog::clearLogs()
{
    m_logView->clear();
}

} // namespace pcp_chat
