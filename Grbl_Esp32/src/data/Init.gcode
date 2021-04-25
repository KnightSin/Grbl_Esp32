;STA
$Sta/SSID=XBoardRouter
$Sta/Password=497838180
$Sta/IPMode=static
$Sta/IP=192.168.0.127
$Sta/Gateway=192.168.0.1
$Sta/Netmask=255.255.255.0
$Radio/Mode=STA 

;AP
$AP/SSID=GRBL_ESP
$AP/Password=12345678
$AP/IP=192.168.0.1
$AP/Channel=1
;$Radio/Mode=AP

;spindle
$Spindle/EM/Invert=on
$Spindle/Type=SERVO

;motor
$X/StepsPerMm=80.000
$Y/StepsPerMm=80.000
$Z/StepsPerMm=80.000
$X/MaxRate=5000.000
$Y/MaxRate=5000.000
$Z/MaxRate=5000.000
$X/Acceleration=2000.000
$Y/Acceleration=2000.000
$Z/Acceleration=2000.000

;start line 
$GCode/Line1=M5
$GCode/Line0=M3S1000

;homing
$Limits/Soft=Off
$Limits/Hard=Off
$Homing/Cycle0=XY






























































