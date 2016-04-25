#ifndef FILEIO_H
#define FILEIO_H

#include <QObject>

class FileIO : public QObject
{
    Q_OBJECT

public:
    Q_PROPERTY(QString source
               READ source
               WRITE setSource
               NOTIFY sourceChanged)

    Q_PROPERTY(QString identity
               READ identity
               WRITE setIdentity
               NOTIFY identityChanged)

    explicit FileIO(QObject *parent = 0);

    Q_INVOKABLE QString read();
    Q_INVOKABLE bool write(const QString& data);
    Q_INVOKABLE QString readAllStatFiles();
    Q_INVOKABLE QString tryToKillProcess(QString pid);

    QString source() { return mSource; };
    QString identity() { return mName; };

public slots:
    void setSource(const QString& source) { mSource = source; };
    void setIdentity(const QString& identity) { mName = identity; };

signals:
    void sourceChanged(const QString& source);
    void identityChanged(const QString& identity);
    void error(const QString& msg);

private:
    QString mSource;
    QString mName;
};

#endif // FILEIO_H
