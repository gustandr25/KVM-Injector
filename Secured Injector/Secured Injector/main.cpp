#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include "sha512.hpp"
#include "skcrypt.hpp"
#include <random>

using namespace std;

auto userstr = xorstr(L"Username: ");
auto passstr = xorstr(L"Password: ");
auto welcomestr = xorstr(L"Welcome");
auto errormsg = xorstr(L"Something went wrong, try again later.");
auto success = xorstr(L"Injected successfully!");


class localdatabase
{
public:
	string hashes = "1e74f02025f7018f709fd460293fe7b5982be51774241226fd2a78da48a9aa32af5449e449ff0a3d78b4991b0b1d81ee199ca682ee6499f2df67dc29dadfb0892399d8c8c38c0ecee3ab9b7429996dbd9d588b858c993956fabbcc6f7e77b046c0e7ca3e3d1a72e5dfff6d28cfc1884fb3fb12287b8d00bfb8e73a64d91eacac"; //username + password + salt hashed and user+pass+salt hashed
	string username;
	string password;
	string combined;
	string salt = "salt"; //you should prob encrypt this


}database;



void checkdebug()
{
	if (IsDebuggerPresent())
	{
		exit(1);
	}
}

void getcreds()
{
	std::cout << R"(
 __    __  __     __  __       __       
/  |  /  |/  |   /  |/  \     /  |      
$$ | /$$/ $$ |   $$ |$$  \   /$$ |      
$$ |/$$/  $$ |   $$ |$$$  \ /$$$ |      
$$  $$<   $$  \ /$$/ $$$$  /$$$$ |      
$$$$$  \   $$  /$$/  $$ $$ $$/$$ |      
$$ |$$  \   $$ $$/   $$ |$$$/ $$ |      
$$ | $$  |   $$$/    $$ | $/  $$ |      
$$/   $$/     $/     $$/      $$/
)";

	cout << "\n";

	wprintf(userstr);
	cin >> database.username;
	wprintf(passstr);
	cin >> database.password;

	database.combined = sha512(database.username + database.password + database.salt);

}

void inject()
{

	DWORD pID = NULL;
	char path[MAX_PATH]; 
	const char* dll_name = "cheat.dll"; 
	const char* hwndtitle = "game.exe";
	GetFullPathName(dll_name, MAX_PATH, path, nullptr); 
	GetWindowThreadProcessId(FindWindow(NULL, hwndtitle), &pID); 
	HANDLE hPRO = OpenProcess(PROCESS_ALL_ACCESS, NULL, pID); 
	void* allocated_memory = VirtualAllocEx(hPRO, nullptr, MAX_PATH, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	WriteProcessMemory(hPRO, allocated_memory, path, MAX_PATH, nullptr); 
	HANDLE h_thread = CreateRemoteThread(hPRO, nullptr, NULL, LPTHREAD_START_ROUTINE(LoadLibraryA), allocated_memory, NULL, nullptr); 
	CloseHandle(hPRO); 
	VirtualFreeEx(hPRO, allocated_memory, NULL, MEM_RELEASE); 

}


void authenticateuser()
{
	if (database.hashes.find(database.combined) != std::string::npos) {
		inject();
	}
	else
	{
		wprintf(errormsg);
		Sleep(2000);
		exit(1);
	}
}

void setrandomtitle()
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<> distr(0, 51);
	std::string name = "";
	char alphabet[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };
	for (int i = 0; i < 15; ++i)
	{
		name = name + alphabet[distr(mt)];
		SetConsoleTitleA(name.c_str());
	} //thanks @ummbop

	HWND hwnd = GetConsoleWindow();
	MoveWindow(hwnd, 0, 0, 500, 500, TRUE); //set window nice :)
}

void finish()
{
	wprintf(success);
	userstr.clear();
	passstr.clear();
	welcomestr.clear();
	errormsg.clear();
	success.clear();
	Sleep(5000);
	exit(1);
}


int main()
{
	system("Color 0B");
	setrandomtitle();
	checkdebug();
	getcreds();
	authenticateuser();
	finish();
}
