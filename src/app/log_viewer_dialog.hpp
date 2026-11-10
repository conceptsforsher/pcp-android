#pragma once

#include <QDialog>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLabel>

namespace pcp_chat {

class LogViewerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LogViewerDialog(QWidget *parent = nullptr);

private:
    void refreshLogs();
    void filterLogs(const QString &filter);
    void exportLogs();
    void clearLogs();

    QTextEdit *m_logView;
    QComboBox *m_levelFilter;
    QComboBox *m_sourceFilter;
    QPushButton *m_refreshBtn;
    QPushButton *m_exportBtn;
    QPushButton *m_clearBtn;
};

} // namespace pcp_chat
