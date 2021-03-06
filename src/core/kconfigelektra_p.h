//
// Created by felix on 28.10.19.
//
#include "kconfigbackend_p.h"

#ifdef FEAT_ELEKTRA
#ifndef KCONFIG_KCONFIGELEKTRA_P_H
#define KCONFIG_KCONFIGELEKTRA_P_H

#include <kdb.hpp>


class KConfigElektra : public KConfigBackend
{

private:
    std::string app_name;
    uint major_version;
    std::string profile;

    std::string read_key();
    std::string write_key();

    kdb::KDB * kdb;
    kdb::KeySet * ks;

    static kdb::KDB open_kdb();

public:
    KConfigElektra(std::string appName, uint majorVersion, std::string profile);
    KConfigElektra(std::string appName, uint majorVersion);

    ParseInfo parseConfig(const QByteArray &locale,
                          KEntryMap &entryMap,
                          ParseOptions options) override;

    ~KConfigElektra() override;

    bool writeConfig(const QByteArray &locale, KEntryMap &entryMap, WriteOptions options) override;

    bool isWritable() const override;

    QString nonWritableErrorMessage() const override;

    KConfigBase::AccessMode accessMode() const override;

    void createEnclosing() override;

    void setFilePath(const QString &path) override;

    bool lock() override;

    void unlock() override;

    bool isLocked() const override;

    QString uniqueGlobalIdentifier() override;

};


#endif //KCONFIG_KCONFIGELEKTRA_P_H

#endif //FEAT_ELEKTRA