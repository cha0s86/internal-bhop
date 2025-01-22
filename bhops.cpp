#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cstdint>
#include <chrono>
#include <thread>

namespace offset
{
	// Client offsets
	// Can be found from: https://github.com/a2x/cs2-dumper/blob/main/output/offsets.hpp
	constexpr std::ptrdiff_t dwLocalPlayerPawn = 0x186DDE8; // Found in offsets.hpp
	constexpr std::ptrdiff_t dwForceJump = 0x1866B20; 		// Found in buttons.hpp

	// Player offsets
	// Can be found from: https://github.com/a2x/cs2-dumper/blob/main/output/client_dll.hpp
	constexpr std::ptrdiff_t m_iHealth = 0x344; // Found in client_dll.hpp
	constexpr std::ptrdiff_t m_fFlags = 0x63; 	// Found in client_dll.hpp
}

// hack fn
void BunnyHop(const HMODULE instance) noexcept
{
	const auto client = reinterpret_cast<std::uintptr_t>(GetModuleHandleA("client.dll"));

	// hack loop
	while (!GetAsyncKeyState(VK_END))
	{
		// If space is not pressed
		if (!GetAsyncKeyState(VK_SPACE)) {
		 	continue;
		}

		/*
		const auto localPlayer = *reinterpret_cast<std::uintptr_t*>(client + offset::dwLocalPlayerPawn);
		
		// If no local player
		if (!localPlayer) {
			continue;
		}

		const auto health = *reinterpret_cast<std::int32_t*>(localPlayer + offset::m_iHealth);

		// If no health
		if (!health) {
			continue;
		}

		const auto flags = *reinterpret_cast<std::int32_t*>(localPlayer + offset::m_fFlags);
		*/

		if (GetAsyncKeyState(VK_SPACE)) {
			*reinterpret_cast<std::uintptr_t*>(client + offset::dwForceJump) = 65537;
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			*reinterpret_cast<std::uintptr_t*>(client + offset::dwForceJump) = 256;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(2));
	}

	// uninject
	FreeLibraryAndExitThread(instance, 0);
}

// entry point
int __stdcall DllMain(
	const HMODULE instance,
	const std::uintptr_t reason,
	const void* reserved
)
{
	// DLL_PROCESS_ATTACH
	if (reason == 1)
	{
		DisableThreadLibraryCalls(instance);

		// create hack thread
		const auto thread = CreateThread(
			nullptr,
			0,
			reinterpret_cast<LPTHREAD_START_ROUTINE>(BunnyHop),
			instance,
			0,
			nullptr
		);

		if (thread)
			CloseHandle(thread);
	}

	return 1;
}