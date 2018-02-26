#include "ConnectOptionDialog.h"
#include "ui_ConnectOptionDialog.h"

#include "QFileDialog"
#include "QApplication"

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

    connect(ui->checkBoxSsl, SIGNAL(toggled(bool)), this, SLOT(useSslToggled(bool)));
    useSslToggled(false);

    connect(ui->toolButtonCertifiacte, SIGNAL(clicked(bool)), this, SLOT(btnCertificateClicked()));
    connect(ui->toolButtonPrivateKey, SIGNAL(clicked(bool)), this, SLOT(btnPriavteKeyClicked()));
}

ConnectOptionDialog::~ConnectOptionDialog()
{
    delete ui;
}

bool ConnectOptionDialog::needWebsocket() const
{
    bool result = false;

    if( ui->comboBoxNetwork->currentIndex() == 1 )
    {
        result = true;
    }

    return result;
}

quint32 ConnectOptionDialog::port() const
{
    return ui->spinBoxPort->value();
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

bool ConnectOptionDialog::useSsl() const
{
    return ui->checkBoxSsl->isChecked();
}

QSslCertificate ConnectOptionDialog::certificate() const
{
//    QSslCertificate result;

//    QString certFileName = ui->lineEditCertifiacte->text();
//    if( certFileName.isEmpty() ) return QSslCertificate();

//    QFile file(certFileName);
//    if( file.open(QIODevice::ReadOnly) )
//    {
//        QByteArray data = file.readAll();
//        file.close();
//        if( certFileName.contains(".pem") )
//        {
//            result = QSslCertificate(data, QSsl::Pem);
//        }
//        else if( certFileName.contains(".der") )
//        {
//            result = QSslCertificate(data, QSsl::Der);
//        }
//    }

//    return result;

    QList<QSslCertificate> lstCerts;
    QString certFileName = ui->lineEditCertifiacte->text();
    if( certFileName.isEmpty() ) return QSslCertificate();

    lstCerts = QSslCertificate::fromPath(certFileName);
    if( lstCerts.count() <= 0 ) return QSslCertificate();

    return lstCerts.first();
}

QSslKey ConnectOptionDialog::privateKey() const
{
    QSslKey result;
    QString keyFileName = ui->lineEditPrivateKey->text();
    if( keyFileName.isEmpty() ) return QSslKey();

    QFile file(keyFileName);
    if( file.open(QIODevice::ReadOnly) )
    {
        result = QSslKey(file.readAll(), QSsl::Rsa);
    }

    return result;
}

bool ConnectOptionDialog::ignoreSelfSigned() const
{
    return ui->checkBoxIgnoreSelfSigned->isChecked();
}

void ConnectOptionDialog::setEditable(bool editable)
{
    ui->labelNetwork->setEnabled(editable);
    ui->comboBoxNetwork->setEnabled(editable);
    ui->labelPort->setEnabled(editable);
    ui->spinBoxPort->setEnabled(editable);

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

    ui->checkBoxSsl->setEnabled(editable);
    ui->labelCertificate->setEnabled(editable);
    ui->lineEditCertifiacte->setEnabled(editable);
    ui->toolButtonCertifiacte->setEnabled(editable);
    ui->labelPrivateKey->setEnabled(editable);
    ui->lineEditPrivateKey->setEnabled(editable);
    ui->toolButtonPrivateKey->setEnabled(editable);
    ui->checkBoxIgnoreSelfSigned->setEnabled(editable);

    if( editable )
    {
        willFlagToggled(ui->checkBoxWillFlag->isChecked());
        userNameFlagToggled(ui->checkBoxUserNameFlag->isChecked());
        autoReconnectToggled(ui->checkBoxAutoReconnect->isChecked());
        useSslToggled(ui->checkBoxSsl->isChecked());
    }
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

void ConnectOptionDialog::useSslToggled(bool toggled)
{
    ui->labelCertificate->setEnabled(toggled);
    ui->lineEditCertifiacte->setEnabled(toggled);
    ui->toolButtonCertifiacte->setEnabled(toggled);
    ui->labelPrivateKey->setEnabled(toggled);
    ui->lineEditPrivateKey->setEnabled(toggled);
    ui->toolButtonPrivateKey->setEnabled(toggled);
    ui->checkBoxIgnoreSelfSigned->setEnabled(toggled);
}

void ConnectOptionDialog::btnCertificateClicked()
{
    QFileDialog dialog(this, "Certificate", QApplication::applicationDirPath(), "Certificate (*.pem *.der)");
    if( dialog.exec() )
    {
        ui->lineEditCertifiacte->setText(dialog.selectedFiles().first());
    }
}

void ConnectOptionDialog::btnPriavteKeyClicked()
{
    QFileDialog dialog(this, "Private Key", QApplication::applicationDirPath(), "Private Key (*.pem)");
    if( dialog.exec() )
    {
        ui->lineEditPrivateKey->setText(dialog.selectedFiles().first());
    }
}
