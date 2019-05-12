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
	// 키
	const char *key64bit = "Se0KyoN9";

	// 테이블 초기화
	InitTable();

	// 메뉴
	ChangeCR(CR_DGREEN, CR_WHITE);
	cout << "[ DES Cryptography Program ] ver 1.00" << endl;
	cout << "제작자: 2010305049 이태훈" << endl;
	cout << "        2010305018 김진우" << endl;
	cout << "        2010305045 위지연" << "		- 서경대학교 컴퓨터공학과" << endl << endl;
	ChangeCR(CR_BLACK, CR_WHITE);

	// 평문
	char str[1024];
	ChangeCR(CR_BLUE, CR_WHITE);
	cout << ">> 평문 입력(1~1024문자): ";
	cin.getline(str, sizeof(str));
	cout << endl;
	ChangeCR(CR_BLACK, CR_WHITE);
	

	// 데스 클래스를 이용하여 암호화 시작
	double firstTime = timeGetTime() / 1000.0;
	DES des(str, key64bit);

	// 암호화 수행
	des.encryption();
	ChangeCR(CR_DBLUE, CR_WHITE);   des.printPlaneText(); cout << endl;
	ChangeCR(CR_DPURPLE, CR_WHITE); des.printCipherText(); cout << endl;
	double secondTime = timeGetTime() / 1000.0;
	ChangeCR(CR_RED, CR_WHITE); cout << "암호화 소요 시간: " << secondTime - firstTime << "초" << endl << endl;
	ChangeCR(CR_RED, CR_WHITE); cout << "복호화를 시작합니다..." << endl << endl; ChangeCR(CR_BLACK, CR_WHITE);
	firstTime = secondTime;
	
	// 복호화 수행
	des.decryption();
	ChangeCR(CR_DPURPLE, CR_WHITE); des.printPlaneText(); cout << endl;
	ChangeCR(CR_DBLUE, CR_WHITE);   des.printCipherText(); cout << endl;
	secondTime = timeGetTime() / 1000.0;
	ChangeCR(CR_RED, CR_WHITE); cout << "복호화 소요 시간: " << secondTime - firstTime << "초" << endl << endl; ChangeCR(CR_BLACK, CR_WHITE);

	// 서브키 출력
	ChangeCR(CR_GOLD, CR_WHITE); des.printKey(); cout << endl; ChangeCR(CR_BLACK, CR_WHITE);

	// 종료
	return 0;
}
