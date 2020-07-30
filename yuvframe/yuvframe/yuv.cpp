#include<iostream>
#include<cmath>
#include<io.h>
#include<conio.h>
#include<string.h>
using namespace std;

#define MAX_FRAME 3000000
#define MAX_SUM 0xFFFFFFFF
#define DEFAULT_STARTFRAME_DIFF 3000000				// 시작컨텐츠가 존재하기 위한 최소 차
#define DEFAULT_ENDFRAME_DIFF 3000000				// 끝컨텐츠가 존재하기 위한 최소 차


int main(int argc,char* argv[]) {

	int Y_SIZE;
	int X_SIZE;
	int YUV_SIZE;
	char* ysize = new char[100];
	char* xsize = new char[100];
	char* start_fpath = new char[300];
	char* end_fpath = new char[300];
	char* folder_path = new char[300];

	// ex) 1920 1080 D:\Work\template_1.yuv D:\Work\template_10.yuv D:\Work\Decoded_yuv\decoded_300ms\*.*
	// 모든 폴더안에 있는 파일을 얻기 위해 *.* 입력
	cin >> ysize>>xsize>>start_fpath >> end_fpath >> folder_path;
	Y_SIZE = atoi(ysize);
	X_SIZE = atoi(xsize);
	YUV_SIZE = Y_SIZE * X_SIZE * 3 / 2;


	// 시작 컨텐츠 
	unsigned char* start_frame = new unsigned char[YUV_SIZE];
	FILE* startfile;
	fopen_s(&startfile, start_fpath, "rb");
	fread(start_frame, sizeof(unsigned char), YUV_SIZE, startfile);

	// 마지막 컨텐츠
	unsigned char* end_frame = new unsigned char[YUV_SIZE];
	FILE* endfile;
	fopen_s(&endfile, end_fpath, "rb");
	fread(end_frame, sizeof(unsigned char), YUV_SIZE, endfile);
	

	_finddata_t filedata;		// 파일 정보 구조체
	long handle = _findfirst(folder_path, &filedata);		// filedata에 첫 번째 파일 정보 대입
	int result = 0;		// 파일 정보를 잘 받았는지 확인하는 변수


	
	// 폴더내의 파일을 모두 탐색시에 path\. 과 path\..이 추가적으로 있어서 제외
	char s1[300] = ".";
	char s2[300] = "..";
	do {
		char file_path[300];
		strncpy_s(file_path, sizeof(file_path), folder_path,strlen(folder_path)-3);

		if (strcmp(filedata.name,s1)==0 || strcmp(filedata.name,s2)==0) {
			result = _findnext(handle, &filedata);
			continue;
		}
		strcat_s(file_path, 300, filedata.name);

		// YUV 라이브 스트리밍
		unsigned char* yuv_frame = new unsigned char[YUV_SIZE];
		FILE* yuvfile;
		fopen_s(&yuvfile, file_path, "rb");


		long long* startyframe_diff = new long long[MAX_FRAME];				// 시작컨텐츠의 Y값과 동영상 각 프레임의 Y값의 차
		long long* endyframe_diff = new long long[MAX_FRAME];				// 끝컨텐츠의 Y값과 동영상 각 프레임의 Y값의 차
		long long* startuvframe_diff = new long long[MAX_FRAME];			// 시작컨텐츠의 UV값과 동영상 각 프레임의 UV값의 차
		long long* enduvframe_diff = new long long[MAX_FRAME];				// 끝컨텐츠의 UV값과 동영상 각 프레임의 UV값의 차

		int NUMBER_OF_FRAME = 0;			// 프레임 수 자동 체크
		for (int i = 0; i < MAX_FRAME; i++) {
			// 한 프레임 읽기
			bool fsize = fread(yuv_frame, sizeof(unsigned char), YUV_SIZE, yuvfile);		
			if (feof(yuvfile)) {
				break;
			}
			NUMBER_OF_FRAME++;

			
			long long start_ysum = 0;				// 한 프레임에서 시작 컨텐츠 모든 픽셀 Y차이를 합한 값
			long long end_ysum = 0;					// 한 프레임에서 끝 컨텐츠 모든 픽셀 Y차이를 합한 값
			long long start_uvsum = 0;				// 한 프레임에서 시작 컨텐츠 모든 픽셀 UV차이를 합한 값
			long long end_uvsum = 0;				// 한 프레임에서 끝 컨텐츠 모든 픽셀 UV차이를 합한 값

			// Y의 값은 메모리에서 Y_SIZE*X_SIZE만큼 존재 => YUV420에 해당
			for (int j = 0; j < Y_SIZE * X_SIZE; j++) {
				start_ysum += abs(yuv_frame[j] - start_frame[j]);
				end_ysum += abs(yuv_frame[j] - end_frame[j]);
			}

			// UV의 값은 Y의 메모리 값 끝 위치 부터 (Y_SIZE*X_SIZE)/2만큼 존재 => YUV420에 해당
			for (int j = Y_SIZE * X_SIZE; j < YUV_SIZE; j++) {
				start_uvsum += abs(yuv_frame[j] - start_frame[j]);
				end_uvsum += abs(yuv_frame[j] - end_frame[j]);

			}
			startyframe_diff[i] = start_ysum;
			endyframe_diff[i] = end_ysum;
			startuvframe_diff[i] = start_uvsum;
			enduvframe_diff[i] = end_uvsum;
		}


		int start = 0;						// 시작 컨텐츠 위치
		int end = NUMBER_OF_FRAME;			// 끝 컨텐츠 위치
		long long start_uvsum = 0;			// startuvframe_diff 배열에서 전후 차이 중 가장 최소 값을 찾기 위한 변수
		long long end_uvsum = 0;			// enduvframe_diff 배열에서 전후 차이 중 가장 최소 값을 찾기 위한 변수	
		long long start_ysum = MAX_SUM;		// startyframe_diff 배열에서 시작 컨텐츠와 프레임의 차가 가장 작은 값을 찾기 위한 변수
		long long end_ysum = MAX_SUM;		// endyframe_diff 배열에서 시작 컨텐츠와 프레임의 차가 가장 작은 값을 찾기 위한 변수


		for (int i = 0; i < NUMBER_OF_FRAME - 1; i++) {
			// 시작 UV의 연속된 값의 차
			long long start_uvdiff = abs(startuvframe_diff[i] - startuvframe_diff[i + 1]);

			// 끝 UV의 연속된 값의 차
			long long end_uvdiff = abs(enduvframe_diff[i] - enduvframe_diff[i + 1]);

			// 연속된 값의 차가 더 큰 컨텐츠만 확인 => template_1과 template_10의 이미지 차이가 크지 않기 때문에 오류발생 가능성 존재
			if (start_uvdiff > end_uvdiff) {
				if (start_uvsum <= start_uvdiff) {
					
					// 연속된 값이 확 커지고, 현재 프레임이 시작컨텐츠와의 차이보다 작을 때
					if (start_ysum >= startyframe_diff[i] && startyframe_diff[i] <= DEFAULT_STARTFRAME_DIFF) {
						start_uvsum = start_uvdiff;
						start_ysum = startyframe_diff[i];
						start = i + 1;
						if (start >= end) {
							end_ysum = MAX_SUM;
						}
					}
				}
			}
			else {
				if (end_uvsum <= end_uvdiff) {
					if (end_ysum >= endyframe_diff[i + 1] && endyframe_diff[i + 1] <= DEFAULT_ENDFRAME_DIFF) {
						end_uvsum = end_uvdiff;
						end_ysum = endyframe_diff[i + 1];
						end = i + 1;
					}
				}
			}
		}
		printf("file name : %s,start : %d, end : %d, length : %d\n", filedata.name,start, end, end - start);
		delete[] yuv_frame;
		delete[] startyframe_diff;
		delete[] endyframe_diff;
		delete[] startuvframe_diff;
		delete[] enduvframe_diff;
		result = _findnext(handle, &filedata);
	} while (result != -1);
	
	
}
