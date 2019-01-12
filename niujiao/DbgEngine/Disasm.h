#pragma once
/*
���巴�������Ĳ��ֽṹ�����ش�����
*/
#include <Windows.h>
#include <stdio.h>
#include "ImageInfo.h"
#include "define.h"

//��������ṹ��
enum {
	PLATFORM_8BIT,
	PLATFORM_16BIT,
	PLATFORM_32BIT,
	PLATFORM_64BIT
};
typedef struct disasm_result
{
	LPVOID CodeMap;
	UINT64 CurFileOffset;
	BYTE  MachineCode[16];
	bool IsData; //����������
	UINT SIB_Prefix;
	DWORD PrefixState;
	char Opcode[15];
	UINT CurrentLen;
	UINT OperandNum;
	char FirstOperand[64];
	char SecondOperand[64];
	char ThirdOperand[64];
	char ForthOperand[64];
	char  Memo[32];

}DISASM_RESULT;
typedef struct disasm_point
{
	CImageInfo* ImageInfo;
	UINT PlatForm;
	UINT VirtualAddress;  //�ڴ�����ַ
	UINT64 BaseOfCodeInFile;  //�ļ������ַ
	UINT64 BaseOfImage; //�ڴ�ӳ���ַ 
	UINT64 FileStartOfCode; //�ļ��д���ο�ʼλ��
	UINT64 FileEndOfCode; //�ļ��д���ν���λ��
	DWORD LenOfCode;//����γ���
	UINT64 CurMemAddr;
	UINT TotalDisasmRecord;
	LPVOID MapFileAddr; //�ļ�ӳ���ڴ��ַ
}DISASM_POINT;  //���ھ�̬������Ե��  ʹ�ö���һ�ݶ�����imageinfo 

typedef struct str_map_code
{
	BYTE Num;  //��������� û���õ���ֻΪǰ�ڼ����������õ�
	char OpStr[15];  //������αָ���ַ���
	UINT64 Operand;  //������������ĸ���ǰ��λΪ���������������Ϊ�ģ�����ÿ����ַ����������������͸�ռ��λ���ܵ���ЧλΪ43λ��
	bool(*DisasmFunc)(DISASM_RESULT*, DISASM_POINT*,int* IsFinished); //�������Ӧ�Ĵ�����ָ��
}STR_MAP_CODE;

typedef struct disasm_addr_queue
{
	LPVOID Addr;
	struct disasm_addr_queue* next;
} DISASM_ADDR_QUEUE;


class Disasm
{
private:
	DISASM_RESULT** DisasmResult; //���ڵ�ַ�����ķ������
	LPVOID *DisasmResultBySeq; //���ڽ������ķ������

	DISASM_POINT* DisasmPoint;
	CImageInfo* ImageInfo;


public:
	//����൥��ָ��
	Disasm();
	~Disasm();
	DISASM_RESULT* GetDisasmResult(DWORD) const;

	DISASM_RESULT* GetDisasmResult() const;
	LPVOID* GetDisasmResultBySeq() const;
	CImageInfo* GetImageInfo() const;
	DWORD GetTotalDisasmRecord() const;
	void IndexResultBySeq(DISASM_POINT* ImageInfo); //������Ž��������������

	/*����������� */
	static bool PushDisasmFuncAddr(LPVOID Addr);
	static LPVOID PopDisasmFuncAddr();
	static int GetDisasmFuncAddrCount();
	/*����������� */

	bool DisasmFromHandle(HANDLE hFile);
	bool DisasmFromFile(LPCTSTR* FileName);
	bool DisasmFromFile(char* FileName);
	bool DisasmFromStr(char* Str, int platForm, int length, DISASM_RESULT * DisasmResult);
	bool DisasmFile();
	bool TestPrintToFile() const;
	static bool SetDataType(DISASM_POINT*  DisasmPoint, DWORD addr, int size);

	static bool GernelDisasm(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static char* GetNumbericType(DISASM_RESULT*DisasmResult,int base,int seg=0);
	static int GetOperAndAddrSize(int platFrom,UINT64 Prefix,int* addrSize,int* OperandSize);
	static bool Disasm_reg_or_imm(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished); // ������Ĵ������������������ָ���еĲ�����
	static bool Disasm_ModRM(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_SIB(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, PDWORD Len, char*);
	static bool Disasm_grp_80_83(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_grp_ff(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_RexPrefix(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_ret(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_set_prefix(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_grp_c6_c7(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_grp_shift(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_no_else(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_pusha(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_0xd5_0xd6(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_popa(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_pushf(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_popf(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_grp_f6_f7(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_grp_fe(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_int(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_reserve(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_bound(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_0x63(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_imul(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_grp_8f(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_0x90(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_ESC_0xd8(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_ESC_0xd9(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_ESC_0xda(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_ESC_0xdb(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_ESC_0xdc(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_ESC_0xdd(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_ESC_0xde(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_ESC_0xdf(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_two_opcode(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);


	//���ֽڲ�����
	static bool Disasm_TWO_grp_0x00(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_TWO_grp_0x01(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_TWO_grp_0xae(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_TWO_0xbx(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_TWO_0x6x_0xdx_0xex_0xfx(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_TWO_three_opcode38(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_TWO_three_opcode3A(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_TWO_0x10(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_TWO_0x11(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_TWO_0x12(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_TWO_0x13(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_TWO_0x14(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_TWO_0x15(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_TWO_0x16(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_TWO_0x17(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_TWO_grp_16(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_TWO_0x1a(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_TWO_0x1b(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_TWO_0x28(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_TWO_0x29(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_TWO_0x2a(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_TWO_0x2b(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_TWO_0x2c(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_TWO_0x2d(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_TWO_0x2e_0x2f(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_TWO_0x50(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_TWO_0x51(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_TWO_0x52_0x53(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_TWO_0x54_0x55_0x56_0x57(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_TWO_0x58_0x59_0x5c_0x5d_0x5e_0x5f(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_TWO_0x5a(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_TWO_0x5b(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_TWO_0x70(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_TWO_grp12_grp13_grp14(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_TWO_0x74_0x75_0x76(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_TWO_0x77(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_TWO_0x7c_0x7d(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_TWO_0x7e(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_TWO_0x7f(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_TWO_0xc0_0xc1_0xc3(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_TWO_0xc2(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_TWO_0xc4(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_TWO_0xc5(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_TWO_0xc6(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_TWO_grp9(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_TWO_0xc8_0xcf(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);

	//���ֽڲ�����38
	static bool Disasm_THREE38_0x0x(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_THREE38_gernel(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_THREE38_0x1c_0x1d_0x1e(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_THREE38_0xf0(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_THREE38_0xf1(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_THREE38_0xf2(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_THREE38_grp17(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_THREE38_0xf5(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_THREE38_0xf6(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_THREE38_0xf7(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	//���ֽڲ�����3a
	static bool Disasm_THREE3A_gernel(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
	static bool Disasm_THREE3A_0x0f(DISASM_RESULT*DisasmResult, DISASM_POINT*  DisasmPoint, int* IsFinished);
};
