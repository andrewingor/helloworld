#ifndef CPUSAGE_HPP
#define CPUSAGE_HPP

#define MB 1048576

static float CalculateCPULoad();
static unsigned long long FileTimeToInt64();
float GetCPULoad();

class cpusage
{
    MEMORYSTATUSEX memStat;
    SYSTEMTIME sysTime;

public: 
    cpusage() {
        memStat.dwLength = sizeof memStat;
        GlobalMemoryStatusEx(&memStat);
        GetSystemTime(&sysTime);
    }
    ~cpusage() {}

float GetCPULoad() {
    FILETIME idleTime, kernelTime, userTime;
    return GetSystemTimes(&idleTime, &kernelTime, &userTime) ? CalculateCPULoad(FileTimeToInt64(idleTime), FileTimeToInt64(kernelTime) + FileTimeToInt64(userTime)) : -1.0f;
}

int Idle () {
    return 100 - (int) ( GetCPULoad() * 100);
}

private: 
static float CalculateCPULoad(unsigned long long idleTicks, unsigned long long totalTicks)
{
    static unsigned long long _previousTotalTicks = 0;
    static unsigned long long _previousIdleTicks = 0;

    unsigned long long totalTicksSinceLastTime = totalTicks - _previousTotalTicks;
    unsigned long long idleTicksSinceLastTime = idleTicks - _previousIdleTicks;


    float ret = 1.0f - ((totalTicksSinceLastTime > 0) ? ((float)idleTicksSinceLastTime) / totalTicksSinceLastTime : 0);

    _previousTotalTicks = totalTicks;
    _previousIdleTicks = idleTicks;
    return ret;
}

static unsigned long long FileTimeToInt64(const FILETIME & ft)
{
    return (((unsigned long long)(ft.dwHighDateTime)) << 32) | ((unsigned long long)ft.dwLowDateTime);
}

};//class cpusage;

#endif
