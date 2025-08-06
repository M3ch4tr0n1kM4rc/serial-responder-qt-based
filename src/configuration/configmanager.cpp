// SPDX-License-Identifier: GPL-3.0-only

#include "configmanager.h"

#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

ConfigManager::ConfigManager(DataTableModel *data_model)
    : m_responder_data_model(data_model)
{
}

bool ConfigManager::load(const QString &filePath)
{
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly)) {
		qWarning() << "ConfigManager::load – cannot open file" << filePath;
		return false;
	}

	const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
	file.close();
	if (!doc.isObject()) {
		qWarning() << "ConfigManager::load – invalid JSON structure";
		return false;
	}

	QJsonObject root = doc.object();

	if (root.contains("serial")) {
		QJsonObject js = root["serial"].toObject();
		portName = js.value("portName").toString(portName);
		baudRate = js.value("baudRate").toInt(int(baudRate));
		dataBits =
			QSerialPort::DataBits(js.value("dataBits").toInt(int(dataBits)));
		parity = QSerialPort::Parity(js.value("parity").toInt(int(parity)));
		stopBits =
			QSerialPort::StopBits(js.value("stopBits").toInt(int(stopBits)));
		flowControl = QSerialPort::FlowControl(
			js.value("flowControl").toInt(int(flowControl)));
	}

	defaultHexMode = root.value("defaultHexMode").toBool(defaultHexMode);

	m_responder_data_model->clear();
	if (root.contains("inputEntries")) {
		QJsonArray arr = root["inputEntries"].toArray();
		for (auto v : arr) {
			QJsonObject obj = v.toObject();
			QByteArray input_data =
				QByteArray::fromHex(obj.value("input").toString().toUtf8());
			bool inputIsHex = obj.value("inputIsHex").toBool();
			QByteArray output_data =
				QByteArray::fromHex(obj.value("output").toString().toUtf8());
			bool outputIsHex = obj.value("outputIsHex").toBool();
			m_responder_data_model->addEntry(input_data, inputIsHex, output_data,
								   outputIsHex);
		}
	}
	return true;
}

bool ConfigManager::save(const QString &filePath) const
{
	QJsonObject root;

	QJsonObject js;
	js["portName"] = portName;
	js["baudRate"] = int(baudRate);
	js["dataBits"] = int(dataBits);
	js["parity"] = int(parity);
	js["stopBits"] = int(stopBits);
	js["flowControl"] = int(flowControl);
	root["serial"] = js;

	root["defaultHexMode"] = defaultHexMode;

	root["inputEntries"] = m_responder_data_model->toJsonArray();

	QJsonDocument doc(root);
	QFile file(filePath);
	if (!file.open(QIODevice::WriteOnly)) {
		qWarning() << "ConfigManager::save – cannot open file" << filePath;
		return false;
	}
	file.write(doc.toJson(QJsonDocument::Indented));
	file.close();
	return true;
}
