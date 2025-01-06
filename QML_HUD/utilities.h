#ifndef UTILITIES_H
#define UTILITIES_H

#include <QQmlPropertyMap>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>


class ThemeProvider : public QObject {
    Q_OBJECT
    
    Q_PROPERTY(QQmlPropertyMap* themeMap READ themeMap CONSTANT)

public:
    explicit ThemeProvider(QObject* parent = nullptr)
        : QObject(parent), m_themeMap(new QQmlPropertyMap(this)) {}

    QQmlPropertyMap* themeMap() const { return m_themeMap; }

    Q_INVOKABLE bool loadFromJson(const QString& filePath)
    {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly)) {
            qWarning() << "Failed to open theme file:" << filePath;
            return false;
        }

        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        file.close();

        if (!doc.isObject())
        {
            qWarning() << "Invalid JSON structure in file:" << filePath;
            return false;
        }

        QJsonObject obj = doc.object();
        for (auto it = obj.begin(); it != obj.end(); ++it)
        {
            m_themeMap->insert(it.key(), it.value().toVariant());
        }

        emit themeChanged();
        return true;
    }

signals:
    void themeChanged();

private:
    QQmlPropertyMap* m_themeMap;
};

// Concept: class to receive frontend callings and provide background data
class Backend : public QObject
{
    Q_OBJECT

public:
    explicit Backend(QObject* parent = nullptr)
        : QObject(parent) {}

    Q_INVOKABLE void printSth()
    {
        qWarning() << "Screen mirror toggle";
    }
};

#endif // UTILITIES_H