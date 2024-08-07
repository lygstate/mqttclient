#include "mqttclient.h"
#include <QThread>

class MqttClientPrivate
{
public:
	MqttClientPrivate(QObject *parent):
		m_parent(parent),
		m_thread()
	{
		parent->moveToThread(&m_thread);
		m_thread.start();
	}
	QObject *m_parent;
	QThread m_thread;
	QString host;
	uint16_t port;
	QMap<QString, QMqttSubscription *> subsMap;
	QMqttClient *client;
	bool m_connected;
};

Q_DECLARE_METATYPE(MqttOnMessageCallback);

MqttClient::MqttClient(
	QString host,
	uint16_t port,
	QObject *parent)
	: QObject(parent)
{
	qRegisterMetaType<MqttOnMessageCallback>();
	d.reset(new MqttClientPrivate(this));

	d->host = host;
	d->port = port;

	QMetaObject::invokeMethod(this, "onInit", Qt::QueuedConnection);
}

void MqttClient::onInit()
{
	d->client = new QMqttClient(this);
	d->client->setHostname(d->host);
	d->client->setPort(d->port);
	d->client->connectToHost();
	d->m_connected = false;
	QObject::connect(d->client, &QMqttClient::connected, [=](){
		d->m_connected = true;
		qDebug () << "connected";
	});
	QObject::connect(d->client, &QMqttClient::connected, [=](){
		d->m_connected = false;
	});
}

void MqttClient::onPublish(const QString &topic, const QByteArray &content, quint8 qos)
{
	d->client->publish(topic, content, qos);
}

void MqttClient::onSubscribe(const QString &topic, quint8 qos, MqttOnMessageCallback cb)
{
	auto subs = d->client->subscribe(topic, qos);
	if (cb)
	{
		QObject::connect(subs, &QMqttSubscription::messageReceived, [=](QMqttMessage msg){
			cb(msg);
		});
	}
	d->subsMap.insert(topic, subs);
}
