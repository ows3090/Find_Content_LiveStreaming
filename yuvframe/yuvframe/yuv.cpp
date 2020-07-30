#include<iostream>
#include<cmath>
#include<io.h>
#include<conio.h>
#include<string.h>
using namespace std;

#define MAX_FRAME 3000000
#define MAX_SUM 0xFFFFFFFF
#define DEFAULT_STARTFRAME_DIFF 3000000				// ������������ �����ϱ� ���� �ּ� ��
#define DEFAULT_ENDFRAME_DIFF 3000000				// ���������� �����ϱ� ���� �ּ� ��


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
	// ��� �����ȿ� �ִ� ������ ��� ���� *.* �Է�
	cin >> ysize>>xsize>>start_fpath >> end_fpath >> folder_path;
	Y_SIZE = atoi(ysize);
	X_SIZE = atoi(xsize);
	YUV_SIZE = Y_SIZE * X_SIZE * 3 / 2;


	// ���� ������ 
	unsigned char* start_frame = new unsigned char[YUV_SIZE];
	FILE* startfile;
	fopen_s(&startfile, start_fpath, "rb");
	fread(start_frame, sizeof(unsigned char), YUV_SIZE, startfile);

	// ������ ������
	unsigned char* end_frame = new unsigned char[YUV_SIZE];
	FILE* endfile;
	fopen_s(&endfile, end_fpath, "rb");
	fread(end_frame, sizeof(unsigned char), YUV_SIZE, endfile);
	

	_finddata_t filedata;		// ���� ���� ����ü
	long handle = _findfirst(folder_path, &filedata);		// filedata�� ù ��° ���� ���� ����
	int result = 0;		// ���� ������ �� �޾Ҵ��� Ȯ���ϴ� ����


	
	// �������� ������ ��� Ž���ÿ� path\. �� path\..�� �߰������� �־ ����
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

		// YUV ���̺� ��Ʈ����
		unsigned char* yuv_frame = new unsigned char[YUV_SIZE];
		FILE* yuvfile;
		fopen_s(&yuvfile, file_path, "rb");


		long long* startyframe_diff = new long long[MAX_FRAME];				// ������������ Y���� ������ �� �������� Y���� ��
		long long* endyframe_diff = new long long[MAX_FRAME];				// ���������� Y���� ������ �� �������� Y���� ��
		long long* startuvframe_diff = new long long[MAX_FRAME];			// ������������ UV���� ������ �� �������� UV���� ��
		long long* enduvframe_diff = new long long[MAX_FRAME];				// ���������� UV���� ������ �� �������� UV���� ��

		int NUMBER_OF_FRAME = 0;			// ������ �� �ڵ� üũ
		for (int i = 0; i < MAX_FRAME; i++) {
			// �� ������ �б�
			bool fsize = fread(yuv_frame, sizeof(unsigned char), YUV_SIZE, yuvfile);		
			if (feof(yuvfile)) {
				break;
			}
			NUMBER_OF_FRAME++;

			
			long long start_ysum = 0;				// �� �����ӿ��� ���� ������ ��� �ȼ� Y���̸� ���� ��
			long long end_ysum = 0;					// �� �����ӿ��� �� ������ ��� �ȼ� Y���̸� ���� ��
			long long start_uvsum = 0;				// �� �����ӿ��� ���� ������ ��� �ȼ� UV���̸� ���� ��
			long long end_uvsum = 0;				// �� �����ӿ��� �� ������ ��� �ȼ� UV���̸� ���� ��

			// Y�� ���� �޸𸮿��� Y_SIZE*X_SIZE��ŭ ���� => YUV420�� �ش�
			for (int j = 0; j < Y_SIZE * X_SIZE; j++) {
				start_ysum += abs(yuv_frame[j] - start_frame[j]);
				end_ysum += abs(yuv_frame[j] - end_frame[j]);
			}

			// UV�� ���� Y�� �޸� �� �� ��ġ ���� (Y_SIZE*X_SIZE)/2��ŭ ���� => YUV420�� �ش�
			for (int j = Y_SIZE * X_SIZE; j < YUV_SIZE; j++) {
				start_uvsum += abs(yuv_frame[j] - start_frame[j]);
				end_uvsum += abs(yuv_frame[j] - end_frame[j]);

			}
			startyframe_diff[i] = start_ysum;
			endyframe_diff[i] = end_ysum;
			startuvframe_diff[i] = start_uvsum;
			enduvframe_diff[i] = end_uvsum;
		}


		int start = 0;						// ���� ������ ��ġ
		int end = NUMBER_OF_FRAME;			// �� ������ ��ġ
		long long start_uvsum = 0;			// startuvframe_diff �迭���� ���� ���� �� ���� �ּ� ���� ã�� ���� ����
		long long end_uvsum = 0;			// enduvframe_diff �迭���� ���� ���� �� ���� �ּ� ���� ã�� ���� ����	
		long long start_ysum = MAX_SUM;		// startyframe_diff �迭���� ���� �������� �������� ���� ���� ���� ���� ã�� ���� ����
		long long end_ysum = MAX_SUM;		// endyframe_diff �迭���� ���� �������� �������� ���� ���� ���� ���� ã�� ���� ����


		for (int i = 0; i < NUMBER_OF_FRAME - 1; i++) {
			// ���� UV�� ���ӵ� ���� ��
			long long start_uvdiff = abs(startuvframe_diff[i] - startuvframe_diff[i + 1]);

			// �� UV�� ���ӵ� ���� ��
			long long end_uvdiff = abs(enduvframe_diff[i] - enduvframe_diff[i + 1]);

			// ���ӵ� ���� ���� �� ū �������� Ȯ�� => template_1�� template_10�� �̹��� ���̰� ũ�� �ʱ� ������ �����߻� ���ɼ� ����
			if (start_uvdiff > end_uvdiff) {
				if (start_uvsum <= start_uvdiff) {
					
					// ���ӵ� ���� Ȯ Ŀ����, ���� �������� �������������� ���̺��� ���� ��
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
