#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>

namespace pcp_chat {

class StatusMessageEditor : public QWidget
{
    Q_OBJECT

public:
    explicit StatusMessageEditor(QWidget *parent = nullptr);
    void setStatus(const QString &status);

signals:
    void statusChanged(const QString &newStatus);

private:
    QLineEdit *m_statusInput;
    QPushButton *m_setBtn;
};

} // namespace pcp_chat
