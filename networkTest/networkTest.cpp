#include "../dataTransfer/dataTransfer/dataTransfer.h"
#include <comdef.h> 
#include <codecvt>

std::shared_ptr<wname::io::iocp::CIocp> g_pIocp;

class CNetworkTestCmd : public datatransfer::networktest::CNetworkTest
{
	#pragma region Public_Method
	public:
	//==========================================================================
		/**
		* ����������� ������������ ���� ��� ������.
		* @param strIp - IP ����� �������.
		* @param wPort - ���� �������.
		* @param dwCountClient - ���������� ��������.
		* @param sInfoClient - ��������� ������������ ����.
		*/
		CNetworkTestCmd(
			const std::string strIp,
			const WORD wPort,
			const DWORD dwCountClient,
			const SInfoClient& sInfoClient)
			:CNetworkTest(strIp, wPort, dwCountClient, sInfoClient, g_pIocp)
		{
			
		}
	//==========================================================================
		/**
		* ����������� ������������ ���� ��� ������.
		* @param strIp - IP ����� �������.
		* @param wPort - ���� �������.
		*/
		CNetworkTestCmd(
			const std::string strIp,
			const WORD wPort)
			:CNetworkTest(strIp, wPort, g_pIocp) 
		{

		}
	//==========================================================================
		/**
		* ����������� � ����������� �������.
		* @param pClient - ������.
		*/
		void connectedClientHandler(
			INetworkTestStatistic* const pClient) noexcept override
		{
			try
			{
				wprintf(L"\n������ �����������: %s", pClient->getAddress().getAddress().c_str());
			}
			catch (const std::exception& ex)
			{
				_pIocp->log(wname::logger::EMessageType::warning, ex);
			}		
		}
	//==========================================================================
		/**
		* ����������� �� ���� ����� ����������.
		* @param statisticClients - ����������.
		*/
		void statisticHandler(
			const std::unordered_map<INetworkTestStatistic*, SStatisticClient> statisticClients) noexcept override
		{
			auto format = [] (UINT64 nValue)
			{ 
				if (nValue < (1024 * 10))
				{
					wprintf(L" %8I64u ����", nValue);
					return;
				}

				nValue /= 1024;
				if (nValue < (1024 * 10))
				{
					wprintf(L"%8I64u �����", nValue);
					return;
				}

				nValue /= 1024;
				if (nValue < (1024 * 10))
				{
					wprintf(L"%8I64u �����", nValue);
					return;
				}

				nValue /= 1024;
				if (nValue < (1024 * 10))
				{
					wprintf(L"%8I64u �����", nValue);
					return;
				}

				nValue /= 1024;
				if (nValue < (1024 * 10))
				{
					wprintf(L"%8I64u �����", nValue);
					return;
				}
			};
			wprintf(L"\n\n\t����� �������          Recv      Recv avg      Recv all          Send      Send avg      Send all");
			UINT64 nRecvDataClients = 0;
			UINT64 nAvgRecvDataClients = 0;
			UINT64 nAllRecvDataClients = 0;
			UINT64 nSendDataClients = 0;
			UINT64 nAvgSendDataClients = 0;
			UINT64 nAllSendDataClients = 0;

			for (auto& [k, v] : _statisticClients)
			{
				assert(k != nullptr);

				wprintf(L"\n%21s", k->getAddress().getAddress().c_str()); 
				format(v.nRecvData);
				format(v.nAvgRecvData);
				format(v.nAllRecvData);
				format(v.nSendData);
				format(v.nAvgSendData);
				format(v.nAllSendData);

				/** ����� ���������� */
				nRecvDataClients += v.nRecvData;
				nAvgRecvDataClients += v.nAvgRecvData;
				nAllRecvDataClients += v.nAllRecvData;
				nSendDataClients += v.nSendData;
				nAvgSendDataClients += v.nAvgSendData;
				nAllSendDataClients += v.nAllSendData;
			}

			wprintf(L"\n%21s", L"����� ������");
			format(nRecvDataClients);
			format(nAvgRecvDataClients);
			format(nAllRecvDataClients);
			format(nSendDataClients);
			format(nAvgSendDataClients);
			format(nAllSendDataClients);
		}
	//==========================================================================
		/**
		* ���������� �������.
		* @param pClient - ������.
		*/
		void disconnectedClientHandler(
			INetworkTestStatistic* const pClient) noexcept override
		{
			try
			{
				wprintf(L"\n������ ����������: %s", pClient->getAddress().getAddress().c_str());
			}
			catch (const std::exception& ex)
			{
				_pIocp->log(wname::logger::EMessageType::warning, ex);
			}
		}
	//==========================================================================
		/**
		* ���������� ������������ ����.
		*/
		~CNetworkTestCmd()
		{

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
	wprintf(L"\n�������������: networkTest xxx.xxx.xxx.xxx:xxxxx [-c | -s] [-n <����������>] [-l <������>] [-q <����������>] [-in | -out | -inout]");
	wprintf(L"\n���������:");
	wprintf(L"\n\txxx.xxx.xxx.xxx:xxxxx\t����� �����������.");
	wprintf(L"\n\t-c\t\t\t��������� ��� �������.");
	wprintf(L"\n\t-s\t\t\t��������� ��� ������.");
	wprintf(L"\n\t-n <����������>\t\t���������� ������. ��-��������� 1.");
	wprintf(L"\n\t-l <������>\t\t������ ������. ��-��������� 1024.");
	wprintf(L"\n\t-q <����������>\t\t���������� ������� � �������. ��-��������� 1.");
	wprintf(L"\n\t-in\t\t\t���� ������.");
	wprintf(L"\n\t-out\t\t\t���� ��������.");
	wprintf(L"\n\t-inout\t\t\t���� ������ � ��������. ��-��������� ����������.");
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

	g_pIocp = std::make_shared<wname::io::iocp::CIocp>();

	std::wstring wStrAddress = argv[1];
	std::string strAddress = to_string(wStrAddress);
	std::string strIp(strAddress, 0, strAddress.find_last_of(L':'));
	int port = atoi(&strAddress[strIp.size() + 1]);

	if(_wcsicmp(argv[2], L"-s") == 0)
	{
		/** ��������� ��� ������ */
		if (argc != 3)
		{
			/** ������� ����� ���������� */
			printHelp();
			return ERROR_BAD_ARGUMENTS;
		}

		CNetworkTestCmd server(strIp.c_str(), port);

		system("pause");
		return ERROR_SUCCESS;
	}

	if (_wcsicmp(argv[2], L"-c") == 0)
	{
		/** ��������� ��� ������ */
		CNetworkTestCmd::SInfoClient sInfoClient = { 0 };
		sInfoClient.dwBandWidth = 1024 * 1024 * 10;
		sInfoClient.dwSizeBuffer = 1024;
		sInfoClient.wBufferCount = 10;
		sInfoClient.bIsRecv = true;
		sInfoClient.bIsSend = true;
		DWORD dwCountSession = 1;

		for (DWORD i = 3; i < argc; i++)
		{
			if (_wcsicmp(argv[i], L"-n") == 0 && (i + 1) < argc)
			{
				dwCountSession = _wtol(argv[i + 1]);
			}

			if (_wcsicmp(argv[i], L"-l") == 0 && (i + 1) < argc)
			{
				sInfoClient.dwSizeBuffer = _wtol(argv[i + 1]);
			}

			if (_wcsicmp(argv[i], L"-q") == 0 && (i + 1) < argc)
			{
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

		CNetworkTestCmd server(strIp.c_str(), port, dwCountSession, sInfoClient);

		system("pause");
		return ERROR_SUCCESS;	
	}

	printHelp();
	return ERROR_BAD_ARGUMENTS;
}