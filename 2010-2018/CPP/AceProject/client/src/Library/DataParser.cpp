#include "DataParser.h"

DataParser::DataParser() {
	pHeaderPtr = NULL;
	pHeaderEndPtr = NULL;
	pAttributePtr = NULL;
	pAttributeData = NULL;
	pDataBuffer = NULL;
	iDataBufferByteSize = 0;
}

DataParser::~DataParser() {
	// ���� �Ҵ�� ���۸� ���
	clearDataBuffer();

	// ������ �ʱ�ȭ
	pHeaderPtr = NULL;
	pHeaderEndPtr = NULL;
	pAttributePtr = NULL;
	pAttributeData = NULL;
}

bool DataParser::openFile(char *fileName) {
	FILE *pFile = NULL;		// ���� ������
	char *pPtr = fileName;	// ���ϸ� ������
	int iFileByteSize = 0;	// ���� ����Ʈ ũ�⸦ ����

	// Ȯ���ڰ� .dat �������� Ȯ��
	if ((pPtr += (strlen(fileName) - 4))) {
		if (pPtr[0] != '.' || pPtr[1] != 'd' || pPtr[2] != 'a' || pPtr[3] != 't') {
			printf_debug("DataParser::openFile() Error - The file '");	printf_debug(fileName);	printf_debug("' is not a '*.dat' file.\n");
			return false;
		}
	}

	// ���� ����
	pFile = fopen(fileName, "rb");
	if (pFile == NULL) {
		printf_debug("DataParser::openFile() Error - Fail to open '");	printf_debug(fileName);	printf_debug("'./n");
		return false;
	}

	// ���� ũ�⸸ŭ ���� ���� ����
	fseek(pFile, 0, SEEK_END);
	if ((iFileByteSize = ftell(pFile)) <= 0) {
		printf_debug("DataParser::openFile() Error - The file '%s' is empty or damaged.\n");
		fclose(pFile);
		return false;
	}
	if ((pDataBuffer = new char[iFileByteSize + 1]) == NULL) {
		printf_debug("DataParser::openFile() Error - Fail to allocate buffer.\n");
		fclose(pFile);
		return false;
	}
	fseek(pFile, 0, SEEK_SET);

	// ���� ������ �о ���ۿ� ����
	if (fread(this->pDataBuffer, 1, iFileByteSize, pFile) == -1) {
		printf_debug("DataParser::openFile() Error - Fail to read file.\n");
		fclose(pFile);
		return false;
	}
	pDataBuffer[iFileByteSize] = '\0';

	// ������ �ݰ� ���� ��ȯ
	fclose(pFile);
	return true;
}

void DataParser::printDataBuffer() {
	if (pDataBuffer != NULL) {
		printf("%s\n", pDataBuffer);
	}
	else {
		printf_debug("DataParser::printDataBuffer() Error - Data buffer is empty.\n");
	}
}

void DataParser::clearDataBuffer() {
	if (pDataBuffer != NULL) {
		delete[]pDataBuffer;
	}
	pDataBuffer = NULL;
}

void DataParser::initPtrAll() {
	initHeaderPtr();
	initAttributePtr();
}

void DataParser::initHeaderPtr() {
	pHeaderPtr = NULL;
}

void DataParser::initAttributePtr() {
	pAttributePtr = NULL;
}

bool DataParser::findHeader(char *headerName) {
	// [���� ����]
	int iHeaderLength = 0;

	// [���� �˻�]
	// ��� �̸��� NULL�� ���
	if (headerName == NULL) {
		printf_debug("DataParser::findHeader() Canceled - Enter correct header name pointer.\n");
		return false;
	}
	// ��� �̸��� ���̰� 0���� �۰ų� �ִ� ���̸� �ʰ��� ���
	iHeaderLength = strlen(headerName);
	if (iHeaderLength <= 0 || iHeaderLength > DEFAULT_HEADERNAME_MAX_LENGTH) {
		printf_debug("DataParser::findHeader() Canceled - Header name is too short or too long.\n");
		return false;
	}
	// ������ ���۰� ����ִ� ���
	if (pDataBuffer == NULL) {
		printf_debug("DataParser::findHeader() Canceled - Data buffer is empty. Open the file first.\n");
		return false;
	}

	// [�ʱ�ȭ �� �������� �߰� ����]
	// HeaderPtr�� �ʱ�ȭ�Ǿ��ų�, ó������ ������ �����ϴ� ���
	if (pHeaderPtr == NULL) {
		pHeaderPtr = pDataBuffer;	// ������ ���� �������� ����
		initAttributePtr();			// �Ӽ����� ����Ű�� �����͸� NULL�� �ʱ�ȭ
	}

	char headerBuffer[DEFAULT_HEADERNAME_MAX_LENGTH];	// ������� �ӽ� ������ ����
	char *pBuffer = NULL;								// ������� �ӽ� ������ ������ ���� ���ڰ� ���Ե� ��ġ�� ����Ű�� ������
	bool bFoundHeader = false;							// ����� �߰� ���� (bFoundHeader = bHeaderBegin && bHeaderEnd)
	bool bHeaderBegin = false;							// ��� ���ۺκ��� �߰� ����
	bool bHeaderEnd = false;							// ��� ����κа� ��Ī�� �߰� ����
	
	// 1.[��� ã�� ����]
	// ������� pDataBuffer�� ��ȸ�ϸ� ��� ã�� ����
	do {
		// ���� ��� ������ '<' �� �߰����� ���� ���
		if (bHeaderBegin == false) {
			if (pHeaderPtr[0] == '<') {
				bHeaderBegin = true;
				strnset(headerBuffer, '\0', sizeof(headerBuffer));
				pBuffer = &headerBuffer[0];
				// printf("<"); // Debugging Code
			}
		}
		// ��� ������ '<' �� �߰� �� '>' ������ ������ ��� ���
		else if (bHeaderBegin == true && bHeaderEnd == false) {
			// '>' �� �߰��� �� ������ ������ ���ۿ� ����
			if (pHeaderPtr[0] != '>') {
				*pBuffer = pHeaderPtr[0];
				// printf("%c", *pBuffer); // Debugging Code

				// ���� ���� Ž��
				if ((++pBuffer) == &headerBuffer[DEFAULT_HEADERNAME_MAX_LENGTH]) {
					printf_debug("DataParser::findHeader() - Error : Header length is out of buffer!\n");
					break; 
				}
			}
			// '>' �� �߰��ϸ� ���ۿ� ��� ����� ã�� ����� �̸��� ��
			else {
				if (strcmp(headerName, headerBuffer) == 0) {
					// printf("> �߰�!"); // Debugging Code
					bHeaderEnd = true;
				}
				else {
					// printf("> �߰�! - ��ġ���� �ʴ� ���.\n"); // Debugging Code
					bHeaderBegin = false;
				}
			}

			// ��� �߰� �ϷῩ�� �˻�
			if (bHeaderBegin == true && bHeaderEnd == true) {
				// printf(" - ��ġ�ϴ� ���.\n"); // Debugging Code
				bFoundHeader = true;
			}
		}

		// ���� ���� ����
		while (1) {
			++pHeaderPtr;
			if ((pHeaderPtr[0] > 31 && pHeaderPtr[0] < 127) || (pHeaderPtr[0] == '\0')) break; // ���� ���� (/n, /r ���� �ǳ� ��)
		}
	} while (bFoundHeader == false && pHeaderPtr[0] != '\0');

	// [��� ��ȸ ���]
	// ����� ã�� ������ ���
	if (bFoundHeader == false) {
		printf_debug("DataParser::findHeader() - Fail : Fail to find header <"); printf_debug(headerName);	printf_debug(">\n");

		// [pHeaderPtr]�� NULL�� ����Ű���� �ϰ�,
		// [pAttributePtr]�� NULL�� ����Ű���� ��.
		initPtrAll();

		return false;
	}
	// ����� ã�� ���
	else {
		// [pHeaderPtr]�� ��� �ٷ� ���� ������ ��ġ�� ����Ű�� �ְ� �ǰ�,
		// [pAttributePtr]�� ��� �����Ͱ� ����Ű�� ��ġ�� ����Ű�� �ְ� �ȴ�.
		pAttributePtr = pHeaderPtr;

		// printf("��� <%s> �� ���� ������:\n%s\n", headerName, pHeaderPtr);		// Debugging code
		// printf("��� <%s> �� �Ӽ� ������:\n %s\n", headerName, pAttributePtr);	// Debugging code
	}

	// 2.[��� ���� �κ� ã�� ����]
	// pHeaderPtr ������ �����͸� ��ȸ�ϸ� ���� �κ�(pHeaderEndPtr) ã�� ����
	bFoundHeader = false;
	bHeaderBegin = false;
	bHeaderEnd = false;
	pHeaderEndPtr = pHeaderPtr;
	do {
		// ���� ��� ������ '<' �� �߰����� ���� ���
		if (bHeaderBegin == false) {
			if (pHeaderEndPtr[0] == '<') {
				bHeaderBegin = true;
				strnset(headerBuffer, '\0', sizeof(headerBuffer));
				pBuffer = &headerBuffer[0];
				// printf("<"); // Debugging Code
			}
		}
		// ��� ������ '<' �� �߰� �� '>' ������ ������ ��� ���
		else if (bHeaderBegin == true && bHeaderEnd == false) {
			// '>' �� �߰��� �� ������ ������ ���ۿ� ����
			if (pHeaderEndPtr[0] != '>') {
				*pBuffer = pHeaderEndPtr[0];
				// printf("%c", *pBuffer); // Debugging Code

				// ���� ���� Ž��
				if ((++pBuffer) == &headerBuffer[DEFAULT_HEADERNAME_MAX_LENGTH]) {
					printf_debug("DataParser::findHeader() - Error : Header length is out of buffer!\n");
					break;
				}
			}
			// '>' �� �߰��ϸ� ���ۿ� ��� ����� ã�� ����� �̸��� ��
			else {
				// ��� ���ᱸ���� ������ '/'���� �˻�
				if (headerBuffer[0] == '/') {
					// '/' ���� ��ġ���� ��� ���ᱸ���� ��� �̸��� headerName�� ��ġ�ϴ��� �˻�
					if (strcmp(headerName, (headerBuffer + 1)) == 0) {
						// printf("> �߰�!"); // Debugging Code
						bHeaderEnd = true;
					}
					else {
						// printf("> �߰�! - ��ġ���� �ʴ� ���� ���.\n"); // Debugging Code
						bHeaderBegin = false;
					}
				}
				else {
					// printf("> �߰�! - �ùٸ��� ���� ��� ���� ����.\n"); // Debugging Code
					bHeaderBegin = false;
				}
			}

			// ��� �߰� �ϷῩ�� �˻�
			if (bHeaderBegin == true && bHeaderEnd == true) {
				// printf(" - ��ġ�ϴ� ���� ���.\n"); // Debugging Code
				bFoundHeader = true;
			}
		}

		// ���� ���� ����
		while (1) {
			++pHeaderEndPtr;
			if ((pHeaderEndPtr[0] > 31 && pHeaderEndPtr[0] < 127) || (pHeaderEndPtr[0] == '\0')) break; // ���� ���� (/n, /r ���� �ǳ� ��)
		}
	} while (bFoundHeader == false && pHeaderEndPtr[0] != '\0');

	// [��� ���� ��ȸ ���]
	// ���� �κ��� ã�� ������ ���
	if (bFoundHeader == false) {
		printf_debug("DataParser::findHeader() - Fail : Fail to find header close. <");	printf_debug(headerName);	printf_debug(">\n");

		// [pHeaderPtr]�� NULL�� ����Ű���� �ϰ�,
		// [pAttributePtr]�� NULL�� ����Ű���� ��.
		initPtrAll();

		return false;
	}
	// ���� �κ��� ã�� ���
	else {
		// [pHeaderPtr]�� ��� �ٷ� ���� ������ ��ġ�� ����Ű�� �ְ� �ǰ�,
		// [pHeaderEndPtr]�� ��� ���� ���� �ٷ� ���� ��ġ�� ����Ű�� �ְ�,
		// [pAttributePtr]�� ��� �����Ͱ� ����Ű�� ��ġ�� ����Ű�� �ְ� �ȴ�.
		
		// ������� �����ؾ� ������ ���� ��ȯ
		return true;
	}
}

bool DataParser::findAttribute(char *attributeName) {
	// [���� ����]
	int iAttributeLength = 0;

	// [���� �˻�]
	// �Ӽ� �̸��� NULL�� ���
	if (attributeName == NULL) {
		printf_debug("DataParser::findAttribute() Canceled - Enter correct attribute name pointer.\n");
		return false;
	}
	// �Ӽ� �̸��� ���̰� 0���� �۰ų� �ִ� ���̸� �ʰ��� ���
	iAttributeLength = strlen(attributeName);
	if (iAttributeLength <= 0 || iAttributeLength > DEFAULT_ATTRIBUTENAME_MAX_LENGTH) {
		printf_debug("DataParser::findAttribute() Canceled - Attribute name is too short or too long.\n");
		return false;
	}
	// ������ ���۰� ����ִ� ���
	if (pDataBuffer == NULL) {
		printf_debug("DataParser::findAttribute() Canceled - Data buffer is empty. Open the file first.\n");
		return false;
	}
	// HeaderPtr�� NULL �� ���
	if (pHeaderPtr == NULL) {
		printf_debug("DataParser::findAttribute() Canceled - Header pointer is NULL. Use this->findHeader(\"...\") function first.\n");
		return false;
	}
	
	// [�ʱ�ȭ �� �������� �߰� ����]
	// �Ӽ����� �����ϱ� �� �׻� �ش� �Ӽ� ���� ����� ������ ���� ��ġ�� �ʱ�ȭ
	pAttributePtr = pHeaderPtr;

	char attributeBuffer[DEFAULT_ATTRIBUTENAME_MAX_LENGTH];		// �Ӽ����� �ӽ� ������ ����
	char *pBuffer = NULL;										// �Ӽ����� �ӽ� ������ ������ ���� ���ڰ� ���Ե� ��ġ�� ����Ű�� ������
	bool bFoundAttribute = false;								// �Ӽ��� �߰� ���� (bFoundAttribute = bAttributeBegin && bAttributeEnd)
	bool bAttributeBegin = false;								// �Ӽ� ���ۺκ��� �߰� ����
	bool bAttributeEnd = false;									// �Ӽ� ����κа� ��Ī�� �߰� ����

	// 1. [�Ӽ� ã�� ����]
	// ������� pHeaderPtr������ pDataBuffer�� ��ȸ�ϸ� �Ӽ� ã�� ����
	do {
		// ���� �Ӽ� ������ '[' �� �߰����� ���� ���
		if (bAttributeBegin == false) {
			if (pAttributePtr[0] == '[') {
				bAttributeBegin = true;
				strnset(attributeBuffer, '\0', sizeof(attributeBuffer));
				pBuffer = &attributeBuffer[0];
				// printf("["); // Debugging Code
			}
		}
		// �Ӽ� ������ '[' �� �߰� �� ']' ������ ������ ��� ���
		else if (bAttributeBegin == true && bAttributeEnd == false) {
			// ']' �� �߰��� �� ������ ������ ���ۿ� ����
			if (pAttributePtr[0] != ']') {
				*pBuffer = pAttributePtr[0];
				 // printf("%c", *pBuffer); // Debugging Code

				// ���� ���� Ž��
				 if ((++pBuffer) == &attributeBuffer[256]) {
					printf_debug("DataParser::findAttribute() - Error : Attribute length is out of buffer!");
					break;
				}
			}
			// ']' �� �߰��ϸ� ���ۿ� ��� ����� ã�� �Ӽ��� �̸��� ��
			else {
				if (strcmp(attributeName, attributeBuffer) == 0) {
					// printf("] �߰�!"); // Debugging Code
					bAttributeEnd = true;
				}
				else {
					// printf("] �߰�! - ��ġ���� �ʴ� �Ӽ�.\n"); // Debugging Code
					bAttributeBegin = false;
				}
			}

			// �Ӽ� �߰� �ϷῩ�� �˻�
			if (bAttributeBegin == true && bAttributeEnd == true) {
				// printf(" - ��ġ�ϴ� �Ӽ�.\n"); // Debugging Code
				bFoundAttribute = true;
			}
		}

		// ���� ���� ����
		while (1) {
			++pAttributePtr;
			if ((pAttributePtr[0] > 31 && pAttributePtr[0] < 127) || (pAttributePtr[0] == '\0')) break; // ���� ���� (/n, /r ���� �ǳ� ��)
		}
	} while (bFoundAttribute == false && pAttributePtr != pHeaderEndPtr); // pAttributePtr != pHeaderEndPtr �κп��� �Ӽ��� ��� ���� ������ �Ѿ �����ϴ� ���� ����

	// [�Ӽ� ã�� ���]
	// �Ӽ��� ã�� ������ ���
	if (bFoundAttribute == false) {
		printf_debug("DataParser::findAttribute() - Fail : Fail to find attribute [");	printf_debug(attributeName);	printf_debug("].\n");

		// [pAttributePtr]�� NULL�� ����Ű���� ��.
		initAttributePtr();

		return false;
	}
	// �Ӽ��� ã�� ���
	else {
		// [pAttributePtr]�� �Ӽ� ������ ���� �κ��� ����Ű�� �ȴ�.
	}

	// 2.[�Ӽ� ���� �κ� ã�� ����]
	// pAttributePtr ������ �����͸� pHeaderEndPtr���� ��ȸ�ϸ� ���� �κ�([/attributeName]) ã�� ����
	char attributeDataBuffer[DEFAULT_ATTRIBUTENAME_MAX_LENGTH];		// �Ӽ��� �����͸� �ӽ� ������ ���� ����
	char *pDataBufferPtr = &attributeDataBuffer[0];					// �Ӽ��� �����͸� �ӽ� ������ ������ ������ �����͸� ���� ��ġ�� ����Ű�� ������
	memset(attributeDataBuffer, '\0', sizeof(attributeDataBuffer));	// ������ �ӽ� ���� ���� �ʱ�ȭ
	bFoundAttribute = false;
	bAttributeBegin = false;
	bAttributeEnd = false;

	do {
		// �Ӽ� �����͸� �ӽ� ������ ���
		*pDataBufferPtr = pAttributePtr[0];

		// �ӽ� ������� ���۰� �ʰ��ϴ��� �˻�
		if ((++pDataBufferPtr) == &attributeDataBuffer[DEFAULT_ATTRIBUTENAME_MAX_LENGTH]) {
			printf_debug("DataParser::findAttribute() - Fail : Attribute data is too long.\n");
			break;
		}

		// ���� �Ӽ� ���� ������ '[' �� �߰����� ���� ���
		if (bAttributeBegin == false) {
			if (pAttributePtr[0] == '[') {
				bAttributeBegin = true;
				strnset(attributeBuffer, '\0', sizeof(attributeBuffer));
				pBuffer = &attributeBuffer[0];
				// printf("["); // Debugging Code
			}
		}
		// �Ӽ� ������ '[' �� �߰� �� ']' ������ ������ ��� ���
		else if (bAttributeBegin == true && bAttributeEnd == false) {
			// ']' �� �߰��� �� ������ ������ ���ۿ� ����
			if (pAttributePtr[0] != ']') {
				*pBuffer = pAttributePtr[0];
				// printf("%c", *pBuffer); // Debugging Code

				// ���� ���� Ž��
				if ((++pBuffer) == &attributeBuffer[DEFAULT_ATTRIBUTENAME_MAX_LENGTH]) {
					// printf("DataParser::findAttribute() - Error : Attribute length is out of buffer[%d]!\n", sizeof(attributeBuffer));
					break;
				}
			}
			// ']' �� �߰��ϸ� ���ۿ� ��� ����� ã�� ����� �̸��� ��
			else {
				if (attributeBuffer[0] == '/') {
					if (strcmp(attributeName, (attributeBuffer + 1)) == 0) {
						// printf("] �߰�!\n"); // Debugging Code
						bAttributeEnd = true;
					}
					else {
						// printf("] �߰�! - ��ġ���� �ʴ� �Ӽ� ����.\n"); // Debugging Code
						bAttributeBegin = false;
					}
				}
				else {
					// printf("] �߰�! - �ùٸ��� ���� �Ӽ� ���� ����.\n"); // Debugging Code
					bAttributeBegin = false;
				}
			}

			// �Ӽ� �߰� �ϷῩ�� �˻�
			if (bAttributeBegin == true && bAttributeEnd == true) {
				// printf(" - ��ġ�ϴ� �Ӽ� �ݱ� ����.\n"); // Debugging Code
				bFoundAttribute = true;
			}
		}

		// ���� ���� ����
		while (1) {
			++pAttributePtr;
			if ((pAttributePtr[0] > 31 && pAttributePtr[0] < 127) || (pAttributePtr[0] == '\0')) break; // ���� ���� (/n, /r ���� �ǳ� ��)
		}
	} while (bFoundAttribute == false && pAttributePtr[0] != '\0');

	// [�Ӽ� ���� ã�� ���]
	// �Ӽ��� ������ �κ��� ã�� ������ ���
	if (bFoundAttribute == false) {
		printf_debug("DataParser::findAttribute() - Fail : Fail to find attribute close. [");	printf_debug(attributeName);	printf_debug("].\n");

		// [pAttributePtr]�� NULL�� ����Ű���� ��.
		initAttributePtr();

		return false;
	}
	// �Ӽ��� ������ �κ��� ã�� ���
	else {
		// [pAttributePtr]�� �Ӽ��� ������ ��ġ�� ����Ű�� �ְ� �ǰ�,
		// [pAttributeData]�� ���� ������ �Ӽ��� �����Ͱ��� ����Ű�� �ְ� �ȴ�.
		
		// �Ӽ� �ݱ� ����[/]�� �Ӽ����� ���̸�ŭ�� �� ��ġ�� '\0' ���ڸ� ���� ([/attributeName]�� Data�� �������� �ʱ� ����)
		*(pDataBufferPtr - (3 + iAttributeLength)) = '\0';

		// ���� ���� ����
		int iDataLength = strlen(attributeDataBuffer);
		if (pAttributeData != NULL) { // ������ �ٸ� �Ӽ��� �����͸� ������ �� �Ľ��� ���� ���� ���
			delete pAttributeData;
		}
		if ((pAttributeData = new char[iDataLength + 1]) == NULL) {
			printf_debug("DataParser::findAttribute() - Fail : Fail to allocate for data buffer.\n");
			return false;
		}
		// ���� ���� ���� ����
		else {
			memcpy_s(pAttributeData, iDataLength, attributeDataBuffer, iDataLength);
			pAttributeData[iDataLength] = '\0';
		}

		// printf("�Ӽ�[%s]�� ������: %s", attributeName, pAttributeData); // Debugging code.

		// ������� �����ϸ� ���� ��ȯ
		return true;
	}
}

int DataParser::parseIntFromAttribute() {
	// ���� ���� ����
	int iReturn = 0;		// �Ľ� ����� ��ȯ�� ��

	// �Ľ��� �Ӽ� �����Ͱ� �����ϸ�
	if (pAttributeData != NULL) {
		// ���ڿ� ������ ���� ����
		if (strlen(pAttributeData) <= 0) {
			printf_debug("DataParser::parseFloatFromAttribute() - Cancled : No float data exist.\n");
			return -1;
		}

		// ���ڿ��� ������ ġȯ
		iReturn = atoi(pAttributeData);

		// �Ľ��ص� ������ �ı�
		delete pAttributeData;
		pAttributeData = NULL;
	}

	// ��ȯ���� -1 �̸� ������ ���ɼ��� ������ ����
	return iReturn;
}

float DataParser::parseFloatFromAttribute() {
	// ���� ���� ����
	float fReturn = 0.0f;		// �Ľ� ����� ��ȯ��re ��

	// �Ľ��� �Ӽ� �����Ͱ� �����ϸ�
	if (pAttributeData != NULL) {
		// ���ڿ� ������ ���� ����
		if (strlen(pAttributeData) <= 0) {
			printf_debug("DataParser::parseFloatFromAttribute() - Cancled : No float data exist.\n");
			return 0.0f;
		}

		// ���ڿ��� float���� ġȯ
		fReturn = (float)atof(pAttributeData);

		// �Ľ��ص� ������ �ı�
		delete pAttributeData;
		pAttributeData = NULL;
	}

	// ��ȯ���� 0.00 �̸� ������ ���ɼ��� ������ ����
	return fReturn;
}

char* DataParser::parseStringFromAttribute() {
	// ���� ���� ����
	char *pStrReturn = NULL;								// ���� ���� �� ��ȯ�� ���ڿ�
	char *pDataPtr = NULL;									// ���ڿ� �м��� ����� ������
	char tempStrBuffer[DEFAULT_ATTRIBUTENAME_MAX_LENGTH];	// ���ڿ��� �����ϱ� ���� �ӽ� ����
	int iTempBufferCnt = 0;									// �ӽ� ���ۿ� ä���� ���ڿ��� ���� ī��Ʈ
	bool bFoundString = false;								// ���� ���� ���鹮�ڸ� �����ϰ� ù ��° ���ڸ� �߰��� ����

	// ���� �ʱ�ȭ
	memset(tempStrBuffer, '\0', sizeof(tempStrBuffer));

	// �Ľ̵� �Ӽ� �����Ͱ� �����ϰ�
	if (pAttributeData != NULL) {
		// �� ���̰� 0�� �ƴϸ�
		if (strlen(pAttributeData) > 0) {
			// [���ڿ��� �� �ڷ� ���� ���� ���� ([Header]   Data   [/Header] => "Data")]
			pDataPtr = pAttributeData;
			do {
				// ���� ù ��° ���ڸ� ������ ���� ���
				if (bFoundString == false) {
					if (isspace(pDataPtr[0]) == 0) {
						// ���� ���ڰ� �ƴ� ��쿡�� ���ۿ� ����
						tempStrBuffer[iTempBufferCnt++] = pDataPtr[0];
						bFoundString = true;
					}
					else {
						// ù ���ڸ� ������ ���� ���� ���� ����
					}
				}
				// ù ��° ���ڸ� ���� ���ķδ� ���鹮�ڵ� ����
				else {
					tempStrBuffer[iTempBufferCnt++] = pDataPtr[0];
				}

				// ���� ���� ������ ����
				++pDataPtr;
			} while (pDataPtr[0] != '\0');
			
			// �� �̻� �ʿ� ���� pAttributeData �Ҵ� ����
			delete pAttributeData;
			pAttributeData = NULL;

			// [�ӽ� ����� ���ڿ����� ���ڿ� �޺κ��� ���� ���� ����]
			// �ӽ� ������ ���ڿ� ���� �˻�
			if (strlen(tempStrBuffer) <= 0) {
				printf_debug("DataParser::parseStringFromAttribute() - Cancled : No string data exist.\n");
				return false;
			}

			// �ӽ� ���� ������ �ڿ������� ����
			pDataPtr = &tempStrBuffer[strlen(tempStrBuffer) - 1];
			
			// ������ �տ� �������� ���� ���
			while (pDataPtr != &tempStrBuffer[0]) {
				// �ش� ��ġ�� ���ڰ� ���� ���ڸ� NULL�� ä������
				if (isspace(pDataPtr[0]) != 0) {
					*pDataPtr = '\0';
				}
				// �ش� ��ġ�� ���ڰ� ���� ���ڰ� �ƴϸ� Ż��
				else {
					break;
				}

				// ���� �����ͷ� ������ �̵�
				--pDataPtr;
			}

			// [���������� ������ ������ ������ ���� ������ �����Ͽ� ����]
			int iStrLen = strlen(tempStrBuffer);
			if ((pStrReturn = new char[iStrLen + 1]) == NULL) {
				printf_debug("DataParser::parseStringFromAttribute() - Fail : Fail to allocate for string data.\n");
				return false;
			}
			memcpy_s(pStrReturn, iStrLen, tempStrBuffer, iStrLen);
			pStrReturn[iStrLen] = '\0';
		}
	}

	// [���� ������ ���ڿ� ��ȯ]
	// ��ȯ���� NULL �̸� ����
	return pStrReturn;
}

int DataParser::printf_debug(char *str) {
#ifdef __DEBUG__
	return printf("%s", str);
#endif
	return 0;
}