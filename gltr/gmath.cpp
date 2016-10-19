#define DLL
#include "gltr.h"
#include <math.h>
#include<string.h>
#define PI 3.1415926
void minus(float *a,float *b,float *outt){
	outt[0] = a[0] - b[0];
	outt[1] = a[1] - b[1];
	outt[2] = a[2] - b[2];
	return;
}
void normalize(float *v,int len){
    float sum = 0;
    for(int i=0;i<len; i++){
        sum += v[i] * v[i];
    }
    float vm = sqrt(sum);
    for(int j=0;j<len;j++){
        v[j] = v[j] / vm;
    }
}

// cross
void cross(float *a,float *b,float* outMatrix){
    outMatrix[0] = a[1] * b[2] - a[2] * b[1];
    outMatrix[1] = a[2] * b[0] - a[0] * b[2];
    outMatrix[2] = a[0] * b[1] - a[1] * b[0];
    return;
}

float dot(float *a,float *b){
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

void creatrotmatrix(int axis, float angle, float* outMatrix)
{
	float t = PI*angle/180;

	float sinC = sin(t);
    float cosC = cos(t);
    if(axis == 0){
		float tmp[] = {
	    1,0,0,0,
	    0,cosC,sinC,0,
	    0,-sinC,cosC,0,
		0,0,0,1};
		memcpy(outMatrix,tmp,16*sizeof(float));
    }
    else  if(axis == 1) {
		float tmp[] = {
            cosC, 0.0, -sinC, 0.0,
            0.0, 1, .0, .0,
            sinC, 0.0, cosC, 0.0,
			0.0, 0.0, 0.0, 1.0};
		memcpy(outMatrix,tmp,16*sizeof(float));
    }
    else{
		float tmp[] = {
            cosC,sinC,.0,.0,
            -sinC,cosC,.0,.0,
            .0,.0,1.0,.0,
			.0,.0,.0,1.0};
		memcpy(outMatrix,tmp,16*sizeof(float));
    }
    return;
}

void multiplyMatrix(float *matrixA, float *matrixB, float *outMatrix) {
    for (int i = 0; i < 4; i++) {
        outMatrix[i] =
            matrixA[i] * matrixB[0] + matrixA[i + 4] * matrixB[1] + matrixA[i + 8] * matrixB[2] + matrixA[i + 12] * matrixB[3];
        outMatrix[i + 4] =
            matrixA[i] * matrixB[4] + matrixA[i + 4] * matrixB[5] + matrixA[i + 8] * matrixB[6] + matrixA[i + 12] * matrixB[7];
        outMatrix[i + 8] =
            matrixA[i] * matrixB[8] + matrixA[i + 4] * matrixB[9] + matrixA[i + 8] * matrixB[10] + matrixA[i + 12] * matrixB[11];
        outMatrix[i + 12] =
            matrixA[i] * matrixB[12] + matrixA[i + 4] * matrixB[13] + matrixA[i + 8] * matrixB[14] + matrixA[i + 12] * matrixB[15];
    }
    return;
}
// 视图矩阵
void getVMatrix2(float eyeX, float eyeY,float eyeZ,float atX,float atY,float atZ,float *outMatrix)
{
	float eye[] = {eyeX,eyeY,eyeZ};
	float lookat[] = {atX,atY,atZ};
	float up[] = {0.0, 1.0, 0.0};
	float x[3],y[3],z[3];
    minus(eye, lookat, z);
    normalize(z, 3);
    normalize(up, 3);
    cross(up,z, x);
    normalize(x,3);
    cross(z,x,y);
	float tmp[] = {
        x[0],y[0],z[0],0,
        x[1],y[1],z[1],0,
        x[2],y[2],z[2],0,
        -dot(x,eye),-dot(y,eye),-dot(z,eye),1
	};
	memcpy(outMatrix,tmp,16*sizeof(float));
    return;
}

// minus


//  v 类型化数组

//toushi
void getTS(float fov, float aspect, float neart, float fart, float *outMatrix)
{
	fov = fov * PI/180; // 弧度
	float tmp[] = {
        1 / (aspect * tan(fov / 2)), 0, 0, 0,
        0, 1 / tan(fov/2), 0, 0,
		0, 0, -(fart+neart) / (fart - neart), -1,
		0, 0, -2*fart*neart/(fart - neart), 0};
	memcpy(outMatrix, tmp, 16*sizeof(float));
}