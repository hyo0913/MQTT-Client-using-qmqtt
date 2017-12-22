#include "SubscribePrintForm.h"
#include "ui_SubscribePrintForm.h"

SubscribePrintForm::SubscribePrintForm(const QString &topic) :
    QWidget(NULL),
    ui(new Ui::SubscribePrintForm)
{
    ui->setupUi(this);

    ui->labelTopic->setText(topic);
}

SubscribePrintForm::~SubscribePrintForm()
{
    delete ui;
}

void SubscribePrintForm::printMessage(const QString &message)
{
    ui->textEditMessages->append(message);
}
