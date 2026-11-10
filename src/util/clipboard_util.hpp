#pragma once

#include <QApplication>
#include <QClipboard>
#include <QMimeData>
#include <QString>

namespace pcp_chat {
namespace util {

inline void copyToClipboard(const QString &text)
{
    QClipboard *clipboard = QApplication::clipboard();
    if (clipboard) {
        clipboard->setText(text);
    }
}

inline void copyImageToClipboard(const QImage &image)
{
    QClipboard *clipboard = QApplication::clipboard();
    if (clipboard) {
        clipboard->setImage(image);
    }
}

inline QString clipboardText()
{
    QClipboard *clipboard = QApplication::clipboard();
    return clipboard ? clipboard->text() : QString();
}

inline bool clipboardHasImage()
{
    QClipboard *clipboard = QApplication::clipboard();
    return clipboard && !clipboard->image().isNull();
}

} // namespace util
} // namespace pcp_chat
