#ifdef DLL
#define DLLAPI __declspec(dllexport)
#else
#define DLLAPI __declspec(dllimport)
#endif

#include "glew.h"
//#include <gl\GL.h>
//#include <gl\GLU.h>

void DLLAPI minus(float *a,float *b,float *outt);
void DLLAPI normalize(float *v,int len);

// cross
void cross(float *a,float *b,float* outMatrix);
float dot(float *a,float *b);

void DLLAPI creatrotmatrix(int axis, float angle, float* outMatrix);

void DLLAPI multiplyMatrix(float *matrixA, float *matrixB, float *outMatrix) ;
// 视图矩阵
void DLLAPI getVMatrix2(float eyeX, float eyeY,float eyeZ,float atX,float atY,float atZ,float *outMatrix);
// minus
//  v 类型化数组
//toushi
void DLLAPI getTS(float fov, float aspect, float neart, float fart, float *outMatrix);
GLuint DLLAPI LoadShaders(const char * vertex_file_path,const char * fragment_file_path);