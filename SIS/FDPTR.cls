VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "cFDPTR"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Option Explicit

Public Enum FDAction
    FDUndefigned = -1
    FDRead = 0
    FDWrite = 1
End Enum

Private Type THFDIS
    ParamID As Variant
    DataType As Variant
    LinkType As Variant
    DataSetName As String
    Addr1 As Variant
    Addr2 As Variant
    Addr3 As Variant
    Addr4 As Variant
    Addr5 As Variant
    Addr6 As Variant
    Date1 As Variant
    Date2 As Variant
    Value As Variant
    Tag As Variant
    Action As FDAction
    SomeID As Variant
End Type

Private Datum As THFDIS
Private Const SDATEFORMAT As String = "MM/dd/YYYY hh:nn:ss"
Private Const SCLSNAME As String = "cFDPTR"
Private Const LENTHLEN As Integer = 3
Private LDB As LEIS.LEISer
Private bLEISConnAllowed As Boolean
Private sLogFile As String

' ds.dll functions
Private Declare Function SetArrsize Lib "ds.dll" (ByVal ArrSize As Integer) As Integer
Private Declare Function SetParamValueAt Lib "ds.dll" (ByVal StrIdx As Integer, ByVal StrVal As String) As Integer
Private Declare Function GetParamValueAt Lib "ds.dll" (ByVal StrIdx As Integer) As String
Private Declare Function GetUpperArrayBound Lib "ds.dll" () As Integer

' convert.dll functions
Private Declare Function HStr2Float Lib "convert.dll" (ByVal s4Bites As String) As Single
Private Declare Function HStr2Long Lib "convert.dll" (ByVal s4Bites As String) As Long
Private Declare Function HStr2LLng Lib "convert.dll" (ByVal s4Bites As String) As Long
Private Declare Function HStr2Str Lib "convert.dll" (ByVal sStr As String) As String
Private Declare Function Float2HStr Lib "convert.dll" (ByVal fVal As Single) As String
Private Declare Function Long2HStr Lib "convert.dll" (ByVal lVal As Long) As String
Private Declare Function LLng2HStr Lib "convert.dll" (ByVal lVal As Long) As String
Private Declare Function Str2HStr Lib "convert.dll" (ByVal sVal As String) As String

Public Property Get Action() As FDAction
    Action = Datum.Action
End Property

Public Property Let Action(ByVal vNewValue As FDAction)
    Datum.Action = vNewValue
End Property

Public Property Get ParamID() As Variant
    ParamID = Datum.ParamID
End Property

Public Property Let ParamID(ByVal vNewValue As Variant)
    Datum.ParamID = vNewValue
End Property

Public Property Get DataType() As Variant
    DataType = Datum.DataType
End Property

Public Property Let DataType(ByVal vNewValue As Variant)
    Datum.DataType = vNewValue
End Property

Public Property Get LinkType() As Variant
    LinkType = Datum.LinkType
End Property

Public Property Let LinkType(ByVal vNewValue As Variant)
    Datum.LinkType = vNewValue
End Property

Public Property Get DataSetName() As Variant
    DataSetName = Datum.DataSetName
End Property

Public Property Let DataSetName(ByVal vNewValue As Variant)
    Datum.DataSetName = vNewValue
End Property

Public Property Get Addr1() As Variant
    Addr1 = Datum.Addr1
End Property

Public Property Let Addr1(ByVal vNewValue As Variant)
    Datum.Addr1 = vNewValue
End Property

Public Property Get Addr2() As Variant
    Addr2 = Datum.Addr2
End Property

Public Property Let Addr2(ByVal vNewValue As Variant)
    Datum.Addr2 = vNewValue
End Property

Public Property Get Addr3() As Variant
    Addr3 = Datum.Addr3
End Property

Public Property Let Addr3(ByVal vNewValue As Variant)
    Datum.Addr3 = vNewValue
End Property

Public Property Get Addr4() As Variant
    Addr4 = Datum.Addr4
End Property

Public Property Let Addr4(ByVal vNewValue As Variant)
    Datum.Addr4 = vNewValue
End Property

Public Property Get Addr5() As Variant
    Addr5 = Datum.Addr5
End Property

Public Property Let Addr5(ByVal vNewValue As Variant)
    Datum.Addr5 = vNewValue
End Property
Public Property Get Addr6() As Variant
    Addr6 = Datum.Addr6
End Property

Public Property Let Addr6(ByVal vNewValue As Variant)
    Datum.Addr6 = vNewValue
End Property

Public Property Get Date1() As Variant
    Date1 = Datum.Date1
End Property

Public Property Let Date1(ByVal vNewValue As Variant)
    Datum.Date1 = vNewValue
End Property

Public Property Get Date2() As Variant
    Date2 = Datum.Date2
End Property

Public Property Let Date2(ByVal vNewValue As Variant)
    Datum.Date2 = vNewValue
End Property

Private Function AlwayString(vParam As Variant) As String
    Select Case VarType(vParam)
        Case vbString
            AlwayString = vParam
        Case vbInteger
            AlwayString = Trim(str(vParam))
        Case vbLong
            AlwayString = Trim(str(vParam))
        Case vbSingle
            AlwayString = Trim(str(vParam))
        Case vbDouble
            AlwayString = Trim(str(vParam))
        Case vbDecimal
            AlwayString = Trim(str(vParam))
        Case vbDate
            AlwayString = Format(vParam, SDATEFORMAT)
        Case vbBoolean
            AlwayString = IIf(vParam, "True", "False")
        Case vbCurrency
            AlwayString = "$" + Trim(str(vParam))
        Case Else
            AlwayString = ""
    End Select
End Function

Public Property Get Value() As Variant
    Value = Datum.Value
End Property

Private Function GetPartValue(ByRef sPool, ByRef pos) As String
  Dim iLen As Integer
  Dim s As String
    s = Mid(sPool, pos, LENTHLEN)
    If s = vbNullString Then
      GetPartValue = "0"
      Exit Function
    End If
    
    iLen = CInt(Mid(sPool, pos, LENTHLEN))
    pos = pos + LENTHLEN
    GetPartValue = Mid(sPool, pos, iLen)
    pos = pos + iLen
End Function

Public Sub ShareValue(ByVal vNewValue As Variant)
    Datum.Value = vNewValue
End Sub

Public Property Let Value(ByVal vNewValue As Variant)
    Dim sRetStr As String
    Dim nId As Integer
    Dim nPos As Integer
    Dim sValue As String
    Dim vDate1 As Date
    Dim vDate2 As Date
    Dim vMeId As Long
    
    Datum.Value = vNewValue
    If IsAuto2LEIS Then
      If Me.LinkType = "SGR" Then
        nPos = 1
        vMeId = Me.ID
        vDate1 = Me.Date1
        vDate2 = Me.Date2
        Do
           nId = CInt(GetPartValue(Datum.Value, nPos))
           If nId = 0 Then Exit Do
           sValue = GetPartValue(Datum.Value, nPos)
           LDB.WriteQueue nId, vDate2, sValue, vMeId, vDate1
        Loop
      Else
            sRetStr = LDB.WriteQueue(Me.ParamID, _
                    Me.Date2, Me.Value, Me.ID, Me.Date1)
      End If
'        If sRetStr <> "" Then
'
'            sRetStr = LDB.WriteQueue(Me.ParamID, _
'                    Me.Date2, Me.Value, Me.ID, Me.Date1)
'
'        End If
    End If
End Property

Public Property Let CValue(ByVal vNewValue As Variant)
    Datum.Action = FDUndefigned
    Datum.Addr1 = ""
    Datum.Addr2 = ""
    Datum.Addr3 = ""
    Datum.Addr4 = ""
    Datum.Addr5 = ""
    Datum.Addr6 = ""
    Datum.DataSetName = ""
    Datum.DataType = "string"
    Datum.Date1 = ""
    Datum.Date2 = ""
    Datum.LinkType = ""
    Datum.ParamID = ""
    Datum.Tag = -1
    Datum.Value = vNewValue
End Property

Public Property Get Tag() As Variant
    Tag = Datum.Tag
End Property

Public Property Let Tag(ByVal vNewValue As Variant)
    Datum.Tag = vNewValue
End Property

Private Function GetPoolStr(ByRef str) As String
  Dim nLen As Integer
    nLen = Len(str)
    If nLen < 10 Then
      GetPoolStr = CStr(nLen)
    Else
      GetPoolStr = Chr(65 + (nLen \ 100))
      nLen = nLen Mod 100
      GetPoolStr = GetPoolStr & Format(nLen, "00")
    End If
'    GetPoolStr = Format(Len(str), String(LENTHLEN, "0"))
    GetPoolStr = GetPoolStr & str
End Function

Public Property Get PoolString() As String
'    Dim sTmp As String
    
    PoolString = SCLSNAME + _
        GetPoolStr(Trim(str(Datum.Action))) + _
        GetPoolStr(Datum.Addr1) + _
        GetPoolStr(Datum.Addr2) + _
        GetPoolStr(Datum.Addr3) + _
        GetPoolStr(Datum.Addr4) + _
        GetPoolStr(Datum.Addr5) + _
        GetPoolStr(Datum.Addr6) + _
        GetPoolStr(Datum.DataSetName) + _
        GetPoolStr(Datum.DataType) + _
        GetPoolStr(Format(Datum.Date1, SDATEFORMAT)) + _
        GetPoolStr(Format(Datum.Date2, SDATEFORMAT)) + _
        GetPoolStr(Datum.LinkType) + _
        GetPoolStr(AlwayString(Datum.ParamID)) + _
        GetPoolStr(AlwayString(Datum.Tag)) + _
        GetPoolStr(AlwayString(Datum.Value))

End Property

Private Function GetPartPoolStr(ByRef sPool, ByRef pos) As String
  Dim iLen As Integer
  Dim s As String
'    s = Mid(sPool, pos, 1)
    iLen = Asc(Mid(sPool, pos, 1))
    If iLen < 65 Then
      iLen = iLen - 48
      pos = pos + 1
    Else
      iLen = (iLen - 65) * 100 + CInt(Mid(sPool, pos + 1, 2))
      pos = pos + 3
    End If
'    iLen = CInt(Mid(sPool, pos, LENTHLEN))
'    pos = pos + LENTHLEN
    GetPartPoolStr = Mid(sPool, pos, iLen)
    pos = pos + iLen
End Function

Public Property Let PoolString(ByVal vNewValue As String)
    Dim iLen As Integer, iComLen As Integer

On Error GoTo ErrorHandler

    If Left(vNewValue, Len(SCLSNAME)) = SCLSNAME Then
        iComLen = 1 + Len(SCLSNAME)
        
        Datum.Action = CLng(GetPartPoolStr(vNewValue, iComLen))
        Datum.Addr1 = GetPartPoolStr(vNewValue, iComLen)
        Datum.Addr2 = GetPartPoolStr(vNewValue, iComLen)
        Datum.Addr3 = GetPartPoolStr(vNewValue, iComLen)
        Datum.Addr4 = GetPartPoolStr(vNewValue, iComLen)
        Datum.Addr5 = GetPartPoolStr(vNewValue, iComLen)
        Datum.Addr6 = GetPartPoolStr(vNewValue, iComLen)
        Datum.DataSetName = GetPartPoolStr(vNewValue, iComLen)
        Datum.DataType = GetPartPoolStr(vNewValue, iComLen)
        Datum.Date1 = Str2Date(GetPartPoolStr(vNewValue, iComLen))
        Datum.Date2 = Str2Date(GetPartPoolStr(vNewValue, iComLen))
        Datum.LinkType = GetPartPoolStr(vNewValue, iComLen)
        Datum.ParamID = CLng(GetPartPoolStr(vNewValue, iComLen))
        Datum.Tag = CLng(GetPartPoolStr(vNewValue, iComLen))
        Datum.Value = GetPartPoolStr(vNewValue, iComLen)
    
    Else
        Err.Raise 0, "cFDPTR.PoolString Let", "Wrong prefix of string. Must start with 'cFDPTR'"
    End If
    
    Exit Property
    
ErrorHandler:
    Err.Raise 0, "cFDPTR.PoolString Let", "Error in reading string"
End Property

'Public Property Get PoolString() As String
'    Dim sTmp As String
'
'    PoolString = SCLSNAME
'
'    sTmp = Trim(Str(Datum.Action))
'    PoolString = PoolString + Format(Len(sTmp), String(LENTHLEN, "0"))
'    PoolString = PoolString + sTmp
'
'    PoolString = PoolString + Format(Len(Datum.Addr1), String(LENTHLEN, "0"))
'    PoolString = PoolString + Datum.Addr1
'
'    PoolString = PoolString + Format(Len(Datum.Addr2), String(LENTHLEN, "0"))
'    PoolString = PoolString + Datum.Addr2
'
'    PoolString = PoolString + Format(Len(Datum.Addr3), String(LENTHLEN, "0"))
'    PoolString = PoolString + Datum.Addr3
'
'    PoolString = PoolString + Format(Len(Datum.Addr4), String(LENTHLEN, "0"))
'    PoolString = PoolString + Datum.Addr4
'
'    PoolString = PoolString + Format(Len(Datum.Addr5), String(LENTHLEN, "0"))
'    PoolString = PoolString + Datum.Addr5
'
'    PoolString = PoolString + Format(Len(Datum.Addr6), String(LENTHLEN, "0"))
'    PoolString = PoolString + Datum.Addr6
'
'    PoolString = PoolString + Format(Len(Datum.DataSetName), String(LENTHLEN, "0"))
'    PoolString = PoolString + Datum.DataSetName
'
'    PoolString = PoolString + Format(Len(Datum.DataType), String(LENTHLEN, "0"))
'    PoolString = PoolString + Datum.DataType
'
'    PoolString = PoolString + Format(Len(SDATEFORMAT), String(LENTHLEN, "0"))
'    PoolString = PoolString + Format(Datum.Date1, SDATEFORMAT)
'
'    PoolString = PoolString + Format(Len(SDATEFORMAT), String(LENTHLEN, "0"))
'    PoolString = PoolString + Format(Datum.Date2, SDATEFORMAT)
'
'    PoolString = PoolString + Format(Len(Datum.LinkType), String(LENTHLEN, "0"))
'    PoolString = PoolString + Datum.LinkType
'
'    sTmp = AlwayString(Datum.ParamID)
'    PoolString = PoolString + Format(Len(sTmp), String(LENTHLEN, "0"))
'    PoolString = PoolString + sTmp
'
'    sTmp = AlwayString(Datum.Tag)
'    PoolString = PoolString + Format(Len(sTmp), String(LENTHLEN, "0"))
'    PoolString = PoolString + sTmp
'
'    sTmp = AlwayString(Datum.Value)
'    PoolString = PoolString + Format(Len(sTmp), String(LENTHLEN, "0"))
'    PoolString = PoolString + sTmp
'End Property
'
'Public Property Let PoolString(ByVal vNewValue As String)
'    Dim iLen As Integer, iComLen As Integer
'
'    If Left(vNewValue, Len(SCLSNAME)) = SCLSNAME Then
'        iComLen = 1 + Len(SCLSNAME)
'
'        iLen = CInt(Mid(vNewValue, iComLen, LENTHLEN))
'        iComLen = iComLen + LENTHLEN
'        Datum.Action = CLng(Mid(vNewValue, iComLen, iLen))
'        iComLen = iLen + iComLen
'
'        iLen = CInt(Mid(vNewValue, iComLen, LENTHLEN))
'        iComLen = iComLen + LENTHLEN
'        Datum.Addr1 = Mid(vNewValue, iComLen, iLen)
'        iComLen = iLen + iComLen
'
'        iLen = CInt(Mid(vNewValue, iComLen, LENTHLEN))
'        iComLen = iComLen + LENTHLEN
'        Datum.Addr2 = Mid(vNewValue, iComLen, iLen)
'        iComLen = iLen + iComLen
'
'        iLen = CInt(Mid(vNewValue, iComLen, LENTHLEN))
'        iComLen = iComLen + LENTHLEN
'        Datum.Addr3 = Mid(vNewValue, iComLen, iLen)
'        iComLen = iLen + iComLen
'
'        iLen = CInt(Mid(vNewValue, iComLen, LENTHLEN))
'        iComLen = iComLen + LENTHLEN
'        Datum.Addr4 = Mid(vNewValue, iComLen, iLen)
'        iComLen = iLen + iComLen
'
'        iLen = CInt(Mid(vNewValue, iComLen, LENTHLEN))
'        iComLen = iComLen + LENTHLEN
'        Datum.Addr5 = Mid(vNewValue, iComLen, iLen)
'        iComLen = iLen + iComLen
'
'        iLen = CInt(Mid(vNewValue, iComLen, LENTHLEN))
'        iComLen = iComLen + LENTHLEN
'        Datum.Addr6 = Mid(vNewValue, iComLen, iLen)
'        iComLen = iLen + iComLen
'
'        iLen = CInt(Mid(vNewValue, iComLen, LENTHLEN))
'        iComLen = iComLen + LENTHLEN
'        Datum.DataSetName = Mid(vNewValue, iComLen, iLen)
'        iComLen = iLen + iComLen
'
'        iLen = CInt(Mid(vNewValue, iComLen, LENTHLEN))
'        iComLen = iComLen + LENTHLEN
'        Datum.DataType = Mid(vNewValue, iComLen, iLen)
'        iComLen = iLen + iComLen
'
'        iLen = CInt(Mid(vNewValue, iComLen, LENTHLEN))
'        iComLen = iComLen + LENTHLEN
'        Datum.Date1 = Str2Date(Mid(vNewValue, iComLen, iLen))
'        iComLen = iLen + iComLen
'
'        iLen = CInt(Mid(vNewValue, iComLen, LENTHLEN))
'        iComLen = iComLen + LENTHLEN
'        Datum.Date2 = Str2Date(Mid(vNewValue, iComLen, iLen))
'        iComLen = iLen + iComLen
'
'        iLen = CInt(Mid(vNewValue, iComLen, LENTHLEN))
'        iComLen = iComLen + LENTHLEN
'        Datum.LinkType = Mid(vNewValue, iComLen, iLen)
'        iComLen = iLen + iComLen
'
'        iLen = CInt(Mid(vNewValue, iComLen, LENTHLEN))
'        iComLen = iComLen + LENTHLEN
'        Datum.ParamID = CLng(Mid(vNewValue, iComLen, iLen))
'        iComLen = iLen + iComLen
'
'        iLen = CInt(Mid(vNewValue, iComLen, LENTHLEN))
'        iComLen = iComLen + LENTHLEN
'        Datum.Tag = CLng(Mid(vNewValue, iComLen, iLen))
'        iComLen = iLen + iComLen
'
'        iLen = CInt(Mid(vNewValue, iComLen, LENTHLEN))
'        iComLen = iComLen + LENTHLEN
'        Datum.Value = Mid(vNewValue, iComLen, iLen)
'        iComLen = iLen + iComLen
'    Else
'        Err.Raise 0, "cFDPTR.PoolString Let", "Wrong prefix of string. Must start with 'cFDPTR'"
'    End If
'End Property

Private Function IsAuto2LEIS() As Boolean
    IsAuto2LEIS = (Len(Datum.DataSetName) > 3)
End Function

Public Property Let sValue(ByVal vNewValue As Variant)
    Datum.Value = vNewValue
End Property

Public Sub SetAllValues(ByVal ParamID As Variant, _
    ByVal DataType As Variant, ByVal LinkType As Variant, _
    ByVal DataSetName As String, ByVal Addr1 As Variant, _
    ByVal Addr2 As Variant, ByVal Addr3 As Variant, _
    ByVal Addr4 As Variant, ByVal Addr5 As Variant, _
    ByVal Addr6 As Variant, ByVal Date1 As Variant, _
    ByVal Date2 As Variant, ByVal Value As Variant, _
    ByVal Tag As Variant, ByVal Action As Variant)

    Datum.ParamID = ParamID
    Datum.DataType = DataType
    Datum.LinkType = LinkType
    Datum.DataSetName = DataSetName
    Datum.Addr1 = Addr1
    Datum.Addr2 = Addr2
    Datum.Addr3 = Addr3
    Datum.Addr4 = Addr4
    Datum.Addr5 = Addr5
    Datum.Addr6 = Addr6
    Datum.Date1 = Date1
    Datum.Date2 = Date2
    Datum.Value = Value
    Datum.Tag = Tag
    Datum.Action = Action
End Sub

Public Sub PutDataByID(ByVal sID As String, ByVal sRes As String)
    GetData CInt(sID)
    Datum.Value = sRes
    ShareData CInt(sID)
End Sub

Public Sub GetData(ByVal iIdx As Integer)
    Me.PoolString = GetParamValueAt(iIdx)
End Sub

Public Sub ShareData(ByVal iIdx As Integer)
    SetParamValueAt iIdx, Me.PoolString
End Sub

Public Property Get ArraySize() As Variant
    ArraySize = GetUpperArrayBound() + 1
End Property

Public Property Let ArraySize(ByVal vNewValue As Variant)
    SetArrsize CInt(vNewValue)
End Property

Private Sub Class_Initialize()
    Set LDB = New LEIS.LEISer
    bLEISConnAllowed = True
    If sLogFile = "" Then
        sLogFile = App.Path & "\EDPS.log"
    End If
    Call Str2File(sLogFile, "cFDPTR exampler created.")
End Sub

Private Sub Class_Terminate()
    Set LDB = Nothing
    Call Str2File(sLogFile, "cFDPTR exampler destroyed.")
End Sub

Public Property Get Hex() As Variant
    Select Case Datum.DataType
    Case "float"
        Hex = Float2HStr(Datum.Value)
    Case "long"
        Hex = Long2HStr(Datum.Value)
    Case "integer"
        Hex = LLng2HStr(Datum.Value)
    Case Else
        If Left(Datum.DataType, 7) = "string*" Then
            Hex = Str2HStr(Datum.Value)
        Else
        End If
    End Select
End Property

Public Property Let Hex(ByVal vNewValue As Variant)
    Select Case Datum.DataType
    Case "long"
        Me.Value = HStr2Long(vNewValue)
    Case "integer"
        Me.Value = HStr2LLng(vNewValue)
    Case "float"
        Me.Value = HStr2Float(vNewValue)
    Case Else
        If Left(Datum.DataType, 7) = "string*" Then
            Me.Value = HStr2Str(vNewValue)
        Else
        End If
    End Select
End Property

Public Property Get HexLen() As Long
    Select Case Datum.DataType
    Case "float"
        HexLen = 8
    Case "long"
        HexLen = 8
    Case "integer"
        HexLen = 4
    Case Else
        If Left(Datum.DataType, 7) = "string*" Then
            HexLen = CLng(Right(Datum.DataType, Len(Datum.DataType) - 7)) * 2
        Else
        End If
    End Select
End Property

Public Property Get ID() As Variant
    ID = Datum.SomeID
End Property

Public Property Let ID(ByVal vNewValue As Variant)
    Datum.SomeID = vNewValue
End Property

Public Property Get LEISConnAllowed() As Boolean
    LEISConnAllowed = bLEISConnAllowed
End Property

Public Property Let LEISConnAllowed(ByVal vNewValue As Boolean)
    bLEISConnAllowed = vNewValue
    If bLEISConnAllowed Then
        LDB.AllowConnection
    Else
        LDB.DenyConnection
    End If
End Property

Private Function Str2Date(ByVal strDate As String) As Date
    Dim dtLocal As Date, iD1 As Integer, iD2 As Integer, iD3 As Integer

    iD1 = CInt(Mid(strDate, 7, 4))
    iD2 = CInt(Mid(strDate, 1, 2))
    iD3 = CInt(Mid(strDate, 4, 2))
    dtLocal = DateSerial(iD1, iD2, iD3)

    iD1 = CInt(Mid(strDate, 12, 2))
    iD2 = CInt(Mid(strDate, 15, 2))
    iD3 = CInt(Mid(strDate, 18, 2))
    dtLocal = DateAdd("h", iD1, dtLocal)
    dtLocal = DateAdd("n", iD2, dtLocal)
    
    Str2Date = DateAdd("s", iD3, dtLocal)
End Function

Public Property Let LogPath(ByVal vNewValue As Variant)
    sLogFile = vNewValue
End Property

Public Property Get LogPath() As Variant
    LogPath = sLogFile
End Property


