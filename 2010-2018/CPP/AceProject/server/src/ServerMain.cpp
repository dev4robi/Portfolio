#include "StdHeader_Server.h"
#include "GameServer.h"

int main(int argc, char **argv) {
	/* ���α׷� ���� */
	try {
		GameServer server;
		server.startMainLoop();
	} 
	catch (exception e) { e.what(); }

	return 0;
}