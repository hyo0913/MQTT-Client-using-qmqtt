#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "QUuid"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_client(new QMQTT::Client()),
    m_connectOptionDialog(new ConnectOptionDialog),
    m_msgId(0),
    m_publishCompleted(false),
    m_subSplitter(new QSplitter(Qt::Horizontal))
{
    ui->setupUi(this);

    // MQTT CLIENT
    connect(m_client, SIGNAL(connected()), this, SLOT(mqttConnected()));
    connect(m_client, SIGNAL(disconnected()), this, SLOT(mqttDisconnected()));
    connect(m_client, SIGNAL(error(QMQTT::ClientError)), this, SLOT(mqttError(QMQTT::ClientError)));

    connect(m_client, SIGNAL(subscribed(QString,quint8)), this, SLOT(mqttSubscribed(QString,quint8)));
    connect(m_client, SIGNAL(unsubscribed(QString)), this, SLOT(mqttUnsubscribed(QString)));
    connect(m_client, SIGNAL(published(QMQTT::Message,quint16)), this, SLOT(mqttPublished(QMQTT::Message,quint16)));
    connect(m_client, SIGNAL(received(QMQTT::Message)), this, SLOT(mqttReceived(QMQTT::Message)));
    connect(m_client, SIGNAL(pingresp()), this, SLOT(mqttPingResp()));

    // UI
    connect(ui->pushButtonConnect, SIGNAL(clicked(bool)), this, SLOT(btnConnectClicked()));
    connect(ui->pushButtonCreateId, SIGNAL(clicked(bool)), this, SLOT(btnCreateIdClicked()));
    connect(ui->pushButtonSubscribe, SIGNAL(clicked(bool)), this, SLOT(btnSubscribeClicked()));
    connect(ui->pushButtonUnsubscribe, SIGNAL(clicked(bool)), this, SLOT(btnUnsubscribeClicked()));
    connect(ui->pushButtonPublish, SIGNAL(clicked(bool)), this, SLOT(btnPublishClicked()));
    connect(ui->toolButtonConnectOption, SIGNAL(clicked(bool)), this, SLOT(btnConnectOptionClicked()));

    btnCreateIdClicked();
    ui->pushButtonPublish->setEnabled(false);
    ui->pushButtonSubscribe->setEnabled(false);
    ui->pushButtonUnsubscribe->setEnabled(false);

    m_subSplitter->setChildrenCollapsible(false);
    ui->horizontalLayoutSubscribes->addWidget(m_subSplitter);
}

MainWindow::~MainWindow()
{
    if( m_client->isConnectedToHost() )
    {
        m_client->disconnectFromHost();
    }
    delete m_client;
    clearSubPrintWidgets();
    delete ui;
}

void MainWindow::appendLog(const QString &logName, const QString &logText)
{
    const QString text = QString("%1 : %2").arg(logName).arg(logText);
    ui->textEditLog->append(text);
}

void MainWindow::clearSubPrintWidgets()
{
    QStringList keys = m_subPrintWidgets.keys();
    SubscribePrintForm* sub;

    while( keys.count() > 0 )
    {
        sub = NULL;
        sub = m_subPrintWidgets.take(keys.takeFirst());
        if( sub != NULL ) delete sub;
    }
}

bool MainWindow::checkTopic(const QString &myTopic, const QString &recvTopic) const
{
    if( myTopic == recvTopic ) return true;

    bool result = true;
    bool singleLevel = false;
    bool multiLevel = false;
    QStringList lstMyTopic = myTopic.split("/");
    QStringList lstRecvTopic = recvTopic.split("/");

    for( int i = 0; i < lstMyTopic.count(); i++ )
    {
        if( recvTopic.count() < i+1 )
        {
            result = false;
            break;
        }

        if( lstMyTopic.at(i) == "#" )
        {
            multiLevel = true;
            break;
        }

        if( lstMyTopic.at(i) == "+" )
        {
            singleLevel = true;
            continue;
        }

        if( lstMyTopic.at(i) != lstRecvTopic.at(i) )
        {
            result = false;
            break;
        }
    }

    if(     (result == true)
            && ((singleLevel == true) || ((singleLevel == false) && (multiLevel == false)))
            && (lstMyTopic.count() != lstRecvTopic.count())
            )
    {
        result = false;
    }

    return result;
}

void MainWindow::mqttConnected()
{
    m_connectOptionDialog->setEditable(false);

    ui->pushButtonConnect->setText("Connected");
    ui->lineEditHostAddress->setEnabled(false);
    ui->lineEditClientId->setEnabled(false);
    ui->pushButtonCreateId->setEnabled(false);

    ui->pushButtonPublish->setEnabled(true);
    ui->pushButtonSubscribe->setEnabled(true);
    ui->pushButtonUnsubscribe->setEnabled(true);

    appendLog("Connected", m_client->host().toString());
}

void MainWindow::mqttDisconnected()
{
    m_msgId = 0;
    m_lastPubTopic.clear();
    m_lastPubPayload.clear();
    m_publishCompleted = false;

    m_connectOptionDialog->setEditable(true);

    ui->pushButtonConnect->setText("Connect");
    ui->pushButtonConnect->setChecked(false);
    ui->lineEditHostAddress->setEnabled(true);
    ui->lineEditClientId->setEnabled(true);
    ui->pushButtonCreateId->setEnabled(true);

    ui->pushButtonPublish->setEnabled(false);
    ui->pushButtonSubscribe->setEnabled(false);
    ui->pushButtonUnsubscribe->setEnabled(false);

    appendLog("Disconnected", m_client->host().toString());

    clearSubPrintWidgets();
}

void MainWindow::mqttError(const QMQTT::ClientError error)
{
    QString errorDetail;
    switch(error)
    {
    case QMQTT::SocketConnectionRefusedError:           errorDetail = "SocketConnectionRefusedError"; break;
    case QMQTT::SocketRemoteHostClosedError:            errorDetail = "SocketRemoteHostClosedError"; break;
    case QMQTT::SocketHostNotFoundError:                errorDetail = "SocketHostNotFoundError"; break;
    case QMQTT::SocketAccessError:                      errorDetail = "SocketAccessError"; break;
    case QMQTT::SocketResourceError:                    errorDetail = "SocketResourceError"; break;
    case QMQTT::SocketTimeoutError:                     errorDetail = "SocketTimeoutError"; break;
    case QMQTT::SocketDatagramTooLargeError:            errorDetail = "SocketDatagramTooLargeError"; break;
    case QMQTT::SocketNetworkError:                     errorDetail = "SocketNetworkError"; break;
    case QMQTT::SocketAddressInUseError:                errorDetail = "SocketAddressInUseError"; break;
    case QMQTT::SocketAddressNotAvailableError:         errorDetail = "SocketAddressNotAvailableError"; break;
    case QMQTT::SocketUnsupportedSocketOperationError:  errorDetail = "SocketUnsupportedSocketOperationError"; break;
    case QMQTT::SocketUnfinishedSocketOperationError:   errorDetail = "SocketUnfinishedSocketOperationError"; break;
    case QMQTT::SocketProxyAuthenticationRequiredError: errorDetail = "SocketProxyAuthenticationRequiredError"; break;
    case QMQTT::SocketSslHandshakeFailedError:          errorDetail = "SocketSslHandshakeFailedError"; break;
    case QMQTT::SocketProxyConnectionRefusedError:      errorDetail = "SocketProxyConnectionRefusedError"; break;
    case QMQTT::SocketProxyConnectionClosedError:       errorDetail = "SocketProxyConnectionClosedError"; break;
    case QMQTT::SocketProxyConnectionTimeoutError:      errorDetail = "SocketProxyConnectionTimeoutError"; break;
    case QMQTT::SocketProxyNotFoundError:               errorDetail = "SocketProxyNotFoundError"; break;
    case QMQTT::SocketProxyProtocolError:               errorDetail = "SocketProxyProtocolError"; break;
    case QMQTT::SocketOperationError:                   errorDetail = "SocketOperationError"; break;
    case QMQTT::SocketSslInternalError:                 errorDetail = "SocketSslInternalError"; break;
    case QMQTT::SocketSslInvalidUserDataError:          errorDetail = "SocketSslInvalidUserDataError"; break;
    case QMQTT::SocketTemporaryError:                   errorDetail = "SocketTemporaryError"; break;
    default:                                            errorDetail = "UnknownError";
    }

    appendLog("ERROR", errorDetail);
}

void MainWindow::mqttSubscribed(const QString &topic, quint8 qos)
{
    QString text = QString("%1 (Qos:%2)").arg(topic).arg(qos);
    appendLog("Subscribed", text);

    if( m_subPrintWidgets.contains(topic) == false )
    {
        SubscribePrintForm* sub = new SubscribePrintForm(topic);
        m_subSplitter->addWidget(sub);
        m_subPrintWidgets.insert(topic, sub);
    }
}

void MainWindow::mqttUnsubscribed(const QString &topic)
{
    appendLog("Unsubscribed", topic);

    if( m_subPrintWidgets.contains(topic) )
    {
        SubscribePrintForm* sub = m_subPrintWidgets.take(topic);
        if( sub != NULL ) delete sub;
    }
}

void MainWindow::mqttPublished(const QMQTT::Message &message, quint16 msgId)
{
    m_publishCompleted = true;

    const QString text = QString("%1:%2 (ID:%3/QoS:%4/Retain:%5/DUP:%6)")
            .arg(message.topic())
            .arg(QString::fromUtf8(message.payload()))
            .arg(msgId)
            .arg(message.qos())
            .arg(message.retain())
            .arg(message.dup());
    appendLog("Published", text);
}

void MainWindow::mqttReceived(const QMQTT::Message &message)
{
    if( message.qos() > 0 )
    {
        m_msgId = message.id();
    }

    const QString text = QString("%1:%2 (ID:%3/Qos:%4/Reatin:%5/DUP:%6)")
            .arg(message.topic())
            .arg(QString::fromUtf8(message.payload()))
            .arg(message.id())
            .arg(message.qos())
            .arg(message.retain())
            .arg(message.dup());
    appendLog("Received", text);

    QStringList lstMySubTopics = m_subPrintWidgets.keys();
    for( int i = 0; i < lstMySubTopics.count(); i++ )
    {
        if( checkTopic(lstMySubTopics.at(i), message.topic()) )
        {
            SubscribePrintForm* sub = m_subPrintWidgets.value(lstMySubTopics.at(i));
            if( sub != NULL )
            {
                sub->printMessage(QString(message.payload()));
            }
        }
    }
}

void MainWindow::mqttPingResp()
{
    ui->textEditLog->append("PING RESP");
}

void MainWindow::btnConnectClicked()
{
    if( m_client->isConnectedToHost() == false )
    {
        QHostAddress hostAddress;
        QString temp = ui->lineEditHostAddress->text();
        if( temp.isEmpty() || temp.contains("localhost", Qt::CaseInsensitive) )
        {
            hostAddress = QHostAddress(QHostAddress::LocalHost);
            ui->lineEditHostAddress->setText(QString("localhost:%1").arg(hostAddress.toString()));
        }
        else
        {
            hostAddress.setAddress(temp);
        }

        // clean session flag, will flags
        m_client->setCleanSession(m_connectOptionDialog->cleanSession());   // cleanSession flag
        if( m_connectOptionDialog->willFlag() ) // will flag
        {
            m_client->setWillTopic(m_connectOptionDialog->willTopic());  // will topic. QMQTT는 will topic의 유무로 will flag를 적용한.
            m_client->setWillMessage(m_connectOptionDialog->willMessage()); // will message
            if( m_connectOptionDialog->willQosFlag() )  // will qos
            {
                m_client->setWillQos(m_connectOptionDialog->willQoS());
            }
            else
            {
                m_client->setWillQos(0);
            }
            m_client->setWillRetain(m_connectOptionDialog->willRetainFlag());   // will retain
        }
        else
        {
            m_client->setWillTopic(QString());
            m_client->setWillMessage(QByteArray());
            m_client->setWillQos(0);
            m_client->setWillRetain(false);
        }

        // username, password
        if( m_connectOptionDialog->userNameFlag() ) // user name
        {
            m_client->setUsername(m_connectOptionDialog->userName());

            if( m_connectOptionDialog->passwordFlag() ) // password
            {
                m_client->setPassword(m_connectOptionDialog->password());
            }
            else
            {
                m_client->setPassword(QByteArray());
            }
        }
        else
        {
            m_client->setUsername(QString());
            m_client->setPassword(QByteArray());
        }

        // auto reconnect
        if( m_connectOptionDialog->autoReconnect() )
        {
            m_client->setAutoReconnect(true);
            m_client->setAutoReconnectInterval(m_connectOptionDialog->autoReconnectInterval()*1000);
            ui->textEditLog->append(QString("Using automatically reconnect (%1sec)").arg(m_connectOptionDialog->autoReconnectInterval()));
        }
        else
        {
            m_client->setAutoReconnect(false);
        }

        m_client->setHost(hostAddress);
        m_client->setPort(ui->spinBoxPort->value());
        m_client->setClientId(ui->lineEditClientId->text());
        m_client->setKeepAlive(m_connectOptionDialog->keepAlive());

        appendLog("Connect", hostAddress.toString());

        m_client->connectToHost();
    }
    else
    {
        m_client->setAutoReconnect(false);
        m_client->disconnectFromHost();
    }
}

void MainWindow::btnCreateIdClicked()
{
    QUuid id = QUuid::createUuid();
    ui->lineEditClientId->setText(id.toString());
}

void MainWindow::btnSubscribeClicked()
{
    m_client->subscribe(ui->lineEditTopic->text(), static_cast<quint8>(ui->spinBoxQoS->value()));
}

void MainWindow::btnUnsubscribeClicked()
{
    m_client->unsubscribe(ui->lineEditTopic->text());
}

void MainWindow::btnPublishClicked()
{
    //QMQTT::Message message(m_msgId, ui->lineEditTopic->text(), ui->lineEditPayload->text().toUtf8());
    // QMQTT::Message에 어떤 파라미터 들이 set 되는지 이해를 돕기위해 생성자에 파라미터를 넘기지 않고 아래 처럼 각각 함수들을 호출했다.
    // 이해했다면 나중엔 편한대로 코딩해라.

    QMQTT::Message message;
    message.setTopic(ui->lineEditTopic->text());
    message.setPayload(ui->lineEditPayload->text().toUtf8());
    message.setQos(static_cast<quint8>(ui->spinBoxQoS->value()));
    message.setRetain(ui->checkBoxRetain->isChecked());
    if( ui->spinBoxQoS->value() > 0 )
    {
        message.setId(++m_msgId);
    }

    if( m_publishCompleted == false && m_lastPubTopic == ui->lineEditTopic->text() && m_lastPubPayload == ui->lineEditPayload->text().toUtf8() )
    {
        message.setDup(true);
    }

    m_lastPubTopic = ui->lineEditTopic->text();
    m_lastPubPayload = ui->lineEditPayload->text().toUtf8();
    m_publishCompleted = false;

    m_msgId = m_client->publish(message);
}

void MainWindow::btnConnectOptionClicked()
{
    m_connectOptionDialog->exec();
}
