EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 7
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text HLabel 5125 2750 1    50   Input ~ 0
VCC
Text HLabel 5975 3450 3    50   Input ~ 0
C
Text HLabel 6075 2850 1    50   Input ~ 0
NO
Text HLabel 5875 2850 1    50   Input ~ 0
NC
Text HLabel 5500 4275 3    50   Input ~ 0
GND
Text HLabel 4600 3750 0    50   Input ~ 0
RELAY
$Comp
L Relay:HF3-03 K?
U 1 1 612F2F1E
P 5775 3150
AR Path="/612F2F1E" Ref="K?"  Part="1" 
AR Path="/612E376C/612F2F1E" Ref="K?"  Part="1" 
AR Path="/61320739/612F2F1E" Ref="K?"  Part="1" 
AR Path="/61324198/612F2F1E" Ref="K?"  Part="1" 
AR Path="/6134001B/612F2F1E" Ref="K?"  Part="1" 
AR Path="/61340045/612F2F1E" Ref="K?"  Part="1" 
AR Path="/6134006F/612F2F1E" Ref="K?"  Part="1" 
F 0 "K?" V 5208 3150 50  0000 C CNN
F 1 "HF3-03" V 5299 3150 50  0000 C CNN
F 2 "Relay_SMD:Relay_SPDT_AXICOM_HF3Series_75ohms_Pitch1.27mm" H 6905 3120 50  0001 C CNN
F 3 "http://hiqsdr.com/images/3/3e/Axicom-HF3.pdf" H 5775 3150 50  0001 C CNN
	1    5775 3150
	1    0    0    -1  
$EndComp
$Comp
L Device:LED_Small D?
U 1 1 612F2F24
P 4800 3225
AR Path="/612F2F24" Ref="D?"  Part="1" 
AR Path="/612E376C/612F2F24" Ref="D?"  Part="1" 
AR Path="/61320739/612F2F24" Ref="D?"  Part="1" 
AR Path="/61324198/612F2F24" Ref="D?"  Part="1" 
AR Path="/6134001B/612F2F24" Ref="D?"  Part="1" 
AR Path="/61340045/612F2F24" Ref="D?"  Part="1" 
AR Path="/6134006F/612F2F24" Ref="D?"  Part="1" 
F 0 "D?" H 4800 3460 50  0000 C CNN
F 1 "C1" H 4800 3369 50  0000 C CNN
F 2 "" V 4800 3225 50  0001 C CNN
F 3 "~" V 4800 3225 50  0001 C CNN
	1    4800 3225
	0    -1   -1   0   
$EndComp
$Comp
L Device:R_Small R?
U 1 1 612F2F2A
P 4800 3025
AR Path="/612F2F2A" Ref="R?"  Part="1" 
AR Path="/612E376C/612F2F2A" Ref="R?"  Part="1" 
AR Path="/61320739/612F2F2A" Ref="R?"  Part="1" 
AR Path="/61324198/612F2F2A" Ref="R?"  Part="1" 
AR Path="/6134001B/612F2F2A" Ref="R?"  Part="1" 
AR Path="/61340045/612F2F2A" Ref="R?"  Part="1" 
AR Path="/6134006F/612F2F2A" Ref="R?"  Part="1" 
F 0 "R?" V 4604 3025 50  0000 C CNN
F 1 "R_Small" V 4695 3025 50  0000 C CNN
F 2 "" H 4800 3025 50  0001 C CNN
F 3 "~" H 4800 3025 50  0001 C CNN
	1    4800 3025
	1    0    0    -1  
$EndComp
$Comp
L Device:D_Small D?
U 1 1 612F2F30
P 5050 3200
AR Path="/612F2F30" Ref="D?"  Part="1" 
AR Path="/612E376C/612F2F30" Ref="D?"  Part="1" 
AR Path="/61320739/612F2F30" Ref="D?"  Part="1" 
AR Path="/61324198/612F2F30" Ref="D?"  Part="1" 
AR Path="/6134001B/612F2F30" Ref="D?"  Part="1" 
AR Path="/61340045/612F2F30" Ref="D?"  Part="1" 
AR Path="/6134006F/612F2F30" Ref="D?"  Part="1" 
F 0 "D?" H 5050 2993 50  0000 C CNN
F 1 "D_Small" H 5050 3084 50  0000 C CNN
F 2 "" V 5050 3200 50  0001 C CNN
F 3 "~" V 5050 3200 50  0001 C CNN
	1    5050 3200
	0    1    1    0   
$EndComp
$Comp
L Device:Q_NPN_BCE Q?
U 1 1 612F2F36
P 5100 3750
AR Path="/612F2F36" Ref="Q?"  Part="1" 
AR Path="/612E376C/612F2F36" Ref="Q?"  Part="1" 
AR Path="/61320739/612F2F36" Ref="Q?"  Part="1" 
AR Path="/61324198/612F2F36" Ref="Q?"  Part="1" 
AR Path="/6134001B/612F2F36" Ref="Q?"  Part="1" 
AR Path="/61340045/612F2F36" Ref="Q?"  Part="1" 
AR Path="/6134006F/612F2F36" Ref="Q?"  Part="1" 
F 0 "Q?" H 5291 3796 50  0000 L CNN
F 1 "Q_NPN_BCE" H 5291 3705 50  0000 L CNN
F 2 "" H 5300 3850 50  0001 C CNN
F 3 "~" H 5100 3750 50  0001 C CNN
	1    5100 3750
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R?
U 1 1 612F2F3C
P 4800 3750
AR Path="/612F2F3C" Ref="R?"  Part="1" 
AR Path="/612E376C/612F2F3C" Ref="R?"  Part="1" 
AR Path="/61320739/612F2F3C" Ref="R?"  Part="1" 
AR Path="/61324198/612F2F3C" Ref="R?"  Part="1" 
AR Path="/6134001B/612F2F3C" Ref="R?"  Part="1" 
AR Path="/61340045/612F2F3C" Ref="R?"  Part="1" 
AR Path="/6134006F/612F2F3C" Ref="R?"  Part="1" 
F 0 "R?" V 4604 3750 50  0000 C CNN
F 1 "R_Small" V 4695 3750 50  0000 C CNN
F 2 "" H 4800 3750 50  0001 C CNN
F 3 "~" H 4800 3750 50  0001 C CNN
	1    4800 3750
	0    1    1    0   
$EndComp
Wire Wire Line
	4700 3750 4600 3750
$EndSCHEMATC
