#include <iostream>
#include <Windows.h>
#include <conio.h>
#include "Bit.h"
#include "DES.h"
#include "Table.h"
#include "excon.h"

#pragma comment(lib, "winmm.lib")

using namespace std;

int main(int argc, char** argv) {
	// Ű
	const char *key64bit = "Se0KyoN9";

	// ���̺� �ʱ�ȭ
	InitTable();

	// �޴�
	ChangeCR(CR_DGREEN, CR_WHITE);
	cout << "[ DES Cryptography Program ] ver 1.00" << endl;
	cout << "������: 2010305049 ������" << endl;
	cout << "        2010305018 ������" << endl;
	cout << "        2010305045 ������" << "		- ������б� ��ǻ�Ͱ��а�" << endl << endl;
	ChangeCR(CR_BLACK, CR_WHITE);

	// ��
	char str[1024];
	ChangeCR(CR_BLUE, CR_WHITE);
	cout << ">> �� �Է�(1~1024����): ";
	cin.getline(str, sizeof(str));
	cout << endl;
	ChangeCR(CR_BLACK, CR_WHITE);
	

	// ���� Ŭ������ �̿��Ͽ� ��ȣȭ ����
	double firstTime = timeGetTime() / 1000.0;
	DES des(str, key64bit);

	// ��ȣȭ ����
	des.encryption();
	ChangeCR(CR_DBLUE, CR_WHITE);   des.printPlaneText(); cout << endl;
	ChangeCR(CR_DPURPLE, CR_WHITE); des.printCipherText(); cout << endl;
	double secondTime = timeGetTime() / 1000.0;
	ChangeCR(CR_RED, CR_WHITE); cout << "��ȣȭ �ҿ� �ð�: " << secondTime - firstTime << "��" << endl << endl;
	ChangeCR(CR_RED, CR_WHITE); cout << "��ȣȭ�� �����մϴ�..." << endl << endl; ChangeCR(CR_BLACK, CR_WHITE);
	firstTime = secondTime;
	
	// ��ȣȭ ����
	des.decryption();
	ChangeCR(CR_DPURPLE, CR_WHITE); des.printPlaneText(); cout << endl;
	ChangeCR(CR_DBLUE, CR_WHITE);   des.printCipherText(); cout << endl;
	secondTime = timeGetTime() / 1000.0;
	ChangeCR(CR_RED, CR_WHITE); cout << "��ȣȭ �ҿ� �ð�: " << secondTime - firstTime << "��" << endl << endl; ChangeCR(CR_BLACK, CR_WHITE);

	// ����Ű ���
	ChangeCR(CR_GOLD, CR_WHITE); des.printKey(); cout << endl; ChangeCR(CR_BLACK, CR_WHITE);

	// ����
	return 0;
}
