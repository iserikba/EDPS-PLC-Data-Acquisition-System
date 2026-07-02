VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
END
Attribute VB_Name = "cFDPTR"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Public Enum FDAction
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
    Action As Variant
End Type

Private Datum As THFDIS
Private Const SDATEFORMAT As String = "MM/dd/YYYY hh:nn:ss"
Private Const SCLSNAME As String = "cFDPTR"
Private Const LENTHLEN As Integer = 3

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
            AlwayString = Format(vParam, SDATEFORMAT)
        Case vbBoolean
            AlwayString = IIf(vParam, "True", "False")
        Case vbCurrency
            AlwayString = "$" + Trim(Str(vParam))
        Case vbNull
            AlwayString = ""
        Case Else
            AlwayString = ""
    End Select
End Function
Public Property Get Value() As Variant
    Value = Datum.Value
End Property

Public Property Let Value(ByVal vNewValue As Variant)
Dim sTmp As String
    Datum.Value = vNewValue
    
'    If Me.IsAuto2LEIS Then
'        LDB.WriteQueue Me.ParamID, _
'        Me.DateTime, Me.Value, Me.DateTime2
'    End If

End Property
Public Property Get Tag() As Variant
    Tag = Datum.Tag
End Property
Public Property Let Tag(ByVal vNewValue As Variant)
    Datum.Tag = vNewValue
End Property
Public Property Get PoolString() As String
Dim sTmp As String
    
    PoolString = SCLSNAME
    
    sTmp = Trim(Str(Datum.Action))
    PoolString = PoolString + Format(Len(sTmp), String(LENTHLEN, "0"))
    PoolString = PoolString + sTmp
    
    PoolString = PoolString + Format(Len(Datum.Addr1), String(LENTHLEN, "0"))
    PoolString = PoolString + Datum.Addr1
    
    PoolString = PoolString + Format(Len(Datum.Addr2), String(LENTHLEN, "0"))
    PoolString = PoolString + Datum.Addr2
    
    PoolString = PoolString + Format(Len(Datum.Addr3), String(LENTHLEN, "0"))
    PoolString = PoolString + Datum.Addr3
    
    PoolString = PoolString + Format(Len(Datum.Addr4), String(LENTHLEN, "0"))
    PoolString = PoolString + Datum.Addr4
    
    PoolString = PoolString + Format(Len(Datum.Addr5), String(LENTHLEN, "0"))
    PoolString = PoolString + Datum.Addr5
    
    PoolString = PoolString + Format(Len(Datum.Addr6), String(LENTHLEN, "0"))
    PoolString = PoolString + Datum.Addr6
    
    PoolString = PoolString + Format(Len(Datum.DataSetName), String(LENTHLEN, "0"))
    PoolString = PoolString + Datum.DataSetName
    
    PoolString = PoolString + Format(Len(Datum.DataType), String(LENTHLEN, "0"))
    PoolString = PoolString + Datum.DataType
    
    PoolString = PoolString + Format(Len(SDATEFORMAT), String(LENTHLEN, "0"))
    PoolString = PoolString + Format(Datum.Date1, SDATEFORMAT)
    
    PoolString = PoolString + Format(Len(SDATEFORMAT), String(LENTHLEN, "0"))
    PoolString = PoolString + Format(Datum.Date2, SDATEFORMAT)
    
    PoolString = PoolString + Format(Len(Datum.LinkType), String(LENTHLEN, "0"))
    PoolString = PoolString + Datum.LinkType
    
    sTmp = AlwayString(Datum.ParamID)
    PoolString = PoolString + Format(Len(sTmp), String(LENTHLEN, "0"))
    PoolString = PoolString + sTmp
    
    sTmp = AlwayString(Datum.Tag)
    PoolString = PoolString + Format(Len(sTmp), String(LENTHLEN, "0"))
    PoolString = PoolString + sTmp
    
    sTmp = AlwayString(Datum.Value)
    PoolString = PoolString + Format(Len(sTmp), String(LENTHLEN, "0"))
    PoolString = PoolString + sTmp
    
End Property
Public Property Let PoolString(ByVal vNewValue As String)
Dim iLen As Integer, iComLen As Integer

    If Left(vNewValue, Len(SCLSNAME)) = SCLSNAME Then
        iComLen = 1 + Len(SCLSNAME)
        
        iLen = CInt(Mid(vNewValue, iComLen, LENTHLEN))
        iComLen = iComLen + LENTHLEN
        Datum.Action = CLng(Mid(vNewValue, iComLen, iLen))
        iComLen = iLen + iComLen
        
        iLen = CInt(Mid(vNewValue, iComLen, LENTHLEN))
        iComLen = iComLen + LENTHLEN
        Datum.Addr1 = Mid(vNewValue, iComLen, iLen)
        iComLen = iLen + iComLen
        
        iLen = CInt(Mid(vNewValue, iComLen, LENTHLEN))
        iComLen = iComLen + LENTHLEN
        Datum.Addr2 = Mid(vNewValue, iComLen, iLen)
        iComLen = iLen + iComLen
        
        iLen = CInt(Mid(vNewValue, iComLen, LENTHLEN))
        iComLen = iComLen + LENTHLEN
        Datum.Addr3 = Mid(vNewValue, iComLen, iLen)
        iComLen = iLen + iComLen
        
        iLen = CInt(Mid(vNewValue, iComLen, LENTHLEN))
        iComLen = iComLen + LENTHLEN
        Datum.Addr4 = Mid(vNewValue, iComLen, iLen)
        iComLen = iLen + iComLen
        
        iLen = CInt(Mid(vNewValue, iComLen, LENTHLEN))
        iComLen = iComLen + LENTHLEN
        Datum.Addr5 = Mid(vNewValue, iComLen, iLen)
        iComLen = iLen + iComLen
        
        iLen = CInt(Mid(vNewValue, iComLen, LENTHLEN))
        iComLen = iComLen + LENTHLEN
        Datum.Addr6 = Mid(vNewValue, iComLen, iLen)
        iComLen = iLen + iComLen
        
        iLen = CInt(Mid(vNewValue, iComLen, LENTHLEN))
        iComLen = iComLen + LENTHLEN
        Datum.DataSetName = Mid(vNewValue, iComLen, iLen)
        iComLen = iLen + iComLen
        
        iLen = CInt(Mid(vNewValue, iComLen, LENTHLEN))
        iComLen = iComLen + LENTHLEN
        Datum.DataType = Mid(vNewValue, iComLen, iLen)
        iComLen = iLen + iComLen
        
        iLen = CInt(Mid(vNewValue, iComLen, LENTHLEN))
        iComLen = iComLen + LENTHLEN
        Datum.Date1 = CDate(Mid(vNewValue, iComLen, iLen))
        iComLen = iLen + iComLen
        
        iLen = CInt(Mid(vNewValue, iComLen, LENTHLEN))
        iComLen = iComLen + LENTHLEN
        Datum.Date2 = CDate(Mid(vNewValue, iComLen, iLen))
        iComLen = iLen + iComLen
        
        iLen = CInt(Mid(vNewValue, iComLen, LENTHLEN))
        iComLen = iComLen + LENTHLEN
        Datum.LinkType = Mid(vNewValue, iComLen, iLen)
        iComLen = iLen + iComLen
        
        iLen = CInt(Mid(vNewValue, iComLen, LENTHLEN))
        iComLen = iComLen + LENTHLEN
        Datum.ParamID = CLng(Mid(vNewValue, iComLen, iLen))
        iComLen = iLen + iComLen
        
        iLen = CInt(Mid(vNewValue, iComLen, LENTHLEN))
        iComLen = iComLen + LENTHLEN
        Datum.Tag = CLng(Mid(vNewValue, iComLen, iLen))
        iComLen = iLen + iComLen
        
        iLen = CInt(Mid(vNewValue, iComLen, LENTHLEN))
        iComLen = iComLen + LENTHLEN
        Datum.Value = Mid(vNewValue, iComLen, iLen)
        iComLen = iLen + iComLen
    Else
        Err.Raise 0, "cFDPTR.PoolString Let", "Wrong prefix of string. Must start with 'cFDPTR'"
    End If
End Property

Public Function IsAuto2LEIS() As Boolean
    IsAuto2LEIS = (Datum.DataSetName <> "")
End Function
Public Property Let SValue(ByVal vNewValue As Variant)
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

