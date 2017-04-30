;******************************************************************************
;* TMS320C6x C/C++ Codegen                                          PC v5.3.0 *
;* Date/Time created: Mon Feb 20 11:47:30 2006                                *
;******************************************************************************
	.compiler_opts --endian=little --hll_source=on --mem_model:code=near --mem_model:data=far_aggregates --silicon_version=6800 --symdebug:dwarf 

;******************************************************************************
;* GLOBAL FILE PARAMETERS                                                     *
;*                                                                            *
;*   Architecture      : TMS320C67xx                                          *
;*   Optimization      : Disabled                                             *
;*   Optimizing for    : Compile time, Ease of Development                    *
;*                       Based on options: no -o, no -ms                      *
;*   Endian            : Little                                               *
;*   Interrupt Thrshld : Disabled                                             *
;*   Data Access Model : Far Aggregate Data                                   *
;*   Pipelining        : Disabled                                             *
;*   Memory Aliases    : Presume are aliases (pessimistic)                    *
;*   Debug Info        : DWARF Debug                                          *
;*                                                                            *
;******************************************************************************

	.asg	A15, FP
	.asg	B14, DP
	.asg	B15, SP
	.global	$bss


$C$DW$CU	.dwtag  DW_TAG_compile_unit
	.dwattr $C$DW$CU, DW_AT_name("board_padk.c")
	.dwattr $C$DW$CU, DW_AT_producer("TMS320C6x C/C++ Codegen PC v5.3.0 Copyright (c) 1996-2005 Texas Instruments Incorporated")
	.dwattr $C$DW$CU, DW_AT_stmt_list(0x00)
	.dwattr $C$DW$CU, DW_AT_TI_version(0x01)

$C$DW$1	.dwtag  DW_TAG_subprogram, DW_AT_name("ADC_Init")
	.dwattr $C$DW$1, DW_AT_TI_symbol_name("_ADC_Init")
	.dwattr $C$DW$1, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$1, DW_AT_declaration(0x01)
	.dwattr $C$DW$1, DW_AT_external(0x01)
$C$DW$2	.dwtag  DW_TAG_formal_parameter, DW_AT_type(*$C$DW$T$41)
	.dwendtag $C$DW$1


$C$DW$3	.dwtag  DW_TAG_subprogram, DW_AT_name("DAC_Init")
	.dwattr $C$DW$3, DW_AT_TI_symbol_name("_DAC_Init")
	.dwattr $C$DW$3, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$3, DW_AT_declaration(0x01)
	.dwattr $C$DW$3, DW_AT_external(0x01)
$C$DW$4	.dwtag  DW_TAG_formal_parameter, DW_AT_type(*$C$DW$T$44)
	.dwendtag $C$DW$3


$C$DW$5	.dwtag  DW_TAG_subprogram, DW_AT_name("DAC_RelayUnmute")
	.dwattr $C$DW$5, DW_AT_TI_symbol_name("_DAC_RelayUnmute")
	.dwattr $C$DW$5, DW_AT_declaration(0x01)
	.dwattr $C$DW$5, DW_AT_external(0x01)

$C$DW$6	.dwtag  DW_TAG_subprogram, DW_AT_name("DAC_RelayMute")
	.dwattr $C$DW$6, DW_AT_TI_symbol_name("_DAC_RelayMute")
	.dwattr $C$DW$6, DW_AT_declaration(0x01)
	.dwattr $C$DW$6, DW_AT_external(0x01)

$C$DW$7	.dwtag  DW_TAG_subprogram, DW_AT_name("CLKGEN_Init")
	.dwattr $C$DW$7, DW_AT_TI_symbol_name("_CLKGEN_Init")
	.dwattr $C$DW$7, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$7, DW_AT_declaration(0x01)
	.dwattr $C$DW$7, DW_AT_external(0x01)
$C$DW$8	.dwtag  DW_TAG_formal_parameter, DW_AT_type(*$C$DW$T$47)
	.dwendtag $C$DW$7


$C$DW$9	.dwtag  DW_TAG_subprogram, DW_AT_name("MEM_valloc")
	.dwattr $C$DW$9, DW_AT_TI_symbol_name("_MEM_valloc")
	.dwattr $C$DW$9, DW_AT_type(*$C$DW$T$33)
	.dwattr $C$DW$9, DW_AT_declaration(0x01)
	.dwattr $C$DW$9, DW_AT_external(0x01)
$C$DW$10	.dwtag  DW_TAG_formal_parameter, DW_AT_type(*$C$DW$T$26)
$C$DW$11	.dwtag  DW_TAG_formal_parameter, DW_AT_type(*$C$DW$T$34)
$C$DW$12	.dwtag  DW_TAG_formal_parameter, DW_AT_type(*$C$DW$T$34)
$C$DW$13	.dwtag  DW_TAG_formal_parameter, DW_AT_type(*$C$DW$T$36)
	.dwendtag $C$DW$9

;	C:\CCStudio_v3.1\C6000\cgtools_530\bin\acp6x.exe -@E:\DOCUME~1\FRANCO~1.LAN\LOCALS~1\Temp\TI04013 
	.sect	".text"
	.global	_PADK_board_control

$C$DW$14	.dwtag  DW_TAG_subprogram, DW_AT_name("PADK_board_control")
	.dwattr $C$DW$14, DW_AT_low_pc(_PADK_board_control)
	.dwattr $C$DW$14, DW_AT_high_pc(0x00)
	.dwattr $C$DW$14, DW_AT_TI_symbol_name("_PADK_board_control")
	.dwattr $C$DW$14, DW_AT_TI_begin_file("board_padk.c")
	.dwattr $C$DW$14, DW_AT_TI_begin_line(0x1e)
	.dwattr $C$DW$14, DW_AT_TI_begin_column(0x07)
	.dwpsn	"board_padk.c",31,1

	.dwfde DW$CIE

;******************************************************************************
;* FUNCTION NAME: PADK_board_control                                          *
;*                                                                            *
;*   Regs Modified     : A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,B0,B1,B2,B3,B4,B5,B6,  *
;*                           B7,B8,B9,SP,A16,A17,A18,A19,A20,A21,A22,A23,A24, *
;*                           A25,A26,A27,A28,A29,A30,A31,B16,B17,B18,B19,B20, *
;*                           B21,B22,B23,B24,B25,B26,B27,B28,B29,B30,B31      *
;*   Regs Used         : A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,B0,B1,B2,B3,B4,B5,B6,  *
;*                           B7,B8,B9,SP,A16,A17,A18,A19,A20,A21,A22,A23,A24, *
;*                           A25,A26,A27,A28,A29,A30,A31,B16,B17,B18,B19,B20, *
;*                           B21,B22,B23,B24,B25,B26,B27,B28,B29,B30,B31      *
;*   Local Frame Size  : 0 Args + 16 Auto + 4 Save = 20 byte                  *
;******************************************************************************
_PADK_board_control:
;** --------------------------------------------------------------------------*
	.dwcfa	0x0e, 0
	.dwcfa	0x09, 128, 19
           STW     .D2T2   B3,*SP--(24)      ; |31| 
           NOP             2
	.dwcfa	0x0e, 24
	.dwcfa	0x80, 19, 0
$C$DW$15	.dwtag  DW_TAG_formal_parameter, DW_AT_name("hPADK")
	.dwattr $C$DW$15, DW_AT_TI_symbol_name("_hPADK")
	.dwattr $C$DW$15, DW_AT_type(*$C$DW$T$52)
	.dwattr $C$DW$15, DW_AT_location[DW_OP_reg4]
$C$DW$16	.dwtag  DW_TAG_formal_parameter, DW_AT_name("cmd")
	.dwattr $C$DW$16, DW_AT_TI_symbol_name("_cmd")
	.dwattr $C$DW$16, DW_AT_type(*$C$DW$T$53)
	.dwattr $C$DW$16, DW_AT_location[DW_OP_reg20]
$C$DW$17	.dwtag  DW_TAG_variable, DW_AT_name("hPADK")
	.dwattr $C$DW$17, DW_AT_TI_symbol_name("_hPADK")
	.dwattr $C$DW$17, DW_AT_type(*$C$DW$T$52)
	.dwattr $C$DW$17, DW_AT_location[DW_OP_breg31 4]
$C$DW$18	.dwtag  DW_TAG_variable, DW_AT_name("cmd")
	.dwattr $C$DW$18, DW_AT_TI_symbol_name("_cmd")
	.dwattr $C$DW$18, DW_AT_type(*$C$DW$T$53)
	.dwattr $C$DW$18, DW_AT_location[DW_OP_breg31 8]
$C$DW$19	.dwtag  DW_TAG_variable, DW_AT_name("pBrdCfg")
	.dwattr $C$DW$19, DW_AT_TI_symbol_name("_pBrdCfg")
	.dwattr $C$DW$19, DW_AT_type(*$C$DW$T$58)
	.dwattr $C$DW$19, DW_AT_location[DW_OP_breg31 12]
$C$DW$20	.dwtag  DW_TAG_variable, DW_AT_name("status")
	.dwattr $C$DW$20, DW_AT_TI_symbol_name("_status")
	.dwattr $C$DW$20, DW_AT_type(*$C$DW$T$50)
	.dwattr $C$DW$20, DW_AT_location[DW_OP_breg31 16]
           STW     .D2T2   B4,*+SP(8)        ; |31| 
           STW     .D2T1   A4,*+SP(4)        ; |31| 
           NOP             2
	.dwpsn	"board_padk.c",41,5
           ZERO    .L2     B4                ; |41| 
           STW     .D2T2   B4,*+SP(16)       ; |41| 
           NOP             2
	.dwpsn	"board_padk.c",42,5
           MV      .L2X    A4,B4
           STW     .D2T2   B4,*+SP(12)       ; |42| 
           NOP             2
	.dwpsn	"board_padk.c",44,5
           B       .S1     $C$L6             ; |44| 
           NOP             5
           ; BRANCH OCCURS {$C$L6}           ; |44| 
;** --------------------------------------------------------------------------*
;** --------------------------------------------------------------------------*
$C$L1:    
	.dwpsn	"board_padk.c",47,9
           MV      .L1     A4,A3
           LDW     .D1T1   *+A3(4),A3        ; |47| 
           ZERO    .L1     A4                ; |47| 
           NOP             3
           STB     .D1T1   A4,*+A3(12)       ; |47| 
           NOP             2
	.dwpsn	"board_padk.c",48,9
           LDW     .D2T1   *+SP(12),A3       ; |48| 
           NOP             4
           LDW     .D1T1   *+A3(4),A3        ; |48| 
           NOP             4
           STB     .D1T1   A4,*+A3(13)       ; |48| 
           NOP             2
	.dwpsn	"board_padk.c",49,9
           LDW     .D2T2   *+SP(12),B4       ; |49| 
           MVKL    .S2     _ADC_Init,B5
           MVKH    .S2     _ADC_Init,B5
           MVKL    .S2     $C$RL0,B3         ; |49| 
           MVKH    .S2     $C$RL0,B3         ; |49| 

           LDW     .D2T2   *+B4(4),B4        ; |49| 
||         CALL    .S2     B5                ; |49| 

           NOP             4
           ADD     .L1X    12,B4,A4          ; |49| 
$C$RL0:    ; CALL OCCURS {B5}                ; |49| 
	.dwpsn	"board_padk.c",50,14
           B       .S1     $C$L7             ; |50| 
           NOP             5
           ; BRANCH OCCURS {$C$L7}           ; |50| 
;** --------------------------------------------------------------------------*
$C$L2:    
	.dwpsn	"board_padk.c",53,9
           MV      .L1     A4,A3
           LDW     .D1T2   *+A3(4),B4        ; |53| 
           ZERO    .L1     A31               ; |53| 
           NOP             3
           STB     .D2T1   A31,*+B4(20)      ; |53| 
           NOP             2
	.dwpsn	"board_padk.c",54,9
           LDW     .D2T1   *+SP(12),A3       ; |54| 
           ZERO    .L1     A30               ; |54| 
           NOP             3
           LDW     .D1T2   *+A3(4),B4        ; |54| 
           NOP             4
           STB     .D2T1   A30,*+B4(21)      ; |54| 
           NOP             2
	.dwpsn	"board_padk.c",55,9
           LDW     .D2T1   *+SP(12),A3       ; |55| 
           ZERO    .L2     B5                ; |55| 
           NOP             3
           LDW     .D1T2   *+A3(4),B4        ; |55| 
           NOP             4
           STB     .D2T2   B5,*+B4(22)       ; |55| 
           NOP             2
	.dwpsn	"board_padk.c",56,9
           LDW     .D2T1   *+SP(12),A3       ; |56| 
           NOP             4
           LDW     .D1T2   *+A3(4),B4        ; |56| 
           NOP             4
           STW     .D2T2   B5,*+B4(16)       ; |56| 
           NOP             2
	.dwpsn	"board_padk.c",57,9
           LDW     .D2T2   *+SP(12),B4       ; |57| 
           MVKL    .S1     _DAC_Init,A5
           MVKH    .S1     _DAC_Init,A5
           MVKL    .S2     $C$RL1,B3         ; |57| 
           MVKH    .S2     $C$RL1,B3         ; |57| 

           LDW     .D2T1   *+B4(4),A3        ; |57| 
||         CALL    .S2X    A5                ; |57| 

           NOP             4
           ADD     .D1     A3,16,A4          ; |57| 
$C$RL1:    ; CALL OCCURS {A5}                ; |57| 
	.dwpsn	"board_padk.c",58,14
           B       .S1     $C$L7             ; |58| 
           NOP             5
           ; BRANCH OCCURS {$C$L7}           ; |58| 
;** --------------------------------------------------------------------------*
$C$L3:    
	.dwpsn	"board_padk.c",61,9
           MVKL    .S2     _DAC_RelayMute,B4
           MVKH    .S2     _DAC_RelayMute,B4
           CALL    .S2     B4                ; |61| 
           MVKL    .S2     $C$RL2,B3         ; |61| 
           MVKH    .S2     $C$RL2,B3         ; |61| 
           NOP             3
$C$RL2:    ; CALL OCCURS {B4}                ; |61| 
	.dwpsn	"board_padk.c",62,14
           B       .S1     $C$L7             ; |62| 
           NOP             5
           ; BRANCH OCCURS {$C$L7}           ; |62| 
;** --------------------------------------------------------------------------*
$C$L4:    
	.dwpsn	"board_padk.c",65,9
           MVKL    .S2     _DAC_RelayUnmute,B4
           MVKH    .S2     _DAC_RelayUnmute,B4
           CALL    .S2     B4                ; |65| 
           MVKL    .S2     $C$RL3,B3         ; |65| 
           MVKH    .S2     $C$RL3,B3         ; |65| 
           NOP             3
$C$RL3:    ; CALL OCCURS {B4}                ; |65| 
	.dwpsn	"board_padk.c",66,14
           B       .S1     $C$L7             ; |66| 
           NOP             5
           ; BRANCH OCCURS {$C$L7}           ; |66| 
;** --------------------------------------------------------------------------*
$C$L5:    
	.dwpsn	"board_padk.c",69,9
           MVK     .S2     -1,B4             ; |69| 
           STW     .D2T2   B4,*+SP(16)       ; |69| 
           NOP             2
	.dwpsn	"board_padk.c",70,14
           B       .S1     $C$L7             ; |70| 
           NOP             5
           ; BRANCH OCCURS {$C$L7}           ; |70| 
;** --------------------------------------------------------------------------*
$C$L6:    
	.dwpsn	"board_padk.c",44,5
           LDW     .D2T2   *+SP(8),B4        ; |44| 
           NOP             4
           SUB     .L2     B4,1,B5           ; |44| 
           CMPGTU  .L2     B5,3,B0           ; |44| 
   [ B0]   B       .S1     $C$L5             ; |44| 
           NOP             5
           ; BRANCHCC OCCURS {$C$L5}         ; |44| 
;** --------------------------------------------------------------------------*

           MVKL    .S1     $C$SW1,A3
||         SHL     .S2     B4,2,B4           ; |44| 

           MVKH    .S1     $C$SW1,A3
||         SUB     .L2     B4,4,B4           ; |44| 

           ADD     .L1X    A3,B4,A3          ; |44| 
           LDW     .D1T1   *A3,A3            ; |44| 
           NOP             4
           B       .S2X    A3                ; |44| 
           NOP             5
           ; BRANCH OCCURS {A3}              ; |44| 
	.sect	".switch"
$C$SW1:	.word	$C$L1	; 1
	.word	$C$L2	; 2
	.word	$C$L3	; 3
	.word	$C$L4	; 4
	.sect	".text"
;** --------------------------------------------------------------------------*
$C$L7:    
	.dwpsn	"board_padk.c",73,5
           LDW     .D2T1   *+SP(16),A4       ; |73| 
           NOP             4
	.dwpsn	"board_padk.c",74,1
           LDW     .D2T2   *++SP(24),B3      ; |74| 
           NOP             4
	.dwcfa	0x0e, 0
	.dwcfa	0xc0, 19
	.dwcfa	0x0e, 0
           RET     .S2     B3                ; |74| 
           NOP             5
           ; BRANCH OCCURS {B3}              ; |74| 
	.dwattr $C$DW$14, DW_AT_TI_end_file("board_padk.c")
	.dwattr $C$DW$14, DW_AT_TI_end_line(0x4a)
	.dwattr $C$DW$14, DW_AT_TI_end_column(0x01)
	.dwendentry
	.dwendtag $C$DW$14

	.sect	".text"
	.global	_PADK_board_open

$C$DW$21	.dwtag  DW_TAG_subprogram, DW_AT_name("PADK_board_open")
	.dwattr $C$DW$21, DW_AT_low_pc(_PADK_board_open)
	.dwattr $C$DW$21, DW_AT_high_pc(0x00)
	.dwattr $C$DW$21, DW_AT_TI_symbol_name("_PADK_board_open")
	.dwattr $C$DW$21, DW_AT_TI_begin_file("board_padk.c")
	.dwattr $C$DW$21, DW_AT_TI_begin_line(0x55)
	.dwattr $C$DW$21, DW_AT_TI_begin_column(0x0d)
	.dwpsn	"board_padk.c",86,1

	.dwfde DW$CIE

;******************************************************************************
;* FUNCTION NAME: PADK_board_open                                             *
;*                                                                            *
;*   Regs Modified     : A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,B0,B1,B2,B3,B4,B5,B6,  *
;*                           B7,B8,B9,SP,A16,A17,A18,A19,A20,A21,A22,A23,A24, *
;*                           A25,A26,A27,A28,A29,A30,A31,B16,B17,B18,B19,B20, *
;*                           B21,B22,B23,B24,B25,B26,B27,B28,B29,B30,B31      *
;*   Regs Used         : A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,B0,B1,B2,B3,B4,B5,B6,  *
;*                           B7,B8,B9,SP,A16,A17,A18,A19,A20,A21,A22,A23,A24, *
;*                           A25,A26,A27,A28,A29,A30,A31,B16,B17,B18,B19,B20, *
;*                           B21,B22,B23,B24,B25,B26,B27,B28,B29,B30,B31      *
;*   Local Frame Size  : 0 Args + 8 Auto + 4 Save = 12 byte                   *
;******************************************************************************
_PADK_board_open:
;** --------------------------------------------------------------------------*
	.dwcfa	0x0e, 0
	.dwcfa	0x09, 128, 19
           STW     .D2T2   B3,*SP--(16)      ; |86| 
           NOP             2
	.dwcfa	0x0e, 16
	.dwcfa	0x80, 19, 0
$C$DW$22	.dwtag  DW_TAG_formal_parameter, DW_AT_name("segid")
	.dwattr $C$DW$22, DW_AT_TI_symbol_name("_segid")
	.dwattr $C$DW$22, DW_AT_type(*$C$DW$T$50)
	.dwattr $C$DW$22, DW_AT_location[DW_OP_reg4]
$C$DW$23	.dwtag  DW_TAG_variable, DW_AT_name("segid")
	.dwattr $C$DW$23, DW_AT_TI_symbol_name("_segid")
	.dwattr $C$DW$23, DW_AT_type(*$C$DW$T$50)
	.dwattr $C$DW$23, DW_AT_location[DW_OP_breg31 4]
$C$DW$24	.dwtag  DW_TAG_variable, DW_AT_name("pBrdCfg")
	.dwattr $C$DW$24, DW_AT_TI_symbol_name("_pBrdCfg")
	.dwattr $C$DW$24, DW_AT_type(*$C$DW$T$58)
	.dwattr $C$DW$24, DW_AT_location[DW_OP_breg31 8]
           STW     .D2T1   A4,*+SP(4)        ; |86| 
           NOP             2
	.dwpsn	"board_padk.c",91,5
           CALL    .S1     _MEM_valloc       ; |91| 
           MVKL    .S2     $C$RL4,B3         ; |91| 
           MVK     .S2     0x8,B4            ; |91| 
           MVKH    .S2     $C$RL4,B3         ; |91| 
           ZERO    .L2     B6                ; |91| 
           MVK     .S1     0x8,A6            ; |91| 
$C$RL4:    ; CALL OCCURS {_MEM_valloc}       ; |91| 
           STW     .D2T1   A4,*+SP(8)        ; |91| 
           NOP             2
	.dwpsn	"board_padk.c",92,5
           CALL    .S1     _MEM_valloc       ; |92| 
           LDW     .D2T1   *+SP(4),A4        ; |92| 
           MVKL    .S2     $C$RL5,B3         ; |92| 
           MVK     .S2     0x18,B4           ; |92| 
           MVKH    .S2     $C$RL5,B3         ; |92| 

           ZERO    .L2     B6                ; |92| 
||         MVK     .S1     0x8,A6            ; |92| 

$C$RL5:    ; CALL OCCURS {_MEM_valloc}       ; |92| 
           LDW     .D2T2   *+SP(8),B4        ; |92| 
           NOP             4
           STW     .D2T1   A4,*+B4(4)        ; |92| 
           NOP             2
	.dwpsn	"board_padk.c",94,5
           LDW     .D2T2   *+SP(8),B0        ; |94| 
           ZERO    .L2     B4                ; |94| 
           NOP             3
   [!B0]   B       .S1     $C$L8             ; |94| 
           NOP             5
           ; BRANCHCC OCCURS {$C$L8}         ; |94| 
;** --------------------------------------------------------------------------*
           MVK     .S2     0x1,B4            ; |94| 
;** --------------------------------------------------------------------------*
$C$L8:    
           CMPEQ   .L2     B4,0,B0           ; |94| 
   [ B0]   B       .S1     $C$L10            ; |94| 
           LDW     .D2T2   *+SP(8),B4        ; |94| 
           ZERO    .L1     A3                ; |94| 
           NOP             3
           ; BRANCHCC OCCURS {$C$L10}        ; |94| 
;** --------------------------------------------------------------------------*
           NOP             4
           LDW     .D2T2   *+B4(4),B0        ; |94| 
           NOP             4
   [!B0]   B       .S1     $C$L9             ; |94| 
           NOP             5
           ; BRANCHCC OCCURS {$C$L9}         ; |94| 
;** --------------------------------------------------------------------------*
           MVK     .S1     0x1,A3            ; |94| 
;** --------------------------------------------------------------------------*
$C$L9:    
           CMPEQ   .L2X    A3,0,B0           ; |94| 
;** --------------------------------------------------------------------------*
$C$L10:    
   [!B0]   B       .S1     $C$L11            ; |94| 
           NOP             5
           ; BRANCHCC OCCURS {$C$L11}        ; |94| 
;** --------------------------------------------------------------------------*
	.dwpsn	"board_padk.c",95,9
           B       .S1     $C$L12            ; |95| 
           NOP             4
           ZERO    .L1     A4                ; |95| 
           ; BRANCH OCCURS {$C$L12}          ; |95| 
;** --------------------------------------------------------------------------*
$C$L11:    
	.dwpsn	"board_padk.c",102,5
           MVKL    .S1     _PADK_board_control,A4

           MVKH    .S1     _PADK_board_control,A4
||         MV      .L1X    B4,A3

           STW     .D1T1   A4,*A3            ; |102| 
           NOP             2
	.dwpsn	"board_padk.c",104,5
           LDW     .D2T1   *+SP(8),A3        ; |104| 
           ZERO    .L2     B4                ; |104| 
           MVKH    .S2     0x40000000,B4     ; |104| 
           NOP             2
           LDW     .D1T1   *+A3(4),A3        ; |104| 
           NOP             4
           STW     .D1T2   B4,*A3            ; |104| 
           NOP             2
	.dwpsn	"board_padk.c",105,5
           LDW     .D2T1   *+SP(8),A3        ; |105| 
           ZERO    .L1     A4                ; |105| 
           NOP             3
           LDW     .D1T1   *+A3(4),A3        ; |105| 
           NOP             4
           STW     .D1T1   A4,*+A3(4)        ; |105| 
           NOP             2
	.dwpsn	"board_padk.c",106,5
           LDW     .D2T1   *+SP(8),A3        ; |106| 
           NOP             4
           LDW     .D1T1   *+A3(4),A3        ; |106| 
           NOP             4
           STW     .D1T1   A4,*+A3(8)        ; |106| 
           NOP             2
	.dwpsn	"board_padk.c",109,5
           LDW     .D2T2   *+SP(8),B5        ; |109| 
           MVKL    .S2     _CLKGEN_Init,B4
           MVKH    .S2     _CLKGEN_Init,B4
           MVKL    .S2     $C$RL6,B3         ; |109| 
           CALL    .S2     B4                ; |109| 
           LDW     .D2T1   *+B5(4),A4        ; |109| 
           NOP             3
           MVKH    .S2     $C$RL6,B3         ; |109| 
$C$RL6:    ; CALL OCCURS {B4}                ; |109| 
	.dwpsn	"board_padk.c",112,5
           LDW     .D2T1   *+SP(8),A4        ; |112| 
;** --------------------------------------------------------------------------*
$C$L12:    
	.dwpsn	"board_padk.c",113,1
           LDW     .D2T2   *++SP(16),B3      ; |113| 
           NOP             4
	.dwcfa	0x0e, 0
	.dwcfa	0xc0, 19
	.dwcfa	0x0e, 0
           RET     .S2     B3                ; |113| 
           NOP             5
           ; BRANCH OCCURS {B3}              ; |113| 
	.dwattr $C$DW$21, DW_AT_TI_end_file("board_padk.c")
	.dwattr $C$DW$21, DW_AT_TI_end_line(0x71)
	.dwattr $C$DW$21, DW_AT_TI_end_column(0x01)
	.dwendentry
	.dwendtag $C$DW$21

;*****************************************************************************
;* UNDEFINED EXTERNAL REFERENCES                                             *
;*****************************************************************************
	.global	_ADC_Init
	.global	_DAC_Init
	.global	_DAC_RelayUnmute
	.global	_DAC_RelayMute
	.global	_CLKGEN_Init
	.global	_MEM_valloc

;******************************************************************************
;* TYPE INFORMATION                                                           *
;******************************************************************************
$C$DW$T$33	.dwtag  DW_TAG_typedef, DW_AT_name("Ptr"), DW_AT_type(*$C$DW$T$3)
	.dwattr $C$DW$T$33, DW_AT_language(DW_LANG_C)

$C$DW$T$37	.dwtag  DW_TAG_subroutine_type, DW_AT_type(*$C$DW$T$33)
	.dwattr $C$DW$T$37, DW_AT_language(DW_LANG_C)
$C$DW$25	.dwtag  DW_TAG_formal_parameter, DW_AT_type(*$C$DW$T$26)
$C$DW$26	.dwtag  DW_TAG_formal_parameter, DW_AT_type(*$C$DW$T$34)
$C$DW$27	.dwtag  DW_TAG_formal_parameter, DW_AT_type(*$C$DW$T$34)
$C$DW$28	.dwtag  DW_TAG_formal_parameter, DW_AT_type(*$C$DW$T$36)
	.dwendtag $C$DW$T$37


$C$DW$T$39	.dwtag  DW_TAG_subroutine_type
	.dwattr $C$DW$T$39, DW_AT_language(DW_LANG_C)
$C$DW$T$10	.dwtag  DW_TAG_base_type, DW_AT_name("int")
	.dwattr $C$DW$T$10, DW_AT_encoding(DW_ATE_signed)
	.dwattr $C$DW$T$10, DW_AT_byte_size(0x04)
$C$DW$T$26	.dwtag  DW_TAG_typedef, DW_AT_name("Int"), DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$T$26, DW_AT_language(DW_LANG_C)

$C$DW$T$42	.dwtag  DW_TAG_subroutine_type, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$T$42, DW_AT_language(DW_LANG_C)
$C$DW$29	.dwtag  DW_TAG_formal_parameter, DW_AT_type(*$C$DW$T$41)
	.dwendtag $C$DW$T$42


$C$DW$T$45	.dwtag  DW_TAG_subroutine_type, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$T$45, DW_AT_language(DW_LANG_C)
$C$DW$30	.dwtag  DW_TAG_formal_parameter, DW_AT_type(*$C$DW$T$44)
	.dwendtag $C$DW$T$45


$C$DW$T$48	.dwtag  DW_TAG_subroutine_type, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$T$48, DW_AT_language(DW_LANG_C)
$C$DW$31	.dwtag  DW_TAG_formal_parameter, DW_AT_type(*$C$DW$T$47)
	.dwendtag $C$DW$T$48

$C$DW$T$50	.dwtag  DW_TAG_typedef, DW_AT_name("Int32"), DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$T$50, DW_AT_language(DW_LANG_C)

$C$DW$T$54	.dwtag  DW_TAG_subroutine_type, DW_AT_type(*$C$DW$T$50)
	.dwattr $C$DW$T$54, DW_AT_language(DW_LANG_C)
$C$DW$32	.dwtag  DW_TAG_formal_parameter, DW_AT_type(*$C$DW$T$52)
$C$DW$33	.dwtag  DW_TAG_formal_parameter, DW_AT_type(*$C$DW$T$53)
	.dwendtag $C$DW$T$54

$C$DW$T$34	.dwtag  DW_TAG_typedef, DW_AT_name("size_t"), DW_AT_type(*$C$DW$T$11)
	.dwattr $C$DW$T$34, DW_AT_language(DW_LANG_C)
$C$DW$T$53	.dwtag  DW_TAG_typedef, DW_AT_name("Uns"), DW_AT_type(*$C$DW$T$11)
	.dwattr $C$DW$T$53, DW_AT_language(DW_LANG_C)
$C$DW$T$41	.dwtag  DW_TAG_pointer_type, DW_AT_type(*$C$DW$T$23)
	.dwattr $C$DW$T$41, DW_AT_address_class(0x20)
$C$DW$T$44	.dwtag  DW_TAG_pointer_type, DW_AT_type(*$C$DW$T$24)
	.dwattr $C$DW$T$44, DW_AT_address_class(0x20)
$C$DW$T$47	.dwtag  DW_TAG_pointer_type, DW_AT_type(*$C$DW$T$22)
	.dwattr $C$DW$T$47, DW_AT_address_class(0x20)
$C$DW$T$52	.dwtag  DW_TAG_typedef, DW_AT_name("PADK_brdHdl"), DW_AT_type(*$C$DW$T$51)
	.dwattr $C$DW$T$52, DW_AT_language(DW_LANG_C)

$C$DW$T$56	.dwtag  DW_TAG_subroutine_type, DW_AT_type(*$C$DW$T$52)
	.dwattr $C$DW$T$56, DW_AT_language(DW_LANG_C)
$C$DW$34	.dwtag  DW_TAG_formal_parameter, DW_AT_type(*$C$DW$T$50)
	.dwendtag $C$DW$T$56

$C$DW$T$58	.dwtag  DW_TAG_pointer_type, DW_AT_type(*$C$DW$T$57)
	.dwattr $C$DW$T$58, DW_AT_address_class(0x20)
$C$DW$T$36	.dwtag  DW_TAG_typedef, DW_AT_name("Char"), DW_AT_type(*$C$DW$T$35)
	.dwattr $C$DW$T$36, DW_AT_language(DW_LANG_C)
$C$DW$T$3	.dwtag  DW_TAG_pointer_type
	.dwattr $C$DW$T$3, DW_AT_address_class(0x20)
$C$DW$T$11	.dwtag  DW_TAG_base_type, DW_AT_name("unsigned int")
	.dwattr $C$DW$T$11, DW_AT_encoding(DW_ATE_unsigned)
	.dwattr $C$DW$T$11, DW_AT_byte_size(0x04)
$C$DW$T$23	.dwtag  DW_TAG_typedef, DW_AT_name("ADC_Params"), DW_AT_type(*$C$DW$T$19)
	.dwattr $C$DW$T$23, DW_AT_language(DW_LANG_C)
$C$DW$T$24	.dwtag  DW_TAG_typedef, DW_AT_name("DAC_Params"), DW_AT_type(*$C$DW$T$20)
	.dwattr $C$DW$T$24, DW_AT_language(DW_LANG_C)
$C$DW$T$22	.dwtag  DW_TAG_typedef, DW_AT_name("CLKGEN_Params"), DW_AT_type(*$C$DW$T$21)
	.dwattr $C$DW$T$22, DW_AT_language(DW_LANG_C)
$C$DW$T$51	.dwtag  DW_TAG_pointer_type, DW_AT_type(*$C$DW$T$32)
	.dwattr $C$DW$T$51, DW_AT_address_class(0x20)
$C$DW$T$57	.dwtag  DW_TAG_typedef, DW_AT_name("PADK_brdCfg"), DW_AT_type(*$C$DW$T$32)
	.dwattr $C$DW$T$57, DW_AT_language(DW_LANG_C)
$C$DW$T$35	.dwtag  DW_TAG_base_type, DW_AT_name("signed char")
	.dwattr $C$DW$T$35, DW_AT_encoding(DW_ATE_signed_char)
	.dwattr $C$DW$T$35, DW_AT_byte_size(0x01)

$C$DW$T$19	.dwtag  DW_TAG_structure_type
	.dwattr $C$DW$T$19, DW_AT_byte_size(0x02)
$C$DW$35	.dwtag  DW_TAG_member, DW_AT_type(*$C$DW$T$6)
	.dwattr $C$DW$35, DW_AT_name("format")
	.dwattr $C$DW$35, DW_AT_TI_symbol_name("_format")
	.dwattr $C$DW$35, DW_AT_data_member_location[DW_OP_plus_uconst 0x0]
	.dwattr $C$DW$35, DW_AT_accessibility(DW_ACCESS_public)
$C$DW$36	.dwtag  DW_TAG_member, DW_AT_type(*$C$DW$T$6)
	.dwattr $C$DW$36, DW_AT_name("rate")
	.dwattr $C$DW$36, DW_AT_TI_symbol_name("_rate")
	.dwattr $C$DW$36, DW_AT_data_member_location[DW_OP_plus_uconst 0x1]
	.dwattr $C$DW$36, DW_AT_accessibility(DW_ACCESS_public)
	.dwendtag $C$DW$T$19


$C$DW$T$20	.dwtag  DW_TAG_structure_type
	.dwattr $C$DW$T$20, DW_AT_byte_size(0x08)
$C$DW$37	.dwtag  DW_TAG_member, DW_AT_type(*$C$DW$T$11)
	.dwattr $C$DW$37, DW_AT_name("fs")
	.dwattr $C$DW$37, DW_AT_TI_symbol_name("_fs")
	.dwattr $C$DW$37, DW_AT_data_member_location[DW_OP_plus_uconst 0x0]
	.dwattr $C$DW$37, DW_AT_accessibility(DW_ACCESS_public)
$C$DW$38	.dwtag  DW_TAG_member, DW_AT_type(*$C$DW$T$6)
	.dwattr $C$DW$38, DW_AT_name("mute")
	.dwattr $C$DW$38, DW_AT_TI_symbol_name("_mute")
	.dwattr $C$DW$38, DW_AT_data_member_location[DW_OP_plus_uconst 0x4]
	.dwattr $C$DW$38, DW_AT_accessibility(DW_ACCESS_public)
$C$DW$39	.dwtag  DW_TAG_member, DW_AT_type(*$C$DW$T$6)
	.dwattr $C$DW$39, DW_AT_name("format")
	.dwattr $C$DW$39, DW_AT_TI_symbol_name("_format")
	.dwattr $C$DW$39, DW_AT_data_member_location[DW_OP_plus_uconst 0x5]
	.dwattr $C$DW$39, DW_AT_accessibility(DW_ACCESS_public)
$C$DW$40	.dwtag  DW_TAG_member, DW_AT_type(*$C$DW$T$6)
	.dwattr $C$DW$40, DW_AT_name("deemphasis")
	.dwattr $C$DW$40, DW_AT_TI_symbol_name("_deemphasis")
	.dwattr $C$DW$40, DW_AT_data_member_location[DW_OP_plus_uconst 0x6]
	.dwattr $C$DW$40, DW_AT_accessibility(DW_ACCESS_public)
	.dwendtag $C$DW$T$20


$C$DW$T$21	.dwtag  DW_TAG_structure_type
	.dwattr $C$DW$T$21, DW_AT_byte_size(0x0c)
$C$DW$41	.dwtag  DW_TAG_member, DW_AT_type(*$C$DW$T$11)
	.dwattr $C$DW$41, DW_AT_name("osc_sel")
	.dwattr $C$DW$41, DW_AT_TI_symbol_name("_osc_sel")
	.dwattr $C$DW$41, DW_AT_data_member_location[DW_OP_plus_uconst 0x0]
	.dwattr $C$DW$41, DW_AT_accessibility(DW_ACCESS_public)
$C$DW$42	.dwtag  DW_TAG_member, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$42, DW_AT_name("adc_dir_clk_src")
	.dwattr $C$DW$42, DW_AT_TI_symbol_name("_adc_dir_clk_src")
	.dwattr $C$DW$42, DW_AT_data_member_location[DW_OP_plus_uconst 0x4]
	.dwattr $C$DW$42, DW_AT_accessibility(DW_ACCESS_public)
$C$DW$43	.dwtag  DW_TAG_member, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$43, DW_AT_name("dac_dit_clk_src")
	.dwattr $C$DW$43, DW_AT_TI_symbol_name("_dac_dit_clk_src")
	.dwattr $C$DW$43, DW_AT_data_member_location[DW_OP_plus_uconst 0x8]
	.dwattr $C$DW$43, DW_AT_accessibility(DW_ACCESS_public)
	.dwendtag $C$DW$T$21


$C$DW$T$32	.dwtag  DW_TAG_structure_type
	.dwattr $C$DW$T$32, DW_AT_name("PADK_brdCfg")
	.dwattr $C$DW$T$32, DW_AT_byte_size(0x08)
$C$DW$44	.dwtag  DW_TAG_member, DW_AT_type(*$C$DW$T$29)
	.dwattr $C$DW$44, DW_AT_name("brdCtrl")
	.dwattr $C$DW$44, DW_AT_TI_symbol_name("_brdCtrl")
	.dwattr $C$DW$44, DW_AT_data_member_location[DW_OP_plus_uconst 0x0]
	.dwattr $C$DW$44, DW_AT_accessibility(DW_ACCESS_public)
$C$DW$45	.dwtag  DW_TAG_member, DW_AT_type(*$C$DW$T$31)
	.dwattr $C$DW$45, DW_AT_name("padkInt")
	.dwattr $C$DW$45, DW_AT_TI_symbol_name("_padkInt")
	.dwattr $C$DW$45, DW_AT_data_member_location[DW_OP_plus_uconst 0x4]
	.dwattr $C$DW$45, DW_AT_accessibility(DW_ACCESS_public)
	.dwendtag $C$DW$T$32

$C$DW$T$6	.dwtag  DW_TAG_base_type, DW_AT_name("unsigned char")
	.dwattr $C$DW$T$6, DW_AT_encoding(DW_ATE_unsigned_char)
	.dwattr $C$DW$T$6, DW_AT_byte_size(0x01)
$C$DW$T$29	.dwtag  DW_TAG_typedef, DW_AT_name("Fxn"), DW_AT_type(*$C$DW$T$28)
	.dwattr $C$DW$T$29, DW_AT_language(DW_LANG_C)
$C$DW$T$31	.dwtag  DW_TAG_pointer_type, DW_AT_type(*$C$DW$T$30)
	.dwattr $C$DW$T$31, DW_AT_address_class(0x20)
$C$DW$T$28	.dwtag  DW_TAG_pointer_type, DW_AT_type(*$C$DW$T$27)
	.dwattr $C$DW$T$28, DW_AT_address_class(0x20)
$C$DW$T$30	.dwtag  DW_TAG_typedef, DW_AT_name("PADK_internal"), DW_AT_type(*$C$DW$T$25)
	.dwattr $C$DW$T$30, DW_AT_language(DW_LANG_C)

$C$DW$T$27	.dwtag  DW_TAG_subroutine_type, DW_AT_type(*$C$DW$T$26)
	.dwattr $C$DW$T$27, DW_AT_language(DW_LANG_C)

$C$DW$T$25	.dwtag  DW_TAG_structure_type
	.dwattr $C$DW$T$25, DW_AT_name("PADK_internal")
	.dwattr $C$DW$T$25, DW_AT_byte_size(0x18)
$C$DW$46	.dwtag  DW_TAG_member, DW_AT_type(*$C$DW$T$22)
	.dwattr $C$DW$46, DW_AT_name("clkgenParams")
	.dwattr $C$DW$46, DW_AT_TI_symbol_name("_clkgenParams")
	.dwattr $C$DW$46, DW_AT_data_member_location[DW_OP_plus_uconst 0x0]
	.dwattr $C$DW$46, DW_AT_accessibility(DW_ACCESS_public)
$C$DW$47	.dwtag  DW_TAG_member, DW_AT_type(*$C$DW$T$23)
	.dwattr $C$DW$47, DW_AT_name("adcParams")
	.dwattr $C$DW$47, DW_AT_TI_symbol_name("_adcParams")
	.dwattr $C$DW$47, DW_AT_data_member_location[DW_OP_plus_uconst 0xc]
	.dwattr $C$DW$47, DW_AT_accessibility(DW_ACCESS_public)
$C$DW$48	.dwtag  DW_TAG_member, DW_AT_type(*$C$DW$T$24)
	.dwattr $C$DW$48, DW_AT_name("dacParams")
	.dwattr $C$DW$48, DW_AT_TI_symbol_name("_dacParams")
	.dwattr $C$DW$48, DW_AT_data_member_location[DW_OP_plus_uconst 0x10]
	.dwattr $C$DW$48, DW_AT_accessibility(DW_ACCESS_public)
	.dwendtag $C$DW$T$25


	.dwattr $C$DW$14, DW_AT_external(0x01)
	.dwattr $C$DW$14, DW_AT_type(*$C$DW$T$50)
	.dwattr $C$DW$21, DW_AT_external(0x01)
	.dwattr $C$DW$21, DW_AT_type(*$C$DW$T$52)
	.dwattr $C$DW$CU, DW_AT_language(DW_LANG_C)

;***************************************************************
;* DWARF CIE ENTRIES                                           *
;***************************************************************

DW$CIE	.dwcie 1, 128
	.dwcfa	0x0c, 31, 0
	.dwcfa	0x07, 0
	.dwcfa	0x07, 1
	.dwcfa	0x07, 2
	.dwcfa	0x07, 3
	.dwcfa	0x07, 4
	.dwcfa	0x07, 5
	.dwcfa	0x07, 6
	.dwcfa	0x07, 7
	.dwcfa	0x07, 8
	.dwcfa	0x07, 9
	.dwcfa	0x08, 10
	.dwcfa	0x08, 11
	.dwcfa	0x08, 12
	.dwcfa	0x08, 13
	.dwcfa	0x08, 14
	.dwcfa	0x08, 15
	.dwcfa	0x07, 16
	.dwcfa	0x07, 17
	.dwcfa	0x07, 18
	.dwcfa	0x07, 19
	.dwcfa	0x07, 20
	.dwcfa	0x07, 21
	.dwcfa	0x07, 22
	.dwcfa	0x07, 23
	.dwcfa	0x07, 24
	.dwcfa	0x07, 25
	.dwcfa	0x08, 26
	.dwcfa	0x08, 27
	.dwcfa	0x08, 28
	.dwcfa	0x08, 29
	.dwcfa	0x08, 30
	.dwcfa	0x08, 31
	.dwcfa	0x08, 32
	.dwcfa	0x07, 33
	.dwcfa	0x07, 34
	.dwcfa	0x07, 35
	.dwcfa	0x07, 36
	.dwcfa	0x07, 37
	.dwcfa	0x07, 38
	.dwcfa	0x07, 39
	.dwcfa	0x07, 40
	.dwcfa	0x07, 41
	.dwcfa	0x07, 42
	.dwcfa	0x07, 43
	.dwcfa	0x07, 44
	.dwcfa	0x07, 45
	.dwcfa	0x07, 46
	.dwcfa	0x07, 47
	.dwcfa	0x07, 48
	.dwcfa	0x07, 49
	.dwcfa	0x07, 50
	.dwcfa	0x07, 51
	.dwcfa	0x07, 52
	.dwcfa	0x07, 53
	.dwcfa	0x07, 54
	.dwcfa	0x07, 55
	.dwcfa	0x07, 56
	.dwcfa	0x07, 57
	.dwcfa	0x07, 58
	.dwcfa	0x07, 59
	.dwcfa	0x07, 60
	.dwcfa	0x07, 61
	.dwcfa	0x07, 62
	.dwcfa	0x07, 63
	.dwcfa	0x07, 64
	.dwcfa	0x07, 65
	.dwcfa	0x07, 66
	.dwcfa	0x07, 67
	.dwcfa	0x07, 68
	.dwcfa	0x07, 69
	.dwcfa	0x07, 70
	.dwcfa	0x07, 71
	.dwcfa	0x07, 72
	.dwcfa	0x07, 73
	.dwcfa	0x07, 74
	.dwcfa	0x07, 75
	.dwcfa	0x07, 76
	.dwcfa	0x07, 77
	.dwcfa	0x07, 78
	.dwcfa	0x07, 79
	.dwcfa	0x07, 80
	.dwcfa	0x07, 81
	.dwcfa	0x07, 82
	.dwcfa	0x07, 83
	.dwcfa	0x07, 84
	.dwcfa	0x07, 85
	.dwcfa	0x07, 86
	.dwcfa	0x07, 87
	.dwcfa	0x07, 88
	.dwcfa	0x07, 89
	.dwcfa	0x07, 90
	.dwcfa	0x07, 91
	.dwcfa	0x07, 92
	.dwcfa	0x07, 93
	.dwcfa	0x07, 94
	.dwcfa	0x07, 95
	.dwcfa	0x07, 96
	.dwcfa	0x07, 97
	.dwcfa	0x07, 98
	.dwcfa	0x07, 99
	.dwcfa	0x07, 100
	.dwcfa	0x07, 101
	.dwcfa	0x07, 102
	.dwcfa	0x07, 103
	.dwcfa	0x07, 104
	.dwcfa	0x07, 105
	.dwcfa	0x07, 106
	.dwcfa	0x07, 107
	.dwcfa	0x07, 108
	.dwcfa	0x07, 109
	.dwcfa	0x07, 110
	.dwcfa	0x07, 111
	.dwcfa	0x07, 112
	.dwcfa	0x07, 113
	.dwcfa	0x07, 114
	.dwcfa	0x07, 115
	.dwcfa	0x07, 116
	.dwcfa	0x07, 117
	.dwcfa	0x07, 118
	.dwcfa	0x07, 119
	.dwcfa	0x07, 120
	.dwcfa	0x07, 121
	.dwcfa	0x07, 122
	.dwcfa	0x07, 123
	.dwcfa	0x07, 124
	.dwcfa	0x07, 125
	.dwcfa	0x07, 126
	.dwcfa	0x07, 127
	.dwcfa	0x07, 128

;***************************************************************
;* DWARF REGISTER MAP                                          *
;***************************************************************

$C$DW$49	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A0")
	.dwattr $C$DW$49, DW_AT_location[DW_OP_reg0]
$C$DW$50	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A1")
	.dwattr $C$DW$50, DW_AT_location[DW_OP_reg1]
$C$DW$51	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A2")
	.dwattr $C$DW$51, DW_AT_location[DW_OP_reg2]
$C$DW$52	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A3")
	.dwattr $C$DW$52, DW_AT_location[DW_OP_reg3]
$C$DW$53	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A4")
	.dwattr $C$DW$53, DW_AT_location[DW_OP_reg4]
$C$DW$54	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A5")
	.dwattr $C$DW$54, DW_AT_location[DW_OP_reg5]
$C$DW$55	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A6")
	.dwattr $C$DW$55, DW_AT_location[DW_OP_reg6]
$C$DW$56	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A7")
	.dwattr $C$DW$56, DW_AT_location[DW_OP_reg7]
$C$DW$57	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A8")
	.dwattr $C$DW$57, DW_AT_location[DW_OP_reg8]
$C$DW$58	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A9")
	.dwattr $C$DW$58, DW_AT_location[DW_OP_reg9]
$C$DW$59	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A10")
	.dwattr $C$DW$59, DW_AT_location[DW_OP_reg10]
$C$DW$60	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A11")
	.dwattr $C$DW$60, DW_AT_location[DW_OP_reg11]
$C$DW$61	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A12")
	.dwattr $C$DW$61, DW_AT_location[DW_OP_reg12]
$C$DW$62	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A13")
	.dwattr $C$DW$62, DW_AT_location[DW_OP_reg13]
$C$DW$63	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A14")
	.dwattr $C$DW$63, DW_AT_location[DW_OP_reg14]
$C$DW$64	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A15")
	.dwattr $C$DW$64, DW_AT_location[DW_OP_reg15]
$C$DW$65	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B0")
	.dwattr $C$DW$65, DW_AT_location[DW_OP_reg16]
$C$DW$66	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B1")
	.dwattr $C$DW$66, DW_AT_location[DW_OP_reg17]
$C$DW$67	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B2")
	.dwattr $C$DW$67, DW_AT_location[DW_OP_reg18]
$C$DW$68	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B3")
	.dwattr $C$DW$68, DW_AT_location[DW_OP_reg19]
$C$DW$69	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B4")
	.dwattr $C$DW$69, DW_AT_location[DW_OP_reg20]
$C$DW$70	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B5")
	.dwattr $C$DW$70, DW_AT_location[DW_OP_reg21]
$C$DW$71	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B6")
	.dwattr $C$DW$71, DW_AT_location[DW_OP_reg22]
$C$DW$72	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B7")
	.dwattr $C$DW$72, DW_AT_location[DW_OP_reg23]
$C$DW$73	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B8")
	.dwattr $C$DW$73, DW_AT_location[DW_OP_reg24]
$C$DW$74	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B9")
	.dwattr $C$DW$74, DW_AT_location[DW_OP_reg25]
$C$DW$75	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B10")
	.dwattr $C$DW$75, DW_AT_location[DW_OP_reg26]
$C$DW$76	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B11")
	.dwattr $C$DW$76, DW_AT_location[DW_OP_reg27]
$C$DW$77	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B12")
	.dwattr $C$DW$77, DW_AT_location[DW_OP_reg28]
$C$DW$78	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B13")
	.dwattr $C$DW$78, DW_AT_location[DW_OP_reg29]
$C$DW$79	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("DP")
	.dwattr $C$DW$79, DW_AT_location[DW_OP_reg30]
$C$DW$80	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("SP")
	.dwattr $C$DW$80, DW_AT_location[DW_OP_reg31]
$C$DW$81	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("FP")
	.dwattr $C$DW$81, DW_AT_location[DW_OP_regx 0x20]
$C$DW$82	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("PC")
	.dwattr $C$DW$82, DW_AT_location[DW_OP_regx 0x21]
$C$DW$83	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("IRP")
	.dwattr $C$DW$83, DW_AT_location[DW_OP_regx 0x22]
$C$DW$84	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("IFR")
	.dwattr $C$DW$84, DW_AT_location[DW_OP_regx 0x23]
$C$DW$85	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("NRP")
	.dwattr $C$DW$85, DW_AT_location[DW_OP_regx 0x24]
$C$DW$86	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A16")
	.dwattr $C$DW$86, DW_AT_location[DW_OP_regx 0x25]
$C$DW$87	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A17")
	.dwattr $C$DW$87, DW_AT_location[DW_OP_regx 0x26]
$C$DW$88	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A18")
	.dwattr $C$DW$88, DW_AT_location[DW_OP_regx 0x27]
$C$DW$89	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A19")
	.dwattr $C$DW$89, DW_AT_location[DW_OP_regx 0x28]
$C$DW$90	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A20")
	.dwattr $C$DW$90, DW_AT_location[DW_OP_regx 0x29]
$C$DW$91	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A21")
	.dwattr $C$DW$91, DW_AT_location[DW_OP_regx 0x2a]
$C$DW$92	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A22")
	.dwattr $C$DW$92, DW_AT_location[DW_OP_regx 0x2b]
$C$DW$93	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A23")
	.dwattr $C$DW$93, DW_AT_location[DW_OP_regx 0x2c]
$C$DW$94	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A24")
	.dwattr $C$DW$94, DW_AT_location[DW_OP_regx 0x2d]
$C$DW$95	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A25")
	.dwattr $C$DW$95, DW_AT_location[DW_OP_regx 0x2e]
$C$DW$96	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A26")
	.dwattr $C$DW$96, DW_AT_location[DW_OP_regx 0x2f]
$C$DW$97	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A27")
	.dwattr $C$DW$97, DW_AT_location[DW_OP_regx 0x30]
$C$DW$98	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A28")
	.dwattr $C$DW$98, DW_AT_location[DW_OP_regx 0x31]
$C$DW$99	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A29")
	.dwattr $C$DW$99, DW_AT_location[DW_OP_regx 0x32]
$C$DW$100	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A30")
	.dwattr $C$DW$100, DW_AT_location[DW_OP_regx 0x33]
$C$DW$101	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A31")
	.dwattr $C$DW$101, DW_AT_location[DW_OP_regx 0x34]
$C$DW$102	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B16")
	.dwattr $C$DW$102, DW_AT_location[DW_OP_regx 0x35]
$C$DW$103	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B17")
	.dwattr $C$DW$103, DW_AT_location[DW_OP_regx 0x36]
$C$DW$104	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B18")
	.dwattr $C$DW$104, DW_AT_location[DW_OP_regx 0x37]
$C$DW$105	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B19")
	.dwattr $C$DW$105, DW_AT_location[DW_OP_regx 0x38]
$C$DW$106	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B20")
	.dwattr $C$DW$106, DW_AT_location[DW_OP_regx 0x39]
$C$DW$107	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B21")
	.dwattr $C$DW$107, DW_AT_location[DW_OP_regx 0x3a]
$C$DW$108	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B22")
	.dwattr $C$DW$108, DW_AT_location[DW_OP_regx 0x3b]
$C$DW$109	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B23")
	.dwattr $C$DW$109, DW_AT_location[DW_OP_regx 0x3c]
$C$DW$110	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B24")
	.dwattr $C$DW$110, DW_AT_location[DW_OP_regx 0x3d]
$C$DW$111	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B25")
	.dwattr $C$DW$111, DW_AT_location[DW_OP_regx 0x3e]
$C$DW$112	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B26")
	.dwattr $C$DW$112, DW_AT_location[DW_OP_regx 0x3f]
$C$DW$113	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B27")
	.dwattr $C$DW$113, DW_AT_location[DW_OP_regx 0x40]
$C$DW$114	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B28")
	.dwattr $C$DW$114, DW_AT_location[DW_OP_regx 0x41]
$C$DW$115	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B29")
	.dwattr $C$DW$115, DW_AT_location[DW_OP_regx 0x42]
$C$DW$116	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B30")
	.dwattr $C$DW$116, DW_AT_location[DW_OP_regx 0x43]
$C$DW$117	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B31")
	.dwattr $C$DW$117, DW_AT_location[DW_OP_regx 0x44]
$C$DW$118	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("AMR")
	.dwattr $C$DW$118, DW_AT_location[DW_OP_regx 0x45]
$C$DW$119	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("CSR")
	.dwattr $C$DW$119, DW_AT_location[DW_OP_regx 0x46]
$C$DW$120	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("ISR")
	.dwattr $C$DW$120, DW_AT_location[DW_OP_regx 0x47]
$C$DW$121	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("ICR")
	.dwattr $C$DW$121, DW_AT_location[DW_OP_regx 0x48]
$C$DW$122	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("IER")
	.dwattr $C$DW$122, DW_AT_location[DW_OP_regx 0x49]
$C$DW$123	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("ISTP")
	.dwattr $C$DW$123, DW_AT_location[DW_OP_regx 0x4a]
$C$DW$124	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("IN")
	.dwattr $C$DW$124, DW_AT_location[DW_OP_regx 0x4b]
$C$DW$125	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("OUT")
	.dwattr $C$DW$125, DW_AT_location[DW_OP_regx 0x4c]
$C$DW$126	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("ACR")
	.dwattr $C$DW$126, DW_AT_location[DW_OP_regx 0x4d]
$C$DW$127	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("ADR")
	.dwattr $C$DW$127, DW_AT_location[DW_OP_regx 0x4e]
$C$DW$128	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("FADCR")
	.dwattr $C$DW$128, DW_AT_location[DW_OP_regx 0x4f]
$C$DW$129	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("FAUCR")
	.dwattr $C$DW$129, DW_AT_location[DW_OP_regx 0x50]
$C$DW$130	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("FMCR")
	.dwattr $C$DW$130, DW_AT_location[DW_OP_regx 0x51]
$C$DW$131	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("GFPGFR")
	.dwattr $C$DW$131, DW_AT_location[DW_OP_regx 0x52]
$C$DW$132	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("DIER")
	.dwattr $C$DW$132, DW_AT_location[DW_OP_regx 0x53]
$C$DW$133	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("REP")
	.dwattr $C$DW$133, DW_AT_location[DW_OP_regx 0x54]
$C$DW$134	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("TSCL")
	.dwattr $C$DW$134, DW_AT_location[DW_OP_regx 0x55]
$C$DW$135	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("TSCH")
	.dwattr $C$DW$135, DW_AT_location[DW_OP_regx 0x56]
$C$DW$136	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("ARP")
	.dwattr $C$DW$136, DW_AT_location[DW_OP_regx 0x57]
$C$DW$137	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("ILC")
	.dwattr $C$DW$137, DW_AT_location[DW_OP_regx 0x58]
$C$DW$138	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("RILC")
	.dwattr $C$DW$138, DW_AT_location[DW_OP_regx 0x59]
$C$DW$139	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("DNUM")
	.dwattr $C$DW$139, DW_AT_location[DW_OP_regx 0x5a]
$C$DW$140	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("SSR")
	.dwattr $C$DW$140, DW_AT_location[DW_OP_regx 0x5b]
$C$DW$141	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("GPLYA")
	.dwattr $C$DW$141, DW_AT_location[DW_OP_regx 0x5c]
$C$DW$142	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("GPLYB")
	.dwattr $C$DW$142, DW_AT_location[DW_OP_regx 0x5d]
$C$DW$143	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("TSR")
	.dwattr $C$DW$143, DW_AT_location[DW_OP_regx 0x5e]
$C$DW$144	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("ITSR")
	.dwattr $C$DW$144, DW_AT_location[DW_OP_regx 0x5f]
$C$DW$145	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("NTSR")
	.dwattr $C$DW$145, DW_AT_location[DW_OP_regx 0x60]
$C$DW$146	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("EFR")
	.dwattr $C$DW$146, DW_AT_location[DW_OP_regx 0x61]
$C$DW$147	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("ECR")
	.dwattr $C$DW$147, DW_AT_location[DW_OP_regx 0x62]
$C$DW$148	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("IERR")
	.dwattr $C$DW$148, DW_AT_location[DW_OP_regx 0x63]
$C$DW$149	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("DMSG")
	.dwattr $C$DW$149, DW_AT_location[DW_OP_regx 0x64]
$C$DW$150	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("CMSG")
	.dwattr $C$DW$150, DW_AT_location[DW_OP_regx 0x65]
$C$DW$151	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("DT_DMA_ADDR")
	.dwattr $C$DW$151, DW_AT_location[DW_OP_regx 0x66]
$C$DW$152	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("DT_DMA_DATA")
	.dwattr $C$DW$152, DW_AT_location[DW_OP_regx 0x67]
$C$DW$153	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("DT_DMA_CNTL")
	.dwattr $C$DW$153, DW_AT_location[DW_OP_regx 0x68]
$C$DW$154	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("TCU_CNTL")
	.dwattr $C$DW$154, DW_AT_location[DW_OP_regx 0x69]
$C$DW$155	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("RTDX_REC_CNTL")
	.dwattr $C$DW$155, DW_AT_location[DW_OP_regx 0x6a]
$C$DW$156	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("RTDX_XMT_CNTL")
	.dwattr $C$DW$156, DW_AT_location[DW_OP_regx 0x6b]
$C$DW$157	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("RTDX_CFG")
	.dwattr $C$DW$157, DW_AT_location[DW_OP_regx 0x6c]
$C$DW$158	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("RTDX_RDATA")
	.dwattr $C$DW$158, DW_AT_location[DW_OP_regx 0x6d]
$C$DW$159	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("RTDX_WDATA")
	.dwattr $C$DW$159, DW_AT_location[DW_OP_regx 0x6e]
$C$DW$160	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("RTDX_RADDR")
	.dwattr $C$DW$160, DW_AT_location[DW_OP_regx 0x6f]
$C$DW$161	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("RTDX_WADDR")
	.dwattr $C$DW$161, DW_AT_location[DW_OP_regx 0x70]
$C$DW$162	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("MFREG0")
	.dwattr $C$DW$162, DW_AT_location[DW_OP_regx 0x71]
$C$DW$163	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("DBG_STAT")
	.dwattr $C$DW$163, DW_AT_location[DW_OP_regx 0x72]
$C$DW$164	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("BRK_EN")
	.dwattr $C$DW$164, DW_AT_location[DW_OP_regx 0x73]
$C$DW$165	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("HWBP0_CNT")
	.dwattr $C$DW$165, DW_AT_location[DW_OP_regx 0x74]
$C$DW$166	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("HWBP0")
	.dwattr $C$DW$166, DW_AT_location[DW_OP_regx 0x75]
$C$DW$167	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("HWBP1")
	.dwattr $C$DW$167, DW_AT_location[DW_OP_regx 0x76]
$C$DW$168	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("HWBP2")
	.dwattr $C$DW$168, DW_AT_location[DW_OP_regx 0x77]
$C$DW$169	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("HWBP3")
	.dwattr $C$DW$169, DW_AT_location[DW_OP_regx 0x78]
$C$DW$170	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("OVERLAY")
	.dwattr $C$DW$170, DW_AT_location[DW_OP_regx 0x79]
$C$DW$171	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("PC_PROF")
	.dwattr $C$DW$171, DW_AT_location[DW_OP_regx 0x7a]
$C$DW$172	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("ATSR")
	.dwattr $C$DW$172, DW_AT_location[DW_OP_regx 0x7b]
$C$DW$173	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("TRR")
	.dwattr $C$DW$173, DW_AT_location[DW_OP_regx 0x7c]
$C$DW$174	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("TCRR")
	.dwattr $C$DW$174, DW_AT_location[DW_OP_regx 0x7d]
$C$DW$175	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("DESR")
	.dwattr $C$DW$175, DW_AT_location[DW_OP_regx 0x7e]
$C$DW$176	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("DETR")
	.dwattr $C$DW$176, DW_AT_location[DW_OP_regx 0x7f]
$C$DW$177	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("CIE_RETA")
	.dwattr $C$DW$177, DW_AT_location[DW_OP_regx 0x80]
	.dwendtag $C$DW$CU

