#include "ConnectOptionDialog.h"
#include "ui_ConnectOptionDialog.h"

ConnectOptionDialog::ConnectOptionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectOptionDialog)
{
    ui->setupUi(this);

    connect(ui->checkBoxWillFlag, SIGNAL(toggled(bool)), this, SLOT(willFlagToggled(bool)));
    willFlagToggled(false);

    connect(ui->checkBoxUserNameFlag, SIGNAL(toggled(bool)), this, SLOT(userNameFlagToggled(bool)));
    userNameFlagToggled(false);

    connect(ui->checkBoxAutoReconnect, SIGNAL(toggled(bool)), this, SLOT(autoReconnectToggled(bool)));
    autoReconnectToggled(false);
}

ConnectOptionDialog::~ConnectOptionDialog()
{
    delete ui;
}

bool ConnectOptionDialog::cleanSession() const
{
    return ui->checkBoxCleanSession->isChecked();
}

bool ConnectOptionDialog::willFlag() const
{
    return ui->checkBoxWillFlag->isChecked();
}

bool ConnectOptionDialog::willQosFlag() const
{
    return ui->checkBoxWillQoS->isChecked();
}

const QString ConnectOptionDialog::willTopic() const
{
    return ui->lineEditWillTopic->text();
}

const QByteArray ConnectOptionDialog::willMessage() const
{
    return ui->lineEditWillMessage->text().toUtf8();
}

quint8 ConnectOptionDialog::willQoS() const
{
    return static_cast<quint8>(ui->spinBoxWillQoS->value());
}

bool ConnectOptionDialog::willRetainFlag() const
{
    return ui->checkBoxWillRetain->isChecked();
}

bool ConnectOptionDialog::userNameFlag() const
{
    return ui->checkBoxUserNameFlag->isChecked();
}

const QString ConnectOptionDialog::userName() const
{
    return ui->lineEditUserName->text();
}

bool ConnectOptionDialog::passwordFlag() const
{
    return ui->checkBoxPasswordFlag->isChecked();
}

const QByteArray ConnectOptionDialog::password() const
{
    return ui->lineEditPassword->text().toUtf8();
}

quint16 ConnectOptionDialog::keepAlive() const
{
    return static_cast<quint16>(ui->spinBoxKeepAlive->value());
}

bool ConnectOptionDialog::autoReconnect() const
{
    return ui->checkBoxAutoReconnect->isChecked();
}

int ConnectOptionDialog::autoReconnectInterval() const
{
    return ui->spinBoxAutoReconnectInterval->value();
}

void ConnectOptionDialog::setEditable(bool editable)
{
    ui->checkBoxCleanSession->setEnabled(editable);
    ui->checkBoxWillFlag->setEnabled(editable);
    ui->labelWillTopic->setEnabled(editable);
    ui->lineEditWillTopic->setEnabled(editable);
    ui->labelWillMessage->setEnabled(editable);
    ui->lineEditWillMessage->setEnabled(editable);
    ui->checkBoxWillQoS->setEnabled(editable);
    ui->spinBoxWillQoS->setEnabled(editable);
    ui->checkBoxWillRetain->setEnabled(editable);
    ui->checkBoxUserNameFlag->setEnabled(editable);
    ui->lineEditUserName->setEnabled(editable);
    ui->checkBoxPasswordFlag->setEnabled(editable);
    ui->lineEditPassword->setEnabled(editable);
    ui->labelKeepAlive->setEnabled(editable);
    ui->spinBoxKeepAlive->setEnabled(editable);
    ui->labelKeepAliveSec->setEnabled(editable);
    ui->checkBoxAutoReconnect->setEnabled(editable);
    ui->labelAutoReconnectInterval->setEnabled(editable);
    ui->spinBoxAutoReconnectInterval->setEnabled(editable);
    ui->labelAutoReconnectIntervalSec->setEnabled(editable);
}

void ConnectOptionDialog::willFlagToggled(bool toggled)
{
    if( toggled == false )
    {
        ui->checkBoxWillQoS->setChecked(false);
        ui->checkBoxWillRetain->setChecked(false);
    }

    ui->labelWillTopic->setEnabled(toggled);
    ui->labelWillMessage->setEnabled(toggled);
    ui->lineEditWillTopic->setEnabled(toggled);
    ui->lineEditWillMessage->setEnabled(toggled);
    ui->checkBoxWillQoS->setEnabled(toggled);
    ui->spinBoxWillQoS->setEnabled(toggled);
    ui->checkBoxWillRetain->setEnabled(toggled);
}

void ConnectOptionDialog::userNameFlagToggled(bool toggled)
{
    if( toggled == false )
    {
        ui->checkBoxPasswordFlag->setChecked(false);
    }

    ui->checkBoxPasswordFlag->setEnabled(toggled);
    ui->lineEditPassword->setEnabled(toggled);
}

void ConnectOptionDialog::autoReconnectToggled(bool toggled)
{
    ui->labelAutoReconnectInterval->setEnabled(toggled);
    ui->spinBoxAutoReconnectInterval->setEnabled(toggled);
    ui->labelAutoReconnectIntervalSec->setEnabled(toggled);
}
