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
	// 동적 할당된 버퍼를 비움
	clearDataBuffer();

	// 포인터 초기화
	pHeaderPtr = NULL;
	pHeaderEndPtr = NULL;
	pAttributePtr = NULL;
	pAttributeData = NULL;
}

bool DataParser::openFile(char *fileName) {
	FILE *pFile = NULL;		// 파일 포인터
	char *pPtr = fileName;	// 파일명 포인터
	int iFileByteSize = 0;	// 파일 바이트 크기를 저장

	// 확장자가 .dat 파일인지 확인
	if ((pPtr += (strlen(fileName) - 4))) {
		if (pPtr[0] != '.' || pPtr[1] != 'd' || pPtr[2] != 'a' || pPtr[3] != 't') {
			printf_debug("DataParser::openFile() Error - The file '");	printf_debug(fileName);	printf_debug("' is not a '*.dat' file.\n");
			return false;
		}
	}

	// 파일 열기
	pFile = fopen(fileName, "rb");
	if (pFile == NULL) {
		printf_debug("DataParser::openFile() Error - Fail to open '");	printf_debug(fileName);	printf_debug("'./n");
		return false;
	}

	// 파일 크기만큼 버퍼 공간 생성
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

	// 파일 내용을 읽어서 버퍼에 삽입
	if (fread(this->pDataBuffer, 1, iFileByteSize, pFile) == -1) {
		printf_debug("DataParser::openFile() Error - Fail to read file.\n");
		fclose(pFile);
		return false;
	}
	pDataBuffer[iFileByteSize] = '\0';

	// 파일을 닫고 성공 반환
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
	// [지역 변수]
	int iHeaderLength = 0;

	// [오류 검사]
	// 헤더 이름이 NULL인 경우
	if (headerName == NULL) {
		printf_debug("DataParser::findHeader() Canceled - Enter correct header name pointer.\n");
		return false;
	}
	// 헤더 이름의 길이가 0보다 작거나 최대 길이를 초과한 경우
	iHeaderLength = strlen(headerName);
	if (iHeaderLength <= 0 || iHeaderLength > DEFAULT_HEADERNAME_MAX_LENGTH) {
		printf_debug("DataParser::findHeader() Canceled - Header name is too short or too long.\n");
		return false;
	}
	// 데이터 버퍼가 비어있는 경우
	if (pDataBuffer == NULL) {
		printf_debug("DataParser::findHeader() Canceled - Data buffer is empty. Open the file first.\n");
		return false;
	}

	// [초기화 및 지역변수 추가 선언]
	// HeaderPtr이 초기화되었거나, 처음으로 파일을 조사하는 경우
	if (pHeaderPtr == NULL) {
		pHeaderPtr = pDataBuffer;	// 버퍼의 시작 지점으로 설정
		initAttributePtr();			// 속성값을 가르키는 포인터를 NULL로 초기화
	}

	char headerBuffer[DEFAULT_HEADERNAME_MAX_LENGTH];	// 헤더명을 임시 저장할 공간
	char *pBuffer = NULL;								// 헤더명을 임시 저장할 공간의 다음 문자가 삽입될 위치를 가르키는 포인터
	bool bFoundHeader = false;							// 헤더의 발견 여부 (bFoundHeader = bHeaderBegin && bHeaderEnd)
	bool bHeaderBegin = false;							// 헤더 시작부분의 발견 여부
	bool bHeaderEnd = false;							// 헤더 종료부분과 명칭의 발견 여부
	
	// 1.[헤더 찾기 시작]
	// 여기부터 pDataBuffer를 순회하며 헤더 찾기 시작
	do {
		// 아직 헤더 시작인 '<' 를 발견하지 못한 경우
		if (bHeaderBegin == false) {
			if (pHeaderPtr[0] == '<') {
				bHeaderBegin = true;
				strnset(headerBuffer, '\0', sizeof(headerBuffer));
				pBuffer = &headerBuffer[0];
				// printf("<"); // Debugging Code
			}
		}
		// 헤더 시작인 '<' 를 발견 후 '>' 사이의 내용을 담는 경우
		else if (bHeaderBegin == true && bHeaderEnd == false) {
			// '>' 를 발견할 때 까지의 내용을 버퍼에 담음
			if (pHeaderPtr[0] != '>') {
				*pBuffer = pHeaderPtr[0];
				// printf("%c", *pBuffer); // Debugging Code

				// 범위 오류 탐지
				if ((++pBuffer) == &headerBuffer[DEFAULT_HEADERNAME_MAX_LENGTH]) {
					printf_debug("DataParser::findHeader() - Error : Header length is out of buffer!\n");
					break; 
				}
			}
			// '>' 를 발견하면 버퍼에 담긴 내용과 찾는 헤더의 이름을 비교
			else {
				if (strcmp(headerName, headerBuffer) == 0) {
					// printf("> 발견!"); // Debugging Code
					bHeaderEnd = true;
				}
				else {
					// printf("> 발견! - 일치하지 않는 헤더.\n"); // Debugging Code
					bHeaderBegin = false;
				}
			}

			// 헤더 발견 완료여부 검사
			if (bHeaderBegin == true && bHeaderEnd == true) {
				// printf(" - 일치하는 헤더.\n"); // Debugging Code
				bFoundHeader = true;
			}
		}

		// 다음 문자 조사
		while (1) {
			++pHeaderPtr;
			if ((pHeaderPtr[0] > 31 && pHeaderPtr[0] < 127) || (pHeaderPtr[0] == '\0')) break; // 제어 문자 (/n, /r 등을 건너 뜀)
		}
	} while (bFoundHeader == false && pHeaderPtr[0] != '\0');

	// [헤더 조회 결과]
	// 헤더를 찾지 못했을 경우
	if (bFoundHeader == false) {
		printf_debug("DataParser::findHeader() - Fail : Fail to find header <"); printf_debug(headerName);	printf_debug(">\n");

		// [pHeaderPtr]이 NULL을 가르키도록 하고,
		// [pAttributePtr]이 NULL을 가르키도록 함.
		initPtrAll();

		return false;
	}
	// 헤더를 찾은 경우
	else {
		// [pHeaderPtr]이 헤더 바로 다음 문자의 위치를 가르키고 있게 되고,
		// [pAttributePtr]은 헤더 포인터가 가르키는 위치를 가르키고 있게 된다.
		pAttributePtr = pHeaderPtr;

		// printf("헤더 <%s> 의 다음 데이터:\n%s\n", headerName, pHeaderPtr);		// Debugging code
		// printf("헤더 <%s> 의 속성 데이터:\n %s\n", headerName, pAttributePtr);	// Debugging code
	}

	// 2.[헤더 닫힘 부분 찾기 시작]
	// pHeaderPtr 이후의 데이터를 순회하며 닫힘 부분(pHeaderEndPtr) 찾기 시작
	bFoundHeader = false;
	bHeaderBegin = false;
	bHeaderEnd = false;
	pHeaderEndPtr = pHeaderPtr;
	do {
		// 아직 헤더 시작인 '<' 를 발견하지 못한 경우
		if (bHeaderBegin == false) {
			if (pHeaderEndPtr[0] == '<') {
				bHeaderBegin = true;
				strnset(headerBuffer, '\0', sizeof(headerBuffer));
				pBuffer = &headerBuffer[0];
				// printf("<"); // Debugging Code
			}
		}
		// 헤더 시작인 '<' 를 발견 후 '>' 사이의 내용을 담는 경우
		else if (bHeaderBegin == true && bHeaderEnd == false) {
			// '>' 를 발견할 때 까지의 내용을 버퍼에 담음
			if (pHeaderEndPtr[0] != '>') {
				*pBuffer = pHeaderEndPtr[0];
				// printf("%c", *pBuffer); // Debugging Code

				// 범위 오류 탐지
				if ((++pBuffer) == &headerBuffer[DEFAULT_HEADERNAME_MAX_LENGTH]) {
					printf_debug("DataParser::findHeader() - Error : Header length is out of buffer!\n");
					break;
				}
			}
			// '>' 를 발견하면 버퍼에 담긴 내용과 찾는 헤더의 이름을 비교
			else {
				// 헤더 종료구문의 시작이 '/'인지 검사
				if (headerBuffer[0] == '/') {
					// '/' 다음 위치부터 헤더 종료구문의 헤더 이름이 headerName과 일치하는지 검사
					if (strcmp(headerName, (headerBuffer + 1)) == 0) {
						// printf("> 발견!"); // Debugging Code
						bHeaderEnd = true;
					}
					else {
						// printf("> 발견! - 일치하지 않는 종료 헤더.\n"); // Debugging Code
						bHeaderBegin = false;
					}
				}
				else {
					// printf("> 발견! - 올바르지 않은 헤더 종료 구문.\n"); // Debugging Code
					bHeaderBegin = false;
				}
			}

			// 헤더 발견 완료여부 검사
			if (bHeaderBegin == true && bHeaderEnd == true) {
				// printf(" - 일치하는 종료 헤더.\n"); // Debugging Code
				bFoundHeader = true;
			}
		}

		// 다음 문자 조사
		while (1) {
			++pHeaderEndPtr;
			if ((pHeaderEndPtr[0] > 31 && pHeaderEndPtr[0] < 127) || (pHeaderEndPtr[0] == '\0')) break; // 제어 문자 (/n, /r 등을 건너 뜀)
		}
	} while (bFoundHeader == false && pHeaderEndPtr[0] != '\0');

	// [헤더 닫힘 조회 결과]
	// 닫힘 부분을 찾지 못했을 경우
	if (bFoundHeader == false) {
		printf_debug("DataParser::findHeader() - Fail : Fail to find header close. <");	printf_debug(headerName);	printf_debug(">\n");

		// [pHeaderPtr]이 NULL을 가르키도록 하고,
		// [pAttributePtr]이 NULL을 가르키도록 함.
		initPtrAll();

		return false;
	}
	// 닫힘 부분을 찾은 경우
	else {
		// [pHeaderPtr]은 헤더 바로 다음 문자의 위치를 가르키고 있게 되고,
		// [pHeaderEndPtr]은 헤더 종료 구문 바로 다음 위치를 가르키고 있고,
		// [pAttributePtr]은 헤더 포인터가 가르키는 위치를 가르키고 있게 된다.
		
		// 여기까지 도달해야 유일한 성공 반환
		return true;
	}
}

bool DataParser::findAttribute(char *attributeName) {
	// [지역 변수]
	int iAttributeLength = 0;

	// [오류 검사]
	// 속성 이름이 NULL인 경우
	if (attributeName == NULL) {
		printf_debug("DataParser::findAttribute() Canceled - Enter correct attribute name pointer.\n");
		return false;
	}
	// 속성 이름의 길이가 0보다 작거나 최대 길이를 초과한 경우
	iAttributeLength = strlen(attributeName);
	if (iAttributeLength <= 0 || iAttributeLength > DEFAULT_ATTRIBUTENAME_MAX_LENGTH) {
		printf_debug("DataParser::findAttribute() Canceled - Attribute name is too short or too long.\n");
		return false;
	}
	// 데이터 버퍼가 비어있는 경우
	if (pDataBuffer == NULL) {
		printf_debug("DataParser::findAttribute() Canceled - Data buffer is empty. Open the file first.\n");
		return false;
	}
	// HeaderPtr이 NULL 인 경우
	if (pHeaderPtr == NULL) {
		printf_debug("DataParser::findAttribute() Canceled - Header pointer is NULL. Use this->findHeader(\"...\") function first.\n");
		return false;
	}
	
	// [초기화 및 지역변수 추가 선언]
	// 속성값을 조사하기 전 항상 해당 속성 소유 헤더의 데이터 시작 위치로 초기화
	pAttributePtr = pHeaderPtr;

	char attributeBuffer[DEFAULT_ATTRIBUTENAME_MAX_LENGTH];		// 속성명을 임시 저장할 공간
	char *pBuffer = NULL;										// 속성명을 임시 저장할 공간의 다음 문자가 삽입될 위치를 가르키는 포인터
	bool bFoundAttribute = false;								// 속성의 발견 여부 (bFoundAttribute = bAttributeBegin && bAttributeEnd)
	bool bAttributeBegin = false;								// 속성 시작부분의 발견 여부
	bool bAttributeEnd = false;									// 속성 종료부분과 명칭의 발견 여부

	// 1. [속성 찾기 시작]
	// 여기부터 pHeaderPtr이후의 pDataBuffer를 순회하며 속성 찾기 시작
	do {
		// 아직 속성 시작인 '[' 를 발견하지 못한 경우
		if (bAttributeBegin == false) {
			if (pAttributePtr[0] == '[') {
				bAttributeBegin = true;
				strnset(attributeBuffer, '\0', sizeof(attributeBuffer));
				pBuffer = &attributeBuffer[0];
				// printf("["); // Debugging Code
			}
		}
		// 속성 시작인 '[' 를 발견 후 ']' 사이의 내용을 담는 경우
		else if (bAttributeBegin == true && bAttributeEnd == false) {
			// ']' 를 발견할 때 까지의 내용을 버퍼에 담음
			if (pAttributePtr[0] != ']') {
				*pBuffer = pAttributePtr[0];
				 // printf("%c", *pBuffer); // Debugging Code

				// 범위 오류 탐지
				 if ((++pBuffer) == &attributeBuffer[256]) {
					printf_debug("DataParser::findAttribute() - Error : Attribute length is out of buffer!");
					break;
				}
			}
			// ']' 를 발견하면 버퍼에 담긴 내용과 찾는 속성의 이름을 비교
			else {
				if (strcmp(attributeName, attributeBuffer) == 0) {
					// printf("] 발견!"); // Debugging Code
					bAttributeEnd = true;
				}
				else {
					// printf("] 발견! - 일치하지 않는 속성.\n"); // Debugging Code
					bAttributeBegin = false;
				}
			}

			// 속성 발견 완료여부 검사
			if (bAttributeBegin == true && bAttributeEnd == true) {
				// printf(" - 일치하는 속성.\n"); // Debugging Code
				bFoundAttribute = true;
			}
		}

		// 다음 문자 조사
		while (1) {
			++pAttributePtr;
			if ((pAttributePtr[0] > 31 && pAttributePtr[0] < 127) || (pAttributePtr[0] == '\0')) break; // 제어 문자 (/n, /r 등을 건너 뜀)
		}
	} while (bFoundAttribute == false && pAttributePtr != pHeaderEndPtr); // pAttributePtr != pHeaderEndPtr 부분에서 속성이 헤더 종료 구문을 넘어서 조사하는 일을 예방

	// [속성 찾기 결과]
	// 속성을 찾지 못했을 경우
	if (bFoundAttribute == false) {
		printf_debug("DataParser::findAttribute() - Fail : Fail to find attribute [");	printf_debug(attributeName);	printf_debug("].\n");

		// [pAttributePtr]이 NULL을 가르키도록 함.
		initAttributePtr();

		return false;
	}
	// 속성을 찾은 경우
	else {
		// [pAttributePtr]은 속성 시작의 다음 부분을 가르키게 된다.
	}

	// 2.[속성 닫힘 부분 찾기 시작]
	// pAttributePtr 이후의 데이터를 pHeaderEndPtr까지 순회하며 닫힘 부분([/attributeName]) 찾기 시작
	char attributeDataBuffer[DEFAULT_ATTRIBUTENAME_MAX_LENGTH];		// 속성의 데이터를 임시 저장할 공간 생성
	char *pDataBufferPtr = &attributeDataBuffer[0];					// 속성의 데이터를 임시 저장할 공간에 다음에 데이터를 넣을 위치를 가르키는 포인터
	memset(attributeDataBuffer, '\0', sizeof(attributeDataBuffer));	// 데이터 임시 저장 공간 초기화
	bFoundAttribute = false;
	bAttributeBegin = false;
	bAttributeEnd = false;

	do {
		// 속성 데이터를 임시 공간에 기록
		*pDataBufferPtr = pAttributePtr[0];

		// 임시 저장공간 버퍼가 초과하는지 검사
		if ((++pDataBufferPtr) == &attributeDataBuffer[DEFAULT_ATTRIBUTENAME_MAX_LENGTH]) {
			printf_debug("DataParser::findAttribute() - Fail : Attribute data is too long.\n");
			break;
		}

		// 아직 속성 닫힘 시작인 '[' 를 발견하지 못한 경우
		if (bAttributeBegin == false) {
			if (pAttributePtr[0] == '[') {
				bAttributeBegin = true;
				strnset(attributeBuffer, '\0', sizeof(attributeBuffer));
				pBuffer = &attributeBuffer[0];
				// printf("["); // Debugging Code
			}
		}
		// 속성 시작인 '[' 를 발견 후 ']' 사이의 내용을 담는 경우
		else if (bAttributeBegin == true && bAttributeEnd == false) {
			// ']' 를 발견할 때 까지의 내용을 버퍼에 담음
			if (pAttributePtr[0] != ']') {
				*pBuffer = pAttributePtr[0];
				// printf("%c", *pBuffer); // Debugging Code

				// 범위 오류 탐지
				if ((++pBuffer) == &attributeBuffer[DEFAULT_ATTRIBUTENAME_MAX_LENGTH]) {
					// printf("DataParser::findAttribute() - Error : Attribute length is out of buffer[%d]!\n", sizeof(attributeBuffer));
					break;
				}
			}
			// ']' 를 발견하면 버퍼에 담긴 내용과 찾는 헤더의 이름을 비교
			else {
				if (attributeBuffer[0] == '/') {
					if (strcmp(attributeName, (attributeBuffer + 1)) == 0) {
						// printf("] 발견!\n"); // Debugging Code
						bAttributeEnd = true;
					}
					else {
						// printf("] 발견! - 일치하지 않는 속성 닫힘.\n"); // Debugging Code
						bAttributeBegin = false;
					}
				}
				else {
					// printf("] 발견! - 올바르지 않은 속성 종료 구문.\n"); // Debugging Code
					bAttributeBegin = false;
				}
			}

			// 속성 발견 완료여부 검사
			if (bAttributeBegin == true && bAttributeEnd == true) {
				// printf(" - 일치하는 속성 닫기 구문.\n"); // Debugging Code
				bFoundAttribute = true;
			}
		}

		// 다음 문자 조사
		while (1) {
			++pAttributePtr;
			if ((pAttributePtr[0] > 31 && pAttributePtr[0] < 127) || (pAttributePtr[0] == '\0')) break; // 제어 문자 (/n, /r 등을 건너 뜀)
		}
	} while (bFoundAttribute == false && pAttributePtr[0] != '\0');

	// [속성 닫힘 찾기 결과]
	// 속성이 닫히는 부분을 찾지 못했을 경우
	if (bFoundAttribute == false) {
		printf_debug("DataParser::findAttribute() - Fail : Fail to find attribute close. [");	printf_debug(attributeName);	printf_debug("].\n");

		// [pAttributePtr]이 NULL을 가르키도록 함.
		initAttributePtr();

		return false;
	}
	// 속성이 닫히는 부분을 찾은 경우
	else {
		// [pAttributePtr]은 속성이 닫히는 위치를 가르키고 있게 되고,
		// [pAttributeData]는 동적 생성된 속성의 데이터값을 가르키고 있게 된다.
		
		// 속성 닫기 구문[/]과 속성명의 길이만큼을 뺀 위치에 '\0' 문자를 넣음 ([/attributeName]을 Data로 간주하지 않기 위함)
		*(pDataBufferPtr - (3 + iAttributeLength)) = '\0';

		// 동적 공간 생성
		int iDataLength = strlen(attributeDataBuffer);
		if (pAttributeData != NULL) { // 기존에 다른 속성의 데이터를 저장한 후 파싱한 적이 없는 경우
			delete pAttributeData;
		}
		if ((pAttributeData = new char[iDataLength + 1]) == NULL) {
			printf_debug("DataParser::findAttribute() - Fail : Fail to allocate for data buffer.\n");
			return false;
		}
		// 동적 공간 생성 성공
		else {
			memcpy_s(pAttributeData, iDataLength, attributeDataBuffer, iDataLength);
			pAttributeData[iDataLength] = '\0';
		}

		// printf("속성[%s]의 데이터: %s", attributeName, pAttributeData); // Debugging code.

		// 여기까지 도달하면 성공 반환
		return true;
	}
}

int DataParser::parseIntFromAttribute() {
	// 지역 변수 선언
	int iReturn = 0;		// 파싱 결과로 반환될 값

	// 파싱한 속성 데이터가 존재하면
	if (pAttributeData != NULL) {
		// 문자열 데이터 길이 조사
		if (strlen(pAttributeData) <= 0) {
			printf_debug("DataParser::parseFloatFromAttribute() - Cancled : No float data exist.\n");
			return -1;
		}

		// 문자열을 정수로 치환
		iReturn = atoi(pAttributeData);

		// 파싱해둔 데이터 파괴
		delete pAttributeData;
		pAttributeData = NULL;
	}

	// 반환값이 -1 이면 오류일 가능성이 있음에 주의
	return iReturn;
}

float DataParser::parseFloatFromAttribute() {
	// 지역 변수 선언
	float fReturn = 0.0f;		// 파싱 결과로 반환될re 값

	// 파싱한 속성 데이터가 존재하면
	if (pAttributeData != NULL) {
		// 문자열 데이터 길이 조사
		if (strlen(pAttributeData) <= 0) {
			printf_debug("DataParser::parseFloatFromAttribute() - Cancled : No float data exist.\n");
			return 0.0f;
		}

		// 문자열을 float으로 치환
		fReturn = (float)atof(pAttributeData);

		// 파싱해둔 데이터 파괴
		delete pAttributeData;
		pAttributeData = NULL;
	}

	// 반환값이 0.00 이면 오류일 가능성이 있음에 주의
	return fReturn;
}

char* DataParser::parseStringFromAttribute() {
	// 지역 변수 선언
	char *pStrReturn = NULL;								// 동적 생성 후 반환될 문자열
	char *pDataPtr = NULL;									// 문자열 분석에 사용할 포인터
	char tempStrBuffer[DEFAULT_ATTRIBUTENAME_MAX_LENGTH];	// 문자열을 추출하기 위한 임시 버퍼
	int iTempBufferCnt = 0;									// 임시 버퍼에 채워진 문자열의 수를 카운트
	bool bFoundString = false;								// 문자 앞의 공백문자를 제외하고 첫 번째 문자를 발견한 여부

	// 변수 초기화
	memset(tempStrBuffer, '\0', sizeof(tempStrBuffer));

	// 파싱된 속성 데이터가 존재하고
	if (pAttributeData != NULL) {
		// 그 길이가 0이 아니면
		if (strlen(pAttributeData) > 0) {
			// [문자열의 앞 뒤로 공백 문자 제거 ([Header]   Data   [/Header] => "Data")]
			pDataPtr = pAttributeData;
			do {
				// 아직 첫 번째 문자를 만나지 못한 경우
				if (bFoundString == false) {
					if (isspace(pDataPtr[0]) == 0) {
						// 공백 문자가 아닌 경우에만 버퍼에 저장
						tempStrBuffer[iTempBufferCnt++] = pDataPtr[0];
						bFoundString = true;
					}
					else {
						// 첫 문자를 만나기 전의 공백 문자 무시
					}
				}
				// 첫 번째 문자를 만난 이후로는 공백문자도 저장
				else {
					tempStrBuffer[iTempBufferCnt++] = pDataPtr[0];
				}

				// 다음 문자 데이터 조사
				++pDataPtr;
			} while (pDataPtr[0] != '\0');
			
			// 더 이상 필요 없는 pAttributeData 할당 해제
			delete pAttributeData;
			pAttributeData = NULL;

			// [임시 저장된 문자열에서 문자열 뒷부분의 공백 문자 제거]
			// 임시 버퍼의 문자열 길이 검사
			if (strlen(tempStrBuffer) <= 0) {
				printf_debug("DataParser::parseStringFromAttribute() - Cancled : No string data exist.\n");
				return false;
			}

			// 임시 저장 버퍼의 뒤에서부터 조사
			pDataPtr = &tempStrBuffer[strlen(tempStrBuffer) - 1];
			
			// 버퍼의 앞에 도달하지 않은 경우
			while (pDataPtr != &tempStrBuffer[0]) {
				// 해당 위치의 문자가 공백 문자면 NULL로 채워넣음
				if (isspace(pDataPtr[0]) != 0) {
					*pDataPtr = '\0';
				}
				// 해당 위치의 문자가 공백 문자가 아니면 탈출
				else {
					break;
				}

				// 이전 데이터로 포인터 이동
				--pDataPtr;
			}

			// [최종적으로 정리된 버퍼의 내용을 동적 공간을 생성하여 담음]
			int iStrLen = strlen(tempStrBuffer);
			if ((pStrReturn = new char[iStrLen + 1]) == NULL) {
				printf_debug("DataParser::parseStringFromAttribute() - Fail : Fail to allocate for string data.\n");
				return false;
			}
			memcpy_s(pStrReturn, iStrLen, tempStrBuffer, iStrLen);
			pStrReturn[iStrLen] = '\0';
		}
	}

	// [동적 생성된 문자열 반환]
	// 반환값이 NULL 이면 오류
	return pStrReturn;
}

int DataParser::printf_debug(char *str) {
#ifdef __DEBUG__
	return printf("%s", str);
#endif
	return 0;
}