// Author: Kang Lin <kl222@126.com>

#ifndef CPluginFreeRdp_H_KL_2021_07_23
#define CPluginFreeRdp_H_KL_2021_07_23

#pragma once

#include "PluginViewer.h"

class CPluginFreeRdp : public CPluginViewer
{
    Q_OBJECT
    Q_INTERFACES(CPluginViewer)

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    Q_PLUGIN_METADATA(IID CPluginViewer_iid)
#endif
    
public:
    explicit CPluginFreeRdp(QObject *parent = nullptr);
    virtual ~CPluginFreeRdp() override;
    
    virtual const QString Name() const override;
    virtual const QString Description() const override;
    virtual const QString Protol() const override;
    virtual const QIcon Icon() const override;
    
    virtual CConnecter* CreateConnecter(const QString& szProtol) override;
    
private:
    QTranslator m_Translator;
};

#endif // CPluginFreeRdp_H_KL_2021_07_23
