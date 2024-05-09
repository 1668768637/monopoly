#include <JsonTool.h>

QJsonObject JsonTool::stringToJson(const QString &jsonString)
{
    // 尝试解析输入的JSON字符串
    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8(), &parseError);

    if (parseError.error != QJsonParseError::NoError || !jsonDoc.isObject()) {
        // 如果解析失败或结果不是一个对象，返回一个带错误信息的QJsonObject
        QJsonObject error;
        error.insert("error",parseError.errorString());
    }

    // 返回解析后的QJsonObject
    return jsonDoc.object();
}

QString JsonTool::jsonToString(const QJsonObject *jsonObject)
{
    // 创建一个QJsonDocument并将QJsonObject包装其中
    QJsonDocument jsonDoc(*jsonObject);

    // 使用toJson方法将QJsonDocument转换为QString
    return QString(jsonDoc.toJson(QJsonDocument::Compact));
}
