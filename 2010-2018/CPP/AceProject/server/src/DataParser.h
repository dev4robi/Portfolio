#ifndef __DATA_PARSER_H__
#define _CRT_SECURE_NO_WARNINGS

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Name : DataParser.h(v1)
*	- Last Update : 2016.10.18
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : *.dat ������ �о ���ϴ� ������ ����, �Ǽ�(float), ���ڿ� �� �̾Ƴ��� Ŭ����.
*
*	// *.dat ������ ������ �Ʒ��� ����. //
*	[Header]
*	[Attribute1] Data1 [/Attribute1]
*	[Attribute1] Data2 [/Attribute1]
*	[/Header]
*
*	- Dependency : [StdHeader.h]
*
*   - Version(v1): ������ �ļ� 1�� ���� �ϼ�. (2016.10.18)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdio.h>
#include <stdlib.h>
#include <string>

class DataParser {
private:
	char *pHeaderPtr;			// Find �Լ��� ã�� ��� ��ġ�� ����Ű�� ������
	char *pHeaderEndPtr;		// Find �Լ��� ã�� ����� ������ ��ġ�� ����Ű�� ������
	char *pAttributePtr;		// Find �Լ��� ã�� �Ӽ� ��ġ�� ����Ű�� ������
	char *pAttributeData;		// Find �Լ��� ã�� �Ӽ��� ���� �Ҵ�� �����͸� ����Ű�� ������
	char *pDataBuffer;			// *.dat ������ ����Ʈ ��Ʈ���� �ӽ� ������ �� ����
	int iDataBufferByteSize;	// ������ ������ ����Ʈ ũ��

public:
	// ���
	static const int DEFAULT_HEADERNAME_MAX_LENGTH = 256;		// ������� �ִ� ����
	static const int DEFAULT_ATTRIBUTENAME_MAX_LENGTH = 256;	// �Ӽ����� �ִ� ����

	// ������ & �Ҹ���
	DataParser();
	~DataParser();

	// ���� & ����
	// ...

	// �޼ҵ�
	bool openFile(char *fileName);		// ������ ��� ������ ���۷� �ű�
	void printDataBuffer();				// ������ ������ ������ ���
	void clearDataBuffer();				// �ӽ� ������ ����Ʈ ��Ʈ���� ����
	void initPtrAll();					// ��� �����Ϳ� �Ӽ� �������� ��ġ�� NULL�� �ʱ�ȭ
	void initHeaderPtr();				// ��� �������� ��ġ�� NULL�� �ʱ�ȭ
	void initAttributePtr();			// �Ӽ� �������� ��ġ�� ��� �������� ��ġ�� �ʱ�ȭ

	// �Ľ� �޼ҵ�
	bool findHeader(char *headerName);			// �ش� ��Ī�� ����� ã��
	bool findAttribute(char *attributeName);	// �ش� ��Ī�� �Ӽ��� ã��
	int parseIntFromAttribute();				// pAttribute�� ����Ű�� �����͸� int�� �Ľ��Ͽ� ��ȯ
	float parseFloatFromAttribute();			// pAttribute�� ����Ű�� �����͸� float���� �Ľ��Ͽ� ��ȯ
	char* parseStringFromAttribute();			// pAttribute�� ����Ű�� �����͸� ���ڿ��� �Ľ��Ͽ� ���� �Ҵ�� ������ ��ȯ (���� �Ҵ� ������ ������� ��)

	// �׽�Ʈ �޼ҵ�
	int printf_debug(char *str);				// __DEBUG__ �� ����Ǿ� ���� �� ����Ǵ� ����׿� ��� �Լ�
};

#endif