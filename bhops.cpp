#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <thread>
#include <offsets/buttons.hpp>

// Client
const auto client = reinterpret_cast<std::uintptr_t>(GetModuleHandleA("client.dll"));

// Hack function
void BunnyHop(const HMODULE instance) {
	
	// Hack loop
	while (!GetAsyncKeyState(VK_END))
	{
		if (GetAsyncKeyState(VK_SPACE)) {
			*reinterpret_cast<std::uintptr_t*>(client + cs2_dumper::buttons::jump) = 65537;
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			*reinterpret_cast<std::uintptr_t*>(client + cs2_dumper::buttons::jump) = 256;
		}
	}
	
	// Uninject
	FreeLibraryAndExitThread(instance, 0);
}

// Entry point
int __stdcall DllMain(const HMODULE instance, const std::uintptr_t reason, const void* reserved) {
	
	// DLL_PROCESS_ATTACH
	if (reason == 1)
	{
		DisableThreadLibraryCalls(instance);

		// Create hack thread
		const auto thread = CreateThread(
			nullptr,
			0,
			reinterpret_cast<LPTHREAD_START_ROUTINE>(BunnyHop),
			instance,
			0,
			nullptr
		);

		if (thread) {
			CloseHandle(thread);
		}
	}

	return 1;
}