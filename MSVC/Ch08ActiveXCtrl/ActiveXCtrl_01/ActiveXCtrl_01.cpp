// ActiveXCtrl_01.cpp : Defines the class behaviors for the application.
//

#include "StdAfx.h"
#include "ActiveXCtrl_01.h"
#include "ActiveXCtrlDlg.h"
#include <initguid.h>
#include "ActiveXCtrl_01_i.c"
#include "Calendar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CActiveXCtrlApp


class CActiveXCtrl_01Module :
    public CAtlMfcModule {
public:
    DECLARE_LIBID(LIBID_ActiveXCtrl_01Lib);
    DECLARE_REGISTRY_APPID_RESOURCEID(IDR_ACTIVEXCTRL_01, "{B8050A59-BC31-46ED-A65D-B5405B17E193}");
};

CActiveXCtrl_01Module _AtlModule;

BEGIN_MESSAGE_MAP(CActiveXCtrlApp, CWinApp)
    ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CActiveXCtrlApp construction

CActiveXCtrlApp::CActiveXCtrlApp()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}


// The one and only CActiveXCtrlApp object

CActiveXCtrlApp theApp;


// CActiveXCtrlApp initialization

BOOL CActiveXCtrlApp::InitInstance()
{
    AfxOleInit();

    // InitCommonControlsEx() is required on Windows XP if an application
    // manifest specifies use of ComCtl32.dll version 6 or later to enable
    // visual styles.  Otherwise, any window creation will fail.
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    // Set this to include all the common control classes you want to use
    // in your application.
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();

    // 当建立工程时，选择支持 Active Control 时，就会增加下面的 AfxEnableControlContainer()
    // 以提供 ActiveX 控件的包容器类 (Container) 支持，同时也会增加 #include <afxdisp.h>
    AfxEnableControlContainer();

    // Parse command line for standard shell commands, DDE, file open
    CCommandLineInfo cmdInfo;
    ParseCommandLine(cmdInfo);
#if !defined(_WIN32_WCE) || defined(_CE_DCOM)
    // Register class factories via CoRegisterClassObject().
    if (FAILED(_AtlModule.RegisterClassObjects(CLSCTX_LOCAL_SERVER, REGCLS_MULTIPLEUSE)))
        return FALSE;
#endif // !defined(_WIN32_WCE) || defined(_CE_DCOM)
    // App was launched with /Embedding or /Automation switch.
    // Run app as automation server.
    if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated) {
        // Don't show the main window
        return TRUE;
    }
    // App was launched with /Unregserver or /Unregister switch.
    if (cmdInfo.m_nShellCommand == CCommandLineInfo::AppUnregister) {
        _AtlModule.UpdateRegistryAppId(FALSE);
        _AtlModule.UnregisterServer(TRUE);
        return FALSE;
    }
    // App was launched with /Register or /Regserver switch.
    if (cmdInfo.m_nShellCommand == CCommandLineInfo::AppRegister) {
        _AtlModule.UpdateRegistryAppId(TRUE);
        _AtlModule.RegisterServer(TRUE);
        return FALSE;
    }

    // Standard initialization
    // If you are not using these features and wish to reduce the size
    // of your final executable, you should remove from the following
    // the specific initialization routines you do not need
    // Change the registry key under which our settings are stored
    // TODO: You should modify this string to be something appropriate
    // such as the name of your company or organization
    SetRegistryKey(_T("Local AppWizard-Generated Applications"));

    CActiveXCtrlDlg dlg;
    m_pMainWnd = &dlg;
    INT_PTR nResponse = dlg.DoModal();
    if (nResponse == IDOK) {
        // TODO: Place code here to handle when the dialog is
        //  dismissed with OK
    }
    else if (nResponse == IDCANCEL) {
        // TODO: Place code here to handle when the dialog is
        //  dismissed with Cancel
    }

    // Since the dialog has been closed, return FALSE so that we exit the
    //  application, rather than start the application's message pump.
    return FALSE;
}

BOOL CActiveXCtrlApp::ExitInstance(void)
{
#if !defined(_WIN32_WCE) || defined(_CE_DCOM)
    _AtlModule.RevokeClassObjects();
#endif
    return CWinApp::ExitInstance();
}
