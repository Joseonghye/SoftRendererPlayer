struct WindowTimer
{
public:
	static LONGLONG CheckMilliSeconds()
	{
		LARGE_INTEGER frequency;
		if (!QueryPerformanceFrequency(&frequency))
		{
			return 0;
		}
	}
};