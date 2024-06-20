// Решение фазовой проблемы с помощью алгоритма Фиенупа

// IT_5Dlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "IT_5.h"
#include "IT_5Dlg.h"
#include "afxdialogex.h"
#include "Drawer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//bool killtimer = false; // флаг убийства таймера
//int timer; // переменная для таймера

// Диалоговое окно CIT5Dlg

CIT5Dlg::CIT5Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_IT_5_DIALOG, pParent)
	, _A1(1)
	, _m1(0.05)
	, _sigma1(0.001)
	, _A2(3)
	, _m2(0.2)
	, _sigma2(0.002)
	, _A3(2)
	, _m3(0.4)
	, _sigma3(0.003)
	, _N(512)
	, _fd(1000)
	, _t0(0)
	, _eps(1e-6)
	, err(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIT5Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_A1, _A1);
	DDX_Text(pDX, IDC_M1, _m1);
	DDX_Text(pDX, IDC_SIGMA1, _sigma1);
	DDX_Text(pDX, IDC_A2, _A2);
	DDX_Text(pDX, IDC_M2, _m2);
	DDX_Text(pDX, IDC_SIGMA2, _sigma2);
	DDX_Text(pDX, IDC_A3, _A3);
	DDX_Text(pDX, IDC_M3, _m3);
	DDX_Text(pDX, IDC_SIGMA3, _sigma3);
	DDX_Text(pDX, IDC_FD, _fd);
	DDX_Text(pDX, IDC_N, _N);
	DDX_Text(pDX, IDC_EDIT1, _t0);
	DDX_Text(pDX, IDC_DELTA, _eps);
	DDX_Text(pDX, IDC_ERR, err);
}

BEGIN_MESSAGE_MAP(CIT5Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CIT5Dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_SDVIG, &CIT5Dlg::OnBnClickedSdvig)
	ON_BN_CLICKED(IDC_REVERSE, &CIT5Dlg::OnBnClickedReverse)
	//ON_WM_TIMER()
	ON_BN_CLICKED(IDC_VOSST, &CIT5Dlg::OnBnClickedVosst)
END_MESSAGE_MAP()


// Обработчики сообщений CIT5Dlg

BOOL CIT5Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию
	Signal.Create(GetDlgItem(IDC_SIGNAL)->GetSafeHwnd());
	Amp_Spectr.Create(GetDlgItem(IDC_AMP_SPECTR)->GetSafeHwnd());

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CIT5Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CIT5Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// функция исходного сигнала (сумма гауссовых куполов)
double CIT5Dlg::X(int n)
{
	double dt = 1 / _fd;
	double s1 = _A1 * exp(-(pow((n * dt) - _m1, 2) / (2 * _sigma1 * _sigma1)));
	double s2 = _A2 * exp(-(pow((n * dt) - _m2, 2) / (2 * _sigma2 * _sigma2)));
	double s3 = _A3 * exp(-(pow((n * dt) - _m3, 2) / (2 * _sigma3 * _sigma3)));
	return s1 + s2 + s3;
}

// быстрое преобразование Фурье
void CIT5Dlg::fourea(struct cmplx* data, int n, int is)
{
	int i, j, istep;
	int m, mmax;
	float r, r1, theta, w_r, w_i, temp_r, temp_i;
	float pi = 3.1415926f;

	r = pi * is;
	j = 0;
	for (i = 0; i < n; i++)
	{
		if (i < j)
		{
			temp_r = data[j].real;
			temp_i = data[j].image;
			data[j].real = data[i].real;
			data[j].image = data[i].image;
			data[i].real = temp_r;
			data[i].image = temp_i;
		}
		m = n >> 1;
		while (j >= m)
		{
			j -= m;
			m = (m + 1) / 2;
		}
		j += m;
	}
	mmax = 1;
	while (mmax < n)
	{
		istep = mmax << 1;
		r1 = r / (float)mmax;
		for (m = 0; m < mmax; m++)
		{
			theta = r1 * m;
			w_r = (float)cos((double)theta);
			w_i = (float)sin((double)theta);
			for (i = m; i < n; i += istep)
			{
				j = i + mmax;
				temp_r = w_r * data[j].real - w_i * data[j].image;
				temp_i = w_r * data[j].image + w_i * data[j].real;
				data[j].real = data[i].real - temp_r;
				data[j].image = data[i].image - temp_i;
				data[i].real += temp_r;
				data[i].image += temp_i;
			}
		}
		mmax = istep;
	}
	if (is > 0)
		for (i = 0; i < n; i++)
		{
			data[i].real /= (float)n;
			data[i].image /= (float)n;
		}
}

//спектр
void CIT5Dlg::SpectrCalc()
{
	//массив спектра
	A_amp_spectr = 0;
	for (int i = 0; i < _N; i++)
	{
		Cmplx = new cmplx[_N];
		for (int j = 0; j < _N; j++)
		{
			Cmplx[j].real = (double)signal[j];
			Cmplx[j].image = 0;
		}

		fourea(Cmplx, _N, -1);

		amp_spectr.push_back(sqrt(Cmplx[i].image * Cmplx[i].image + Cmplx[i].real * Cmplx[i].real));
		N.push_back(i * _fd / _N);
		if (amp_spectr[i] > A_amp_spectr) A_amp_spectr = amp_spectr[i];
	}
	delete[] Cmplx;
}

// кнопка для построения исходного сигнала и его амплитудного спектра
void CIT5Dlg::OnBnClickedOk()
{
	signal.clear();
	N.clear();
	amp_spectr.clear();

	UpdateData(true);

	// гауссовы купола (сигнал)
	A = 0;
	for (int n = 0; n < _N; n++)
	{
		double res = X(n);
		signal.push_back(res);
		if (signal[n] > A) A = signal[n];
		N.push_back(n);
	}
	// график сигнала
	Signal.Draw(3, signal, N, _N, A, N[_N - 1]);
	Signal.DrawRis(3, 1, signal, N, _N, A, N[_N - 1]);

	// амплитудный спектр
	N.clear();
	SpectrCalc();
	Amp_Spectr.Draw(2, amp_spectr, N, _N, A_amp_spectr, N[_N - 1]);
	Amp_Spectr.DrawRis(2, 1, amp_spectr, N, _N, A_amp_spectr, N[_N - 1]);
}

// ф-ция для рассчета разности
double CIT5Dlg::diff(vector<double> a, vector<double> b)
{
	err = 0;
	for (int i = 0; i < _N; i++)
	{
		err += sqrt((a[i] - b[i]) * (a[i] - b[i]));
	}
	return err;
}

// функция восстановления сигнала
void CIT5Dlg::X_vosst()
{
	//UpdateData(true); killtimer = false;
	for (int i = 0; i < _N; i++) signal_vosst.push_back(0);
	cmplx* x_k = new cmplx[_N];
	vector<double> amp_spectr_2(_N); // для пересчета модуля спектра
	vector<double> phase(_N); // фаза

	while (diff(amp_spectr_2, amp_spectr) > _eps && k < 1000)
	{
		if (k == 1) // первая итерация
		{
			double fi_0 = 0;
			for (int i = 0; i < _N; i++)
			{
				fi_0 = 2. * PI * rand() / RAND_MAX - PI;
				x_k[i].real = amp_spectr[i] * cos(fi_0);
				x_k[i].image = amp_spectr[i] * sin(fi_0);
			}
		}
		k++;
		fourea(x_k, _N, 1); // обратное фурье
		// ограничения во временнной области
		A = 0;
		for (int i = 0; i < _N; i++)
		{
			if(x_k[i].real >= 0) signal_vosst[i] = x_k[i].real;
			if (x_k[i].real < 0) x_k[i].real = 0;
			x_k[i].image = 0;

			if (signal_vosst[i] > A) A = signal_vosst[i];
		}
		fourea(x_k, _N, -1); // переход в частотную область
		for (int i = 0; i < _N; i++)
		{
			phase[i] = atan(x_k[i].image / x_k[i].real);
			amp_spectr_2[i] = sqrt(x_k[i].image * x_k[i].image + x_k[i].real * x_k[i].real);
			x_k[i].real = amp_spectr[i] * cos(phase[i]);
			x_k[i].image = amp_spectr[i] * sin(phase[i]);
		}
		
		// графики исходного и восстановленного сигнала
		Signal.Draw(2, signal_vosst, N, _N, A, N[_N - 1]);
		Signal.DrawRis(2, 1, signal, N, _N, A, N[_N - 1]);
		Signal.DrawRis(2, 2, signal_vosst, N, _N, A, N[_N - 1]);

		err = diff(amp_spectr_2, amp_spectr);
		UpdateData(false);
	} 
	//killtimer = true;
}

// кнопка сдвига восстановленного сигнала
void CIT5Dlg::OnBnClickedSdvig()
{
	UpdateData();

	vector<double> sdvig = signal_vosst;

	if (_t0 > 0)
	{
		for (int i = 0; i < _t0; i++)
			signal_vosst[i] = sdvig[_N - _t0 + i];

		for (int i = _t0; i < _N; i++)
			signal_vosst[i] = sdvig[i - _t0];
	}
	else
	{
		for (int i = 0; i < _N + _t0; i++)
		{
			signal_vosst[i] = sdvig[i - _t0];
		}
		for (int i = _N + _t0; i < _N; i++)
		{
			signal_vosst[i] = sdvig[i - _N - _t0];
		}
	}
	// график сдвинутого сигнала
	Signal.Draw(2, signal_vosst, N, _N, A, N[_N - 1]);
	Signal.DrawRis(2, 2, signal_vosst, N, _N, A, N[_N - 1]);
	Signal.DrawRis(2, 1, signal, N, _N, A, N[_N - 1]);
}

// кнопка отражения восстановленного сигнала
void CIT5Dlg::OnBnClickedReverse()
{
	for (int i = 0; i < _N / 2; i++)
	{
		double reverse = signal_vosst[i];
		signal_vosst[i] = signal_vosst[_N - i - 1];
		signal_vosst[_N - i - 1] = reverse;
	}
	// график отраженного сигнала
	Signal.Draw(2, signal_vosst, N, _N, A, N[_N - 1]);
	Signal.DrawRis(2, 2, signal_vosst, N, _N, A, N[_N - 1]);
	Signal.DrawRis(2, 1, signal, N, _N, A, N[_N - 1]);
}

// кнопка для восстановления сигнала
void CIT5Dlg::OnBnClickedVosst()
{
	signal_vosst.clear();

	//UpdateData(true);
	X_vosst();
}