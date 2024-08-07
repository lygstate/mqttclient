#ifndef MQTTCLIENT_H
#define MQTTCLIENT_H

#include <QObject>
#include <QScopedPointer>
#include <functional>
#include <QMqttMessage>
#include <QMqttClient>

typedef std::function<void(QMqttMessage &msg)> MqttOnMessageCallback;

class MqttClientPrivate;
class MqttClient : public QObject
{
	Q_OBJECT
public:
	explicit MqttClient(
		QString host,
		uint16_t port,
		QObject *parent = nullptr);

	// thread safe, can invoke on other thread but async
	void invokeSubscribe(const QString &topic, quint8 qos, MqttOnMessageCallback cb = nullptr)
	{
		QMetaObject::invokeMethod(
			this, "onSubscribe",
			Qt::QueuedConnection,
			Q_ARG(const QString &, topic),
			Q_ARG(quint8, qos),
			Q_ARG(MqttOnMessageCallback, cb)
		);
	}

	// thread safe, can invoke on other thread but async
	void invokePublish(const QString &topic, const QByteArray &content, quint8 qos = 0)
	{
		QMetaObject::invokeMethod(
			this, "onPublish",
			Qt::QueuedConnection,
			Q_ARG(const QString &, topic),
			Q_ARG(const QByteArray &, content),
			Q_ARG(quint8, qos)
		);
	}
private slots:
	void onInit();
public slots:
	void onPublish(const QString &topic, const QByteArray &content, quint8 qos);
	void onSubscribe(const QString &topic, quint8 qos, MqttOnMessageCallback cb);
signals:

private:
	QScopedPointer<MqttClientPrivate> d;
};

#endif // MQTTCLIENT_H
