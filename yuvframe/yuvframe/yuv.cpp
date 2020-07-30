#include<iostream>
#include<cmath>
#include<io.h>
#include<conio.h>
#include<string.h>
using namespace std;

#define MAX_FRAME 3000000
#define MAX_SUM 0xFFFFFFFF
#define DEFAULT_STARTFRAME_DIFF 3000000
#define DEFAULT_ENDFRAME_DIFF 3000000

// C:\Users\HP\Desktop\template_1.yuv C:\Users\HP\Desktop\template_10.yuv D:\Work\Decoded_yuv\decoded_300ms\01_Aerial_6m.yuv
int main(int argc,char* argv[]) {

	int Y_SIZE = 1080;
	int X_SIZE = 1920;
	/*char* start_fpath = argv[1];
	char* end_fpath = argv[2];
	char* yuv_fpath = argv[3];*/

	char* y = new char[100];
	char* x = new char[100];
	char* start_fpath = new char[300];
	char* end_fpath = new char[300];
	char* yuv_fpath = new char[300];
	cin >> y>>x>>start_fpath >> end_fpath >> yuv_fpath;
	Y_SIZE = atoi(y);
	X_SIZE = atoi(x);
	
	int YUV_SIZE = Y_SIZE * X_SIZE * 3 / 2;

	unsigned char* start_frame = new unsigned char[YUV_SIZE];
	FILE* startfile;
	fopen_s(&startfile, start_fpath, "rb");
	fread(start_frame, sizeof(unsigned char), YUV_SIZE, startfile);

	unsigned char* end_frame = new unsigned char[YUV_SIZE];
	FILE* endfile;
	fopen_s(&endfile, end_fpath, "rb");
	fread(end_frame, sizeof(unsigned char), YUV_SIZE, endfile);

	_finddata_t fd;
	long handle = _findfirst(yuv_fpath, &fd);
	int result = 0;
	char s1[260] = ".";
	char s2[260] = "..";
	do {
		char file_path[260] = "D:\\Work\\Decoded_yuv\\decoded_600ms\\";
		strcat_s(file_path, sizeof(file_path), fd.name);
		if (strcmp(fd.name,s1)==0 || strcmp(fd.name,s2)==0) {
			result = _findnext(handle, &fd);
			continue;
		}

		unsigned char* yuv_frame = new unsigned char[YUV_SIZE];
		FILE* yuvfile;
		fopen_s(&yuvfile, file_path, "rb");

		long long* startyframe_diff = new long long[MAX_FRAME];
		long long* endyframe_diff = new long long[MAX_FRAME];
		long long* startuvframe_diff = new long long[MAX_FRAME];
		long long* enduvframe_diff = new long long[MAX_FRAME];

		int NUMBER_OF_FRAME = 0;
		for (int i = 0; i < MAX_FRAME; i++) {
			bool fsize = fread(yuv_frame, sizeof(unsigned char), YUV_SIZE, yuvfile);
			if (feof(yuvfile)) {
				break;
			}
			NUMBER_OF_FRAME++;
			long long start_ysum = 0;
			long long end_ysum = 0;
			long long start_uvsum = 0;
			long long end_uvsum = 0;

			for (int j = 0; j < Y_SIZE * X_SIZE; j++) {
				start_ysum += abs(yuv_frame[j] - start_frame[j]);
				end_ysum += abs(yuv_frame[j] - end_frame[j]);
			}
			for (int j = Y_SIZE * X_SIZE; j < YUV_SIZE; j++) {
				start_uvsum += abs(yuv_frame[j] - start_frame[j]);
				end_uvsum += abs(yuv_frame[j] - end_frame[j]);

			}
			startyframe_diff[i] = start_ysum;
			endyframe_diff[i] = end_ysum;
			startuvframe_diff[i] = start_uvsum;
			enduvframe_diff[i] = end_uvsum;
		}

		int start = 0;
		int end = NUMBER_OF_FRAME;
		long long start_uvsum = 0;
		long long end_uvsum = 0;
		long long start_ysum = MAX_SUM;
		long long end_ysum = MAX_SUM;

		for (int i = 0; i < NUMBER_OF_FRAME - 1; i++) {
			long long start_uvdiff = abs(startuvframe_diff[i] - startuvframe_diff[i + 1]);
			long long end_uvdiff = abs(enduvframe_diff[i] - enduvframe_diff[i + 1]);
			if (start_uvdiff > end_uvdiff) {
				if (start_uvsum <= start_uvdiff) {
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
		printf("file name : %s,start : %d, end : %d, length : %d\n", fd.name,start, end, end - start);
		delete[] yuv_frame;
		delete[] startyframe_diff;
		delete[] endyframe_diff;
		delete[] startuvframe_diff;
		delete[] enduvframe_diff;
		result = _findnext(handle, &fd);
	} while (result != -1);
	
	
}
