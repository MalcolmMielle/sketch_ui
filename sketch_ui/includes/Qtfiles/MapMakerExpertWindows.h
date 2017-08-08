#ifndef MapMakerv3_H_EXPERT_WIN_08042016
#define MapMakerv3_H_EXPERT_WIN_08042016

#include <iostream>
#include <QtGui/QMainWindow>
#include <QtGui/QLabel>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QAction>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QSlider>
#include<QLCDNumber>
#include <QSpinBox>
#include <QTimer>

#include "MapMakerExpert.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <windows.h>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 1024
#define DEFAULT_PORT "8861"
#define SERVER_NAME "10.0.0.3"

#define DEBUG if (false) printf


namespace AASS{
		
	class MapMakerExpertWindows : public MapMakerExpert
	{
		Q_OBJECT
		
		
	private:
		WSADATA wsaData;
		SOCKET client_socket;
		char recvbuf[DEFAULT_BUFLEN];
		int recvbuflen;
		
		int  init_client_socket();
		int  close_client_socket();
		bool senddata(SOCKET sock, void *buf, int buflen);
		bool sendlong(SOCKET sock, long value);
		bool sendfile(SOCKET sock, FILE *f);
		bool readdata(SOCKET sock, void *buf, int buflen);
		bool readlong(SOCKET sock, long *value);
		bool readfile(SOCKET sock, FILE *f);
		void receive_file(SOCKET sock, const char* filename);
		void send_file(SOCKET sock, const char* filename);

	public:
		MapMakerExpertWindows(int x, int y, const QString& path);
		virtual ~MapMakerExpertWindows();
		
	public slots:
		virtual void Sysexport();

	};
}

#endif // MapMakerv3_H
