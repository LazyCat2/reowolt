#ifndef SUBMITABLETEXTEDIT_H
#define SUBMITABLETEXTEDIT_H

#include <QTextEdit>
#include <QKeyEvent>

class SubmitableTextEdit : public QTextEdit
{
public:
    SubmitableTextEdit();
    void keyPressEvent(QKeyEvent *event) override {
        if (
            (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
            && !(event->modifiers() & Qt::ShiftModifier)
        ) emit submit();
    }

signals:
    void submit();
};

#endif // SUBMITABLETEXTEDIT_H
