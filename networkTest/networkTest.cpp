#include "../dataTransfer/dataTransfer/dataTransfer.h"
#include <comdef.h> 
#include <codecvt>

class CNetworkTestCmd : public datatransfer::networktest::CNetworkTest
{
	#pragma region Public_Method
	public:
	//==========================================================================
		/**
		* конструктор тестирования сети как клиент.
		* @param strIp - IP адрес сервера.
		* @param wPort - порт сервера.
		* @param dwCountClient - количество клиентов.
		* @param sInfoClient - параметры тестирования сети.
		* @param pIocp - механизм Iocp.
		*/
		CNetworkTestCmd(
			const std::string strIp,
			const WORD wPort,
			const DWORD dwCountClient,
			const SInfoClient& sInfoClient,
			const std::shared_ptr<wname::io::iocp::CIocp>& pIocp)
			:CNetworkTest(strIp, wPort, dwCountClient, sInfoClient, pIocp)
		{
			
		}
	//==========================================================================
		/**
		* конструктор тестирования сети как сервер.
		* @param strIp - IP адрес сервера.
		* @param wPort - порт сервера.
		* @param pIocp - механизм Iocp.
		*/
		CNetworkTestCmd(
			const std::string strIp,
			const WORD wPort,
			const std::shared_ptr<wname::io::iocp::CIocp>& pIocp)
			:CNetworkTest(strIp, wPort, pIocp)
		{

		}
	//==========================================================================
		/**
		* уведомление о подключение клиента.
		* @param pClient - клиент.
		* @param dwReconnect - количество переподключений.
		* @param ec - ошибка подключения.
		*/
		void connectedClientHandler(
			INetworkTestStatistic* const pClient,
			const DWORD dwReconnect,
			const std::error_code ec) noexcept override
		{
			try
			{
				std::wstring wStrRecconnect;
				if (dwReconnect > 0)
				{
					wStrRecconnect.append(L". Попыток подключения: ");
					wStrRecconnect.append(std::to_wstring(dwReconnect));
				}

				if (ec)
				{
					wprintf(L"\nКлиент %s подключился с ошибкой: %u%s",
						pClient->getAddress().getAddress().c_str(),
						ec.value(),
						wStrRecconnect.c_str());
					return;
				}
				wprintf(L"\nКлиент подключился: %s%s", 
					pClient->getAddress().getAddress().c_str(),
					wStrRecconnect.c_str());
			}
			catch (const std::exception& ex)
			{
				_pIocp->log(wname::logger::EMessageType::warning, ex);
			}		
		}
	//==========================================================================
		/**
		* уведомление об тике сбора статистики.
		* @param statisticClients - статистика.
		*/
		void statisticHandler(
			const std::unordered_map<INetworkTestStatistic*, SStatisticClient>& statisticClients) noexcept override
		{
			auto format = [] (UINT64 nValue)
			{ 
				if (nValue < (1024 * 10))
				{
					wprintf(L" %8I64u байт", nValue);
					return;
				}

				nValue /= 1024;
				if (nValue < (1024 * 10))
				{
					wprintf(L"%8I64u Кбайт", nValue);
					return;
				}

				nValue /= 1024;
				if (nValue < (1024 * 10))
				{
					wprintf(L"%8I64u Мбайт", nValue);
					return;
				}

				nValue /= 1024;
				if (nValue < (1024 * 10))
				{
					wprintf(L"%8I64u Гбайт", nValue);
					return;
				}

				nValue /= 1024;
				if (nValue < (1024 * 10))
				{
					wprintf(L"%8I64u Тбайт", nValue);
					return;
				}
			};
			wprintf(L"\n\n\tАдрес клиента          Recv      Recv avg      Recv all          Send      Send avg      Send all");
			UINT64 nRecvDataClients = 0;
			UINT64 nAvgRecvDataClients = 0;
			UINT64 nAllRecvDataClients = 0;
			UINT64 nSendDataClients = 0;
			UINT64 nAvgSendDataClients = 0;
			UINT64 nAllSendDataClients = 0;

			for (auto& [k, v] : statisticClients)
			{
				assert(k != nullptr);

				wprintf(L"\n%21s", k->getAddress().getAddress().c_str()); 
				format(v.nRecvData);
				format(v.nAvgRecvData);
				format(v.nAllRecvData);
				format(v.nSendData);
				format(v.nAvgSendData);
				format(v.nAllSendData);

				/** общая статистика */
				nRecvDataClients += v.nRecvData;
				nAvgRecvDataClients += v.nAvgRecvData;
				nAllRecvDataClients += v.nAllRecvData;
				nSendDataClients += v.nSendData;
				nAvgSendDataClients += v.nAvgSendData;
				nAllSendDataClients += v.nAllSendData;
			}

			wprintf(L"\n%21s", L"Общие данные");
			format(nRecvDataClients);
			format(nAvgRecvDataClients);
			format(nAllRecvDataClients);
			format(nSendDataClients);
			format(nAvgSendDataClients);
			format(nAllSendDataClients);
		}
	//==========================================================================
		/**
		* отключение клиента.
		* @param pClient - клиент.
		* @param dwReconnect - количество переподключений.
		* @param ec - ошибка отключения.
		*/
		void disconnectedClientHandler(
			INetworkTestStatistic* const pClient,
			const DWORD dwReconnect,
			const std::error_code ec) noexcept override
		{
			try
			{
				std::wstring wStrRecconnect;
				if (dwReconnect > 0)
				{
					wStrRecconnect.append(L". Попыток подключения: ");
					wStrRecconnect.append(std::to_wstring(dwReconnect));
				}

				if (ec)
				{
					wprintf(L"\nКлиент %s отключился с ошибкой: %u%s",
						pClient->getAddress().getAddress().c_str(),
						ec.value(),
						wStrRecconnect.c_str());
					return;
				}

				wprintf(L"\nКлиент отключился: %s%s", 
					pClient->getAddress().getAddress().c_str(),
					wStrRecconnect.c_str());
			}
			catch (const std::exception& ex)
			{
				_pIocp->log(wname::logger::EMessageType::warning, ex);
			}
		}
	//==========================================================================
		/**
		* деструктор тестирования сети.
		*/
		~CNetworkTestCmd()
		{
			/** ждем всех */
			release();
		}
	//==========================================================================
		CNetworkTestCmd(const CNetworkTestCmd&) = delete;
		CNetworkTestCmd(CNetworkTestCmd&&) = delete;
		CNetworkTestCmd& operator=(const CNetworkTestCmd&) = delete;
		CNetworkTestCmd& operator=(CNetworkTestCmd&&) = delete;
	//==========================================================================
	#pragma endregion
};

void printHelp()
{
	wprintf(L"\nИспользование: networkTest xxx.xxx.xxx.xxx:xxxxx [-c | -s] [-n <количество>] [-l <размер>] [-q <количество>] [-in | -out | -inout] [-t <количество>]");
	wprintf(L"\nПараметры:");
	wprintf(L"\n\txxx.xxx.xxx.xxx:xxxxx\tАдрес подключения.");
	wprintf(L"\n\t-c\t\t\tЗапустить как клиента.");
	wprintf(L"\n\t-s\t\t\tЗапустить как сервер.");
	wprintf(L"\n\t-n <количество>\t\tКоличество сессий. По-умолчанию 1. Настройка только для клиента.");
	wprintf(L"\n\t-l <размер>\t\tРазмер буфера. По-умолчанию 1024. Настройка только для клиента.");
	wprintf(L"\n\t-q <количество>\t\tКоличество буфером в очереди. По-умолчанию 1. Настройка только для клиента.");
	wprintf(L"\n\t-in\t\t\tФлаг приема. Настройка только для клиента.");
	wprintf(L"\n\t-out\t\t\tФлаг отправки. Настройка только для клиента.");
	wprintf(L"\n\t-inout\t\t\tФлаг приема и отправки. По-умолчанию установлен. Настройка только для клиента.");
	wprintf(L"\n\t-t <количество>\t\tВремя роботы. По-умолчанию 0xFFFFFFFF.");
}

std::string to_string(const std::wstring& wstr)
{
	#pragma warning(disable: 4244)
	std::string str; 
	str.resize(wstr.size());
	WideCharToMultiByte(CP_ACP, 0, wstr.data(), -1, str.data(), (int)wstr.size(), NULL, NULL);
	return str;
}

int wmain(DWORD argc, PWCHAR* argv)
{
	#pragma warning(disable: 4244)
	setlocale(LC_ALL, "Russian");

	if (argc < 3)
	{
		printHelp();
		return ERROR_SUCCESS;
	}

	auto pLogger = std::make_shared<wname::logger::CLoggerToConsole>();
	auto pIocp = std::make_shared<wname::io::iocp::CIocp>(
		1, std::thread::hardware_concurrency(), pLogger);
	std::shared_ptr<CNetworkTestCmd> pNetworkTest;

	std::wstring wStrAddress = argv[1];
	std::string strAddress = to_string(wStrAddress);
	std::string strIp(strAddress, 0, strAddress.find_last_of(L':'));
	if ((strIp.size() + 1) >= strAddress.size())
	{
		wprintf(L"\n\t Аргумент: \"%s\" введен неверно.", argv[1]);
		printHelp();
		return ERROR_BAD_ARGUMENTS;
	}

	int port = atoi(&strAddress[strIp.size() + 1]);
	DWORD dwTime = INFINITE;

	if(_wcsicmp(argv[2], L"-s") == 0)
	{
		pNetworkTest = std::make_shared<CNetworkTestCmd>(
			strIp.c_str(), port, pIocp);
	}
	else if (_wcsicmp(argv[2], L"-c") == 0)
	{
		/** запускаем как клиент */
		CNetworkTestCmd::SInfoClient sInfoClient = { 0 };
		sInfoClient.dwBandWidth = 1024 * 1024 * 10;
		sInfoClient.dwSizeBuffer = 1024;
		sInfoClient.wBufferCount = 10;
		sInfoClient.bIsRecv = true;
		sInfoClient.bIsSend = true;
		DWORD dwCountSession = 1;

		for (DWORD i = 3; i < argc; i++)
		{
			if (_wcsicmp(argv[i], L"-n") == 0)
			{
				if ((i + 1) >= argc)
				{
					wprintf(L"\n\t Аргумент: \"%s\" не содержит значение.", argv[i]);
					printHelp();
					return ERROR_BAD_ARGUMENTS;
				}

				dwCountSession = _wtol(argv[i + 1]);
			}

			if (_wcsicmp(argv[i], L"-l") == 0)
			{
				if ((i + 1) >= argc)
				{
					wprintf(L"\n\t Аргумент: \"%s\" не содержит значение.", argv[i]);
					printHelp();
					return ERROR_BAD_ARGUMENTS;
				}

				sInfoClient.dwSizeBuffer = _wtol(argv[i + 1]);
			}

			if (_wcsicmp(argv[i], L"-q") == 0)
			{
				if ((i + 1) >= argc)
				{
					wprintf(L"\n\t Аргумент: \"%s\" не содержит значение.", argv[i]);
					printHelp();
					return ERROR_BAD_ARGUMENTS;
				}

				sInfoClient.wBufferCount = (WORD)_wtol(argv[i + 1]);
			}

			if (_wcsicmp(argv[i], L"-in") == 0)
			{
				sInfoClient.bIsRecv = false;
			}

			if (_wcsicmp(argv[i], L"-out") == 0)
			{
				sInfoClient.bIsSend = false;
			}

			if (_wcsicmp(argv[i], L"-inout") == 0)
			{
				sInfoClient.bIsSend = false;
				sInfoClient.bIsRecv = false;
			}
		}

		if (!sInfoClient.bIsSend || !sInfoClient.bIsRecv)
		{
			sInfoClient.bIsSend = !sInfoClient.bIsSend;
			sInfoClient.bIsRecv = !sInfoClient.bIsRecv;
		}

		pNetworkTest = std::make_shared<CNetworkTestCmd>(
			strIp.c_str(), port, dwCountSession, sInfoClient, pIocp);
	}
	else
	{
		wprintf(L"\n\t Аргумент: \"%s\" должен указывать на тип запуска.", argv[2]);
		printHelp();
		return ERROR_BAD_ARGUMENTS;
	}
	
	for (DWORD i = 3; i < argc; i++)
	{
		if (_wcsicmp(argv[i], L"-t") == 0)
		{
			if ((i + 1) >= argc)
			{
				wprintf(L"\n\t Аргумент: \"%s\" не содержит значение.", argv[i]);
				printHelp();
				return ERROR_BAD_ARGUMENTS;
			}

			dwTime = _wtol(argv[i + 1]);
		}
	}
	
	CNetworkTestCmd::SStatistic statistic;
	const auto ec = pNetworkTest->start(statistic, dwTime);
	if (ec)
	{
		wprintf(L"\nОшибка в ходе выполнения работы: %u", ec.value());		
	}

	return ec.value();
}