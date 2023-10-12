#include <Windows.h>
#include <datetimeapi.h>
#include <iostream>
#include <fstream>
#include <tchar.h>
#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"advapi32.lib")
#pragma comment(lib,"mincore.lib")//Api-ms-win-core-version-l1-2-3.dll

using namespace std;

class cFuncoes
{
private:

	SYSTEMTIME Tempo;

public:

	void ObtendoPermissaoParaFuncoes()
	{
		LUID Privilegio;
		LookupPrivilegeValue(0, SE_SYSTEMTIME_NAME, &Privilegio);

		HANDLE Token;
		OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &Token);

		TOKEN_PRIVILEGES Privilegios;
		Privilegios.PrivilegeCount = 1;
		Privilegios.Privileges[0].Luid = Privilegio;
		Privilegios.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

		BOOL Result = AdjustTokenPrivileges(Token, FALSE, &Privilegios, 0, 0, 0);
		if (Result == NULL)
			cout << "Ocorreu um erro ao ajustar os privilégios de acesso ao tempo..\n";
		else
			cout << "Sucesso ao atribuir os privilégios necessários..\n";
	}

	void ObterData(wstring Formato)
	{
		/*
		*
		* Macro LOCALE_NAME_SYSTEM_DEFAULT para obter a data fornecida pelo sistema
		* Macro LOCALE_NAME_USER_DEFAULT para obter a data fornecida por usuário.
		*/
		TCHAR Data[MAX_PATH];
		GetDateFormatEx(LOCALE_NAME_SYSTEM_DEFAULT, DATE_USE_ALT_CALENDAR, 0, Formato.c_str(), Data, sizeof(Data), 0);

		_tprintf(L"Calendário: %s", Data);
	}

	void ObterHorario(wstring Formato)
	{
		/*
		*
		* Macro LOCALE_NAME_SYSTEM_DEFAULT para obter a hora fornecida pelo sistema
		* Macro LOCALE_NAME_USER_DEFAULT para obter a hora fornecida por usuário.
		*/
		TCHAR Hora[MAX_PATH];
		GetTimeFormatEx(LOCALE_NAME_SYSTEM_DEFAULT, TIME_FORCE24HOURFORMAT, 0, Formato.c_str(), Hora, sizeof(Hora));

		_tprintf(L"Hora: %s", Hora);
	}

	void AlterarDataHora(WORD Dia, WORD Mes, WORD Ano, WORD Hora, WORD Minuto, WORD Segundo)
	{
		//Data
		Tempo.wDay = Dia;
		Tempo.wMonth = Mes;
		Tempo.wYear = Ano;

		//Hora
		Tempo.wHour = Hora;
		Tempo.wMinute = Minuto;
		Tempo.wSecond = Segundo;

		SetLocalTime(&Tempo);
		SetSystemTime(&Tempo);
	}

	//Efetuar a sincronização correta de data e hora.
	void AjustePrescisoDeTempo(BOOL Ajustar)
	{
		DWORD64 Tempo;
		DWORD64 Incrementos;
		BOOL Status;
		GetSystemTimeAdjustmentPrecise(&Tempo, &Incrementos, &Status);

		cout << "O ajuste está definido para: " << Tempo << " em incrementos de: " << Incrementos << "\n";
		if (Status == TRUE)
			cout << "Habilitado\n";
		else
			cout << "Desabilitado\n";

		//Ajuste
		SetSystemTimeAdjustmentPrecise(Tempo, Ajustar);
	}

}Funcoes;

int main()
{
	cout << "O assistente está obtendo informações de data e hora e efetuando configurações...\n";

	Funcoes.ObtendoPermissaoParaFuncoes();

	Funcoes.ObterData(L"dddddd',' dd/MM/yyyy\n");
	//Use "hh:mm" para obter hora e minuto ou "hh:mm:ss" para obter hora, minuto e segundo.
	Funcoes.ObterHorario(L"hh:mm\n");

	Funcoes.AlterarDataHora(0, 0, 1601, 0, 0, 0);

	Funcoes.AjustePrescisoDeTempo(TRUE);

	system("pause");
}