
// IT_5.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CIT5App:
// Сведения о реализации этого класса: IT_5.cpp
//

class CIT5App : public CWinApp
{
public:
	CIT5App();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CIT5App theApp;
