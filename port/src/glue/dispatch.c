/* dispatch.c - the port's address -> routine table and the CPU entry points.
 * Written by port/tools/dispatch_gen.py from port/src/game; do not edit. */
#include "nes_runtime.h"

/* the routines push their JSR return addresses (interp.c checks this) */
int g_recomp_push_all_jsr = 1;

uint16_t g_rti_target = 0;   /* RTI's target, func_NMI runs it (the hijacked return) */
uint16_t g_rti_source = 0;
int g_rti_bank = -1;
uint16_t g_rts_target = 0;

void func_C000(void);
void func_C09D(void);
void func_C119(void);
void func_C13B(void);
void func_C14A(void);
void func_C194(void);
void func_C1B1(void);
void func_C1D8(void);
void func_C1DF(void);
void func_C1FD(void);
void func_C201(void);
void func_C222(void);
void func_C265(void);
void func_C26F(void);
void func_C279(void);
void func_C2D1(void);
void func_C31D(void);
void func_C334(void);
void func_C362(void);
void func_C36E(void);
void func_C3B4(void);
void func_C3DD(void);
void func_C44F(void);
void func_C47D(void);
void func_C4A5(void);
void func_C4BD(void);
void func_C527(void);
void func_C556(void);
void func_C5A3(void);
void func_C5CE(void);
void func_C681(void);
void func_C6D2(void);
void func_C844(void);
void func_C85C(void);
void func_C85F(void);
void func_C888(void);
void func_C8A6(void);
void func_C8B4(void);
void func_C8ED(void);
void func_C93B(void);
void func_C987(void);
void func_C99D(void);
void func_C9A7(void);
void func_C9B6(void);
void func_C9B9(void);
void func_CA20(void);
void func_CA39(void);
void func_CA3C(void);
void func_CAA2(void);
void func_CAC8(void);
void func_CAE5(void);
void func_CAF5(void);
void func_CB07(void);
void func_CB22(void);
void func_CB99(void);
void func_CBA0(void);
void func_CBA3(void);
void func_CC85(void);
void func_CCA2(void);
void func_CCB4(void);
void func_CCB5(void);
void func_CCDA(void);
void func_CCFA(void);
void func_CD3B(void);
void func_CD57(void);
void func_CDFB(void);
void func_CE11(void);
void func_CEA4(void);
void func_CEAA(void);
void func_CED1(void);
void func_CF33(void);
void func_CF88(void);
void func_CFD1(void);
void func_D00E(void);
void func_D03F(void);
void func_D18E(void);
void func_D242(void);
void func_D2B7(void);
void func_D30F(void);
void func_D346(void);
void func_D39E(void);
void func_D3B3(void);
void func_D438(void);
void func_D45A(void);
void func_D45E(void);
void func_D477(void);
void func_D47D(void);
void func_D4CF(void);
void func_D4E5(void);
void func_D4E6(void);
void func_D4F2(void);
void func_D50F(void);
void func_D522(void);
void func_D55D(void);
void func_D55E(void);
void func_D567(void);
void func_D570(void);
void func_D58D(void);
void func_D5B7(void);
void func_D5F3(void);
void func_D61F(void);
void func_D641(void);
void func_D6D4(void);
void func_D6DF(void);
void func_D6E0(void);
void func_D70A(void);
void func_D777(void);
void func_D79A(void);
void func_D7A3(void);
void func_D7B8(void);
void func_D7F2(void);
void func_D82E(void);
void func_D83B(void);
void func_D886(void);
void func_D88C(void);
void func_D89A(void);
void func_D8A8(void);
void func_D8B7(void);
void func_D8E5(void);
void func_D93C(void);
void func_D986(void);
void func_D9A3(void);
void func_D9E9(void);
void func_D9FA(void);
void func_DA3F(void);
void func_DB03(void);
void func_DB2C(void);
void func_DB6C(void);
void func_DBD4(void);
void func_DBFD(void);
void func_DC29(void);
void func_DC6C(void);
void func_DCDD(void);
void func_DD6A(void);
void func_DD7F(void);
void func_DD8E(void);
void func_DDB7(void);
void func_DDC1(void);
void func_DDC4(void);
void func_DDD3(void);
void func_DDE2(void);
void func_DDFF(void);
void func_DE0E(void);
void func_DE33(void);
void func_DE5E(void);
void func_DF50(void);
void func_DFE9(void);
void func_E03B(void);
void func_E12F(void);
void func_E139(void);
void func_E143(void);
void func_E16B(void);
void func_E2A3(void);
void func_E3A3(void);
void func_E473(void);
void func_E4DF(void);
void func_E515(void);
void func_E521(void);
void func_E53F(void);
void func_E560(void);
void func_E60F(void);
void func_E61A(void);
void func_E653(void);
void func_E66E(void);
void func_E689(void);
void func_E6AB(void);
void func_E6DD(void);
void func_E7F5(void);
void func_E853(void);
void func_E856(void);
void func_E89B(void);
void func_E8BD(void);
void func_E954(void);
void func_E9BC(void);
void func_E9FE(void);
void func_EA6F(void);
void func_EC33(void);
void func_EC81(void);
void func_ECCF(void);
void func_ED49(void);
void func_EDBA(void);
void func_EEC4(void);
void func_EEDF(void);
void func_EF55(void);
void func_EF6D(void);
void func_EF83(void);
void func_EF9D(void);
void func_F000(void);
void func_F003(void);
void func_F03F(void);
void func_F048(void);
void func_F051(void);
void func_F054(void);
void func_F090(void);
void func_F099(void);
void func_F0A2(void);
void func_F0DC(void);
void func_F0F7(void);
void func_F119(void);
void func_F16E(void);
void func_F183(void);
void func_F1A1(void);
void func_F1BF(void);
void func_F1DF(void);
void func_F1FD(void);
void func_F20E(void);
void func_F22E(void);
void func_F24C(void);
void func_F26E(void);
void func_F288(void);
void func_F294(void);
void func_F2A0(void);
void func_F2BE(void);
void func_F314(void);
void func_F32E(void);
void func_F343(void);
void func_F34A(void);
void func_F34C(void);
void func_F365(void);
void func_F374(void);
void func_F38B(void);
void func_F406(void);
void func_F428(void);
void func_F430(void);
void func_F436(void);
void func_F43B(void);
void func_F440(void);
void func_F443(void);
void func_F449(void);
void func_F44F(void);
void func_F455(void);
void func_F4E1(void);
void func_F522(void);
void func_F558(void);
void func_F55A(void);
void func_F63F(void);
void func_F679(void);
void func_F6A9(void);
void func_F71A(void);
void func_F72A(void);
void func_F738(void);
void func_F7EA(void);
void func_F7EF(void);
void func_F7FF(void);
void func_F833(void);
void func_F83D(void);
void func_F84C(void);
void func_F8D5(void);
void func_F9E5(void);
void func_F9E9(void);
void func_FA84(void);
void func_FA91(void);
void func_FAC4(void);
void func_FB09(void);
void func_FB3E(void);
void func_FB5B(void);
void func_FB72(void);
void func_FBB0(void);
void func_FBD9(void);
void func_FE00(void);
void func_FEBA(void);
void func_FEBD(void);
void func_FF02(void);
void func_FF12(void);
void func_FF1A(void);
void func_FF24(void);
void func_FF26(void);
void func_FF2A(void);
void func_FF31(void);
void func_FF4C(void);
void func_FF4D(void);
void func_FF72(void);
void func_FF8E(void);
void func_8000_b0(void);
void func_8000_b2(void);
void func_8000_b3(void);
void func_8000_b4(void);
void func_8000_b6(void);
void func_8003_b3(void);
void func_8003_b6(void);
void func_8006_b3(void);
void func_800B_b6(void);
void func_8016_b2(void);
void func_8023_b3(void);
void func_8038_b4(void);
void func_803A_b3(void);
void func_805E_b6(void);
void func_8065_b0(void);
void func_8066_b0(void);
void func_806C_b3(void);
void func_807E_b0(void);
void func_8080_b4(void);
void func_8090_b0(void);
void func_80AE_b0(void);
void func_80C2_b4(void);
void func_80C5_b0(void);
void func_80D0_b6(void);
void func_80D3_b6(void);
void func_80D8_b3(void);
void func_80E3_b4(void);
void func_80E4_b4(void);
void func_80EC_b3(void);
void func_80FE_b3(void);
void func_8106_b3(void);
void func_8118_b3(void);
void func_8177_b0(void);
void func_817E_b0(void);
void func_8193_b0(void);
void func_819A_b6(void);
void func_81AE_b3(void);
void func_81B8_b6(void);
void func_81C5_b3(void);
void func_81C8_b3(void);
void func_81D4_b3(void);
void func_81E4_b3(void);
void func_81F0_b6(void);
void func_81F1_b3(void);
void func_821D_b0(void);
void func_821E_b3(void);
void func_821E_b6(void);
void func_8226_b3(void);
void func_822D_b3(void);
void func_822F_b6(void);
void func_8234_b3(void);
void func_8241_b6(void);
void func_8244_b6(void);
void func_824A_b3(void);
void func_8252_b3(void);
void func_8256_b6(void);
void func_825B_b3(void);
void func_827D_b6(void);
void func_82AF_b0(void);
void func_82DE_b3(void);
void func_8300_b0(void);
void func_8326_b3(void);
void func_8333_b3(void);
void func_8347_b6(void);
void func_8386_b3(void);
void func_8393_b3(void);
void func_83A1_b0(void);
void func_83A8_b6(void);
void func_83CD_b3(void);
void func_83DC_b0(void);
void func_8429_b0(void);
void func_8479_b0(void);
void func_847E_b3(void);
void func_8491_b3(void);
void func_8497_b0(void);
void func_8497_b3(void);
void func_849C_b0(void);
void func_84D9_b3(void);
void func_84DD_b3(void);
void func_84E1_b3(void);
void func_84E8_b3(void);
void func_84EF_b6(void);
void func_84F0_b6(void);
void func_84F1_b3(void);
void func_84FF_b3(void);
void func_8505_b3(void);
void func_8510_b3(void);
void func_8515_b3(void);
void func_851B_b3(void);
void func_851F_b3(void);
void func_8523_b3(void);
void func_8527_b3(void);
void func_8540_b0(void);
void func_8541_b0(void);
void func_855A_b3(void);
void func_8575_b3(void);
void func_8580_b3(void);
void func_8592_b3(void);
void func_8593_b6(void);
void func_85A3_b3(void);
void func_85AA_b0(void);
void func_85AE_b3(void);
void func_85BF_b0(void);
void func_85C8_b6(void);
void func_85DE_b3(void);
void func_85F0_b6(void);
void func_85F9_b0(void);
void func_85FE_b0(void);
void func_8618_b6(void);
void func_8636_b3(void);
void func_8638_b6(void);
void func_863D_b6(void);
void func_863E_b0(void);
void func_8642_b6(void);
void func_8643_b0(void);
void func_8644_b3(void);
void func_8647_b6(void);
void func_864C_b6(void);
void func_8651_b6(void);
void func_8656_b6(void);
void func_865A_b3(void);
void func_865B_b6(void);
void func_8660_b6(void);
void func_8665_b6(void);
void func_866A_b6(void);
void func_866F_b3(void);
void func_866F_b6(void);
void func_8674_b6(void);
void func_8679_b6(void);
void func_867E_b6(void);
void func_8683_b6(void);
void func_8684_b3(void);
void func_86A1_b3(void);
void func_86A7_b3(void);
void func_86AD_b3(void);
void func_86BA_b3(void);
void func_86D1_b3(void);
void func_86E6_b3(void);
void func_86FF_b0(void);
void func_8702_b3(void);
void func_871A_b3(void);
void func_8720_b3(void);
void func_87AA_b0(void);
void func_87E2_b0(void);
void func_87F7_b0(void);
void func_8832_b0(void);
void func_884E_b0(void);
void func_8864_b0(void);
void func_889F_b0(void);
void func_88A0_b3(void);
void func_88AA_b0(void);
void func_8900_b2(void);
void func_8900_b4(void);
void func_8914_b3(void);
void func_891B_b0(void);
void func_892C_b4(void);
void func_893C_b4(void);
void func_8940_b2(void);
void func_8955_b4(void);
void func_8956_b4(void);
void func_8992_b0(void);
void func_89C0_b2(void);
void func_89D2_b4(void);
void func_89D7_b4(void);
void func_89E3_b6(void);
void func_89F2_b6(void);
void func_89F6_b0(void);
void func_89F8_b6(void);
void func_8A08_b6(void);
void func_8A1E_b6(void);
void func_8A2C_b4(void);
void func_8A31_b0(void);
void func_8A31_b6(void);
void func_8A40_b2(void);
void func_8A43_b4(void);
void func_8A44_b4(void);
void func_8A53_b4(void);
void func_8A55_b0(void);
void func_8A62_b4(void);
void func_8A76_b4(void);
void func_8A8D_b4(void);
void func_8A9D_b4(void);
void func_8AAB_b4(void);
void func_8AAC_b4(void);
void func_8ABF_b4(void);
void func_8AC0_b2(void);
void func_8AD3_b0(void);
void func_8AD3_b4(void);
void func_8AEA_b4(void);
void func_8AEE_b6(void);
void func_8AF9_b4(void);
void func_8B00_b0(void);
void func_8B08_b4(void);
void func_8B15_b0(void);
void func_8B1C_b4(void);
void func_8B35_b4(void);
void func_8B40_b2(void);
void func_8B44_b4(void);
void func_8B58_b4(void);
void func_8B64_b6(void);
void func_8B68_b4(void);
void func_8B7C_b4(void);
void func_8B8F_b4(void);
void func_8B98_b0(void);
void func_8B9F_b4(void);
void func_8BB1_b0(void);
void func_8BB6_b4(void);
void func_8BC2_b6(void);
void func_8BC5_b4(void);
void func_8BD4_b4(void);
void func_8BD6_b6(void);
void func_8BE8_b4(void);
void func_8BED_b6(void);
void func_8BFF_b4(void);
void func_8C00_b2(void);
void func_8C00_b6(void);
void func_8C08_b6(void);
void func_8C0E_b4(void);
void func_8C1C_b6(void);
void func_8C1E_b4(void);
void func_8C32_b4(void);
void func_8C38_b0(void);
void func_8C3F_b6(void);
void func_8C43_b4(void);
void func_8C57_b4(void);
void func_8C6E_b4(void);
void func_8C7D_b4(void);
void func_8C8B_b4(void);
void func_8C8C_b4(void);
void func_8C98_b6(void);
void func_8CA5_b4(void);
void func_8CBD_b4(void);
void func_8CC4_b0(void);
void func_8CD2_b4(void);
void func_8CE1_b4(void);
void func_8CF5_b4(void);
void func_8D00_b2(void);
void func_8D00_b6(void);
void func_8D0C_b4(void);
void func_8D1B_b4(void);
void func_8D2A_b4(void);
void func_8D3A_b4(void);
void func_8D4B_b6(void);
void func_8D51_b4(void);
void func_8D60_b4(void);
void func_8D6F_b4(void);
void func_8D83_b4(void);
void func_8D88_b6(void);
void func_8D8D_b0(void);
void func_8D98_b4(void);
void func_8DA8_b4(void);
void func_8DD1_b0(void);
void func_8DD8_b2(void);
void func_8DEC_b2(void);
void func_8DEE_b6(void);
void func_8DEF_b6(void);
void func_8E3E_b0(void);
void func_8E43_b6(void);
void func_8E56_b2(void);
void func_8E5A_b6(void);
void func_8E65_b2(void);
void func_8EB4_b6(void);
void func_8ED4_b2(void);
void func_8EE5_b6(void);
void func_8EEA_b0(void);
void func_8EED_b2(void);
void func_8F07_b2(void);
void func_8F45_b6(void);
void func_8F82_b0(void);
void func_8F8D_b6(void);
void func_8FA5_b6(void);
void func_8FAF_b0(void);
void func_8FC6_b6(void);
void func_8FF8_b0(void);
void func_8FF9_b6(void);
void func_9000_b2(void);
void func_9000_b4(void);
void func_9011_b4(void);
void func_9036_b6(void);
void func_9038_b4(void);
void func_9043_b0(void);
void func_9072_b0(void);
void func_9078_b6(void);
void func_90AC_b0(void);
void func_90BA_b6(void);
void func_90CB_b0(void);
void func_9100_b2(void);
void func_9128_b6(void);
void func_914D_b0(void);
void func_916E_b0(void);
void func_917E_b6(void);
void func_9181_b2(void);
void func_9199_b6(void);
void func_91B2_b0(void);
void func_91B7_b6(void);
void func_91BE_b0(void);
void func_91BF_b2(void);
void func_921C_b4(void);
void func_9276_b6(void);
void func_9297_b0(void);
void func_92BA_b0(void);
void func_92E1_b0(void);
void func_92F0_b6(void);
void func_9300_b2(void);
void func_930B_b6(void);
void func_931D_b4(void);
void func_932E_b6(void);
void func_9332_b6(void);
void func_9357_b6(void);
void func_938A_b2(void);
void func_9398_b2(void);
void func_93AF_b0(void);
void func_93B0_b2(void);
void func_93D1_b6(void);
void func_93EF_b0(void);
void func_9410_b0(void);
void func_9413_b2(void);
void func_942A_b2(void);
void func_942E_b6(void);
void func_942F_b6(void);
void func_943F_b0(void);
void func_9457_b6(void);
void func_945E_b0(void);
void func_94B4_b6(void);
void func_94B5_b6(void);
void func_94D4_b6(void);
void func_94F2_b0(void);
void func_94F7_b0(void);
void func_9531_b6(void);
void func_9532_b6(void);
void func_9551_b6(void);
void func_9565_b0(void);
void func_958B_b0(void);
void func_95AE_b6(void);
void func_95AF_b6(void);
void func_95BA_b0(void);
void func_95CE_b6(void);
void func_95FA_b0(void);
void func_9600_b2(void);
void func_9602_b6(void);
void func_9610_b0(void);
void func_9615_b0(void);
void func_961A_b0(void);
void func_961F_b0(void);
void func_963D_b0(void);
void func_965C_b6(void);
void func_968D_b6(void);
void func_96C3_b4(void);
void func_96EB_b6(void);
void func_96EF_b0(void);
void func_96FC_b4(void);
void func_972A_b4(void);
void func_9730_b6(void);
void func_9733_b6(void);
void func_977F_b4(void);
void func_9787_b4(void);
void func_9788_b4(void);
void func_979F_b4(void);
void func_97A6_b4(void);
void func_97A9_b0(void);
void func_97AD_b4(void);
void func_97B4_b4(void);
void func_97C7_b4(void);
void func_97C8_b0(void);
void func_97CE_b4(void);
void func_97E4_b6(void);
void func_97E5_b4(void);
void func_97EC_b4(void);
void func_97F3_b4(void);
void func_97FA_b4(void);
void func_9800_b2(void);
void func_980F_b2(void);
void func_9813_b4(void);
void func_9818_b6(void);
void func_9819_b0(void);
void func_981A_b4(void);
void func_981B_b6(void);
void func_9821_b4(void);
void func_9828_b4(void);
void func_982F_b4(void);
void func_983B_b6(void);
void func_9842_b4(void);
void func_9849_b4(void);
void func_984F_b0(void);
void func_9860_b4(void);
void func_9867_b4(void);
void func_986E_b4(void);
void func_986E_b6(void);
void func_9875_b4(void);
void func_987A_b0(void);
void func_988C_b4(void);
void func_9893_b4(void);
void func_989A_b4(void);
void func_98A1_b4(void);
void func_98AF_b2(void);
void func_98B4_b4(void);
void func_98BB_b4(void);
void func_98BD_b2(void);
void func_98D2_b4(void);
void func_98D9_b4(void);
void func_98DD_b2(void);
void func_98DE_b6(void);
void func_98E0_b4(void);
void func_98E7_b4(void);
void func_98FA_b4(void);
void func_9901_b4(void);
void func_9912_b6(void);
void func_9918_b4(void);
void func_991F_b4(void);
void func_9926_b4(void);
void func_9947_b4(void);
void func_995E_b4(void);
void func_9962_b6(void);
void func_9965_b4(void);
void func_996C_b4(void);
void func_9973_b4(void);
void func_998A_b4(void);
void func_9991_b4(void);
void func_9998_b4(void);
void func_999B_b2(void);
void func_999F_b4(void);
void func_99B4_b4(void);
void func_99BB_b4(void);
void func_99D6_b2(void);
void func_99DD_b2(void);
void func_99FC_b6(void);
void func_9A00_b0(void);
void func_9A13_b0(void);
void func_9A1D_b0(void);
void func_9A2D_b6(void);
void func_9A3F_b6(void);
void func_9A74_b2(void);
void func_9A8F_b2(void);
void func_9A98_b0(void);
void func_9A9E_b2(void);
void func_9AA0_b2(void);
void func_9AC0_b0(void);
void func_9AC2_b2(void);
void func_9AD1_b2(void);
void func_9AD8_b6(void);
void func_9AD9_b2(void);
void func_9AF4_b2(void);
void func_9B04_b6(void);
void func_9B07_b6(void);
void func_9B16_b0(void);
void func_9B18_b2(void);
void func_9B1F_b6(void);
void func_9B62_b6(void);
void func_9B6B_b6(void);
void func_9B74_b6(void);
void func_9B7E_b2(void);
void func_9BA4_b2(void);
void func_9BE1_b6(void);
void func_9BFC_b6(void);
void func_9C00_b5(void);
void func_9C29_b6(void);
void func_9C55_b6(void);
void func_9C7A_b6(void);
void func_9CAD_b6(void);
void func_9CED_b6(void);
void func_9CF1_b6(void);
void func_9CF5_b6(void);
void func_9D71_b6(void);
void func_9D9E_b6(void);
void func_9DA4_b6(void);
void func_9DAA_b6(void);
void func_9E20_b6(void);
void func_9E24_b6(void);
void func_9E28_b6(void);
void func_9E2E_b6(void);
void func_9E32_b6(void);
void func_9E36_b6(void);
void func_9E3C_b6(void);
void func_9E40_b6(void);
void func_9E44_b6(void);
void func_9EC5_b6(void);
void func_9EC9_b6(void);
void func_9ECD_b6(void);
void func_9EF4_b6(void);
void func_9EF8_b6(void);
void func_9F01_b6(void);
void func_9F05_b6(void);
void func_9F0E_b6(void);
void func_9F12_b6(void);
void func_9FBB_b6(void);
void func_9FF6_b6(void);
void func_A000_b0(void);
void func_A000_b4(void);
void func_A003_b0(void);
void func_A006_b0(void);
void func_A009_b0(void);
void func_A00C_b0(void);
void func_A00F_b0(void);
void func_A012_b0(void);
void func_A013_b4(void);
void func_A015_b0(void);
void func_A018_b0(void);
void func_A01B_b0(void);
void func_A01E_b0(void);
void func_A021_b0(void);
void func_A024_b0(void);
void func_A027_b0(void);
void func_A02A_b0(void);
void func_A02D_b0(void);
void func_A030_b0(void);
void func_A031_b0(void);
void func_A04C_b6(void);
void func_A060_b6(void);
void func_A085_b6(void);
void func_A088_b6(void);
void func_A090_b0(void);
void func_A09B_b0(void);
void func_A0E8_b6(void);
void func_A0FD_b0(void);
void func_A101_b0(void);
void func_A102_b6(void);
void func_A139_b6(void);
void func_A13C_b0(void);
void func_A169_b0(void);
void func_A18B_b6(void);
void func_A18E_b6(void);
void func_A210_b6(void);
void func_A22A_b0(void);
void func_A28B_b0(void);
void func_A2A6_b6(void);
void func_A2C3_b6(void);
void func_A2ED_b0(void);
void func_A300_b4(void);
void func_A321_b4(void);
void func_A340_b6(void);
void func_A3EA_b0(void);
void func_A3F5_b6(void);
void func_A411_b6(void);
void func_A417_b0(void);
void func_A41D_b6(void);
void func_A473_b0(void);
void func_A494_b6(void);
void func_A512_b6(void);
void func_A53B_b0(void);
void func_A54C_b0(void);
void func_A54F_b6(void);
void func_A557_b0(void);
void func_A592_b6(void);
void func_A5CF_b0(void);
void func_A5DA_b6(void);
void func_A633_b0(void);
void func_A651_b0(void);
void func_A652_b6(void);
void func_A6D6_b0(void);
void func_A6FC_b0(void);
void func_A700_b2(void);
void func_A700_b4(void);
void func_A703_b2(void);
void func_A706_b2(void);
void func_A709_b2(void);
void func_A70C_b2(void);
void func_A70F_b2(void);
void func_A712_b2(void);
void func_A713_b4(void);
void func_A715_b2(void);
void func_A717_b6(void);
void func_A718_b2(void);
void func_A71B_b2(void);
void func_A71D_b4(void);
void func_A71E_b2(void);
void func_A721_b2(void);
void func_A724_b2(void);
void func_A727_b2(void);
void func_A72A_b2(void);
void func_A72D_b2(void);
void func_A730_b2(void);
void func_A731_b2(void);
void func_A732_b6(void);
void func_A736_b0(void);
void func_A73A_b6(void);
void func_A780_b6(void);
void func_A788_b4(void);
void func_A78E_b4(void);
void func_A7A5_b2(void);
void func_A7AD_b6(void);
void func_A7BB_b0(void);
void func_A7D0_b4(void);
void func_A7E1_b0(void);
void func_A82A_b2(void);
void func_A82C_b6(void);
void func_A83A_b0(void);
void func_A850_b2(void);
void func_A851_b2(void);
void func_A882_b0(void);
void func_A89C_b6(void);
void func_A8A8_b2(void);
void func_A8B7_b0(void);
void func_A8E4_b6(void);
void func_A8E8_b6(void);
void func_A8EF_b0(void);
void func_A8F3_b6(void);
void func_A916_b2(void);
void func_A918_b0(void);
void func_A953_b6(void);
void func_A957_b2(void);
void func_A971_b0(void);
void func_A995_b6(void);
void func_A9CA_b0(void);
void func_A9E3_b6(void);
void func_A9E4_b2(void);
void func_A9EB_b6(void);
void func_A9F5_b0(void);
void func_A9F6_b6(void);
void func_AA1D_b6(void);
void func_AA33_b0(void);
void func_AA60_b6(void);
void func_AA82_b2(void);
void func_AAA0_b6(void);
void func_AAAE_b6(void);
void func_AAD7_b6(void);
void func_AAD8_b0(void);
void func_AB00_b4(void);
void func_AB00_b6(void);
void func_AB08_b2(void);
void func_AB0A_b0(void);
void func_AB29_b6(void);
void func_AB34_b4(void);
void func_AB41_b2(void);
void func_AB4F_b6(void);
void func_AB77_b4(void);
void func_AB93_b0(void);
void func_AB9F_b6(void);
void func_ABB0_b0(void);
void func_ABB6_b0(void);
void func_ABC4_b2(void);
void func_ABD0_b4(void);
void func_ABDF_b4(void);
void func_ABE1_b2(void);
void func_AC13_b6(void);
void func_AC20_b4(void);
void func_AC34_b6(void);
void func_AC4B_b6(void);
void func_AC4E_b0(void);
void func_AC58_b2(void);
void func_AC60_b6(void);
void func_AC75_b4(void);
void func_AC75_b6(void);
void func_AC7D_b2(void);
void func_AC81_b4(void);
void func_AC8E_b6(void);
void func_ACB3_b6(void);
void func_ACB7_b4(void);
void func_ACC1_b0(void);
void func_ACE1_b2(void);
void func_ACE1_b4(void);
void func_AD0B_b0(void);
void func_AD0B_b4(void);
void func_AD37_b2(void);
void func_AD50_b0(void);
void func_AD58_b2(void);
void func_AD5E_b0(void);
void func_AD8C_b0(void);
void func_ADBA_b0(void);
void func_AE19_b0(void);
void func_B300_b1(void);
void func_B300_b5(void);
void func_B303_b1(void);
void func_B303_b5(void);
void func_B306_b1(void);
void func_B306_b5(void);
void func_B309_b1(void);
void func_B309_b5(void);
void func_B30C_b1(void);
void func_B30C_b5(void);
void func_B30F_b1(void);
void func_B30F_b5(void);
void func_B312_b1(void);
void func_B312_b5(void);
void func_B315_b1(void);
void func_B315_b5(void);
void func_B318_b1(void);
void func_B318_b5(void);
void func_B31B_b1(void);
void func_B31B_b5(void);
void func_B31E_b1(void);
void func_B31E_b5(void);
void func_B321_b1(void);
void func_B321_b5(void);
void func_B324_b1(void);
void func_B324_b5(void);
void func_B327_b1(void);
void func_B327_b5(void);
void func_B32A_b1(void);
void func_B32A_b5(void);
void func_B32D_b1(void);
void func_B32D_b5(void);
void func_B330_b1(void);
void func_B330_b5(void);
void func_B331_b1(void);
void func_B331_b5(void);
void func_B349_b5(void);
void func_B366_b5(void);
void func_B390_b1(void);
void func_B39B_b1(void);
void func_B3F4_b1(void);
void func_B403_b5(void);
void func_B411_b5(void);
void func_B46F_b5(void);
void func_B512_b5(void);
void func_B543_b5(void);
void func_B546_b1(void);
void func_B57B_b1(void);
void func_B581_b5(void);
void func_B5AF_b5(void);
void func_B5D7_b5(void);
void func_B615_b1(void);
void func_B625_b5(void);
void func_B660_b1(void);
void func_B666_b1(void);
void func_B696_b5(void);
void func_B6F5_b1(void);
void func_B743_b5(void);
void func_B7BD_b1(void);
void func_B7C9_b1(void);
void func_B80B_b5(void);
void func_B825_b5(void);
void func_B84B_b5(void);
void func_B86F_b5(void);
void func_B879_b1(void);
void func_B88C_b5(void);
void func_B8D3_b5(void);
void func_B912_b1(void);
void func_B925_b1(void);
void func_B929_b5(void);
void func_B958_b5(void);
void func_B977_b1(void);
void func_B97B_b5(void);
void func_B9B7_b5(void);
void func_BA01_b1(void);
void func_BA1C_b1(void);
void func_BA53_b1(void);
void func_BA68_b5(void);
void func_BA88_b1(void);
void func_BA9A_b5(void);
void func_BAD0_b1(void);
void func_BAD0_b5(void);
void func_BAE8_b5(void);
void func_BB05_b5(void);
void func_BB0A_b5(void);
void func_BB29_b1(void);
void func_BB53_b5(void);
void func_BB8C_b5(void);
void func_BB91_b1(void);
void func_BBAC_b5(void);
void func_BBCB_b1(void);
void func_BBE1_b5(void);
void func_BBEC_b5(void);
void func_BC6B_b5(void);
void func_BCC8_b5(void);
void func_BCD1_b1(void);
void func_BCD9_b5(void);
void func_BD46_b1(void);
void func_BD7C_b1(void);
void func_BD7F_b5(void);
void func_BD9B_b5(void);
void func_BDAF_b1(void);
void func_BDBF_b5(void);
void func_BDCE_b1(void);
void func_BDE6_b5(void);
void func_BE23_b5(void);
void func_BE72_b1(void);
void func_BE79_b5(void);
void func_BE9B_b1(void);
void func_BECE_b5(void);
void func_BED2_b1(void);
void func_BF2B_b5(void);
void func_BF38_b1(void);
void func_BF48_b1(void);
void func_BF5B_b5(void);
void func_BF75_b5(void);
void func_BF90_b5(void);
void func_BF95_b1(void);
void func_BFAF_b5(void);
void func_BFBE_b1(void);

int call_by_address_cb(uint16_t addr, int caller_bank) {
    extern int g_mmc3_win_bank8k[4];
    uint16_t cpu_addr = addr;
    int b8, bank;
    if (addr < 0x8000) return nes_interp_dispatch(addr);
    b8 = g_mmc3_win_bank8k[(addr >> 13) & 3];
    bank = b8 >> 1;
    (void)caller_bank;
    addr = (uint16_t)((bank == 7 ? 0xC000 : 0x8000) + ((b8 & 1) ? 0x2000 : 0) + (addr & 0x1FFF));
    switch (addr) {
    case 0xC000: func_C000(); break;
    case 0xC09D: func_C09D(); break;
    case 0xC119: func_C119(); break;
    case 0xC13B: func_C13B(); break;
    case 0xC14A: func_C14A(); break;
    case 0xC194: func_C194(); break;
    case 0xC1B1: func_C1B1(); break;
    case 0xC1D8: func_C1D8(); break;
    case 0xC1DF: func_C1DF(); break;
    case 0xC1FD: func_C1FD(); break;
    case 0xC201: func_C201(); break;
    case 0xC222: func_C222(); break;
    case 0xC265: func_C265(); break;
    case 0xC26F: func_C26F(); break;
    case 0xC279: func_C279(); break;
    case 0xC2D1: func_C2D1(); break;
    case 0xC31D: func_C31D(); break;
    case 0xC334: func_C334(); break;
    case 0xC362: func_C362(); break;
    case 0xC36E: func_C36E(); break;
    case 0xC3B4: func_C3B4(); break;
    case 0xC3DD: func_C3DD(); break;
    case 0xC44F: func_C44F(); break;
    case 0xC47D: func_C47D(); break;
    case 0xC4A5: func_C4A5(); break;
    case 0xC4BD: func_C4BD(); break;
    case 0xC527: func_C527(); break;
    case 0xC556: func_C556(); break;
    case 0xC5A3: func_C5A3(); break;
    case 0xC5CE: func_C5CE(); break;
    case 0xC681: func_C681(); break;
    case 0xC6D2: func_C6D2(); break;
    case 0xC844: func_C844(); break;
    case 0xC85C: func_C85C(); break;
    case 0xC85F: func_C85F(); break;
    case 0xC888: func_C888(); break;
    case 0xC8A6: func_C8A6(); break;
    case 0xC8B4: func_C8B4(); break;
    case 0xC8ED: func_C8ED(); break;
    case 0xC93B: func_C93B(); break;
    case 0xC987: func_C987(); break;
    case 0xC99D: func_C99D(); break;
    case 0xC9A7: func_C9A7(); break;
    case 0xC9B6: func_C9B6(); break;
    case 0xC9B9: func_C9B9(); break;
    case 0xCA20: func_CA20(); break;
    case 0xCA39: func_CA39(); break;
    case 0xCA3C: func_CA3C(); break;
    case 0xCAA2: func_CAA2(); break;
    case 0xCAC8: func_CAC8(); break;
    case 0xCAE5: func_CAE5(); break;
    case 0xCAF5: func_CAF5(); break;
    case 0xCB07: func_CB07(); break;
    case 0xCB22: func_CB22(); break;
    case 0xCB99: func_CB99(); break;
    case 0xCBA0: func_CBA0(); break;
    case 0xCBA3: func_CBA3(); break;
    case 0xCC85: func_CC85(); break;
    case 0xCCA2: func_CCA2(); break;
    case 0xCCB4: func_CCB4(); break;
    case 0xCCB5: func_CCB5(); break;
    case 0xCCDA: func_CCDA(); break;
    case 0xCCFA: func_CCFA(); break;
    case 0xCD3B: func_CD3B(); break;
    case 0xCD57: func_CD57(); break;
    case 0xCDFB: func_CDFB(); break;
    case 0xCE11: func_CE11(); break;
    case 0xCEA4: func_CEA4(); break;
    case 0xCEAA: func_CEAA(); break;
    case 0xCED1: func_CED1(); break;
    case 0xCF33: func_CF33(); break;
    case 0xCF88: func_CF88(); break;
    case 0xCFD1: func_CFD1(); break;
    case 0xD00E: func_D00E(); break;
    case 0xD03F: func_D03F(); break;
    case 0xD18E: func_D18E(); break;
    case 0xD242: func_D242(); break;
    case 0xD2B7: func_D2B7(); break;
    case 0xD30F: func_D30F(); break;
    case 0xD346: func_D346(); break;
    case 0xD39E: func_D39E(); break;
    case 0xD3B3: func_D3B3(); break;
    case 0xD438: func_D438(); break;
    case 0xD45A: func_D45A(); break;
    case 0xD45E: func_D45E(); break;
    case 0xD477: func_D477(); break;
    case 0xD47D: func_D47D(); break;
    case 0xD4CF: func_D4CF(); break;
    case 0xD4E5: func_D4E5(); break;
    case 0xD4E6: func_D4E6(); break;
    case 0xD4F2: func_D4F2(); break;
    case 0xD50F: func_D50F(); break;
    case 0xD522: func_D522(); break;
    case 0xD55D: func_D55D(); break;
    case 0xD55E: func_D55E(); break;
    case 0xD567: func_D567(); break;
    case 0xD570: func_D570(); break;
    case 0xD58D: func_D58D(); break;
    case 0xD5B7: func_D5B7(); break;
    case 0xD5F3: func_D5F3(); break;
    case 0xD61F: func_D61F(); break;
    case 0xD641: func_D641(); break;
    case 0xD6D4: func_D6D4(); break;
    case 0xD6DF: func_D6DF(); break;
    case 0xD6E0: func_D6E0(); break;
    case 0xD70A: func_D70A(); break;
    case 0xD777: func_D777(); break;
    case 0xD79A: func_D79A(); break;
    case 0xD7A3: func_D7A3(); break;
    case 0xD7B8: func_D7B8(); break;
    case 0xD7F2: func_D7F2(); break;
    case 0xD82E: func_D82E(); break;
    case 0xD83B: func_D83B(); break;
    case 0xD886: func_D886(); break;
    case 0xD88C: func_D88C(); break;
    case 0xD89A: func_D89A(); break;
    case 0xD8A8: func_D8A8(); break;
    case 0xD8B7: func_D8B7(); break;
    case 0xD8E5: func_D8E5(); break;
    case 0xD93C: func_D93C(); break;
    case 0xD986: func_D986(); break;
    case 0xD9A3: func_D9A3(); break;
    case 0xD9E9: func_D9E9(); break;
    case 0xD9FA: func_D9FA(); break;
    case 0xDA3F: func_DA3F(); break;
    case 0xDB03: func_DB03(); break;
    case 0xDB2C: func_DB2C(); break;
    case 0xDB6C: func_DB6C(); break;
    case 0xDBD4: func_DBD4(); break;
    case 0xDBFD: func_DBFD(); break;
    case 0xDC29: func_DC29(); break;
    case 0xDC6C: func_DC6C(); break;
    case 0xDCDD: func_DCDD(); break;
    case 0xDD6A: func_DD6A(); break;
    case 0xDD7F: func_DD7F(); break;
    case 0xDD8E: func_DD8E(); break;
    case 0xDDB7: func_DDB7(); break;
    case 0xDDC1: func_DDC1(); break;
    case 0xDDC4: func_DDC4(); break;
    case 0xDDD3: func_DDD3(); break;
    case 0xDDE2: func_DDE2(); break;
    case 0xDDFF: func_DDFF(); break;
    case 0xDE0E: func_DE0E(); break;
    case 0xDE33: func_DE33(); break;
    case 0xDE5E: func_DE5E(); break;
    case 0xDF50: func_DF50(); break;
    case 0xDFE9: func_DFE9(); break;
    case 0xE03B: func_E03B(); break;
    case 0xE12F: func_E12F(); break;
    case 0xE139: func_E139(); break;
    case 0xE143: func_E143(); break;
    case 0xE16B: func_E16B(); break;
    case 0xE2A3: func_E2A3(); break;
    case 0xE3A3: func_E3A3(); break;
    case 0xE473: func_E473(); break;
    case 0xE4DF: func_E4DF(); break;
    case 0xE515: func_E515(); break;
    case 0xE521: func_E521(); break;
    case 0xE53F: func_E53F(); break;
    case 0xE560: func_E560(); break;
    case 0xE60F: func_E60F(); break;
    case 0xE61A: func_E61A(); break;
    case 0xE653: func_E653(); break;
    case 0xE66E: func_E66E(); break;
    case 0xE689: func_E689(); break;
    case 0xE6AB: func_E6AB(); break;
    case 0xE6DD: func_E6DD(); break;
    case 0xE7F5: func_E7F5(); break;
    case 0xE853: func_E853(); break;
    case 0xE856: func_E856(); break;
    case 0xE89B: func_E89B(); break;
    case 0xE8BD: func_E8BD(); break;
    case 0xE954: func_E954(); break;
    case 0xE9BC: func_E9BC(); break;
    case 0xE9FE: func_E9FE(); break;
    case 0xEA6F: func_EA6F(); break;
    case 0xEC33: func_EC33(); break;
    case 0xEC81: func_EC81(); break;
    case 0xECCF: func_ECCF(); break;
    case 0xED49: func_ED49(); break;
    case 0xEDBA: func_EDBA(); break;
    case 0xEEC4: func_EEC4(); break;
    case 0xEEDF: func_EEDF(); break;
    case 0xEF55: func_EF55(); break;
    case 0xEF6D: func_EF6D(); break;
    case 0xEF83: func_EF83(); break;
    case 0xEF9D: func_EF9D(); break;
    case 0xF000: func_F000(); break;
    case 0xF003: func_F003(); break;
    case 0xF03F: func_F03F(); break;
    case 0xF048: func_F048(); break;
    case 0xF051: func_F051(); break;
    case 0xF054: func_F054(); break;
    case 0xF090: func_F090(); break;
    case 0xF099: func_F099(); break;
    case 0xF0A2: func_F0A2(); break;
    case 0xF0DC: func_F0DC(); break;
    case 0xF0F7: func_F0F7(); break;
    case 0xF119: func_F119(); break;
    case 0xF16E: func_F16E(); break;
    case 0xF183: func_F183(); break;
    case 0xF1A1: func_F1A1(); break;
    case 0xF1BF: func_F1BF(); break;
    case 0xF1DF: func_F1DF(); break;
    case 0xF1FD: func_F1FD(); break;
    case 0xF20E: func_F20E(); break;
    case 0xF22E: func_F22E(); break;
    case 0xF24C: func_F24C(); break;
    case 0xF26E: func_F26E(); break;
    case 0xF288: func_F288(); break;
    case 0xF294: func_F294(); break;
    case 0xF2A0: func_F2A0(); break;
    case 0xF2BE: func_F2BE(); break;
    case 0xF314: func_F314(); break;
    case 0xF32E: func_F32E(); break;
    case 0xF343: func_F343(); break;
    case 0xF34A: func_F34A(); break;
    case 0xF34C: func_F34C(); break;
    case 0xF365: func_F365(); break;
    case 0xF374: func_F374(); break;
    case 0xF38B: func_F38B(); break;
    case 0xF406: func_F406(); break;
    case 0xF428: func_F428(); break;
    case 0xF430: func_F430(); break;
    case 0xF436: func_F436(); break;
    case 0xF43B: func_F43B(); break;
    case 0xF440: func_F440(); break;
    case 0xF443: func_F443(); break;
    case 0xF449: func_F449(); break;
    case 0xF44F: func_F44F(); break;
    case 0xF455: func_F455(); break;
    case 0xF4E1: func_F4E1(); break;
    case 0xF522: func_F522(); break;
    case 0xF558: func_F558(); break;
    case 0xF55A: func_F55A(); break;
    case 0xF63F: func_F63F(); break;
    case 0xF679: func_F679(); break;
    case 0xF6A9: func_F6A9(); break;
    case 0xF71A: func_F71A(); break;
    case 0xF72A: func_F72A(); break;
    case 0xF738: func_F738(); break;
    case 0xF7EA: func_F7EA(); break;
    case 0xF7EF: func_F7EF(); break;
    case 0xF7FF: func_F7FF(); break;
    case 0xF833: func_F833(); break;
    case 0xF83D: func_F83D(); break;
    case 0xF84C: func_F84C(); break;
    case 0xF8D5: func_F8D5(); break;
    case 0xF9E5: func_F9E5(); break;
    case 0xF9E9: func_F9E9(); break;
    case 0xFA84: func_FA84(); break;
    case 0xFA91: func_FA91(); break;
    case 0xFAC4: func_FAC4(); break;
    case 0xFB09: func_FB09(); break;
    case 0xFB3E: func_FB3E(); break;
    case 0xFB5B: func_FB5B(); break;
    case 0xFB72: func_FB72(); break;
    case 0xFBB0: func_FBB0(); break;
    case 0xFBD9: func_FBD9(); break;
    case 0xFE00: func_FE00(); break;
    case 0xFEBA: func_FEBA(); break;
    case 0xFEBD: func_FEBD(); break;
    case 0xFF02: func_FF02(); break;
    case 0xFF12: func_FF12(); break;
    case 0xFF1A: func_FF1A(); break;
    case 0xFF24: func_FF24(); break;
    case 0xFF26: func_FF26(); break;
    case 0xFF2A: func_FF2A(); break;
    case 0xFF31: func_FF31(); break;
    case 0xFF4C: func_FF4C(); break;
    case 0xFF4D: func_FF4D(); break;
    case 0xFF72: func_FF72(); break;
    case 0xFF8E: func_FF8E(); break;
    case 0x8000:
        switch (bank) {
        case 0: func_8000_b0(); break;
        case 2: func_8000_b2(); break;
        case 3: func_8000_b3(); break;
        case 4: func_8000_b4(); break;
        case 6: func_8000_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8003:
        switch (bank) {
        case 3: func_8003_b3(); break;
        case 6: func_8003_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8006:
        switch (bank) {
        case 3: func_8006_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x800B:
        switch (bank) {
        case 6: func_800B_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8016:
        switch (bank) {
        case 2: func_8016_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8023:
        switch (bank) {
        case 3: func_8023_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8038:
        switch (bank) {
        case 4: func_8038_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x803A:
        switch (bank) {
        case 3: func_803A_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x805E:
        switch (bank) {
        case 6: func_805E_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8065:
        switch (bank) {
        case 0: func_8065_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8066:
        switch (bank) {
        case 0: func_8066_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x806C:
        switch (bank) {
        case 3: func_806C_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x807E:
        switch (bank) {
        case 0: func_807E_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8080:
        switch (bank) {
        case 4: func_8080_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8090:
        switch (bank) {
        case 0: func_8090_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x80AE:
        switch (bank) {
        case 0: func_80AE_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x80C2:
        switch (bank) {
        case 4: func_80C2_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x80C5:
        switch (bank) {
        case 0: func_80C5_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x80D0:
        switch (bank) {
        case 6: func_80D0_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x80D3:
        switch (bank) {
        case 6: func_80D3_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x80D8:
        switch (bank) {
        case 3: func_80D8_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x80E3:
        switch (bank) {
        case 4: func_80E3_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x80E4:
        switch (bank) {
        case 4: func_80E4_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x80EC:
        switch (bank) {
        case 3: func_80EC_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x80FE:
        switch (bank) {
        case 3: func_80FE_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8106:
        switch (bank) {
        case 3: func_8106_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8118:
        switch (bank) {
        case 3: func_8118_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8177:
        switch (bank) {
        case 0: func_8177_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x817E:
        switch (bank) {
        case 0: func_817E_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8193:
        switch (bank) {
        case 0: func_8193_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x819A:
        switch (bank) {
        case 6: func_819A_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x81AE:
        switch (bank) {
        case 3: func_81AE_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x81B8:
        switch (bank) {
        case 6: func_81B8_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x81C5:
        switch (bank) {
        case 3: func_81C5_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x81C8:
        switch (bank) {
        case 3: func_81C8_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x81D4:
        switch (bank) {
        case 3: func_81D4_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x81E4:
        switch (bank) {
        case 3: func_81E4_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x81F0:
        switch (bank) {
        case 6: func_81F0_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x81F1:
        switch (bank) {
        case 3: func_81F1_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x821D:
        switch (bank) {
        case 0: func_821D_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x821E:
        switch (bank) {
        case 3: func_821E_b3(); break;
        case 6: func_821E_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8226:
        switch (bank) {
        case 3: func_8226_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x822D:
        switch (bank) {
        case 3: func_822D_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x822F:
        switch (bank) {
        case 6: func_822F_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8234:
        switch (bank) {
        case 3: func_8234_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8241:
        switch (bank) {
        case 6: func_8241_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8244:
        switch (bank) {
        case 6: func_8244_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x824A:
        switch (bank) {
        case 3: func_824A_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8252:
        switch (bank) {
        case 3: func_8252_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8256:
        switch (bank) {
        case 6: func_8256_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x825B:
        switch (bank) {
        case 3: func_825B_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x827D:
        switch (bank) {
        case 6: func_827D_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x82AF:
        switch (bank) {
        case 0: func_82AF_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x82DE:
        switch (bank) {
        case 3: func_82DE_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8300:
        switch (bank) {
        case 0: func_8300_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8326:
        switch (bank) {
        case 3: func_8326_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8333:
        switch (bank) {
        case 3: func_8333_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8347:
        switch (bank) {
        case 6: func_8347_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8386:
        switch (bank) {
        case 3: func_8386_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8393:
        switch (bank) {
        case 3: func_8393_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x83A1:
        switch (bank) {
        case 0: func_83A1_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x83A8:
        switch (bank) {
        case 6: func_83A8_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x83CD:
        switch (bank) {
        case 3: func_83CD_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x83DC:
        switch (bank) {
        case 0: func_83DC_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8429:
        switch (bank) {
        case 0: func_8429_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8479:
        switch (bank) {
        case 0: func_8479_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x847E:
        switch (bank) {
        case 3: func_847E_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8491:
        switch (bank) {
        case 3: func_8491_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8497:
        switch (bank) {
        case 0: func_8497_b0(); break;
        case 3: func_8497_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x849C:
        switch (bank) {
        case 0: func_849C_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x84D9:
        switch (bank) {
        case 3: func_84D9_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x84DD:
        switch (bank) {
        case 3: func_84DD_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x84E1:
        switch (bank) {
        case 3: func_84E1_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x84E8:
        switch (bank) {
        case 3: func_84E8_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x84EF:
        switch (bank) {
        case 6: func_84EF_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x84F0:
        switch (bank) {
        case 6: func_84F0_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x84F1:
        switch (bank) {
        case 3: func_84F1_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x84FF:
        switch (bank) {
        case 3: func_84FF_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8505:
        switch (bank) {
        case 3: func_8505_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8510:
        switch (bank) {
        case 3: func_8510_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8515:
        switch (bank) {
        case 3: func_8515_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x851B:
        switch (bank) {
        case 3: func_851B_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x851F:
        switch (bank) {
        case 3: func_851F_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8523:
        switch (bank) {
        case 3: func_8523_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8527:
        switch (bank) {
        case 3: func_8527_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8540:
        switch (bank) {
        case 0: func_8540_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8541:
        switch (bank) {
        case 0: func_8541_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x855A:
        switch (bank) {
        case 3: func_855A_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8575:
        switch (bank) {
        case 3: func_8575_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8580:
        switch (bank) {
        case 3: func_8580_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8592:
        switch (bank) {
        case 3: func_8592_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8593:
        switch (bank) {
        case 6: func_8593_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x85A3:
        switch (bank) {
        case 3: func_85A3_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x85AA:
        switch (bank) {
        case 0: func_85AA_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x85AE:
        switch (bank) {
        case 3: func_85AE_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x85BF:
        switch (bank) {
        case 0: func_85BF_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x85C8:
        switch (bank) {
        case 6: func_85C8_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x85DE:
        switch (bank) {
        case 3: func_85DE_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x85F0:
        switch (bank) {
        case 6: func_85F0_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x85F9:
        switch (bank) {
        case 0: func_85F9_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x85FE:
        switch (bank) {
        case 0: func_85FE_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8618:
        switch (bank) {
        case 6: func_8618_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8636:
        switch (bank) {
        case 3: func_8636_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8638:
        switch (bank) {
        case 6: func_8638_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x863D:
        switch (bank) {
        case 6: func_863D_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x863E:
        switch (bank) {
        case 0: func_863E_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8642:
        switch (bank) {
        case 6: func_8642_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8643:
        switch (bank) {
        case 0: func_8643_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8644:
        switch (bank) {
        case 3: func_8644_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8647:
        switch (bank) {
        case 6: func_8647_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x864C:
        switch (bank) {
        case 6: func_864C_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8651:
        switch (bank) {
        case 6: func_8651_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8656:
        switch (bank) {
        case 6: func_8656_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x865A:
        switch (bank) {
        case 3: func_865A_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x865B:
        switch (bank) {
        case 6: func_865B_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8660:
        switch (bank) {
        case 6: func_8660_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8665:
        switch (bank) {
        case 6: func_8665_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x866A:
        switch (bank) {
        case 6: func_866A_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x866F:
        switch (bank) {
        case 3: func_866F_b3(); break;
        case 6: func_866F_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8674:
        switch (bank) {
        case 6: func_8674_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8679:
        switch (bank) {
        case 6: func_8679_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x867E:
        switch (bank) {
        case 6: func_867E_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8683:
        switch (bank) {
        case 6: func_8683_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8684:
        switch (bank) {
        case 3: func_8684_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x86A1:
        switch (bank) {
        case 3: func_86A1_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x86A7:
        switch (bank) {
        case 3: func_86A7_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x86AD:
        switch (bank) {
        case 3: func_86AD_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x86BA:
        switch (bank) {
        case 3: func_86BA_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x86D1:
        switch (bank) {
        case 3: func_86D1_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x86E6:
        switch (bank) {
        case 3: func_86E6_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x86FF:
        switch (bank) {
        case 0: func_86FF_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8702:
        switch (bank) {
        case 3: func_8702_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x871A:
        switch (bank) {
        case 3: func_871A_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8720:
        switch (bank) {
        case 3: func_8720_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x87AA:
        switch (bank) {
        case 0: func_87AA_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x87E2:
        switch (bank) {
        case 0: func_87E2_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x87F7:
        switch (bank) {
        case 0: func_87F7_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8832:
        switch (bank) {
        case 0: func_8832_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x884E:
        switch (bank) {
        case 0: func_884E_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8864:
        switch (bank) {
        case 0: func_8864_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x889F:
        switch (bank) {
        case 0: func_889F_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x88A0:
        switch (bank) {
        case 3: func_88A0_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x88AA:
        switch (bank) {
        case 0: func_88AA_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8900:
        switch (bank) {
        case 2: func_8900_b2(); break;
        case 4: func_8900_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8914:
        switch (bank) {
        case 3: func_8914_b3(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x891B:
        switch (bank) {
        case 0: func_891B_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x892C:
        switch (bank) {
        case 4: func_892C_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x893C:
        switch (bank) {
        case 4: func_893C_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8940:
        switch (bank) {
        case 2: func_8940_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8955:
        switch (bank) {
        case 4: func_8955_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8956:
        switch (bank) {
        case 4: func_8956_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8992:
        switch (bank) {
        case 0: func_8992_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x89C0:
        switch (bank) {
        case 2: func_89C0_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x89D2:
        switch (bank) {
        case 4: func_89D2_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x89D7:
        switch (bank) {
        case 4: func_89D7_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x89E3:
        switch (bank) {
        case 6: func_89E3_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x89F2:
        switch (bank) {
        case 6: func_89F2_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x89F6:
        switch (bank) {
        case 0: func_89F6_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x89F8:
        switch (bank) {
        case 6: func_89F8_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8A08:
        switch (bank) {
        case 6: func_8A08_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8A1E:
        switch (bank) {
        case 6: func_8A1E_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8A2C:
        switch (bank) {
        case 4: func_8A2C_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8A31:
        switch (bank) {
        case 0: func_8A31_b0(); break;
        case 6: func_8A31_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8A40:
        switch (bank) {
        case 2: func_8A40_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8A43:
        switch (bank) {
        case 4: func_8A43_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8A44:
        switch (bank) {
        case 4: func_8A44_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8A53:
        switch (bank) {
        case 4: func_8A53_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8A55:
        switch (bank) {
        case 0: func_8A55_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8A62:
        switch (bank) {
        case 4: func_8A62_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8A76:
        switch (bank) {
        case 4: func_8A76_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8A8D:
        switch (bank) {
        case 4: func_8A8D_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8A9D:
        switch (bank) {
        case 4: func_8A9D_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8AAB:
        switch (bank) {
        case 4: func_8AAB_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8AAC:
        switch (bank) {
        case 4: func_8AAC_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8ABF:
        switch (bank) {
        case 4: func_8ABF_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8AC0:
        switch (bank) {
        case 2: func_8AC0_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8AD3:
        switch (bank) {
        case 0: func_8AD3_b0(); break;
        case 4: func_8AD3_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8AEA:
        switch (bank) {
        case 4: func_8AEA_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8AEE:
        switch (bank) {
        case 6: func_8AEE_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8AF9:
        switch (bank) {
        case 4: func_8AF9_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8B00:
        switch (bank) {
        case 0: func_8B00_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8B08:
        switch (bank) {
        case 4: func_8B08_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8B15:
        switch (bank) {
        case 0: func_8B15_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8B1C:
        switch (bank) {
        case 4: func_8B1C_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8B35:
        switch (bank) {
        case 4: func_8B35_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8B40:
        switch (bank) {
        case 2: func_8B40_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8B44:
        switch (bank) {
        case 4: func_8B44_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8B58:
        switch (bank) {
        case 4: func_8B58_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8B64:
        switch (bank) {
        case 6: func_8B64_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8B68:
        switch (bank) {
        case 4: func_8B68_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8B7C:
        switch (bank) {
        case 4: func_8B7C_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8B8F:
        switch (bank) {
        case 4: func_8B8F_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8B98:
        switch (bank) {
        case 0: func_8B98_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8B9F:
        switch (bank) {
        case 4: func_8B9F_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8BB1:
        switch (bank) {
        case 0: func_8BB1_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8BB6:
        switch (bank) {
        case 4: func_8BB6_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8BC2:
        switch (bank) {
        case 6: func_8BC2_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8BC5:
        switch (bank) {
        case 4: func_8BC5_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8BD4:
        switch (bank) {
        case 4: func_8BD4_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8BD6:
        switch (bank) {
        case 6: func_8BD6_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8BE8:
        switch (bank) {
        case 4: func_8BE8_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8BED:
        switch (bank) {
        case 6: func_8BED_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8BFF:
        switch (bank) {
        case 4: func_8BFF_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8C00:
        switch (bank) {
        case 2: func_8C00_b2(); break;
        case 6: func_8C00_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8C08:
        switch (bank) {
        case 6: func_8C08_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8C0E:
        switch (bank) {
        case 4: func_8C0E_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8C1C:
        switch (bank) {
        case 6: func_8C1C_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8C1E:
        switch (bank) {
        case 4: func_8C1E_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8C32:
        switch (bank) {
        case 4: func_8C32_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8C38:
        switch (bank) {
        case 0: func_8C38_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8C3F:
        switch (bank) {
        case 6: func_8C3F_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8C43:
        switch (bank) {
        case 4: func_8C43_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8C57:
        switch (bank) {
        case 4: func_8C57_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8C6E:
        switch (bank) {
        case 4: func_8C6E_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8C7D:
        switch (bank) {
        case 4: func_8C7D_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8C8B:
        switch (bank) {
        case 4: func_8C8B_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8C8C:
        switch (bank) {
        case 4: func_8C8C_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8C98:
        switch (bank) {
        case 6: func_8C98_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8CA5:
        switch (bank) {
        case 4: func_8CA5_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8CBD:
        switch (bank) {
        case 4: func_8CBD_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8CC4:
        switch (bank) {
        case 0: func_8CC4_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8CD2:
        switch (bank) {
        case 4: func_8CD2_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8CE1:
        switch (bank) {
        case 4: func_8CE1_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8CF5:
        switch (bank) {
        case 4: func_8CF5_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8D00:
        switch (bank) {
        case 2: func_8D00_b2(); break;
        case 6: func_8D00_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8D0C:
        switch (bank) {
        case 4: func_8D0C_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8D1B:
        switch (bank) {
        case 4: func_8D1B_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8D2A:
        switch (bank) {
        case 4: func_8D2A_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8D3A:
        switch (bank) {
        case 4: func_8D3A_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8D4B:
        switch (bank) {
        case 6: func_8D4B_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8D51:
        switch (bank) {
        case 4: func_8D51_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8D60:
        switch (bank) {
        case 4: func_8D60_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8D6F:
        switch (bank) {
        case 4: func_8D6F_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8D83:
        switch (bank) {
        case 4: func_8D83_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8D88:
        switch (bank) {
        case 6: func_8D88_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8D8D:
        switch (bank) {
        case 0: func_8D8D_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8D98:
        switch (bank) {
        case 4: func_8D98_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8DA8:
        switch (bank) {
        case 4: func_8DA8_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8DD1:
        switch (bank) {
        case 0: func_8DD1_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8DD8:
        switch (bank) {
        case 2: func_8DD8_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8DEC:
        switch (bank) {
        case 2: func_8DEC_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8DEE:
        switch (bank) {
        case 6: func_8DEE_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8DEF:
        switch (bank) {
        case 6: func_8DEF_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8E3E:
        switch (bank) {
        case 0: func_8E3E_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8E43:
        switch (bank) {
        case 6: func_8E43_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8E56:
        switch (bank) {
        case 2: func_8E56_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8E5A:
        switch (bank) {
        case 6: func_8E5A_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8E65:
        switch (bank) {
        case 2: func_8E65_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8EB4:
        switch (bank) {
        case 6: func_8EB4_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8ED4:
        switch (bank) {
        case 2: func_8ED4_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8EE5:
        switch (bank) {
        case 6: func_8EE5_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8EEA:
        switch (bank) {
        case 0: func_8EEA_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8EED:
        switch (bank) {
        case 2: func_8EED_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8F07:
        switch (bank) {
        case 2: func_8F07_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8F45:
        switch (bank) {
        case 6: func_8F45_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8F82:
        switch (bank) {
        case 0: func_8F82_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8F8D:
        switch (bank) {
        case 6: func_8F8D_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8FA5:
        switch (bank) {
        case 6: func_8FA5_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8FAF:
        switch (bank) {
        case 0: func_8FAF_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8FC6:
        switch (bank) {
        case 6: func_8FC6_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8FF8:
        switch (bank) {
        case 0: func_8FF8_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x8FF9:
        switch (bank) {
        case 6: func_8FF9_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9000:
        switch (bank) {
        case 2: func_9000_b2(); break;
        case 4: func_9000_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9011:
        switch (bank) {
        case 4: func_9011_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9036:
        switch (bank) {
        case 6: func_9036_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9038:
        switch (bank) {
        case 4: func_9038_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9043:
        switch (bank) {
        case 0: func_9043_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9072:
        switch (bank) {
        case 0: func_9072_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9078:
        switch (bank) {
        case 6: func_9078_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x90AC:
        switch (bank) {
        case 0: func_90AC_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x90BA:
        switch (bank) {
        case 6: func_90BA_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x90CB:
        switch (bank) {
        case 0: func_90CB_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9100:
        switch (bank) {
        case 2: func_9100_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9128:
        switch (bank) {
        case 6: func_9128_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x914D:
        switch (bank) {
        case 0: func_914D_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x916E:
        switch (bank) {
        case 0: func_916E_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x917E:
        switch (bank) {
        case 6: func_917E_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9181:
        switch (bank) {
        case 2: func_9181_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9199:
        switch (bank) {
        case 6: func_9199_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x91B2:
        switch (bank) {
        case 0: func_91B2_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x91B7:
        switch (bank) {
        case 6: func_91B7_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x91BE:
        switch (bank) {
        case 0: func_91BE_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x91BF:
        switch (bank) {
        case 2: func_91BF_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x921C:
        switch (bank) {
        case 4: func_921C_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9276:
        switch (bank) {
        case 6: func_9276_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9297:
        switch (bank) {
        case 0: func_9297_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x92BA:
        switch (bank) {
        case 0: func_92BA_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x92E1:
        switch (bank) {
        case 0: func_92E1_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x92F0:
        switch (bank) {
        case 6: func_92F0_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9300:
        switch (bank) {
        case 2: func_9300_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x930B:
        switch (bank) {
        case 6: func_930B_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x931D:
        switch (bank) {
        case 4: func_931D_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x932E:
        switch (bank) {
        case 6: func_932E_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9332:
        switch (bank) {
        case 6: func_9332_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9357:
        switch (bank) {
        case 6: func_9357_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x938A:
        switch (bank) {
        case 2: func_938A_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9398:
        switch (bank) {
        case 2: func_9398_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x93AF:
        switch (bank) {
        case 0: func_93AF_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x93B0:
        switch (bank) {
        case 2: func_93B0_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x93D1:
        switch (bank) {
        case 6: func_93D1_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x93EF:
        switch (bank) {
        case 0: func_93EF_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9410:
        switch (bank) {
        case 0: func_9410_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9413:
        switch (bank) {
        case 2: func_9413_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x942A:
        switch (bank) {
        case 2: func_942A_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x942E:
        switch (bank) {
        case 6: func_942E_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x942F:
        switch (bank) {
        case 6: func_942F_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x943F:
        switch (bank) {
        case 0: func_943F_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9457:
        switch (bank) {
        case 6: func_9457_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x945E:
        switch (bank) {
        case 0: func_945E_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x94B4:
        switch (bank) {
        case 6: func_94B4_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x94B5:
        switch (bank) {
        case 6: func_94B5_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x94D4:
        switch (bank) {
        case 6: func_94D4_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x94F2:
        switch (bank) {
        case 0: func_94F2_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x94F7:
        switch (bank) {
        case 0: func_94F7_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9531:
        switch (bank) {
        case 6: func_9531_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9532:
        switch (bank) {
        case 6: func_9532_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9551:
        switch (bank) {
        case 6: func_9551_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9565:
        switch (bank) {
        case 0: func_9565_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x958B:
        switch (bank) {
        case 0: func_958B_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x95AE:
        switch (bank) {
        case 6: func_95AE_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x95AF:
        switch (bank) {
        case 6: func_95AF_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x95BA:
        switch (bank) {
        case 0: func_95BA_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x95CE:
        switch (bank) {
        case 6: func_95CE_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x95FA:
        switch (bank) {
        case 0: func_95FA_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9600:
        switch (bank) {
        case 2: func_9600_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9602:
        switch (bank) {
        case 6: func_9602_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9610:
        switch (bank) {
        case 0: func_9610_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9615:
        switch (bank) {
        case 0: func_9615_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x961A:
        switch (bank) {
        case 0: func_961A_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x961F:
        switch (bank) {
        case 0: func_961F_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x963D:
        switch (bank) {
        case 0: func_963D_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x965C:
        switch (bank) {
        case 6: func_965C_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x968D:
        switch (bank) {
        case 6: func_968D_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x96C3:
        switch (bank) {
        case 4: func_96C3_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x96EB:
        switch (bank) {
        case 6: func_96EB_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x96EF:
        switch (bank) {
        case 0: func_96EF_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x96FC:
        switch (bank) {
        case 4: func_96FC_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x972A:
        switch (bank) {
        case 4: func_972A_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9730:
        switch (bank) {
        case 6: func_9730_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9733:
        switch (bank) {
        case 6: func_9733_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x977F:
        switch (bank) {
        case 4: func_977F_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9787:
        switch (bank) {
        case 4: func_9787_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9788:
        switch (bank) {
        case 4: func_9788_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x979F:
        switch (bank) {
        case 4: func_979F_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x97A6:
        switch (bank) {
        case 4: func_97A6_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x97A9:
        switch (bank) {
        case 0: func_97A9_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x97AD:
        switch (bank) {
        case 4: func_97AD_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x97B4:
        switch (bank) {
        case 4: func_97B4_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x97C7:
        switch (bank) {
        case 4: func_97C7_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x97C8:
        switch (bank) {
        case 0: func_97C8_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x97CE:
        switch (bank) {
        case 4: func_97CE_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x97E4:
        switch (bank) {
        case 6: func_97E4_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x97E5:
        switch (bank) {
        case 4: func_97E5_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x97EC:
        switch (bank) {
        case 4: func_97EC_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x97F3:
        switch (bank) {
        case 4: func_97F3_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x97FA:
        switch (bank) {
        case 4: func_97FA_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9800:
        switch (bank) {
        case 2: func_9800_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x980F:
        switch (bank) {
        case 2: func_980F_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9813:
        switch (bank) {
        case 4: func_9813_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9818:
        switch (bank) {
        case 6: func_9818_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9819:
        switch (bank) {
        case 0: func_9819_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x981A:
        switch (bank) {
        case 4: func_981A_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x981B:
        switch (bank) {
        case 6: func_981B_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9821:
        switch (bank) {
        case 4: func_9821_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9828:
        switch (bank) {
        case 4: func_9828_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x982F:
        switch (bank) {
        case 4: func_982F_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x983B:
        switch (bank) {
        case 6: func_983B_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9842:
        switch (bank) {
        case 4: func_9842_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9849:
        switch (bank) {
        case 4: func_9849_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x984F:
        switch (bank) {
        case 0: func_984F_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9860:
        switch (bank) {
        case 4: func_9860_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9867:
        switch (bank) {
        case 4: func_9867_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x986E:
        switch (bank) {
        case 4: func_986E_b4(); break;
        case 6: func_986E_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9875:
        switch (bank) {
        case 4: func_9875_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x987A:
        switch (bank) {
        case 0: func_987A_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x988C:
        switch (bank) {
        case 4: func_988C_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9893:
        switch (bank) {
        case 4: func_9893_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x989A:
        switch (bank) {
        case 4: func_989A_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x98A1:
        switch (bank) {
        case 4: func_98A1_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x98AF:
        switch (bank) {
        case 2: func_98AF_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x98B4:
        switch (bank) {
        case 4: func_98B4_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x98BB:
        switch (bank) {
        case 4: func_98BB_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x98BD:
        switch (bank) {
        case 2: func_98BD_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x98D2:
        switch (bank) {
        case 4: func_98D2_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x98D9:
        switch (bank) {
        case 4: func_98D9_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x98DD:
        switch (bank) {
        case 2: func_98DD_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x98DE:
        switch (bank) {
        case 6: func_98DE_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x98E0:
        switch (bank) {
        case 4: func_98E0_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x98E7:
        switch (bank) {
        case 4: func_98E7_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x98FA:
        switch (bank) {
        case 4: func_98FA_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9901:
        switch (bank) {
        case 4: func_9901_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9912:
        switch (bank) {
        case 6: func_9912_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9918:
        switch (bank) {
        case 4: func_9918_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x991F:
        switch (bank) {
        case 4: func_991F_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9926:
        switch (bank) {
        case 4: func_9926_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9947:
        switch (bank) {
        case 4: func_9947_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x995E:
        switch (bank) {
        case 4: func_995E_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9962:
        switch (bank) {
        case 6: func_9962_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9965:
        switch (bank) {
        case 4: func_9965_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x996C:
        switch (bank) {
        case 4: func_996C_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9973:
        switch (bank) {
        case 4: func_9973_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x998A:
        switch (bank) {
        case 4: func_998A_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9991:
        switch (bank) {
        case 4: func_9991_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9998:
        switch (bank) {
        case 4: func_9998_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x999B:
        switch (bank) {
        case 2: func_999B_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x999F:
        switch (bank) {
        case 4: func_999F_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x99B4:
        switch (bank) {
        case 4: func_99B4_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x99BB:
        switch (bank) {
        case 4: func_99BB_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x99D6:
        switch (bank) {
        case 2: func_99D6_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x99DD:
        switch (bank) {
        case 2: func_99DD_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x99FC:
        switch (bank) {
        case 6: func_99FC_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9A00:
        switch (bank) {
        case 0: func_9A00_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9A13:
        switch (bank) {
        case 0: func_9A13_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9A1D:
        switch (bank) {
        case 0: func_9A1D_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9A2D:
        switch (bank) {
        case 6: func_9A2D_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9A3F:
        switch (bank) {
        case 6: func_9A3F_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9A74:
        switch (bank) {
        case 2: func_9A74_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9A8F:
        switch (bank) {
        case 2: func_9A8F_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9A98:
        switch (bank) {
        case 0: func_9A98_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9A9E:
        switch (bank) {
        case 2: func_9A9E_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9AA0:
        switch (bank) {
        case 2: func_9AA0_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9AC0:
        switch (bank) {
        case 0: func_9AC0_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9AC2:
        switch (bank) {
        case 2: func_9AC2_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9AD1:
        switch (bank) {
        case 2: func_9AD1_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9AD8:
        switch (bank) {
        case 6: func_9AD8_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9AD9:
        switch (bank) {
        case 2: func_9AD9_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9AF4:
        switch (bank) {
        case 2: func_9AF4_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9B04:
        switch (bank) {
        case 6: func_9B04_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9B07:
        switch (bank) {
        case 6: func_9B07_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9B16:
        switch (bank) {
        case 0: func_9B16_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9B18:
        switch (bank) {
        case 2: func_9B18_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9B1F:
        switch (bank) {
        case 6: func_9B1F_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9B62:
        switch (bank) {
        case 6: func_9B62_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9B6B:
        switch (bank) {
        case 6: func_9B6B_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9B74:
        switch (bank) {
        case 6: func_9B74_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9B7E:
        switch (bank) {
        case 2: func_9B7E_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9BA4:
        switch (bank) {
        case 2: func_9BA4_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9BE1:
        switch (bank) {
        case 6: func_9BE1_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9BFC:
        switch (bank) {
        case 6: func_9BFC_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9C00:
        switch (bank) {
        case 5: func_9C00_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9C29:
        switch (bank) {
        case 6: func_9C29_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9C55:
        switch (bank) {
        case 6: func_9C55_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9C7A:
        switch (bank) {
        case 6: func_9C7A_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9CAD:
        switch (bank) {
        case 6: func_9CAD_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9CED:
        switch (bank) {
        case 6: func_9CED_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9CF1:
        switch (bank) {
        case 6: func_9CF1_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9CF5:
        switch (bank) {
        case 6: func_9CF5_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9D71:
        switch (bank) {
        case 6: func_9D71_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9D9E:
        switch (bank) {
        case 6: func_9D9E_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9DA4:
        switch (bank) {
        case 6: func_9DA4_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9DAA:
        switch (bank) {
        case 6: func_9DAA_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9E20:
        switch (bank) {
        case 6: func_9E20_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9E24:
        switch (bank) {
        case 6: func_9E24_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9E28:
        switch (bank) {
        case 6: func_9E28_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9E2E:
        switch (bank) {
        case 6: func_9E2E_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9E32:
        switch (bank) {
        case 6: func_9E32_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9E36:
        switch (bank) {
        case 6: func_9E36_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9E3C:
        switch (bank) {
        case 6: func_9E3C_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9E40:
        switch (bank) {
        case 6: func_9E40_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9E44:
        switch (bank) {
        case 6: func_9E44_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9EC5:
        switch (bank) {
        case 6: func_9EC5_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9EC9:
        switch (bank) {
        case 6: func_9EC9_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9ECD:
        switch (bank) {
        case 6: func_9ECD_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9EF4:
        switch (bank) {
        case 6: func_9EF4_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9EF8:
        switch (bank) {
        case 6: func_9EF8_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9F01:
        switch (bank) {
        case 6: func_9F01_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9F05:
        switch (bank) {
        case 6: func_9F05_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9F0E:
        switch (bank) {
        case 6: func_9F0E_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9F12:
        switch (bank) {
        case 6: func_9F12_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9FBB:
        switch (bank) {
        case 6: func_9FBB_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0x9FF6:
        switch (bank) {
        case 6: func_9FF6_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA000:
        switch (bank) {
        case 0: func_A000_b0(); break;
        case 4: func_A000_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA003:
        switch (bank) {
        case 0: func_A003_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA006:
        switch (bank) {
        case 0: func_A006_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA009:
        switch (bank) {
        case 0: func_A009_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA00C:
        switch (bank) {
        case 0: func_A00C_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA00F:
        switch (bank) {
        case 0: func_A00F_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA012:
        switch (bank) {
        case 0: func_A012_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA013:
        switch (bank) {
        case 4: func_A013_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA015:
        switch (bank) {
        case 0: func_A015_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA018:
        switch (bank) {
        case 0: func_A018_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA01B:
        switch (bank) {
        case 0: func_A01B_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA01E:
        switch (bank) {
        case 0: func_A01E_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA021:
        switch (bank) {
        case 0: func_A021_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA024:
        switch (bank) {
        case 0: func_A024_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA027:
        switch (bank) {
        case 0: func_A027_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA02A:
        switch (bank) {
        case 0: func_A02A_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA02D:
        switch (bank) {
        case 0: func_A02D_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA030:
        switch (bank) {
        case 0: func_A030_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA031:
        switch (bank) {
        case 0: func_A031_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA04C:
        switch (bank) {
        case 6: func_A04C_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA060:
        switch (bank) {
        case 6: func_A060_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA085:
        switch (bank) {
        case 6: func_A085_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA088:
        switch (bank) {
        case 6: func_A088_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA090:
        switch (bank) {
        case 0: func_A090_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA09B:
        switch (bank) {
        case 0: func_A09B_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA0E8:
        switch (bank) {
        case 6: func_A0E8_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA0FD:
        switch (bank) {
        case 0: func_A0FD_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA101:
        switch (bank) {
        case 0: func_A101_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA102:
        switch (bank) {
        case 6: func_A102_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA139:
        switch (bank) {
        case 6: func_A139_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA13C:
        switch (bank) {
        case 0: func_A13C_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA169:
        switch (bank) {
        case 0: func_A169_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA18B:
        switch (bank) {
        case 6: func_A18B_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA18E:
        switch (bank) {
        case 6: func_A18E_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA210:
        switch (bank) {
        case 6: func_A210_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA22A:
        switch (bank) {
        case 0: func_A22A_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA28B:
        switch (bank) {
        case 0: func_A28B_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA2A6:
        switch (bank) {
        case 6: func_A2A6_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA2C3:
        switch (bank) {
        case 6: func_A2C3_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA2ED:
        switch (bank) {
        case 0: func_A2ED_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA300:
        switch (bank) {
        case 4: func_A300_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA321:
        switch (bank) {
        case 4: func_A321_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA340:
        switch (bank) {
        case 6: func_A340_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA3EA:
        switch (bank) {
        case 0: func_A3EA_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA3F5:
        switch (bank) {
        case 6: func_A3F5_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA411:
        switch (bank) {
        case 6: func_A411_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA417:
        switch (bank) {
        case 0: func_A417_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA41D:
        switch (bank) {
        case 6: func_A41D_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA473:
        switch (bank) {
        case 0: func_A473_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA494:
        switch (bank) {
        case 6: func_A494_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA512:
        switch (bank) {
        case 6: func_A512_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA53B:
        switch (bank) {
        case 0: func_A53B_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA54C:
        switch (bank) {
        case 0: func_A54C_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA54F:
        switch (bank) {
        case 6: func_A54F_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA557:
        switch (bank) {
        case 0: func_A557_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA592:
        switch (bank) {
        case 6: func_A592_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA5CF:
        switch (bank) {
        case 0: func_A5CF_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA5DA:
        switch (bank) {
        case 6: func_A5DA_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA633:
        switch (bank) {
        case 0: func_A633_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA651:
        switch (bank) {
        case 0: func_A651_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA652:
        switch (bank) {
        case 6: func_A652_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA6D6:
        switch (bank) {
        case 0: func_A6D6_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA6FC:
        switch (bank) {
        case 0: func_A6FC_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA700:
        switch (bank) {
        case 2: func_A700_b2(); break;
        case 4: func_A700_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA703:
        switch (bank) {
        case 2: func_A703_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA706:
        switch (bank) {
        case 2: func_A706_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA709:
        switch (bank) {
        case 2: func_A709_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA70C:
        switch (bank) {
        case 2: func_A70C_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA70F:
        switch (bank) {
        case 2: func_A70F_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA712:
        switch (bank) {
        case 2: func_A712_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA713:
        switch (bank) {
        case 4: func_A713_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA715:
        switch (bank) {
        case 2: func_A715_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA717:
        switch (bank) {
        case 6: func_A717_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA718:
        switch (bank) {
        case 2: func_A718_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA71B:
        switch (bank) {
        case 2: func_A71B_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA71D:
        switch (bank) {
        case 4: func_A71D_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA71E:
        switch (bank) {
        case 2: func_A71E_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA721:
        switch (bank) {
        case 2: func_A721_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA724:
        switch (bank) {
        case 2: func_A724_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA727:
        switch (bank) {
        case 2: func_A727_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA72A:
        switch (bank) {
        case 2: func_A72A_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA72D:
        switch (bank) {
        case 2: func_A72D_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA730:
        switch (bank) {
        case 2: func_A730_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA731:
        switch (bank) {
        case 2: func_A731_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA732:
        switch (bank) {
        case 6: func_A732_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA736:
        switch (bank) {
        case 0: func_A736_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA73A:
        switch (bank) {
        case 6: func_A73A_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA780:
        switch (bank) {
        case 6: func_A780_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA788:
        switch (bank) {
        case 4: func_A788_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA78E:
        switch (bank) {
        case 4: func_A78E_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA7A5:
        switch (bank) {
        case 2: func_A7A5_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA7AD:
        switch (bank) {
        case 6: func_A7AD_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA7BB:
        switch (bank) {
        case 0: func_A7BB_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA7D0:
        switch (bank) {
        case 4: func_A7D0_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA7E1:
        switch (bank) {
        case 0: func_A7E1_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA82A:
        switch (bank) {
        case 2: func_A82A_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA82C:
        switch (bank) {
        case 6: func_A82C_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA83A:
        switch (bank) {
        case 0: func_A83A_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA850:
        switch (bank) {
        case 2: func_A850_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA851:
        switch (bank) {
        case 2: func_A851_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA882:
        switch (bank) {
        case 0: func_A882_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA89C:
        switch (bank) {
        case 6: func_A89C_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA8A8:
        switch (bank) {
        case 2: func_A8A8_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA8B7:
        switch (bank) {
        case 0: func_A8B7_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA8E4:
        switch (bank) {
        case 6: func_A8E4_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA8E8:
        switch (bank) {
        case 6: func_A8E8_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA8EF:
        switch (bank) {
        case 0: func_A8EF_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA8F3:
        switch (bank) {
        case 6: func_A8F3_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA916:
        switch (bank) {
        case 2: func_A916_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA918:
        switch (bank) {
        case 0: func_A918_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA953:
        switch (bank) {
        case 6: func_A953_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA957:
        switch (bank) {
        case 2: func_A957_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA971:
        switch (bank) {
        case 0: func_A971_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA995:
        switch (bank) {
        case 6: func_A995_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA9CA:
        switch (bank) {
        case 0: func_A9CA_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA9E3:
        switch (bank) {
        case 6: func_A9E3_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA9E4:
        switch (bank) {
        case 2: func_A9E4_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA9EB:
        switch (bank) {
        case 6: func_A9EB_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA9F5:
        switch (bank) {
        case 0: func_A9F5_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xA9F6:
        switch (bank) {
        case 6: func_A9F6_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xAA1D:
        switch (bank) {
        case 6: func_AA1D_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xAA33:
        switch (bank) {
        case 0: func_AA33_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xAA60:
        switch (bank) {
        case 6: func_AA60_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xAA82:
        switch (bank) {
        case 2: func_AA82_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xAAA0:
        switch (bank) {
        case 6: func_AAA0_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xAAAE:
        switch (bank) {
        case 6: func_AAAE_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xAAD7:
        switch (bank) {
        case 6: func_AAD7_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xAAD8:
        switch (bank) {
        case 0: func_AAD8_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xAB00:
        switch (bank) {
        case 4: func_AB00_b4(); break;
        case 6: func_AB00_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xAB08:
        switch (bank) {
        case 2: func_AB08_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xAB0A:
        switch (bank) {
        case 0: func_AB0A_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xAB29:
        switch (bank) {
        case 6: func_AB29_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xAB34:
        switch (bank) {
        case 4: func_AB34_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xAB41:
        switch (bank) {
        case 2: func_AB41_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xAB4F:
        switch (bank) {
        case 6: func_AB4F_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xAB77:
        switch (bank) {
        case 4: func_AB77_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xAB93:
        switch (bank) {
        case 0: func_AB93_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xAB9F:
        switch (bank) {
        case 6: func_AB9F_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xABB0:
        switch (bank) {
        case 0: func_ABB0_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xABB6:
        switch (bank) {
        case 0: func_ABB6_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xABC4:
        switch (bank) {
        case 2: func_ABC4_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xABD0:
        switch (bank) {
        case 4: func_ABD0_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xABDF:
        switch (bank) {
        case 4: func_ABDF_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xABE1:
        switch (bank) {
        case 2: func_ABE1_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xAC13:
        switch (bank) {
        case 6: func_AC13_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xAC20:
        switch (bank) {
        case 4: func_AC20_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xAC34:
        switch (bank) {
        case 6: func_AC34_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xAC4B:
        switch (bank) {
        case 6: func_AC4B_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xAC4E:
        switch (bank) {
        case 0: func_AC4E_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xAC58:
        switch (bank) {
        case 2: func_AC58_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xAC60:
        switch (bank) {
        case 6: func_AC60_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xAC75:
        switch (bank) {
        case 4: func_AC75_b4(); break;
        case 6: func_AC75_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xAC7D:
        switch (bank) {
        case 2: func_AC7D_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xAC81:
        switch (bank) {
        case 4: func_AC81_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xAC8E:
        switch (bank) {
        case 6: func_AC8E_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xACB3:
        switch (bank) {
        case 6: func_ACB3_b6(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xACB7:
        switch (bank) {
        case 4: func_ACB7_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xACC1:
        switch (bank) {
        case 0: func_ACC1_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xACE1:
        switch (bank) {
        case 2: func_ACE1_b2(); break;
        case 4: func_ACE1_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xAD0B:
        switch (bank) {
        case 0: func_AD0B_b0(); break;
        case 4: func_AD0B_b4(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xAD37:
        switch (bank) {
        case 2: func_AD37_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xAD50:
        switch (bank) {
        case 0: func_AD50_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xAD58:
        switch (bank) {
        case 2: func_AD58_b2(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xAD5E:
        switch (bank) {
        case 0: func_AD5E_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xAD8C:
        switch (bank) {
        case 0: func_AD8C_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xADBA:
        switch (bank) {
        case 0: func_ADBA_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xAE19:
        switch (bank) {
        case 0: func_AE19_b0(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB300:
        switch (bank) {
        case 1: func_B300_b1(); break;
        case 5: func_B300_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB303:
        switch (bank) {
        case 1: func_B303_b1(); break;
        case 5: func_B303_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB306:
        switch (bank) {
        case 1: func_B306_b1(); break;
        case 5: func_B306_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB309:
        switch (bank) {
        case 1: func_B309_b1(); break;
        case 5: func_B309_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB30C:
        switch (bank) {
        case 1: func_B30C_b1(); break;
        case 5: func_B30C_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB30F:
        switch (bank) {
        case 1: func_B30F_b1(); break;
        case 5: func_B30F_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB312:
        switch (bank) {
        case 1: func_B312_b1(); break;
        case 5: func_B312_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB315:
        switch (bank) {
        case 1: func_B315_b1(); break;
        case 5: func_B315_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB318:
        switch (bank) {
        case 1: func_B318_b1(); break;
        case 5: func_B318_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB31B:
        switch (bank) {
        case 1: func_B31B_b1(); break;
        case 5: func_B31B_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB31E:
        switch (bank) {
        case 1: func_B31E_b1(); break;
        case 5: func_B31E_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB321:
        switch (bank) {
        case 1: func_B321_b1(); break;
        case 5: func_B321_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB324:
        switch (bank) {
        case 1: func_B324_b1(); break;
        case 5: func_B324_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB327:
        switch (bank) {
        case 1: func_B327_b1(); break;
        case 5: func_B327_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB32A:
        switch (bank) {
        case 1: func_B32A_b1(); break;
        case 5: func_B32A_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB32D:
        switch (bank) {
        case 1: func_B32D_b1(); break;
        case 5: func_B32D_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB330:
        switch (bank) {
        case 1: func_B330_b1(); break;
        case 5: func_B330_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB331:
        switch (bank) {
        case 1: func_B331_b1(); break;
        case 5: func_B331_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB349:
        switch (bank) {
        case 5: func_B349_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB366:
        switch (bank) {
        case 5: func_B366_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB390:
        switch (bank) {
        case 1: func_B390_b1(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB39B:
        switch (bank) {
        case 1: func_B39B_b1(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB3F4:
        switch (bank) {
        case 1: func_B3F4_b1(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB403:
        switch (bank) {
        case 5: func_B403_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB411:
        switch (bank) {
        case 5: func_B411_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB46F:
        switch (bank) {
        case 5: func_B46F_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB512:
        switch (bank) {
        case 5: func_B512_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB543:
        switch (bank) {
        case 5: func_B543_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB546:
        switch (bank) {
        case 1: func_B546_b1(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB57B:
        switch (bank) {
        case 1: func_B57B_b1(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB581:
        switch (bank) {
        case 5: func_B581_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB5AF:
        switch (bank) {
        case 5: func_B5AF_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB5D7:
        switch (bank) {
        case 5: func_B5D7_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB615:
        switch (bank) {
        case 1: func_B615_b1(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB625:
        switch (bank) {
        case 5: func_B625_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB660:
        switch (bank) {
        case 1: func_B660_b1(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB666:
        switch (bank) {
        case 1: func_B666_b1(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB696:
        switch (bank) {
        case 5: func_B696_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB6F5:
        switch (bank) {
        case 1: func_B6F5_b1(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB743:
        switch (bank) {
        case 5: func_B743_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB7BD:
        switch (bank) {
        case 1: func_B7BD_b1(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB7C9:
        switch (bank) {
        case 1: func_B7C9_b1(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB80B:
        switch (bank) {
        case 5: func_B80B_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB825:
        switch (bank) {
        case 5: func_B825_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB84B:
        switch (bank) {
        case 5: func_B84B_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB86F:
        switch (bank) {
        case 5: func_B86F_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB879:
        switch (bank) {
        case 1: func_B879_b1(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB88C:
        switch (bank) {
        case 5: func_B88C_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB8D3:
        switch (bank) {
        case 5: func_B8D3_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB912:
        switch (bank) {
        case 1: func_B912_b1(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB925:
        switch (bank) {
        case 1: func_B925_b1(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB929:
        switch (bank) {
        case 5: func_B929_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB958:
        switch (bank) {
        case 5: func_B958_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB977:
        switch (bank) {
        case 1: func_B977_b1(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB97B:
        switch (bank) {
        case 5: func_B97B_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xB9B7:
        switch (bank) {
        case 5: func_B9B7_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xBA01:
        switch (bank) {
        case 1: func_BA01_b1(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xBA1C:
        switch (bank) {
        case 1: func_BA1C_b1(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xBA53:
        switch (bank) {
        case 1: func_BA53_b1(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xBA68:
        switch (bank) {
        case 5: func_BA68_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xBA88:
        switch (bank) {
        case 1: func_BA88_b1(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xBA9A:
        switch (bank) {
        case 5: func_BA9A_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xBAD0:
        switch (bank) {
        case 1: func_BAD0_b1(); break;
        case 5: func_BAD0_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xBAE8:
        switch (bank) {
        case 5: func_BAE8_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xBB05:
        switch (bank) {
        case 5: func_BB05_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xBB0A:
        switch (bank) {
        case 5: func_BB0A_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xBB29:
        switch (bank) {
        case 1: func_BB29_b1(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xBB53:
        switch (bank) {
        case 5: func_BB53_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xBB8C:
        switch (bank) {
        case 5: func_BB8C_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xBB91:
        switch (bank) {
        case 1: func_BB91_b1(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xBBAC:
        switch (bank) {
        case 5: func_BBAC_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xBBCB:
        switch (bank) {
        case 1: func_BBCB_b1(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xBBE1:
        switch (bank) {
        case 5: func_BBE1_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xBBEC:
        switch (bank) {
        case 5: func_BBEC_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xBC6B:
        switch (bank) {
        case 5: func_BC6B_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xBCC8:
        switch (bank) {
        case 5: func_BCC8_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xBCD1:
        switch (bank) {
        case 1: func_BCD1_b1(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xBCD9:
        switch (bank) {
        case 5: func_BCD9_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xBD46:
        switch (bank) {
        case 1: func_BD46_b1(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xBD7C:
        switch (bank) {
        case 1: func_BD7C_b1(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xBD7F:
        switch (bank) {
        case 5: func_BD7F_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xBD9B:
        switch (bank) {
        case 5: func_BD9B_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xBDAF:
        switch (bank) {
        case 1: func_BDAF_b1(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xBDBF:
        switch (bank) {
        case 5: func_BDBF_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xBDCE:
        switch (bank) {
        case 1: func_BDCE_b1(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xBDE6:
        switch (bank) {
        case 5: func_BDE6_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xBE23:
        switch (bank) {
        case 5: func_BE23_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xBE72:
        switch (bank) {
        case 1: func_BE72_b1(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xBE79:
        switch (bank) {
        case 5: func_BE79_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xBE9B:
        switch (bank) {
        case 1: func_BE9B_b1(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xBECE:
        switch (bank) {
        case 5: func_BECE_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xBED2:
        switch (bank) {
        case 1: func_BED2_b1(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xBF2B:
        switch (bank) {
        case 5: func_BF2B_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xBF38:
        switch (bank) {
        case 1: func_BF38_b1(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xBF48:
        switch (bank) {
        case 1: func_BF48_b1(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xBF5B:
        switch (bank) {
        case 5: func_BF5B_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xBF75:
        switch (bank) {
        case 5: func_BF75_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xBF90:
        switch (bank) {
        case 5: func_BF90_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xBF95:
        switch (bank) {
        case 1: func_BF95_b1(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xBFAF:
        switch (bank) {
        case 5: func_BFAF_b5(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    case 0xBFBE:
        switch (bank) {
        case 1: func_BFBE_b1(); break;
        default: return nes_interp_dispatch_bank(cpu_addr, addr, bank);
        }
        break;
    default:
        return nes_interp_dispatch_bank(cpu_addr, addr, bank);
    }
    return 1;
}

int call_by_address(uint16_t addr) { return nes_dispatch_call(addr, -1); }

void func_RESET(void) {
    uint16_t wb = g_code_window_base;
    g_code_window_base = 0xE000;
    func_FE00();
    g_code_window_base = wb;
}

/* The runner pushes the NMI's hardware frame; the handler's RTI leaves its
 * target in g_rti_target, and a hijacked return (nmi_tail) runs after it,
 * outside the NMI, as on the 6502. */
void func_NMI(void) {
    uint16_t wb = g_code_window_base;
    g_rti_target = 0;
    g_code_window_base = 0xC000;
    func_C000();
    g_code_window_base = wb;
    if (g_rti_target != 0) {
        runtime_begin_post_nmi();
        call_by_address(g_rti_target);
        runtime_end_post_nmi();
    }
}

void func_IRQ(void) {
    uint16_t wb = g_code_window_base;
    g_code_window_base = 0xC000;
    func_C13B();
    g_code_window_base = wb;
}
