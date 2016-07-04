#ifndef SFOCONTEXT_H
#define SFOCONTEXT_H

#include <FJCaller.h>

#include "SFOPartner.h"

#include <FJTypes.h>

#include <QDate>
#include <QJsonDocument>
#include <QList>
#include <QMap>
#include <QString>

FJ_DECLARE_PTRS(FJClient)
FJ_DECLARE_PTRS(FJOperation)

typedef QMap<QString, QString> QStringMap;

class SFOContext : public FJCaller
{
Q_OBJECT

public:
    static SFOContext* GetInstance();

    virtual ~SFOContext();

    SFOPartnerList GetPartners() const;

    void Refresh();

    QStringMap GetEnToJpDict() const;
    QStringMap GetJpToEnDict() const;

    void AddWordTranslation(const QString& word,
                            const QStringList& translations);

    void LoadFromDisk();
    void FlushToDisk();

signals:
    void PartnersUpdated();
    void DictionariesUpdated();

public slots:
    virtual void HandleResponse(const QJsonDocument& document, FJError error,
                                const FJOperation* operation);

protected:
    explicit SFOContext(QObject *parent = 0);

    void _WriteCacheFile(const QJsonDocument& doc, const QString& filename);
    QJsonDocument _LoadCacheFile(const QString& filename);

    QVariantMap _DictToVariantMap(const QStringMap& dict) const;
    QVariantMap _GetMapFromJson(const QJsonDocument& doc) const;

    QString _GetFilePath(const QString& filename) const;

    bool _IsJapanese(const QString& word) const;

    void _HandleStartResponse(const QJsonDocument& data);
    void _HandlePartnersResponse(const QJsonDocument& data);
    void _HandleDictResponse(const QJsonDocument& data);
    void _HandleSubmitResponse(const QJsonDocument& data);

    QJsonDocument _CreateJsonContent(const QDateTime asOfDate) const;
    QJsonDocument _CreateJsonContent(const QVariantMap& dict) const;

    void _UpdatePartnersIfNecessary();
    void _UpdateDictionaryIfNecessary();

protected:
    static const QString DateTimeStampFileName;
    static const QString PartnerCacheFileName;
    static const QString DictionaryCacheFileName;

    static const QString LastPartnerDateKey;
    static const QString LastDictDateKey;

private:
    static SFOContext* _instance;

    FJClientSharedPtr _client;

    QDateTime _lastPartnerDate;
    SFOPartnerList _partners;
    bool _partnersDirty;
    QDateTime _lastDictDate;
    QStringMap _enToJpDict;
    QStringMap _jpToEnDict;
    bool _dictDirty;
};

#endif // SFOCONTEXT_H
