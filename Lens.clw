; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CStatDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "lens.h"
LastPage=0

ClassCount=8
Class1=CChildFrame
Class2=CGLEnabledView
Class3=CLensApp
Class4=CAboutDlg
Class5=CLensDoc
Class6=CLensView
Class7=CMainFrame

ResourceCount=5
Resource1=IDR_MAINFRAME
Resource2=IDR_LENSTYPE
Resource3=IDD_ABOUTBOX
Resource4=IDR_LENSTYPE_CNTR_IP
Class8=CStatDlg
Resource5=IDD_STATDLG

[CLS:CChildFrame]
Type=0
BaseClass=CMDIChildWnd
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp
Filter=M
VirtualFilter=mfWC
LastObject=CChildFrame

[CLS:CGLEnabledView]
Type=0
BaseClass=CView
HeaderFile=GLEnabledView.h
ImplementationFile=GLEnabledView.cpp
Filter=C
VirtualFilter=VWC
LastObject=CGLEnabledView

[CLS:CLensApp]
Type=0
BaseClass=CWinApp
HeaderFile=Lens.h
ImplementationFile=Lens.cpp
Filter=N
VirtualFilter=AC

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=Lens.cpp
ImplementationFile=Lens.cpp
LastObject=CAboutDlg

[CLS:CLensDoc]
Type=0
BaseClass=COleDocument
HeaderFile=LensDoc.h
ImplementationFile=LensDoc.cpp
Filter=N
VirtualFilter=ODC
LastObject=ID_INITIALVALUE

[CLS:CLensView]
Type=0
BaseClass=CGLEnabledView
HeaderFile=LensView.h
ImplementationFile=LensView.cpp
Filter=C
VirtualFilter=VWC
LastObject=CLensView

[CLS:CMainFrame]
Type=0
BaseClass=CMDIFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
VirtualFilter=fWC
LastObject=CMainFrame

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
Command9=ID_TONICINPUT
Command10=ID_INITIALVALUE
Command11=ID_SETWALL
Command12=ID_RESTARTSIM
Command13=ID_SIMRESUME
Command14=ID_SIMPAUSE
Command15=ID_WRITEAVI
CommandCount=15

[MNU:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_RESTARTSIM
Command7=ID_SIMPAUSE
Command8=ID_SIMRESUME
Command9=ID_WRITEAVI
Command10=ID_VIEW_TOOLBAR
Command11=ID_VIEW_STATUS_BAR
Command12=ID_APP_ABOUT
CommandCount=12

[MNU:IDR_LENSTYPE]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_PRINT
Command7=ID_FILE_PRINT_PREVIEW
Command8=ID_FILE_PRINT_SETUP
Command9=ID_FILE_MRU_FILE1
Command10=ID_APP_EXIT
Command11=ID_EDIT_UNDO
Command12=ID_EDIT_CUT
Command13=ID_EDIT_COPY
Command14=ID_EDIT_PASTE
Command15=ID_EDIT_PASTE_SPECIAL
Command16=ID_OLE_INSERT_NEW
Command17=ID_OLE_EDIT_LINKS
Command18=ID_OLE_VERB_FIRST
Command19=ID_VIEW_TOOLBAR
Command20=ID_VIEW_STATUS_BAR
Command21=ID_WINDOW_NEW
Command22=ID_WINDOW_CASCADE
Command23=ID_WINDOW_TILE_HORZ
Command24=ID_WINDOW_ARRANGE
Command25=ID_APP_ABOUT
CommandCount=25

[MNU:IDR_LENSTYPE_CNTR_IP]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_PRINT
Command7=ID_FILE_PRINT_PREVIEW
Command8=ID_FILE_PRINT_SETUP
Command9=ID_FILE_MRU_FILE1
Command10=ID_APP_EXIT
Command11=ID_WINDOW_NEW
Command12=ID_WINDOW_CASCADE
Command13=ID_WINDOW_TILE_HORZ
Command14=ID_WINDOW_ARRANGE
CommandCount=14

[ACL:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_EDIT_COPY
Command2=ID_FILE_NEW
Command3=ID_FILE_OPEN
Command4=ID_FILE_PRINT
Command5=ID_RESTARTSIM
Command6=ID_FILE_SAVE
Command7=ID_EDIT_PASTE
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_CUT
Command10=ID_CANCEL_EDIT_CNTR
Command11=ID_WRITEAVI
Command12=ID_SIMPAUSE
Command13=ID_SIMRESUME
Command14=ID_NEXT_PANE
Command15=ID_PREV_PANE
Command16=ID_EDIT_COPY
Command17=ID_EDIT_PASTE
Command18=ID_EDIT_CUT
Command19=ID_EDIT_UNDO
CommandCount=19

[ACL:IDR_LENSTYPE_CNTR_IP]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_NEXT_PANE
Command6=ID_PREV_PANE
Command7=ID_CANCEL_EDIT_CNTR
CommandCount=7

[DLG:IDD_STATDLG]
Type=1
Class=CStatDlg
ControlCount=15
Control1=IDC_STATUS,static,1342177294
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDITVALUE,edit,1350631552
Control5=IDC_EDITSIZE,edit,1350631552
Control6=IDC_RADIO1,button,1342181385
Control7=IDC_RADIO2,button,1342181385
Control8=IDC_RADIO3,button,1342181385
Control9=IDC_EDITX,edit,1350631552
Control10=IDC_EDITY,edit,1350631552
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_RESTART,button,1342242816
Control14=IDC_STATICX,static,1342308352
Control15=IDC_STATICY,static,1342308352

[CLS:CStatDlg]
Type=0
HeaderFile=StatDlg.h
ImplementationFile=StatDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CStatDlg
VirtualFilter=dWC

