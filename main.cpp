#include <iostream>
#include <windows.h>
#include <intrin.h>
inline bool CheckTimingAnomaly() {
	uint64_t t1 = __rdtsc();
	volatile int a = 0;
	for (int i = 0; i < 1000; i++)
	{
		a += i;
	}
	uint64_t t2 = __rdtsc();
	if ((t2 - t1) > 0xFFFFF) {
		return true;
	}
	return false;
}
inline bool CheckHardwareBreakpoints() {
	CONTEXT ctx = { 0 };
	ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;
	HANDLE hThread = GetCurrentThread();
	if (GetThreadContext(hThread, &ctx)) {
		if (ctx.Dr0 != 0 || ctx.Dr1 != 0 || ctx.Dr2 != 0 || ctx.Dr3 != 0) {
			return true;
		}
	}
	return false;
}
inline bool CheckPebSafe() {
	unsigned char* peb = (unsigned char*)__readgsqword(0x60);
	if (!peb)return false;

	if (peb[0x02] == 1)return true;
	uint32_t* ntGlobalFlag = (uint32_t*)(peb + 0xBC);
	if (*ntGlobalFlag & 0x70) { return true; }
	return false;
}



int main() {
	if (CheckTimingAnomaly() || CheckHardwareBreakpoints() || CheckPebSafe()) {
		return 0;
	}
	std::cout << "HACKED\n";
	std::cin.get();
}