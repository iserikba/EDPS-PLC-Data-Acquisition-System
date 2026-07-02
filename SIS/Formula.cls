VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
END
Attribute VB_Name = "cFormula"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Attribute VB_Ext_KEY = "SavedWithClassBuilder" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
Option Explicit

Private lFormulaID As Long
Private LineID_Min As Long
Private LineID_Max As Long
Private LineID_Cnt As Long
Private sLogFile As String

Private Type TFormula
    Line_ID As Variant
    FunctionCode As Variant
    Operand1 As Variant
    Type1 As Variant
    Line1 As Variant
    Operand2 As Variant
    Type2 As Variant
    Line2 As Variant
    Operand3 As Variant
    Type3 As Variant
    Line3 As Variant
    Descr As Variant
End Type

Private Formulas() As TFormula

Public Property Let FormulaID(ByVal vData As Long)
    lFormulaID = vData
End Property

Public Property Get FormulaID() As Long
    FormulaID = lFormulaID
End Property

Public Property Get First() As Long
    First = LineID_Min
End Property

Public Property Get Last() As Long
    Last = LineID_Max
End Property

Public Property Get Current() As Long
    Current = LineID_Cnt
End Property

Public Property Let Current(ByVal vNewValue As Long)
    LineID_Cnt = vNewValue
End Property

Public Property Let FunctionCode(ByVal vData As Variant)
    Formulas(LineID_Cnt).FunctionCode = vData
End Property

Public Property Get FunctionCode() As Variant
    FunctionCode = Formulas(LineID_Cnt).FunctionCode
End Property

Public Property Let Line_ID(ByVal vData As Variant)
    Formulas(LineID_Cnt).Line_ID = vData
End Property

Public Property Get Line_ID() As Variant
    Line_ID = Formulas(LineID_Cnt).Line_ID
End Property

Public Property Let Line1(ByVal vData As Variant)
    Formulas(LineID_Cnt).Line1 = vData
End Property

Public Property Get Line1() As Variant
    Line1 = Formulas(LineID_Cnt).Line1
End Property

Public Property Let Line2(ByVal vData As Variant)
    Formulas(LineID_Cnt).Line2 = vData
End Property

Public Property Get Line2() As Variant
    Line2 = Formulas(LineID_Cnt).Line2
End Property

Public Property Let Line3(ByVal vData As Variant)
    Formulas(LineID_Cnt).Line3 = vData
End Property

Public Property Get Line3() As Variant
    Line3 = Formulas(LineID_Cnt).Line3
End Property

Public Property Let Operand1(ByVal vData As Variant)
    Formulas(LineID_Cnt).Operand1 = vData
End Property

Public Property Get Operand1() As Variant
    Operand1 = Formulas(LineID_Cnt).Operand1
End Property

Public Property Let Operand2(ByVal vData As Variant)
    Formulas(LineID_Cnt).Operand2 = vData
End Property

Public Property Get Operand2() As Variant
    Operand2 = Formulas(LineID_Cnt).Operand2
End Property

Public Property Let Operand3(ByVal vData As Variant)
    Formulas(LineID_Cnt).Operand3 = vData
End Property

Public Property Get Operand3() As Variant
    Operand3 = Formulas(LineID_Cnt).Operand3
End Property

Public Property Let Type1(ByVal vData As Variant)
    Formulas(LineID_Cnt).Type1 = vData
End Property

Public Property Get Type1() As Variant
    Type1 = Formulas(LineID_Cnt).Type1
End Property

Public Property Let Type2(ByVal vData As Variant)
    Formulas(LineID_Cnt).Type2 = vData
End Property

Public Property Get Type2() As Variant
    Type2 = Formulas(LineID_Cnt).Type2
End Property

Public Property Let Type3(ByVal vData As Variant)
    Formulas(LineID_Cnt).Type3 = vData
End Property

Public Property Get Type3() As Variant
    Type3 = Formulas(LineID_Cnt).Type3
End Property

Private Sub Class_Initialize()
    ReDim Formulas(0 To 0)
    LineID_Min = 2147483647
    LineID_Max = -2147483647
    LineID_Cnt = 0

    If sLogFile = "" Then
        sLogFile = App.Path & "\EDPS.log"
    End If
    
    Call Str2File(sLogFile, "cFormula exampler created.")
End Sub

Private Sub Class_Terminate()
    ReDim Formulas(0 To 0)
    Call Str2File(sLogFile, "cFormula exampler destroyed.")
End Sub

Public Sub CreateLine( _
    ByVal Line_ID As Variant, _
    ByVal FunctionCode As Variant, _
    ByVal Operand1 As Variant, _
    ByVal Type1 As Variant, _
    ByVal Line1 As Variant, _
    ByVal Operand2 As Variant, _
    ByVal Type2 As Variant, _
    ByVal Line2 As Variant, _
    ByVal Operand3 As Variant, _
    ByVal Type3 As Variant, _
    ByVal Line3 As Variant, _
    ByVal Descr As Variant)
    
    If UBound(Formulas, 1) < Line_ID Then
        ReDim Preserve Formulas(LBound(Formulas, 1) To Line_ID)
    End If
    If Line_ID > LineID_Max Then LineID_Max = Line_ID
    If Line_ID < LineID_Min Then LineID_Min = Line_ID
    
    Formulas(Line_ID).FunctionCode = FunctionCode
    Formulas(Line_ID).Line_ID = Line_ID
    Formulas(Line_ID).Line1 = Line1
    Formulas(Line_ID).Line2 = Line2
    Formulas(Line_ID).Line3 = Line3
    Formulas(Line_ID).Operand1 = Operand1
    Formulas(Line_ID).Operand2 = Operand2
    Formulas(Line_ID).Operand3 = Operand3
    Formulas(Line_ID).Type1 = Type1
    Formulas(Line_ID).Type2 = Type2
    Formulas(Line_ID).Type3 = Type3
    Formulas(Line_ID).Descr = Descr
End Sub

Public Property Get MaxLine() As Variant
    MaxLine = LineID_Max
End Property

Public Property Get MinLine() As Variant
    MinLine = LineID_Min
End Property

Public Property Get Descr() As Variant
    Descr = Formulas(LineID_Cnt).Descr
End Property

Public Property Let Descr(ByVal vNewValue As Variant)
    Formulas(LineID_Cnt).Descr = vNewValue
End Property

Public Property Let LogPath(ByVal vNewValue As Variant)
    sLogFile = vNewValue
End Property

Public Property Get LogPath() As Variant
    LogPath = sLogFile
End Property


