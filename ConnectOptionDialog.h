#ifndef CONNECTOPTIONDIALOG_H
#define CONNECTOPTIONDIALOG_H

#include <QDialog>
#include <QSslCertificate>
#include <QSslKey>

namespace Ui {
class ConnectOptionDialog;
}

class ConnectOptionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectOptionDialog(QWidget *parent = 0);
    ~ConnectOptionDialog();

    bool needWebsocket() const;
    quint32 port() const;

    bool cleanSession() const;
    bool willFlag() const;
    bool willQosFlag() const;
    const QString willTopic() const;
    const QByteArray willMessage() const;
    quint8 willQoS() const;
    bool willRetainFlag() const;
    bool userNameFlag() const;
    const QString userName() const;
    bool passwordFlag() const;
    const QByteArray password() const;
    quint16 keepAlive() const;

    bool autoReconnect() const;
    int autoReconnectInterval() const;

    bool useSsl() const;
    QSslCertificate certificate() const;
    QSslKey privateKey() const;
    bool ignoreSelfSigned() const;

    void setEditable(bool editable);

private:
    Ui::ConnectOptionDialog *ui;

private slots:
    void willFlagToggled(bool toggled);
    void userNameFlagToggled(bool toggled);
    void autoReconnectToggled(bool toggled);
    void useSslToggled(bool toggled);
    void btnCertificateClicked();
    void btnPriavteKeyClicked();
};

#endif // CONNECTOPTIONDIALOG_H
