// Author: Kang Lin <kl222@126.com>

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QTabWidget>
#include <QActionGroup>

#include "ViewTable.h"
#include "ManageConnecter.h"
#include "RabbitRecentMenu.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class CFrmFullScreenToolBar;

class MainWindow : public QMainWindow, CManageConnecter::Handle
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow() override;

Q_SIGNALS:
    void sigFullScreen();
    void sigShowNormal();
    
private Q_SLOTS:
    void on_actionExit_E_triggered();
    void on_actionFull_screen_F_triggered();
    
    void on_actionAbout_A_triggered();
    void on_actionUpdate_U_triggered();
    void on_actionStatusBar_S_triggered();
    void on_actionToolBar_T_triggered();
    
    void on_actionOpenStyle_O_triggered();
    void on_actionDefaultStyle_D_triggered();

    void on_actionSend_ctl_alt_del_triggered();
    
    void on_actionOriginal_O_toggled(bool arg1);
    void on_actionZoom_Z_toggled(bool arg1);
    void on_actionKeep_AspectRation_K_toggled(bool arg1);
    void slotAdaptWindows(const CFrmViewer::ADAPT_WINDOWS aw);
    
    void on_actionDisconnect_D_triggered();
    void slotCloseView(const QWidget* pView);
    
    void slotRecentFileTriggered(const QString& szFile);
    void on_actionOpen_O_triggered();
    void slotConnect();

    void slotConnected();
    void slotDisconnected();
    void slotError(const int nError, const QString &szInfo);
    void slotInformation(const QString& szInfo);
    void slotUpdateServerName(const QString& szName);
    
    void on_actionShow_TabBar_B_triggered();
    void slotShowTabBar(bool bShow);

protected:
    virtual void keyReleaseEvent(QKeyEvent *event) override;
    virtual void closeEvent(QCloseEvent *event) override;
    int Connect(CConnecter* p, bool set);
    
private:
    Ui::MainWindow *ui;
    QActionGroup* m_pGBView;
    RabbitCommon::CRecentMenu* m_pRecentMenu;
    CView* m_pView;
    QVector<CConnecter*> m_Connecters;
    CFrmFullScreenToolBar* m_pFullScreenToolBar;
    bool m_bFullScreen;

public:
    virtual int onProcess(const QString &id, CPluginViewer *pFactory) override;
private:
    CManageConnecter m_ManageConnecter;
};

#endif // MAINWINDOW_H
