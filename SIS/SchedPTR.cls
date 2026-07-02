VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
END
Attribute VB_Name = "cSchedPTR"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Attribute VB_Ext_KEY = "SavedWithClassBuilder" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
Option Explicit

Private Type SchedPTR
    Formula_ID As Variant
    TimeLoop As Variant
    Left2Run As Variant
    Line_ID As Variant
    Descr As Variant
End Type

Private Datum As SchedPTR
Private Const SCLSNAME As String = "cSchedPTR"
Private Const LENTHLEN As Integer = 3
Private sLogFile As String

Public Property Get Formula_ID() As Variant
    Formula_ID = Datum.Formula_ID
End Property

Public Property Let Formula_ID(ByVal vNewValue As Variant)
    Datum.Formula_ID = vNewValue
End Property

Public Property Get Left2Run() As Variant
    Left2Run = Datum.Left2Run
End Property

Public Property Let Left2Run(ByVal vNewValue As Variant)
    Datum.Left2Run = vNewValue
End Property

Public Property Get Line_ID() As Variant
    Line_ID = Datum.Line_ID
End Property

Public Property Let Line_ID(ByVal vNewValue As Variant)
    Datum.Line_ID = vNewValue
End Property

Public Property Get TimeLoop() As Variant
    TimeLoop = Datum.TimeLoop
End Property

Public Property Let TimeLoop(ByVal vNewValue As Variant)
    Datum.TimeLoop = vNewValue
    Datum.Left2Run = vNewValue
End Property

Public Property Get PoolString() As String
    Dim sTmp As String
    
    PoolString = SCLSNAME
        
    sTmp = AlwayString(Datum.Formula_ID)
    PoolString = PoolString + Format(Len(sTmp), String(LENTHLEN, "0"))
    PoolString = PoolString + sTmp
    
    sTmp = AlwayString(Datum.Line_ID)
    PoolString = PoolString + Format(Len(sTmp), String(LENTHLEN, "0"))
    PoolString = PoolString + sTmp
End Property

Public Property Let PoolString(ByVal vNewValue As String)
    Dim iLen As Integer, iComLen As Integer

    If Left(vNewValue, Len(SCLSNAME)) = SCLSNAME Then
        iComLen = 1 + Len(SCLSNAME)
        
        iLen = CInt(Mid(vNewValue, iComLen, LENTHLEN))
        iComLen = iComLen + LENTHLEN
        Datum.Formula_ID = CLng(Mid(vNewValue, iComLen, iLen))
        iComLen = iLen + iComLen
        
        iLen = CInt(Mid(vNewValue, iComLen, LENTHLEN))
        iComLen = iComLen + LENTHLEN
        Datum.Line_ID = CLng(Mid(vNewValue, iComLen, iLen))
        iComLen = iLen + iComLen
    Else
        Err.Raise 0, "cFDPTR.PoolString Let", "Wrong prefix of string. Must start with 'cFDPTR'"
    End If
End Property

Private Function AlwayString(vParam As Variant) As String
    Select Case VarType(vParam)
        Case vbString
            AlwayString = vParam
        Case vbInteger
            AlwayString = Trim(Str(vParam))
        Case vbLong
            AlwayString = Trim(Str(vParam))
        Case vbSingle
            AlwayString = Trim(Str(vParam))
        Case vbDouble
            AlwayString = Trim(Str(vParam))
        Case vbDecimal
            AlwayString = Trim(Str(vParam))
        Case vbDate
            AlwayString = "vbDate not known"
        Case vbBoolean
            AlwayString = IIf(vParam, "True", "False")
        Case vbCurrency
            AlwayString = "$" + Trim(Str(vParam))
        Case Else
            AlwayString = ""
    End Select
End Function

Public Sub SetAllValues(ByVal Formula_ID As Variant, _
    ByVal TimeLoop As Variant, ByVal Left2Run As Variant, _
    ByVal Line_ID As Variant, ByVal Descr As Variant)

    Datum.Formula_ID = Formula_ID
    Datum.TimeLoop = TimeLoop
    Datum.Left2Run = Left2Run
    Datum.Line_ID = Line_ID
    Datum.Descr = Descr
End Sub

Public Property Get Descr() As Variant
    Descr = Datum.Descr
End Property

Public Property Let Descr(ByVal vNewValue As Variant)
    Datum.Descr = vNewValue
End Property

Private Sub Class_Initialize()
    If sLogFile = "" Then
        sLogFile = App.Path & "\EDPS.log"
    End If
    
    Call Str2File(sLogFile, "cSchedPTR exampler created.")
End Sub

Private Sub Class_Terminate()
    Call Str2File(sLogFile, "cSchedPTR exampler destroyed.")
End Sub

Public Property Let LogPath(ByVal vNewValue As Variant)
    sLogFile = vNewValue
End Property

Public Property Get LogPath() As Variant
    LogPath = sLogFile
End Property


