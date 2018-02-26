#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qmqtt.h>
#include <ConnectOptionDialog.h>
#include <SubscribePrintForm.h>

#include <QSplitter>
#include <QHash>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QMQTT::Client* m_client;
    ConnectOptionDialog* m_connectOptionDialog;
    quint16 m_msgId;
    bool m_publishCompleted;
    QString m_lastPubTopic;
    QByteArray m_lastPubPayload;
    QSplitter* m_subSplitter;
    QHash<QString, SubscribePrintForm*> m_subPrintWidgets;

    void initClient();
    void appendLog(const QString &logName, const QString &logText);

    void clearSubPrintWidgets();
    bool checkTopic(const QString &myTopic, const QString &recvTopic) const;

private slots:
    // MQTT CLIENT
    void mqttConnected();
    void mqttDisconnected();
    void mqttError(const QMQTT::ClientError error);

    void mqttSubscribed(const QString &topic, quint8 qos);
    void mqttUnsubscribed(const QString &topic);
    void mqttPublished(const QMQTT::Message &message, quint16 msgId);
    void mqttReceived(const QMQTT::Message &message);
    void mqttPingResp();

    // UI
    void btnConnectClicked();
    void btnCreateIdClicked();
    void btnSubscribeClicked();
    void btnUnsubscribeClicked();
    void btnPublishClicked();
    void btnConnectOptionClicked();
};

#endif // MAINWINDOW_H
