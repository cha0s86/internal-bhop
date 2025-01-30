#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cstdint>
#include <chrono>
#include <thread>

namespace offsets {
	constexpr std::ptrdiff_t dwLocalPlayerPawn = 0x187CED8;
	constexpr std::ptrdiff_t jump = 0x1875C20;

	constexpr std::ptrdiff_t m_iHealth = 0x344;
	constexpr std::ptrdiff_t m_fFlags = 0x63;
}



// Hack function
void BunnyHop(const HMODULE instance) noexcept
{
	const auto client = reinterpret_cast<std::uintptr_t>(GetModuleHandleA("client.dll"));

	while (!GetAsyncKeyState(VK_END))
	{
		std::this_thread::sleep_for(std::chrono::microseconds(1000));

		// make sure space is pressed
		if (!GetAsyncKeyState(VK_SPACE)) {
			continue;
		}

		// get local player
		const auto localPlayer = *reinterpret_cast<std::uintptr_t*>(client + offsets::dwLocalPlayerPawn);

		// is valid
		if (!localPlayer) {
			continue;
		}

		const auto health = *reinterpret_cast<std::int32_t*>(localPlayer + offsets::m_iHealth);

		// is alive
		if (!health) {
			continue;
		}

		const auto flags = *reinterpret_cast<std::int32_t*>(localPlayer + offsets::m_fFlags);

		// on ground
		if (GetAsyncKeyState(VK_SPACE)) {
			mem.write<int32_t>(client + jump, 65537); // Write to dwForceJump
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
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