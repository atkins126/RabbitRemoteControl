#include "ServiceLibVNCServer.h"
#include "RabbitCommonLog.h"
#include "RabbitCommonTools.h"
#include <rfb/keysym.h>
#include <QScreen>
#include <QApplication>
#include <QDesktopWidget>

CServiceLibVNCServer::CServiceLibVNCServer(QObject *parent) : CService(parent)
{
}

bool CServiceLibVNCServer::Enable()
{
    return false;
}

static rfbBool checkPassword(struct _rfbClientRec* cl,const char* encryptedPassWord,int len)
{
    LOG_MODEL_DEBUG("Service LibVNCServer", "Password:%s", encryptedPassWord);
    return false;
}

static void dokey(rfbBool down,rfbKeySym key,rfbClientPtr cl)
{
    LOG_MODEL_DEBUG("Service LibVNCServer", "key: %d;down:%d", key, down);
}

static void doptr(int buttonMask,int x,int y,rfbClientPtr cl)
{
    LOG_MODEL_DEBUG("Service LibVNCServer", "Mouse: button:%d;x:%d;y:%d",
                    buttonMask, x, y);
}

static enum rfbNewClientAction newclient(rfbClientPtr cl)
{
    LOG_MODEL_DEBUG("Service LibVNCServer", "New client:%s", cl->host);
//  cl->clientData = (void*)calloc(sizeof(ClientData),1);
//  cl->clientGoneHook = clientgone;
  return RFB_CLIENT_ACCEPT;
}

int CServiceLibVNCServer::OnInit()
{
    //LOG_MODEL_DEBUG("CServiceLibVNCServer", "CServiceLibVNCServer Init ......");
    QScreen* pScreen = QApplication::primaryScreen();

    int w = 640;
    int h = 480;
    int bpp = 4;
    
    if(pScreen)
    {
        w = pScreen->geometry().width();
        h = pScreen->geometry().height();
    }
    m_rfbScreen = rfbGetScreen(0, nullptr, w, h, 8, 3, bpp);
    if(!m_rfbScreen)
        return -1;
    QString name = RabbitCommon::CTools::GetHostName()
            + "@" + RabbitCommon::CTools::GetCurrentUser();
    m_rfbScreen->desktopName = strdup(name.toStdString().c_str());
    m_rfbScreen->frameBuffer = (char*)malloc(w * h * bpp);
    m_rfbScreen->alwaysShared = TRUE;
    m_rfbScreen->ptrAddEvent = doptr;
    m_rfbScreen->kbdAddEvent = dokey;
    m_rfbScreen->newClientHook = newclient;
    m_rfbScreen->passwordCheck = checkPassword;
//    m_rfbScreen->httpDir = "../webclients";
//    m_rfbScreen->httpEnableProxyConnect = TRUE;
        
    /* initialize the server */
    rfbInitServer(m_rfbScreen);
    return 0;
}

int CServiceLibVNCServer::OnClean()
{
    LOG_MODEL_DEBUG("CServiceLibVNCServer", "CServiceLibVNCServer Clean ......");
    free(m_rfbScreen->frameBuffer);
    rfbScreenCleanup(m_rfbScreen);
    return 0;
}

int CServiceLibVNCServer::OnProcess()
{
    //LOG_MODEL_DEBUG("CServiceLibVNCServer", "Process...");
    int i;
    if(rfbIsActive(m_rfbScreen))
        rfbProcessEvents(m_rfbScreen, 100000);
    return 0;
}
