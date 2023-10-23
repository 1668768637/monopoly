#ifndef JSONTOOL_H
#define JSONTOOL_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QString>

class JsonTool{
public:
    static QJsonObject stringToJson(const QString &jsonString);
    static QString jsonToString(const QJsonObject* jsonObject);
};

#endif // JSONTOOL_H
