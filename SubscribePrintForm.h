#ifndef SUBSCRIBEPRINTFORM_H
#define SUBSCRIBEPRINTFORM_H

#include <QWidget>

namespace Ui {
class SubscribePrintForm;
}

class SubscribePrintForm : public QWidget
{
    Q_OBJECT

public:
    explicit SubscribePrintForm(const QString &topic);
    ~SubscribePrintForm();

    void printMessage(const QString &message);

private:
    Ui::SubscribePrintForm *ui;
};

#endif // SUBSCRIBEPRINTFORM_H
