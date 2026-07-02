Attribute VB_Name = "modDeclares"
Option Explicit

' Arrays for MultiRead / MultiWrite operations
Global oSource() As ToRead
Global oResult() As Results
Global MFDPTR As Sis.cFDPTR

Private Const LENTHLEN As Integer = 3

Public Type Results
    sParID As String
    sValue As String
End Type

Public Type ToRead
    sCP As String
    sConn As String
    sVarName As String
    sType As String
    sParID As String
End Type

Public Type ToWrite
    sCP As String
    sConn As String
    sVarName As String
    sType As String
    sValue As String
    sLen As String
End Type

Public Function CombineToRead(ByVal sSource As String) As String
    Dim iI As Integer, iJ As Integer
    
    CombineToRead = ""
    
    iJ = ParseGroup(sSource)
    If iJ <= 0 Then Exit Function
    
    For iI = 0 To iJ
        CombineToRead = CombineToRead + Format(Len(oSource(iI).sCP), String(LENTHLEN, "0"))
        CombineToRead = CombineToRead + oSource(iI).sCP
        
        CombineToRead = CombineToRead + Format(Len(oSource(iI).sConn), String(LENTHLEN, "0"))
        CombineToRead = CombineToRead + oSource(iI).sConn
        
        CombineToRead = CombineToRead + Format(Len(oSource(iI).sVarName), String(LENTHLEN, "0"))
        CombineToRead = CombineToRead + oSource(iI).sVarName
        
        CombineToRead = CombineToRead + Format(Len(oSource(iI).sType), String(LENTHLEN, "0"))
        CombineToRead = CombineToRead + oSource(iI).sType
        
        CombineToRead = CombineToRead + Format(Len(oSource(iI).sParID), String(LENTHLEN, "0"))
        CombineToRead = CombineToRead + oSource(iI).sParID
    Next iI
End Function

Public Sub ParseResult(ByVal sResult As String, ByRef oRes() As Results)
    Dim iI As Integer
    Dim iLen As Integer, iComLen As Integer

    iComLen = 1
    
    For iI = LBound(oRes) To UBound(oRes)
        iLen = CInt(Mid(sResult, iComLen, LENTHLEN))
        iComLen = iComLen + LENTHLEN
        oRes(iI).sParID = Mid(sResult, iComLen, iLen)
        iComLen = iLen + iComLen
        
        iLen = CInt(Mid(sResult, iComLen, LENTHLEN))
        iComLen = iComLen + LENTHLEN
        oRes(iI).sValue = Mid(sResult, iComLen, iLen)
        iComLen = iLen + iComLen
    Next iI
End Sub

Public Function CombineToWrite(oSour() As ToWrite) As String
    Dim iI As Integer
    
    CombineToWrite = ""
    For iI = LBound(oSour) To UBound(oSour)
        CombineToWrite = CombineToWrite + Format(Len(oSour(iI).sCP), String(LENTHLEN, "0"))
        CombineToWrite = CombineToWrite + oSour(iI).sCP
        
        CombineToWrite = CombineToWrite + Format(Len(oSour(iI).sConn), String(LENTHLEN, "0"))
        CombineToWrite = CombineToWrite + oSour(iI).sConn
        
        CombineToWrite = CombineToWrite + Format(Len(oSour(iI).sVarName), String(LENTHLEN, "0"))
        CombineToWrite = CombineToWrite + oSour(iI).sVarName
        
        CombineToWrite = CombineToWrite + Format(Len(oSour(iI).sType), String(LENTHLEN, "0"))
        CombineToWrite = CombineToWrite + oSour(iI).sType
        
        CombineToWrite = CombineToWrite + Format(Len(oSour(iI).sValue), String(LENTHLEN, "0"))
        CombineToWrite = CombineToWrite + oSour(iI).sValue
    
        CombineToWrite = CombineToWrite + Format(Len(oSour(iI).sLen), String(LENTHLEN, "0"))
        CombineToWrite = CombineToWrite + oSour(iI).sLen
    Next iI
End Function

Private Function ParseGroup(ByVal sSource As String) As Integer
    Dim iI As Integer, iLast As Integer, iCurr As Integer
    iCurr = 1
    iI = 0
    
    ReDim oSource(0 To 0)
    
    iLast = InStr(iCurr, sSource, ";")
    Do While iLast > 0
        ReDim Preserve oSource(0 To iI)
        
        oSource(iI).sParID = Trim(Mid(sSource, iCurr, iLast - iCurr))
        iCurr = iLast + 1
        If oSource(iI).sParID <> "" And oSource(iI).sParID <> ";" Then
            MFDPTR.GetData oSource(iI).sParID
            oSource(iI).sCP = MFDPTR.Addr1
            oSource(iI).sConn = MFDPTR.Addr2
            oSource(iI).sVarName = MFDPTR.Addr3
            oSource(iI).sType = MFDPTR.Addr4
            iI = iI + 1
        End If
        
        iLast = InStr(iCurr, sSource, ";")
    Loop
    If Trim(Right(sSource, Len(sSource) - iCurr + 1)) <> "" _
        And Trim(Right(sSource, Len(sSource) - iCurr + 1)) <> ";" Then
        ReDim Preserve oSource(0 To iI)
        oSource(iI).sParID = Trim(Right(sSource, Len(sSource) - iCurr + 1))
        MFDPTR.GetData oSource(iI).sParID
        oSource(iI).sCP = MFDPTR.Addr1
        oSource(iI).sConn = MFDPTR.Addr2
        oSource(iI).sVarName = MFDPTR.Addr3
        oSource(iI).sType = MFDPTR.Addr4
        iI = iI + 1
    End If
    ReDim oResult(0 To iI - 1)
    ParseGroup = iI - 1
End Function

Public Sub SpreadResuts(ByVal sRes As String)
    Dim iI As Integer
    
    If Not IsNumeric(Mid(sRes, 1, LENTHLEN)) Then Exit Sub
    ParseResult sRes, oResult
    
    For iI = 0 To UBound(oResult)
        Call MFDPTR.PutDataByID(oResult(iI).sParID, oResult(iI).sValue)
    Next iI
End Sub
