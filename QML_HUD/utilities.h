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
        Q_PROPERTY(qreal value READ value WRITE setValue NOTIFY valueChanged)
        Q_PROPERTY(bool mirror READ mirror WRITE setMirror NOTIFY mirrorChanged)

public:
    explicit Backend(QObject* parent = nullptr)
        : QObject(parent) {}

    Q_INVOKABLE void printSth()
    {
        qWarning() << "Screen mirror toggle";
    }

    qreal value() const { return m_value; }
    bool  mirror() const { return m_mirror; }

    void setValue(qreal value)
    {
        if (m_value != value)
        {
            m_value = value;
            emit valueChanged();
        }
    }

    void setMirror(bool value)
    {
        if (m_mirror != value)
        {
            m_mirror = value;
            emit mirrorChanged();
        }
    }


signals:
    void    valueChanged();
    void    mirrorChanged();

public: 
    qreal   m_value = 0;
    bool    m_mirror = false;
    // TODO:
    // usedTheme
};

#endif // UTILITIES_H