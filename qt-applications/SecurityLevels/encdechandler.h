#ifndef ENCDECHANDLER_H
#define ENCDECHANDLER_H

#include <QObject>
#include <QtCore>

class EncDecHandler : public QObject
{
    Q_OBJECT
public:
    explicit EncDecHandler(QObject *parent = 0);

    Q_INVOKABLE bool initiateFileEncryption(QString filepath, QString password);
    Q_INVOKABLE bool initiateFileDecryption(QString filepath, QString password);

    Q_INVOKABLE bool initiateFilesystemEncryption(QString filesystempath, QString password, bool encryptFileNamesToo, bool recognizeMe);
    Q_INVOKABLE bool initiateFilesystemDecryption(QString filesystempath, QString password);
    bool unmountFS(QString filesystemPath);
    bool mountFS(QString filesystempath, QString password, QString sig, bool encryptFileNamesToo, QString fnekSig);

    Q_INVOKABLE QString getLocalFilePath(QString fileurl);
    bool saveFilePathPasswordPair(QString filepath, QString password);
    bool checkFilePathPasswordPair(QString filepath, QString password);

    bool saveFilesystemPathPasswordEtc(QString filesystempath, QString password, bool recognizeMe, bool encryptFileNamesToo);
    bool checkFilesystemPathPasswordPair(QString filesystempath, QString password, QString& sig, bool& recognizeMe, bool& encryptFileNamesToo, QString& fnekSig);
    QString getSignatureFromPassword(QString password, bool encryptFileNamesToo, QString& fnekSig);

    Q_INVOKABLE QStringList getListOfEncryptedFilesystemNames();


signals:

public slots:
};

#endif // ENCDECHANDLER_H
