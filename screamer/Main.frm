VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "comdlg32.ocx"
Object = "{BDC217C8-ED16-11CD-956C-0000C04E4C0A}#1.1#0"; "tabctl32.ocx"
Object = "{3B7C8863-D78F-101B-B9B5-04021C009402}#1.2#0"; "RICHTX32.OCX"
Object = "{648A5603-2C6E-101B-82B6-000000000014}#1.1#0"; "MSCOMM32.OCX"
Begin VB.Form Main 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Screamer v1.91 - BoostBloader"
   ClientHeight    =   5175
   ClientLeft      =   5325
   ClientTop       =   5325
   ClientWidth     =   8385
   Icon            =   "Main.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   5175
   ScaleWidth      =   8385
   Begin TabDlg.SSTab SSTab1 
      Height          =   4995
      Left            =   0
      TabIndex        =   0
      Top             =   105
      Width           =   8325
      _ExtentX        =   14684
      _ExtentY        =   8811
      _Version        =   393216
      Tabs            =   2
      TabsPerRow      =   5
      TabHeight       =   520
      ShowFocusRect   =   0   'False
      TabCaption(0)   =   "Setup"
      TabPicture(0)   =   "Main.frx":0ECA
      Tab(0).ControlEnabled=   -1  'True
      Tab(0).Control(0)=   "Label4"
      Tab(0).Control(0).Enabled=   0   'False
      Tab(0).Control(1)=   "Label1"
      Tab(0).Control(1).Enabled=   0   'False
      Tab(0).Control(2)=   "Label2"
      Tab(0).Control(2).Enabled=   0   'False
      Tab(0).Control(3)=   "Label3"
      Tab(0).Control(3).Enabled=   0   'False
      Tab(0).Control(4)=   "lblDownloadBarBG"
      Tab(0).Control(4).Enabled=   0   'False
      Tab(0).Control(5)=   "lblDownloadBarFG"
      Tab(0).Control(5).Enabled=   0   'False
      Tab(0).Control(6)=   "lblCurrentBuffer"
      Tab(0).Control(6).Enabled=   0   'False
      Tab(0).Control(7)=   "sptConnect"
      Tab(0).Control(7).Enabled=   0   'False
      Tab(0).Control(8)=   "sptDownload"
      Tab(0).Control(8).Enabled=   0   'False
      Tab(0).Control(9)=   "sptComplete"
      Tab(0).Control(9).Enabled=   0   'False
      Tab(0).Control(10)=   "lblDownload"
      Tab(0).Control(10).Enabled=   0   'False
      Tab(0).Control(11)=   "lblRetries"
      Tab(0).Control(11).Enabled=   0   'False
      Tab(0).Control(12)=   "lblResend"
      Tab(0).Control(12).Enabled=   0   'False
      Tab(0).Control(13)=   "lblStatus"
      Tab(0).Control(13).Enabled=   0   'False
      Tab(0).Control(14)=   "lblStat"
      Tab(0).Control(14).Enabled=   0   'False
      Tab(0).Control(15)=   "InfoLabel"
      Tab(0).Control(15).Enabled=   0   'False
      Tab(0).Control(16)=   "cd1"
      Tab(0).Control(16).Enabled=   0   'False
      Tab(0).Control(17)=   "cmdDownload2(2)"
      Tab(0).Control(17).Enabled=   0   'False
      Tab(0).Control(18)=   "cmdDownload2(1)"
      Tab(0).Control(18).Enabled=   0   'False
      Tab(0).Control(19)=   "cmdDownload2(0)"
      Tab(0).Control(19).Enabled=   0   'False
      Tab(0).Control(20)=   "cmdOpen(2)"
      Tab(0).Control(20).Enabled=   0   'False
      Tab(0).Control(21)=   "cmdOpen(1)"
      Tab(0).Control(21).Enabled=   0   'False
      Tab(0).Control(22)=   "cmdOpen(0)"
      Tab(0).Control(22).Enabled=   0   'False
      Tab(0).Control(23)=   "txtFile(2)"
      Tab(0).Control(23).Enabled=   0   'False
      Tab(0).Control(24)=   "txtFile(1)"
      Tab(0).Control(24).Enabled=   0   'False
      Tab(0).Control(25)=   "txtFile(0)"
      Tab(0).Control(25).Enabled=   0   'False
      Tab(0).Control(26)=   "chkLaunchTerm"
      Tab(0).Control(26).Enabled=   0   'False
      Tab(0).Control(27)=   "cboOscillator"
      Tab(0).Control(27).Enabled=   0   'False
      Tab(0).Control(28)=   "cboChip"
      Tab(0).Control(28).Enabled=   0   'False
      Tab(0).Control(29)=   "cboBaud"
      Tab(0).Control(29).Enabled=   0   'False
      Tab(0).Control(30)=   "cmdBreak"
      Tab(0).Control(30).Enabled=   0   'False
      Tab(0).Control(31)=   "Timer1"
      Tab(0).Control(31).Enabled=   0   'False
      Tab(0).Control(32)=   "Text1"
      Tab(0).Control(32).Enabled=   0   'False
      Tab(0).Control(33)=   "txtComPort"
      Tab(0).Control(33).Enabled=   0   'False
      Tab(0).Control(34)=   "MSComm1"
      Tab(0).Control(34).Enabled=   0   'False
      Tab(0).ControlCount=   35
      TabCaption(1)   =   "Terminal"
      TabPicture(1)   =   "Main.frx":0EE6
      Tab(1).ControlEnabled=   0   'False
      Tab(1).Control(0)=   "Label5"
      Tab(1).Control(1)=   "optDec"
      Tab(1).Control(2)=   "optHex"
      Tab(1).Control(3)=   "optAsc"
      Tab(1).Control(4)=   "chkTerm"
      Tab(1).Control(5)=   "cmdClearDebug"
      Tab(1).Control(6)=   "txtTermOld"
      Tab(1).Control(7)=   "cboTermSpeed"
      Tab(1).Control(8)=   "cboTermDataBits"
      Tab(1).Control(9)=   "cboTermParity"
      Tab(1).Control(10)=   "cboTermStopBits"
      Tab(1).Control(11)=   "chkTermEcho"
      Tab(1).Control(12)=   "cmdOpenTerminal"
      Tab(1).Control(13)=   "txtTerm"
      Tab(1).ControlCount=   14
      Begin MSCommLib.MSComm MSComm1 
         Left            =   1320
         Top             =   3480
         _ExtentX        =   1005
         _ExtentY        =   1005
         _Version        =   393216
         DTREnable       =   -1  'True
      End
      Begin VB.TextBox txtComPort 
         Height          =   285
         Left            =   960
         TabIndex        =   42
         Text            =   "2"
         Top             =   600
         Width           =   375
      End
      Begin RichTextLib.RichTextBox txtTerm 
         Height          =   4335
         Left            =   -74880
         TabIndex        =   41
         Top             =   480
         Width           =   6495
         _ExtentX        =   11456
         _ExtentY        =   7646
         _Version        =   393217
         Enabled         =   -1  'True
         ScrollBars      =   2
         DisableNoScroll =   -1  'True
         TextRTF         =   $"Main.frx":0F02
      End
      Begin VB.CommandButton cmdOpenTerminal 
         Caption         =   "Open"
         Height          =   375
         Left            =   -68160
         TabIndex        =   40
         Top             =   3960
         Width           =   1095
      End
      Begin VB.CheckBox chkTermEcho 
         Caption         =   "Echo"
         Height          =   275
         Left            =   -68160
         TabIndex        =   39
         Top             =   960
         Width           =   800
      End
      Begin VB.ComboBox cboTermStopBits 
         Height          =   315
         ItemData        =   "Main.frx":0F84
         Left            =   -68160
         List            =   "Main.frx":0F91
         Style           =   2  'Dropdown List
         TabIndex        =   38
         Top             =   2040
         Width           =   615
      End
      Begin VB.ComboBox cboTermParity 
         Height          =   315
         ItemData        =   "Main.frx":0FA0
         Left            =   -67560
         List            =   "Main.frx":0FAD
         Style           =   2  'Dropdown List
         TabIndex        =   37
         Top             =   1680
         Width           =   495
      End
      Begin VB.ComboBox cboTermDataBits 
         Height          =   315
         ItemData        =   "Main.frx":0FBA
         Left            =   -68160
         List            =   "Main.frx":0FC4
         Style           =   2  'Dropdown List
         TabIndex        =   36
         Top             =   1680
         Width           =   495
      End
      Begin VB.ComboBox cboTermSpeed 
         Height          =   315
         ItemData        =   "Main.frx":0FCE
         Left            =   -68160
         List            =   "Main.frx":0FE7
         Style           =   2  'Dropdown List
         TabIndex        =   35
         Top             =   1320
         Width           =   1095
      End
      Begin VB.TextBox Text1 
         Height          =   285
         Left            =   4680
         MultiLine       =   -1  'True
         TabIndex        =   33
         TabStop         =   0   'False
         Top             =   2160
         Visible         =   0   'False
         Width           =   495
      End
      Begin VB.Timer Timer1 
         Enabled         =   0   'False
         Left            =   240
         Top             =   3480
      End
      Begin VB.TextBox txtTermOld 
         BeginProperty Font 
            Name            =   "Courier New"
            Size            =   9
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   330
         Left            =   -67440
         Locked          =   -1  'True
         MultiLine       =   -1  'True
         ScrollBars      =   2  'Vertical
         TabIndex        =   32
         Top             =   2400
         Width           =   615
      End
      Begin VB.CommandButton cmdClearDebug 
         Caption         =   "Clear"
         Height          =   375
         Left            =   -68160
         TabIndex        =   31
         Top             =   4440
         Width           =   1095
      End
      Begin VB.CheckBox chkTerm 
         Caption         =   "Enable"
         Height          =   275
         Left            =   -68160
         TabIndex        =   30
         Top             =   720
         Width           =   800
      End
      Begin VB.OptionButton optAsc 
         Caption         =   "ASCII"
         Height          =   255
         Left            =   -68160
         TabIndex        =   29
         Top             =   2760
         Value           =   -1  'True
         Width           =   975
      End
      Begin VB.OptionButton optHex 
         Caption         =   "Hex"
         Height          =   255
         Left            =   -68160
         TabIndex        =   28
         Top             =   3000
         Width           =   855
      End
      Begin VB.OptionButton optDec 
         Caption         =   "Dec"
         Height          =   255
         Left            =   -68160
         TabIndex        =   27
         Top             =   3240
         Width           =   735
      End
      Begin VB.CommandButton cmdBreak 
         Caption         =   "&Stop Waiting"
         Enabled         =   0   'False
         Height          =   615
         Left            =   5280
         TabIndex        =   14
         Top             =   3360
         Width           =   1575
      End
      Begin VB.ComboBox cboBaud 
         Height          =   315
         ItemData        =   "Main.frx":1018
         Left            =   960
         List            =   "Main.frx":102B
         Style           =   2  'Dropdown List
         TabIndex        =   1
         Top             =   1080
         Width           =   1100
      End
      Begin VB.ComboBox cboChip 
         Height          =   315
         ItemData        =   "Main.frx":1052
         Left            =   960
         List            =   "Main.frx":1062
         Style           =   2  'Dropdown List
         TabIndex        =   2
         Top             =   1560
         Width           =   1100
      End
      Begin VB.ComboBox cboOscillator 
         Height          =   315
         ItemData        =   "Main.frx":1088
         Left            =   960
         List            =   "Main.frx":10A1
         Style           =   2  'Dropdown List
         TabIndex        =   3
         Top             =   2040
         Width           =   1100
      End
      Begin VB.CheckBox chkLaunchTerm 
         Caption         =   "Open Terminal After Download"
         Height          =   495
         Left            =   480
         TabIndex        =   4
         Top             =   2760
         Width           =   1455
      End
      Begin VB.TextBox txtFile 
         Height          =   315
         Index           =   0
         Left            =   2400
         TabIndex        =   5
         Top             =   600
         Width           =   3015
      End
      Begin VB.TextBox txtFile 
         Height          =   315
         Index           =   1
         Left            =   2400
         TabIndex        =   8
         Top             =   1080
         Width           =   3015
      End
      Begin VB.TextBox txtFile 
         Height          =   315
         Index           =   2
         Left            =   2400
         TabIndex        =   11
         Top             =   1560
         Width           =   3015
      End
      Begin VB.CommandButton cmdOpen 
         Caption         =   "Open"
         Height          =   375
         Index           =   0
         Left            =   5520
         TabIndex        =   6
         Top             =   600
         Width           =   615
      End
      Begin VB.CommandButton cmdOpen 
         Caption         =   "Open"
         Height          =   375
         Index           =   1
         Left            =   5520
         TabIndex        =   9
         Top             =   1080
         Width           =   615
      End
      Begin VB.CommandButton cmdOpen 
         Caption         =   "Open"
         Height          =   375
         Index           =   2
         Left            =   5520
         TabIndex        =   12
         Top             =   1560
         Width           =   615
      End
      Begin VB.CommandButton cmdDownload2 
         Caption         =   "Download"
         Height          =   375
         Index           =   0
         Left            =   6240
         TabIndex        =   7
         Top             =   600
         Width           =   975
      End
      Begin VB.CommandButton cmdDownload2 
         Caption         =   "Download"
         Height          =   375
         Index           =   1
         Left            =   6240
         TabIndex        =   10
         Top             =   1080
         Width           =   975
      End
      Begin VB.CommandButton cmdDownload2 
         Caption         =   "Download"
         Height          =   375
         Index           =   2
         Left            =   6240
         TabIndex        =   13
         Top             =   1560
         Width           =   975
      End
      Begin MSComDlg.CommonDialog cd1 
         Left            =   720
         Top             =   3480
         _ExtentX        =   847
         _ExtentY        =   847
         _Version        =   393216
      End
      Begin VB.Label InfoLabel 
         Caption         =   "Nothing happening yet"
         Height          =   615
         Left            =   2400
         TabIndex        =   43
         Top             =   4200
         Width           =   4815
      End
      Begin VB.Label Label5 
         Caption         =   "Display:"
         Height          =   255
         Left            =   -68160
         TabIndex        =   34
         Top             =   2520
         Width           =   855
      End
      Begin VB.Label lblStat 
         Caption         =   "Status:"
         Height          =   255
         Left            =   2760
         TabIndex        =   26
         Top             =   3000
         Width           =   495
      End
      Begin VB.Label lblStatus 
         Caption         =   "Idle"
         Height          =   255
         Left            =   3360
         TabIndex        =   25
         Top             =   3000
         Width           =   3735
      End
      Begin VB.Label lblResend 
         Caption         =   "0"
         Height          =   255
         Left            =   3360
         TabIndex        =   24
         Top             =   3360
         Width           =   1095
      End
      Begin VB.Label lblRetries 
         Caption         =   "Retries:"
         Height          =   255
         Left            =   2760
         TabIndex        =   23
         Top             =   3360
         Width           =   615
      End
      Begin VB.Label lblDownload 
         Caption         =   "Idle"
         Height          =   255
         Left            =   2760
         TabIndex        =   22
         Top             =   3720
         Width           =   1935
      End
      Begin VB.Shape sptComplete 
         FillColor       =   &H000000FF&
         FillStyle       =   0  'Solid
         Height          =   255
         Left            =   2400
         Shape           =   3  'Circle
         Top             =   3720
         Width           =   255
      End
      Begin VB.Shape sptDownload 
         FillColor       =   &H000000FF&
         FillStyle       =   0  'Solid
         Height          =   255
         Left            =   2400
         Shape           =   3  'Circle
         Top             =   3360
         Width           =   255
      End
      Begin VB.Shape sptConnect 
         FillColor       =   &H000000FF&
         FillStyle       =   0  'Solid
         Height          =   255
         Left            =   2400
         Shape           =   3  'Circle
         Top             =   3000
         Width           =   255
      End
      Begin VB.Label lblCurrentBuffer 
         Caption         =   "Buffer Empty"
         BeginProperty Font 
            Name            =   "Courier"
            Size            =   9.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   255
         Left            =   2520
         TabIndex        =   21
         Top             =   2280
         Width           =   4695
      End
      Begin VB.Label lblDownloadBarFG 
         BackColor       =   &H00800000&
         Height          =   255
         Left            =   2400
         TabIndex        =   20
         Top             =   2550
         Width           =   495
      End
      Begin VB.Label lblDownloadBarBG 
         Appearance      =   0  'Flat
         BackColor       =   &H80000005&
         BorderStyle     =   1  'Fixed Single
         ForeColor       =   &H80000008&
         Height          =   255
         Left            =   2400
         TabIndex        =   19
         Top             =   2550
         Width           =   4695
      End
      Begin VB.Label Label3 
         Alignment       =   1  'Right Justify
         Caption         =   "Speed:"
         Height          =   315
         Left            =   120
         TabIndex        =   18
         Top             =   1125
         Width           =   795
      End
      Begin VB.Label Label2 
         Alignment       =   1  'Right Justify
         Caption         =   "Comm:"
         Height          =   315
         Left            =   120
         TabIndex        =   17
         Top             =   645
         Width           =   795
      End
      Begin VB.Label Label1 
         Alignment       =   1  'Right Justify
         Caption         =   "Chip:"
         Height          =   315
         Left            =   120
         TabIndex        =   16
         Top             =   1605
         Width           =   795
      End
      Begin VB.Label Label4 
         Alignment       =   1  'Right Justify
         Caption         =   "Oscillator:"
         Height          =   315
         Left            =   120
         TabIndex        =   15
         Top             =   2085
         Width           =   795
      End
   End
   Begin VB.Menu File_Menu 
      Caption         =   "&File"
      Begin VB.Menu Spacer 
         Caption         =   "------"
      End
      Begin VB.Menu File_Close 
         Caption         =   "Exit"
      End
   End
   Begin VB.Menu About_Menu 
      Caption         =   "&About"
   End
End
Attribute VB_Name = "Main"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'Option Explicit

' 1-20-04 v1.0
' 1-21-04 New v1.1 Added boot vector redirection to memory location 4

' 2-12-04 New v1.2 Added baud rate control.
'         Fixed the glitch - when the config word was seen, it was higher
'         then the bootloader, and erroneusly reported the HEX file as
'         being too large.
'         Added a settings file - Screamer now remembers your settings!

' 5-20-04 New v1.3 Added better boot loader protection
'         Bloader now transmits the absolute location of the boot loader
'         Screamer will not write anything above this address.
        
' 6-10-04 v1.4 Added chip selection so that we can control the boot vector load

' 8-24-04 v1.5, Mike Kerna, Added INHX32 support by ignoring type 04 records
'         in the hex file. (May have to change this in the future if this ever
'         supports non-14-bit chips.)
'         Browse directory now defaults to the file's directory,
'         or the application's directory if no file in settings.txt on startup.
'         Added terminal window (receive only) and config settings for it.
'         Added 10MHz oscillator option in case Sparky compiles a compatible
'         Bloader for me. :)

' 8-25-04 v1.5, Mike Kerna, Added terminal transmit.
'         Removed flicker from debug window.

' 8-26-04 v1.5, Mike Kerna, Terminal window can display ASCII, hex, or decimal.
'         Moved Form_Terminate code to Form_Unload, where it should be.  Checking
'         the comm port in Form_Terminate caused the Load event to fire, which
'         caused some (yeah, SOME) of the config settings to not save.  It took
'         some time to figure this one out.
'         Exit via File menu now saves config settings.

' 8-28-04 v1.5, Mike Kerna, Got rid of lockup when terminal receive is too fast.
'         I removed the OnComm event procedure and used a timer instead.

' 9-03-04 v1.5, Mike Kerna, Enabled changing terminal parameters while port is open

' 9-14-04 v1.6, Mike Kerna, Moved everything onto tab control.
'         Added 3 source files
'
' 10-24-04 v1.6, Mike Kerna, Added local echo checkbox


Dim CommPort As Integer
Dim CommSpeed As Double
Dim PICSpeed As Double
Dim PICType As String
Dim TermBaud As Double
Dim TermParity As String
Dim TermStopBits As Double
Dim TermDataBits As Integer
Dim TermAutoLaunch As Boolean
Dim TermMode As Boolean
Dim TermType As String
Dim TermEcho As Boolean
Dim txtUserFile(3) As String

Dim Stop_Waiting As Boolean

Private Sub About_Menu_Click()
    frmAbout.Show
End Sub

Private Sub cboBaud_Click()
    CommSpeed = cboBaud.List(cboBaud.ListIndex)
End Sub

Private Sub cboChip_Click()
    PICType = cboChip.List(cboChip.ListIndex)
End Sub

Private Sub cboCommPort_Click()
    CommPort = cboCommPort.ListIndex + 1
End Sub

Private Sub cboOscillator_Click()
    PICSpeed = cboOscillator.ItemData(cboOscillator.ListIndex)
End Sub

Private Sub cboTermDataBits_Click()
    If TermDataBits <> cboTermDataBits.Text Then
        TermDataBits = cboTermDataBits.Text
        If TermMode Then TermWindow (True)
    End If
End Sub

Private Sub cboTermParity_Click()
    If TermParity <> cboTermParity.Text Then
        TermParity = cboTermParity.Text
        If TermMode Then TermWindow (True)
    End If
End Sub

Private Sub cboTermSpeed_Click()
    If TermBaud <> cboTermSpeed.Text Then
        TermBaud = cboTermSpeed.Text
        If TermMode Then TermWindow (True)
    End If
End Sub

Private Sub cboTermStopBits_Click()
    If TermStopBits <> cboTermStopBits.Text Then
        TermStopBits = cboTermStopBits.Text
        If TermMode Then TermWindow (True)
    End If
End Sub

Private Sub chkTerm_Click()
    If chkTerm.Value = 1 Then
        TermWindow (True)
    Else
        TermWindow (False)
    End If
End Sub

Private Sub chkLaunchTerm_Click()
    TermAutoLaunch = (chkLaunchTerm.Value = 1)
End Sub

Private Sub chkTermEcho_Click()
    If chkTermEcho.Value = 1 Then
        TermEcho = True
    Else
        TermEcho = False
    End If
End Sub

Private Sub cmdBreak_Click()
    Stop_Waiting = True
End Sub

Function RoundNear(varNumber As Variant, varDelta As Variant) As Variant
'***********
'Name:      RoundNear (Function)
'Purpose:   rounds varnumber to the nearest fraction equal
'   varDelta
'Inputs:    varNumber - number to round
'           varDelta - the fraction used as measure of
'   rounding
'Example:   RoundNear(53,6) = 54
'           RoundNear(1.16,0.25) = 1.25
'           RoundNear(1.12,0.25) = 1.00
'           RoundNear(1.125,0.25)= 1.25
'Output:    varNumber rounded to nearest
'           multiple of varDelta.
'***********

 Dim varDec As Variant
 Dim intX As Integer
 Dim varX As Variant
 
 varX = varNumber / varDelta
 intX = Int(varX)
 varDec = CDec(varX) - intX
 
 If varDec >= 0.5 Then
   RoundNear = varDelta * (intX + 1)
 Else
   RoundNear = varDelta * intX
 End If
End Function

Private Sub cmdClearDebug_Click()
    txtTerm.Text = ""
    If TermMode Then txtTerm.SetFocus
End Sub

Private Sub TermWindow(bolEnable As Boolean)

On Error GoTo EH

    If bolEnable Then
        lblStatus.Caption = "Debug"
        chkTerm.Value = "1"
        txtTerm.Locked = False
        Timer1.Interval = 1
        Timer1.Enabled = True
    Else
        lblStatus.Caption = "Idle"
        chkTerm.Value = "0"
        txtTerm.Locked = True
        Timer1.Enabled = False
    End If
    
    If TermMode And Not bolEnable Then
        If MSComm1.PortOpen = True Then MSComm1.PortOpen = False
    End If
    
    TermMode = bolEnable
    If Not bolEnable Then Exit Sub
    
    TermMode = True
    SSTab1.Tab = 1
    
    With MSComm1
        If .PortOpen = True Then .PortOpen = False
        .CommPort = CommPort
        .InputLen = 0
        .Handshaking = comNone
        .RTSEnable = False
        .RThreshold = 1
        .SThreshold = 1
        .InBufferSize = 1024
        '.Settings = TermBaud & ","
        .Settings = TermBaud & "," _
                    & TermParity & "," _
                    & TermDataBits & "," _
                    & TermStopBits
        .PortOpen = True
    End With
    
    txtTerm.SetFocus
    
    Exit Sub
    
EH:
    If Err.Number = 8005 Then
        MsgBox "COM" & CommPort & " already open."
        chkTerm.Value = "0"
        lblStatus.Caption = "Error"
    End If
    
End Sub

Private Function Hex_Convert(nate As String) As Integer
'This function takes a two character string and converts it to an integer
Dim new_hex As Long
Dim temp As Integer
Dim i As Integer
    
    new_hex = 0
    
    For i = 0 To Len(nate) - 1
    
        'Peel off first letter
        temp = AscB(Right(Left(nate, 1 + i), 1))
        
        'Convert it to a number
        If temp >= AscB("A") And temp <= AscB("F") Then
            temp = temp - AscB("A") + 10
        ElseIf temp >= AscB("0") And temp <= AscB("9") Then
            temp = temp - AscB("0")
        End If
        
        'Shift the number
        new_hex = new_hex * 16 + temp
    
    Next
    
    Hex_Convert = new_hex

End Function

Private Sub cmdDownload2_Click(Index As Integer)

On Error GoTo ErrHandler
    
Dim outBuffer
Dim totalLen, currentLen
Dim fnum As Integer
Dim fileContentLines() As String
Dim i As Long
Dim j As Integer
    
Dim nate As String
Dim temp As Integer

Dim Record_Length As Integer

Dim DownloadSpeed As Integer

Dim Memory_Address_High As Integer
Dim Memory_Address_Low As Integer

Dim End_Record As Integer
Dim Outgoing_Data(50) As Integer
Dim Check_Sum As Integer

Dim Bloader_Start_High As Integer
Dim Bloader_Start_Low As Integer
'Dim Bloader_Jump_Low As Integer

Dim User_Boot_Vector_High As Integer
Dim User_Boot_Vector_Low As Integer
Dim temp_block As String
Dim Total_Code_Words As Integer

    TermMode = False
    TermWindow (False)
    
    'Reset the user break command
    Stop_Waiting = False
    Total_Code_Words = 0
    lblDownload.Caption = "Downloading..."
    
    'If port already opened then close it
    If MSComm1.PortOpen = True Then
        MSComm1.Output = Chr(4) ' output the magic character to kick off download
        MSComm1.PortOpen = False
    End If
    
    'Read in the HEX file - clean it up
    If txtFile(Index).Text <> "" Then
        fnum = FreeFile()
        Open txtFile(Index).Text For Input As #fnum
        
        'Read each line into fileContentLines() array
        fileContentLines() = Split(Input(LOF(fnum), fnum), vbCrLf)
        Close #fnum
        
        'Remove empty lines + lines not starting with a ":"
        For i = 0 To UBound(fileContentLines)
            If Len(fileContentLines(i)) = 0 Then fileContentLines(i) = vbNullChar
            If Left(fileContentLines(i), 1) <> ":" Then fileContentLines(i) = vbNullChar
            If Mid(fileContentLines(i), 8, 2) = "04" Then fileContentLines(i) = vbNullChar
        Next
        
        fileContentLines() = Filter(fileContentLines(), vbNullChar, False)
        totalLen = UBound(fileContentLines)
        
    Else
        MsgBox "Error, no HEX file found", vbOKOnly
        Exit Sub
    End If

    sptConnect.FillColor = &HFF&
    sptDownload.FillColor = &HFF&
    sptComplete.FillColor = &HFF&
    
    lblResend.Caption = 0
    lblDownload.Caption = "Running..."
    
    'Calculate download speed
    'This is the SPBRG register value that will be sent to the PIC
    DownloadSpeed = RoundNear(PICSpeed * 1000000 / (16 * CommSpeed) - 1, 0.25)
    'MsgBox DownloadSpeed
    'Exit Sub
    
    'Check to see if the PIC is outputting the load key ASC(5)
    lblStatus.Caption = "Waiting for PIC boot broadcast"
    MSComm1.CommPort = CommPort
    MSComm1.InputLen = 1
    MSComm1.Settings = "9600,n,8,1"
    MSComm1.PortOpen = True
    
    
    'Disable certain buttons
    cmdDownload2(0).Enabled = False
    cmdDownload2(1).Enabled = False
    cmdDownload2(2).Enabled = False
    cmdOpen(0).Enabled = False
    cmdOpen(1).Enabled = False
    cmdOpen(2).Enabled = False
    cmdBreak.Enabled = True
    cmdBreak.Caption = "&Stop Waiting"
    
    
    
    Do While MSComm1.Input <> Chr(5)
        
        'See if user wants to stop
        If Stop_Waiting = True Then
            MsgBox "The PIC did not enter load mode.", vbOKOnly
        
            lblStatus.Caption = "Idle"
            lblDownload.Caption = "Idle"
            cmdDownload2(0).Enabled = True
            cmdDownload2(1).Enabled = True
            cmdDownload2(2).Enabled = True
            cmdBreak.Enabled = False
            cmdOpen(0).Enabled = True
            cmdOpen(1).Enabled = True
            cmdOpen(2).Enabled = True
            Exit Sub
        End If
        
        DoEvents
            
    Loop
    cmdBreak.Caption = "Abort"

    'Output 6 to cause the PIC to go into load mode
    lblStatus.Caption = "Load Mode Command Sent"
    sptConnect.FillColor = &HFF00& 'Green
    MSComm1.Output = Chr(6)

    'Transmit the user's download speed
    MSComm1.Output = Chr(DownloadSpeed)
    
    'Now move to the new speed
    If CommSpeed <> "9600" Then
        MSComm1.PortOpen = False
        MSComm1.Settings = CommSpeed & ",n,8,1"
        MSComm1.PortOpen = True
    End If
    
    'Snag the bloader starting memory location info from PIC
    Do While MSComm1.InBufferCount < 2
        'See if user wants to stop
        If Stop_Waiting = True Then
            MsgBox "The PIC did not finish loading. You will likely experience unexpected program execution.", vbOKOnly
            
            lblStatus.Caption = "Idle"
            lblDownload.Caption = "Idle"
            cmdDownload2(0).Enabled = True
            cmdDownload2(1).Enabled = True
            cmdDownload2(2).Enabled = True
            cmdBreak.Enabled = False
            cmdOpen(0).Enabled = True
            cmdOpen(1).Enabled = True
            cmdOpen(2).Enabled = True
            
            Exit Sub
        End If
        
        DoEvents
    Loop
    Bloader_Start_High = Asc(MSComm1.Input) '31 '0x1F
    Bloader_Start_Low = Asc(MSComm1.Input)
    'Bloader_Jump_Low = Asc(MSComm1.Input)

    For i = 0 To UBound(fileContentLines)
        
        'See if user wants to stop
        If Stop_Waiting = True Then
            MsgBox "The PIC did not finish loading. You will likely experience unexpected program execution.", vbOKOnly
            
            lblStatus.Caption = "Idle"
            lblDownload.Caption = "Idle"
            cmdDownload2(0).Enabled = True
            cmdDownload2(1).Enabled = True
            cmdDownload2(2).Enabled = True
            cmdBreak.Enabled = False
            cmdOpen(0).Enabled = True
            cmdOpen(1).Enabled = True
            cmdOpen(2).Enabled = True
            
            Exit Sub
        End If
        
        'Wait for PIC to tell us he's ready
        InfoLabel.Caption = "Waiting for pic to say he is ready"
        'InfoLabel.Refresh
        Do While MSComm1.InBufferCount = 0
            DoEvents
        
            'See if user has aborted - most likely freeze error
            If Stop_Waiting = True Then
                MsgBox "The PIC did not finish loading. You will likely experience unexpected program execution.", vbOKOnly
                
                lblStatus.Caption = "Idle"
                lblDownload.Caption = "Idle"
                cmdDownload2(0).Enabled = True
                cmdDownload2(1).Enabled = True
                cmdDownload2(2).Enabled = True
                cmdBreak.Enabled = False
                cmdOpen(0).Enabled = True
                cmdOpen(1).Enabled = True
                cmdOpen(2).Enabled = True
                
                Exit Sub
            End If
            
        Loop
        
        Text1 = MSComm1.Input
        InfoLabel.Caption = "Got response from PIC"
        'InfoLabel.Refresh
        
        If Text1 = "T" Then 'All is well
            sptDownload.FillColor = &HFF00& 'Green
            Total_Code_Words = Total_Code_Words + (Record_Length / 2)
        ElseIf Text1 = Chr(7) Then 'Re-send line
            i = i - 1
            lblResend.Caption = lblResend.Caption + 1
            sptDownload.FillColor = &HFFFF& 'Yellow
            DoEvents
        Else
            MsgBox "Error : Incorrect response - " & Text1 & " from PIC. Programming is incomplete and will now halt."
            
            lblStatus.Caption = "Idle"
            lblDownload.Caption = "Idle"
            cmdDownload2(0).Enabled = True
            cmdDownload2(1).Enabled = True
            cmdDownload2(2).Enabled = True
            cmdBreak.Enabled = False
            cmdOpen(0).Enabled = True
            cmdOpen(1).Enabled = True
            cmdOpen(2).Enabled = True
        
            Exit Sub
        End If
        
Hard_Coded_Next_line:

        'Pull the next line from the file
        InfoLabel.Caption = "Get next line from file"
        'InfoLabel.Refresh

        'i = 0
        outBuffer = fileContentLines(i)
        lblCurrentBuffer.Caption = fileContentLines(i)
    
        'Do the fancy GUI updates
        lblStatus.Caption = "Loaded " & Total_Code_Words & " code words"
        lblDownloadBarFG.Width = lblDownloadBarBG.Width * (i / totalLen)
    
        'Here is where we rearrange data for direct sending
        '=============================================
        Text1.Text = fileContentLines(i)
        
        'Peel off ':'
        Text1 = Right(Text1, Len(Text1) - 1)
        
        'Peel off record length
        Record_Length = Hex_Convert(Left(Text1, 2))
        Text1 = Right(Text1, Len(Text1) - 2)
        'Catch a record length that is less than a multiple of 8
        Dim Length_Padding As Integer
        Length_Padding = 0
        Do While ((Record_Length + Length_Padding) Mod 8) <> 0
            Length_Padding = Length_Padding + 1
        Loop
        
        'Peel off memory address
        Memory_Address_High = Hex_Convert(Left(Text1, 2))
        Text1 = Right(Text1, Len(Text1) - 2)
        Memory_Address_Low = Hex_Convert(Left(Text1, 2))
        Text1 = Right(Text1, Len(Text1) - 2)
        
        'Divide Memory address by 2
        If (Memory_Address_High Mod 2) <> 0 Then Memory_Address_Low = Memory_Address_Low + 256
        Memory_Address_High = Memory_Address_High \ 2
        Memory_Address_Low = Memory_Address_Low \ 2
        
        'If this memory address is in the boot loader memory space, skip it!
        If Memory_Address_High = Bloader_Start_High Then
            If Memory_Address_Low >= Bloader_Start_Low - 8 Then
                
                If lblDownload.Caption <> "HEX File too long" Then
                    lblDownload.Caption = "HEX File too long"
                    MsgBox "This HEX File spills over into reserved Bloader space. You may experience unexpected program execution."
                    MsgBox "Stopping boot load!"
                    GoTo ErrExit
                    
                End If
                i = i + 1
                GoTo Hard_Coded_Next_line
                
            End If
        End If
        'If this memory address is in the config word space, skip it!
        If Memory_Address_High >= 32 Then '32 = 0x20 = 0x40 / 2
            i = i + 1
            GoTo Hard_Coded_Next_line
        End If
        
        '============================================================
        
        'Peel off and check for end of file tage
        End_Record = Hex_Convert(Left(Text1, 2))
        Text1 = Right(Text1, Len(Text1) - 2)
        If End_Record = 1 Then
            GoTo Last_Line
        End If
        
        'Calculate our OWN Checksum
        Check_Sum = Record_Length + Length_Padding + Memory_Address_High + Memory_Address_Low
        
        'Load program data into the outgoing_data array
        For j = 0 To (Record_Length / 2) - 1
            
            'Catch first byte and store it in second spot
            Outgoing_Data((j * 2) + 1) = Hex_Convert(Left(Text1, 2))
            Text1 = Right(Text1, Len(Text1) - 2)
            
            'Catch second byte and store it in first spot
            Outgoing_Data(j * 2) = Hex_Convert(Left(Text1, 2))
            Text1 = Right(Text1, Len(Text1) - 2)
            
            'This is the very special case of the boot vector
            'Memory location 0 should be a goto main
            'We will insert the bloader jump vector and shift the goto main to
            'memory location 4
            'If Memory_Address_High = 0 And Memory_Address_Low = 0 Then
                'Make sure the user has created the HEX file properly with
                'the required pragma origin statement
             '   If Record_Length <> 2 Then
             '       MsgBox "This HEX file does not appear to have the correct goto statement. Please double check that the proper 'origin 4' statement has been inserted at the beginning of the C program."
             '       GoTo ErrExit
             '   End If
                
                'Originally we broadcasted the entire boot vector to the PIC. This has since
                'been moved to the PIC so that the boot vector is ALWAYS written during
                'a boot load so that we don't run the risk of losing it.
                
                '6-10-04 While this protected boot vector worked great for the 16F88, it causes the 16F87xA series
                'to overwrite the boot vector... So now we need a chip select! Great...
                
             '   If PICType = "16F88" Then
                    'Repoint initial user's goto
             '       Outgoing_Data(6) = Outgoing_Data(0)
             '       Outgoing_Data(7) = Outgoing_Data(1)
                    '0x158A
             '       Outgoing_Data(0) = Hex_Convert("3F")
             '       Outgoing_Data(1) = Hex_Convert("FF")
                    '0x160A
             '       Outgoing_Data(2) = Hex_Convert("3F")
             '       Outgoing_Data(3) = Hex_Convert("FF")
                    '0x2F99
             '       Outgoing_Data(4) = Hex_Convert("3F")
             '       Outgoing_Data(5) = Hex_Convert("FF")
              '  ElseIf PICType = "16F877A" Or PICType = "16F876A" Or PICType = "16F873A" Then
                    'Repoint initial user's goto
              '      Outgoing_Data(6) = Outgoing_Data(0)
              '      Outgoing_Data(7) = Outgoing_Data(1)
                    '0x158A
              '      Outgoing_Data(0) = Hex_Convert("15")
              '      Outgoing_Data(1) = Hex_Convert("8A")
                    '0x160A
              '      Outgoing_Data(2) = Hex_Convert("16")
              '      Outgoing_Data(3) = Hex_Convert("0A")
                    '0x2F40
              '      Outgoing_Data(4) = Hex_Convert("2F")
              '      Outgoing_Data(5) = Hex_Convert("40")
               ' End If
                
                
                'Fix Checksum
              '  Check_Sum = Check_Sum + Outgoing_Data(2) + Outgoing_Data(3) + Outgoing_Data(4) + Outgoing_Data(5) + Outgoing_Data(6) + Outgoing_Data(7)
                'Reset padding
              '  Length_Padding = 0
              '  Record_Length = 8
           ' End If
            
            Check_Sum = Check_Sum + Outgoing_Data(j * 2)
            Check_Sum = Check_Sum + Outgoing_Data((j * 2) + 1)
        Next
        
        'Add padding if this hex line is less than a multiple of 4
        For j = (Record_Length / 2) To (Length_Padding / 2) + (Record_Length / 2) - 1
            
            'Add padding
            Outgoing_Data(j * 2) = 63 '3F
            Outgoing_Data((j * 2) + 1) = 255 'FF
            
            Check_Sum = Check_Sum + Outgoing_Data(j * 2)
            Check_Sum = Check_Sum + Outgoing_Data((j * 2) + 1)
        Next
        
        'Now reduce check_sum to 8 bits
        Do While Check_Sum >= 256
            Check_Sum = Check_Sum - 256
        Loop
        'Now take 2's compliment
        Check_Sum = 256 - Check_Sum
        'Catch the special case
        If Check_Sum = 256 Then Check_Sum = 0
        
        InfoLabel.Caption = "Sending start character"
        'InfoLabel.Refresh

        
        '=============================================
        'Send the start character
        temp_block = ":"
        
        'InfoLabel.Caption = "Sending record length"
        'InfoLabel.Refresh
        'Send the record length
        temp_block = temp_block & Chr(Record_Length + Length_Padding)
        
        'InfoLabel.Caption = "Sending block address"
        'InfoLabel.Refresh

        'Send this block's address
        temp_block = temp_block & Chr(Memory_Address_High)
        temp_block = temp_block & Chr(Memory_Address_Low)
        
        'InfoLabel.Caption = "Sending checksum"
        'InfoLabel.Refresh

        'Send this block's check sum
        temp_block = temp_block & Chr(Check_Sum)
        
        'InfoLabel.Caption = "Sending block size " & Record_Length + Length_Padding
        'InfoLabel.Refresh

        'Send the block
        'temp_block = ""
        For j = 0 To (Record_Length + Length_Padding) - 1
            'MSComm1.Output = Chr(Outgoing_Data(j))
            temp_block = temp_block & Chr(Outgoing_Data(j))
            'InfoLabel.Caption = "Sending block - char " & Chr(Outgoing_Data(j)) & " pos " & j
            'InfoLabel.Refresh
            
        Next
        MSComm1.Output = temp_block
        '=============================================
        InfoLabel.Caption = "Done sending block"
        'InfoLabel.Refresh
           
    Next
        
Last_Line:
    'Tell the PIC we are done transmitting data
    MSComm1.Output = ":"
    MSComm1.Output = "S"
    
    lblDownload.Caption = "Download Complete!"
    sptComplete.FillColor = &HFF00& 'Green
    sptConnect.FillColor = &HFF00& 'Green '&HFFFF& 'Yellow
        
    'Turn on the browse buttons
    cmdOpen(0).Enabled = True
    cmdOpen(1).Enabled = True
    cmdOpen(2).Enabled = True
    cmdDownload2(0).Enabled = True
    cmdDownload2(1).Enabled = True
    cmdDownload2(2).Enabled = True
    cmdBreak.Enabled = False
    
    'If port already opened then close it
    If MSComm1.PortOpen = True Then
        MSComm1.PortOpen = False
    End If

    If TermAutoLaunch Then TermWindow (True)
    
    Exit Sub

ErrHandler:
    If Err.Number = 8020 Or Err.Number = 8015 Then
        MsgBox "The PIC seems to be using the UART at a different baud rate. Please power down the PIC or hold it in reset before initiating download."
    ElseIf Err.Number = 8005 Then
        MsgBox "COM" & CommPort & " already open."
    Else
        MsgBox "Error, " & Err.Description, vbOKOnly
        MsgBox Err.Number
    End If

ErrExit:
    
    'If port already opened then close it
    If MSComm1.PortOpen = True Then
        MSComm1.PortOpen = False
    End If
    
    cmdDownload2(0).Enabled = True
    cmdDownload2(1).Enabled = True
    cmdDownload2(2).Enabled = True
    cmdOpen(0).Enabled = True
    cmdOpen(1).Enabled = True
    cmdOpen(2).Enabled = True
    cmdBreak.Enabled = False
    
    lblStatus.Caption = "Error"
    lblCurrentBuffer.Caption = "Buffer Empty"
    lblDownload.Caption = "Idle"
        
End Sub

Private Sub cmdOpen_Click(Index As Integer)

On Error GoTo Error_Handler

    'Open a file dialog and let the user select a HEX file
    cd1.Filter = "Hex Files (*.HEX)|*.HEX| All Files (*.*)|*.*"
    cd1.FilterIndex = 1
    cd1.DialogTitle = "Select a file "
    
    If InStrRev(txtUserFile(Index), "\") > 0 Then
        ChDir (Left(txtUserFile(Index), InStrRev(txtUserFile(Index), "\")))
    Else
        ChDir (CurDir)
    End If
        
    cd1.InitDir = CurDir
    cd1.ShowOpen
    If cd1.FileName <> "" Then
        txtFile(Index).Text = cd1.FileName
        txtUserFile(Index) = cd1.FileName
    End If
    
    Exit Sub
    
Error_Handler:
    MsgBox Err.Number & " : " & Err.Description
    
End Sub

Private Sub cmdOpenTerminal_Click()
    TermWindow True
End Sub

Private Sub File_Close_Click()
    Unload Me
End Sub

Private Sub SetDefaults()
    cboCommPort.ListIndex = 0
    cboBaud.ListIndex = 0
    cboOscillator.ListIndex = 1
    cboChip.ListIndex = 0
    
    cboTermSpeed.ListIndex = 3
    cboTermDataBits.ListIndex = 1
    cboTermParity.ListIndex = 2
    cboTermStopBits.ListIndex = 0
    optAsc.Value = True
    
    chkLaunchTerm.Value = 1

End Sub

Private Sub Form_Load()

On Error GoTo EH

    lblDownloadBarFG.Width = 0
    
    'Read in the settings file - if available
    Open App.Path & "\settings.txt" For Input As #1
        Input #1, CommPort, CommSpeed, PICSpeed, PICType, txtUserFile(0), txtUserFile(1), txtUserFile(2), _
                TermBaud, TermDataBits, TermParity, TermStopBits, TermType, TermEcho, TermAutoLaunch
    Close #1
    
    txtFile(0).Text = txtUserFile(0)
    txtFile(1).Text = txtUserFile(1)
    txtFile(2).Text = txtUserFile(2)
    
    txtComPort.Text = CommPort
    'cboCommPort.ListIndex = CommPort - 1
    
    If CommSpeed = 9600 Then cboBaud.ListIndex = 0
    If CommSpeed = 19200 Then cboBaud.ListIndex = 1
    If CommSpeed = 38400 Then cboBaud.ListIndex = 2
    If CommSpeed = 57600 Then cboBaud.ListIndex = 3
    If CommSpeed = 115200 Then cboBaud.ListIndex = 4
    
    If PICSpeed = 4 Then cboOscillator.ListIndex = 0
    If PICSpeed = 8 Then cboOscillator.ListIndex = 1
    If PICSpeed = 9 Then cboOscillator.ListIndex = 2
    If PICSpeed = 10 Then cboOscillator.ListIndex = 3
    If PICSpeed = 12 Then cboOscillator.ListIndex = 4
    If PICSpeed = 20 Then cboOscillator.ListIndex = 5
    
    If PICType = "16F873A" Then cboChip.ListIndex = 0
    If PICType = "16F876A" Then cboChip.ListIndex = 1
    If PICType = "16F877A" Then cboChip.ListIndex = 2
    If PICType = "16F88" Then cboChip.ListIndex = 3
        
    If TermBaud = 1200 Then
        cboTermSpeed.ListIndex = 0
    ElseIf TermBaud = 2400 Then
        cboTermSpeed.ListIndex = 1
    ElseIf TermBaud = 4800 Then
        cboTermSpeed.ListIndex = 2
    ElseIf TermBaud = 19200 Then
        cboTermSpeed.ListIndex = 4
    ElseIf TermBaud = 28800 Then
        cboTermSpeed.ListIndex = 5
    ElseIf TermBaud = 38400 Then
        cboTermSpeed.ListIndex = 6
    ElseIf TermBaud = 57600 Then
        cboTermSpeed.ListIndex = 7
    ElseIf TermBaud = 115200 Then
        cboTermSpeed.ListIndex = 8
    Else
        cboTermSpeed.ListIndex = 3
    End If
    
    If TermDataBits = 7 Then
        cboTermDataBits.ListIndex = 0
    Else
        cboTermDataBits.ListIndex = 1
    End If
    
    If TermParity = "O" Then
        cboTermParity.ListIndex = 0
    ElseIf TermParity = "E" Then
        cboTermParity.ListIndex = 1
    Else
        cboTermParity.ListIndex = 2
    End If
    
    If TermStopBits = 1.5 Then
        cboTermStopBits.ListIndex = 1
    ElseIf TermStopBits = 2 Then
        cboTermStopBits.ListIndex = 2
    Else
        cboTermStopBits.ListIndex = 0
    End If
    
    If TermType = "hex" Then
        optHex.Value = True
    ElseIf TermType = "dec" Then
        optDec.Value = True
    Else
        optAsc.Value = True
    End If
    
    If TermEcho Then chkTermEcho.Value = 1
    
    If TermAutoLaunch Then chkLaunchTerm.Value = 1
    
    Exit Sub

EH:
    
    If Err.Number = 53 Then 'No File found
        'Default values
        SetDefaults
    ElseIf Err.Number = 62 Then
        'Problem with the file, close it and set default options
        Close #1
        'Default values
        SetDefaults
    Else
        MsgBox Err.Number & " = " & Err.Description
    End If
    
End Sub

Private Sub Form_Unload(Cancel As Integer)

On Error GoTo EH
    
    If TermMode And MSComm1.PortOpen Then MSComm1.PortOpen = False
    
    Open App.Path & "\settings.txt" For Output As #1
        Write #1, CommPort, CommSpeed, PICSpeed, PICType, txtUserFile(0), txtUserFile(1), txtUserFile(2), _
                TermBaud, TermDataBits, TermParity, TermStopBits, TermType, TermEcho, TermAutoLaunch
    Close #1
    
    Exit Sub

EH:
    MsgBox Err.Number & " = " & Err.Description
    
End Sub

Private Function StringToHex(s As String) As String
        Dim i As Integer
        Dim j As Integer
        Dim result As String
        
    For i = 1 To Len(s)
        j = Asc(Mid(s, i, 1))
        result = result & " " & IIf(j < 17, "0" & Hex(j), Hex(j))
    Next i
    
    StringToHex = result
    
End Function

Private Function StringToDec(s As String) As String
        Dim result As String
        
    For i = 1 To Len(s)
        result = result & " " & Format(Asc(Mid(s, i, 1)), "000")
    Next i
    
    StringToDec = result
    
End Function

'Private Sub MSComm1_OnComm()
'        Dim buffer As String
'
'    If MSComm1.PortOpen = False Then Exit Sub
'
'    If TermMode Then
'        Select Case MSComm1.CommEvent
'            Case comEvReceive
'                    buffer = MSComm1.Input
'                    Select Case TermType
'                        Case "asc"
'
'                        Case "dec"
'                            buffer = StringToDec(buffer)
'                        Case "hex"
'                            buffer = StringToHex(buffer)
'                        Case Else
'
'                    End Select
'
'                    With txtTerm
'                        .SelStart = Len(txtTerm.Text)
'                        .SelText = buffer
'                        .SelStart = Len(txtTerm.Text)
'                    End With
'        End Select
'    End If
'
'End Sub

Private Sub GetSerial()
        Dim buffer As String
    
    If MSComm1.PortOpen = False Then Exit Sub
    
    If TermMode Then
        If MSComm1.InBufferCount > 0 Then
            buffer = MSComm1.Input
            Select Case TermType
                Case "asc"
                
                Case "dec"
                    buffer = StringToDec(buffer)
                Case "hex"
                    buffer = StringToHex(buffer)
                Case Else
                
            End Select
            
            With txtTerm
                If Len(.Text) > 15000 Then
                    .Text = Right(.Text, 10000)
                End If
                .SelStart = Len(.Text)
                .SelText = buffer
                .SelStart = Len(.Text)
            End With
        End If
    End If
    
End Sub

Private Sub optAsc_Click()
    TermType = "asc"
    If TermMode Then txtTerm.SetFocus
End Sub

Private Sub optDec_Click()
    TermType = "dec"
    If TermMode Then txtTerm.SetFocus
End Sub

Private Sub optHex_Click()
    TermType = "hex"
    If TermMode Then txtTerm.SetFocus
End Sub

Private Sub Timer1_Timer()
    DoEvents
    GetSerial
    DoEvents
End Sub

Private Sub txtComPort_Change()
    
On Error GoTo Error_Handler
    
    If CInt(txtComPort.Text) > 99 Then txtComPort.Text = "99"

    CommPort = CInt(txtComPort.Text)
    
    Exit Sub
    
Error_Handler:
    MsgBox "Comport number must be a number from 1 to 99."
    txtComPort.Text = "1"

End Sub

Private Sub txtFile_Change(Index As Integer)
    txtUserFile(Index) = txtFile(Index).Text
End Sub

Private Sub txtTerm_KeyPress(KeyAscii As Integer)
    If MSComm1.PortOpen Then MSComm1.Output = Chr(KeyAscii)
    If Not TermEcho Then KeyAscii = 0
End Sub
