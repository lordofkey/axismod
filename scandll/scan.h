#ifdef DLL
#define DLLAPI __declspec(dllexport)
#else
#define DLLAPI __declspec(dllimport)
#endif

double DLLAPI framdet(double dx, double dy);