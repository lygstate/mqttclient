#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "mqttclient.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
	Q_OBJECT

public:
	Widget(QWidget *parent = nullptr);
	~Widget();

private slots:
	void on_startSubs_clicked();
	void on_startPublish_clicked();

public slots:
	void updateTestContent(QString content);

private:
	Ui::Widget *ui;
	MqttClient *client;
};
#endif // WIDGET_H
