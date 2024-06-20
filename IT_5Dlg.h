
// IT_5Dlg.h: файл заголовка
//

#pragma once
#include "Drawer.h"
#include <vector>
#include <cmath>

#define PI 3.1415926535

// Диалоговое окно CIT5Dlg
class CIT5Dlg : public CDialogEx
{
// Создание
public:
	CIT5Dlg(CWnd* pParent = nullptr);	// стандартный конструктор

	struct cmplx
	{
		float real;
		float image;
};
	cmplx* Cmplx;
	void fourea(struct cmplx* data, int n, int is);

	void SpectrCalc();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IT_5_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	//afx_msg void OnTimer(UINT_PTR nIDEvent); // таймер
public:
	afx_msg void OnBnClickedOk(); // кнопка для построения графиков исходного сигнала и его амплитудного спектра
	afx_msg void OnBnClickedVosst(); // кнопка для восстановления сигнала
	afx_msg void OnBnClickedSdvig(); // кнопка сдвига восстановленного сигнала
	afx_msg void OnBnClickedReverse(); // кнопка отражения восстановленного сигнала

	double X(int); // функция исходного сигнала
	// 1-ый купол
	double _A1;
	double _m1;
	double _sigma1;
	// 2-ой купол
	double _A2;
	double _m2;
	double _sigma2;
	// 3-ий купол
	double _A3;
	double _m3;
	double _sigma3;

	// исходный сигнал
	Drawer Signal;
	vector<double> signal;

	vector<double> N; // точки

	// амплитудный спектр
	Drawer Amp_Spectr;
	vector<double> amp_spectr;

	// восстановленный сигнал
	vector<double> signal_vosst;
	void X_vosst();
	double diff(vector<double>, vector<double>);
	double err;
	double _eps;

	double A; // максимальная амплитуда сигнала
	double A_amp_spectr;// максимальная амплитуда спектра (амплитудного)

	double _fd; // частота дискретизации
	int _N; // количество отсчетов

	int k = 0;

	double _t0; // сдвиг

	vector<double> amp_spectr_2;
};