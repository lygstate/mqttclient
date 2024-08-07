#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::Widget)
{
	ui->setupUi(this);
	client = new MqttClient("127.0.0.1", 1883);
}

Widget::~Widget()
{
	delete ui;
}

void Widget::on_startSubs_clicked()
{
	client->invokeSubscribe("test_subs", 0, [=](QMqttMessage &msg){
		QString content = QString::fromUtf8(msg.payload());
		qDebug() <<  content;
		QMetaObject::invokeMethod(this, "updateTestContent", Qt::QueuedConnection, Q_ARG(QString, content));
	});
}

void Widget::on_startPublish_clicked()
{
	client->invokePublish("test_subs", ui->publishLineEdit->text().toUtf8());
}

void Widget::updateTestContent(QString content)
{
	ui->subsShowLabel->setText(content);
}
