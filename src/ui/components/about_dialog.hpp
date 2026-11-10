#pragma once

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QTextBrowser>

namespace pcp_chat {

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = nullptr);

private:
    void setupUi();
    void setupAboutTab();
    void setupCreditsTab();
    void setupLicensesTab();

    QTabWidget *m_tabWidget;
    QLabel *m_logoLabel;
    QLabel *m_versionLabel;
    QTextBrowser *m_licenseView;
    QTextBrowser *m_creditsView;
};

} // namespace pcp_chat
