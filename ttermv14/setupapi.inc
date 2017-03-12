GLOBAL hSetup AS INTEGER
GLOBAL fFrameInit AS INTEGER
GLOBAL fInstallInit AS INTEGER

CONST MB_OK          = 0
CONST MB_ICONHAND    = 16
CONST cmoVital       = 1
CONST cmoOverwrite   = 1024
CONST cmoNone        = 0
CONST grcUserQuit   = 48
CONST cbSymValMax = 512
CONST STFERR      = 1024
CONST STFQUIT     = 1025

DECLARE FUNCTION DoMsgBox LIB "mscomstf.dll" (lpText$,lpCaption$,wType%) As INTEGER
DECLARE FUNCTION GetProfileString LIB "kernel" (szSect$, szKey$, szDefault$, szBuf$, cbBuf%) AS INTEGER
DECLARE FUNCTION FOpenInf LIB "mscomstf.dll" (szFile$, fCheck%, fCheckSyms%) AS INTEGER
DECLARE FUNCTION FSetBitmap LIB "msshlstf.dll" (szDll$, Bitmap%) AS INTEGER
DECLARE FUNCTION FDoDialog LIB "msuilstf.dll" (hwnd%, szDll$, Dlg%, szDlgProc$, HelpDlg%, szHelpProc$) AS INTEGER
DECLARE FUNCTION FKillNDialogs LIB "msuilstf.dll" (n%) AS INTEGER
DECLARE SUB SetWindowText LIB "User" (hwnd%, lpString$)
DECLARE FUNCTION FSetSymbolValue LIB "msshlstf.dll" (szSymbol$, szValue$) AS INTEGER
DECLARE FUNCTION CbGetSymbolValue LIB "mscomstf.dll" (szSymbol$, szValue$, Length%) AS INTEGER
DECLARE FUNCTION UsGetListLength LIB "mscomstf.dll" (szSymbol$) AS INTEGER
DECLARE FUNCTION CbGetListItem LIB "mscomstf.dll" (szListSymbol$, n%, szListItem$, cbMax%) AS INTEGER
DECLARE FUNCTION FAddListItem LIB "mscomstf.dll" (szListSymbol$, szListItem$) AS INTEGER
DECLARE FUNCTION InitializeFrame LIB "msshlstf.dll" (szCmdLine$) AS INTEGER
DECLARE FUNCTION HwndFrame LIB "msshlstf.dll" AS INTEGER
DECLARE FUNCTION HinstFrame LIB "msshlstf.dll" AS INTEGER
DECLARE FUNCTION FCreateDir LIB "msinsstf.dll" (szDir$, cmo%) AS INTEGER
DECLARE FUNCTION FAddSectionFilesToCopyList LIB "mscomstf.dll" (szSect$, szSrc$, szDest$) AS INTEGER
DECLARE FUNCTION GrcCopyFilesInCopyList LIB "msinsstf.dll" (hInstance%) AS INTEGER
DECLARE FUNCTION FCreateIniKeyValue LIB "msinsstf.dll" (szFile$, szSect$, szKey$, szValue$, cmo%) AS INTEGER
DECLARE FUNCTION FInitializeInstall LIB "msinsstf.dll" (hinst%, hwndFrame%) AS INTEGER
DECLARE FUNCTION FIsDirWritable LIB "msinsstf.dll" (szDir$) AS INTEGER
DECLARE SUB SetBitmap(szDll$, Bitmap%)
DECLARE FUNCTION UIStartDlg(szDll$, Dlg%, szDlgProc$, HelpDlg%, szHelpProc$) AS STRING
DECLARE SUB UIPop (n%)
DECLARE SUB UIPopAll
DECLARE SUB SetTitle (sz$)
DECLARE SUB ReadInfFile (szFile$)
DECLARE SUB SetSymbolValue (szSymbol$, szValue$)
DECLARE FUNCTION GetSymbolValue (szSymbol$) AS STRING
DECLARE FUNCTION GetListLength(szSymbol$) AS INTEGER
DECLARE FUNCTION GetListItem(szSymbol$, n%) AS STRING
DECLARE SUB AddListItem(szSymbol$, szItem$)
DECLARE FUNCTION InitFrame (szCmdLine$) AS INTEGER
DECLARE FUNCTION InitSetup (szCmdLine$) AS INTEGER
DECLARE SUB CreateDir (szDir$, cmo%)
DECLARE SUB AddSectionFilesToCopyList (szSect$, szSrc$, szDest$)
DECLARE SUB CopyFilesInCopyList
DECLARE SUB CreateIniKeyValue (szFile$, szSect$, szKey$, szValue$, cmo%)
DECLARE SUB InitInstall
DECLARE FUNCTION IsDirWritable (szDir$) AS INTEGER

FUNCTION InitSetup(szCmdLine$) STATIC AS INTEGER
    fFrameInit = 0
    fInstallInit = 0

    i% = InitFrame(szCmdLine$)
    fFrameInit = 1

    InitInstall
    fInstallInit = 1

    InitSetup = i%
END FUNCTION

FUNCTION InitFrame(szCmdLine$) STATIC AS INTEGER
    IF hSetup > 0 THEN
    ELSE
        i% = InitializeFrame(szCmdLine$)
        IF i% = -1 THEN
            END
        ELSEIF i% = 0 THEN
            END
        ELSE
            InitFrame = i%
        END IF
    END IF
END FUNCTION

SUB SetBitmap(szDll$, Bitmap%) STATIC
    IF FSetBitmap(szDll$, Bitmap%) = 0 THEN
        ERROR STFERR
    END IF
END SUB

SUB SetTitle(sz$) STATIC
    SetWindowText HwndFrame(), sz$
END SUB

SUB ReadInfFile (szFile$) STATIC
    IF FOpenInf(szFile$, 1, 0) = 0 THEN
        ERROR STFERR
    END IF
END SUB

FUNCTION UIStartDlg(szDll$, Dlg%, szDlgProc$, HelpDlg%, szHelpProc$) STATIC AS STRING
    IF FDoDialog(HwndFrame(), szDll$, Dlg%, szDlgProc$, HelpDlg%, szHelpProc$) = 0 THEN
        ERROR STFERR
    ELSE
        UIStartDlg = GetSymbolValue("DLGEVENT")
    END IF
END FUNCTION

SUB UIPop (n%) STATIC
    IF FKillNDialogs(n%) = 0 THEN
        ERROR STFERR
    END IF
END SUB

SUB UIPopAll STATIC
    IF FKillNDialogs(65535) = 0 THEN
        ERROR STFERR
    END IF
END SUB

FUNCTION GetSymbolValue(szSymbol$) STATIC AS STRING
    szValue$ = string$(cbSymValMax,32)
    Length% = CbGetSymbolValue(szSymbol$, szValue$, cbSymValMax)
    GetSymbolValue = szValue$
    IF Length% >= cbSymValMax THEN
        res% = DoMsgBox("Buffer Overflow", "MS-Setup Error", MB_ICONHAND+MB_OK)
        ERROR STFERR
    END IF
    IF szSymbol$ = "" THEN
        ERROR STFERR
    END IF
    szValue$ = ""
END FUNCTION

FUNCTION GetListLength(szSymbol$) STATIC AS INTEGER
    GetListLength = UsGetListLength(szSymbol$)
    IF szSymbol$ = "" THEN
        ERROR STFERR
    END IF
END FUNCTION

FUNCTION GetListItem(szListSymbol$, nItem%) STATIC AS STRING
    szListItem$ = string$(cbSymValMax,32)
    Length% = CbGetListItem(szListSymbol$, nItem%, szListItem$, cbSymValMax)
    GetListItem = szListItem$
    IF Length% >= cbSymValMax THEN
        res% = DoMsgBox("Buffer Overflow", "MS-Setup Error", MB_ICONHAND+MB_OK)
        ERROR STFERR
    END IF
    IF szListSymbol$ = "" THEN
        ERROR STFERR
    END IF
    if nItem% <= 0 or nItem% > GetListLength(szListSymbol$) then
        ERROR STFERR
    end if
    szListItem$ = ""
END FUNCTION

SUB AddListItem(szSymbol$, szItem$) STATIC
    IF FAddListItem(szSymbol$, szItem$) = 0 THEN
        ERROR STFERR
    END IF
END SUB

SUB SetSymbolValue(szSymbol$, szValue$) STATIC
    IF FSetSymbolValue(szSymbol$, szValue$) = 0 THEN
        ERROR STFERR
    END IF
END SUB

SUB InitInstall STATIC
    IF hSetup > 0 THEN
    ELSEIF FInitializeInstall(HinstFrame(), HwndFrame()) = 0 THEN
        END
    END IF
END SUB

SUB CreateDir (szDir$, cmo%) STATIC
    IF FCreateDir(szDir$, cmo%) = 0 THEN
        ERROR STFERR
    END IF
END SUB

SUB CreateIniKeyValue (szFile$, szSect$, szKey$, szValue$, cmo%) STATIC
    IF FCreateIniKeyValue(szFile$, szSect$, szKey$, szValue$, cmo%) = 0 THEN
        ERROR STFERR
    END IF
END SUB

SUB CopyFilesInCopyList STATIC
    grc% = GrcCopyFilesInCopyList (HinstFrame())

    IF grc% = grcUserQuit THEN
        ERROR STFQUIT
    ELSEIF grc% > 0 THEN
        ERROR STFERR
    END IF
END SUB

SUB AddSectionFilesToCopyList (szSect$, szSrc$, szDest$) STATIC
    IF FAddSectionFilesToCopyList (szSect$, szSrc$, szDest$) = 0 THEN
        ERROR STFERR
    END IF
END SUB

FUNCTION IsDirWritable (szDir$) STATIC AS INTEGER
    IsDirWritable = FIsDirWritable(szDir$)
END FUNCTION

TRAP CleanupTrap From "MSSHLSTF.DLL"
End Trap

hSetup = InitSetup(COMMAND$)

ON ERROR GOTO QUIT

