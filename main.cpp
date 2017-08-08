/********************************************************************************************/
/************************************** Harris�ǵ��� **************************************/
/********************************************************************************************/
/* ���Ի�����VS2015 opencv310_vc14_x64_debug*/
#include<opencv2\opencv.hpp>
#include<iostream>
#include<sstream>
#include<string>
#include "warpping.h"
#include "matOperations.h"

//#include <cv.h>
//#include <highgui.h>
//#include <stdio.h>
//#pragma comment(lib,"opencv_ml2411.lib")

int main()
{
	/********************************************************************************************/
	/************************************** Harris�ǵ��� **************************************/
	/********************************************************************************************/
	cv::Mat iniImg;
	//std::string imgBathPath = "C:\\Users\\Happy\\Documents\\Visual Studio 2015\\Projects\\HappyKLT_Tracker20160114\\SeqImages\\01";
	std::string imgBathPath = "G:\\Visual Studio 2015\\HappyKLT_Tracker20160114\\SeqImages\\01";
	//std::string imgBathPath = "C:\\Users\\Happy\\Documents\\Visual Studio 2015\\Projects\\HappyKLT_Tracker20160114\\SeqImages2\\01";
	std::string imgSuffix = ".jpeg";
	//std::string imgSuffix = ".jpg";
	iniImg = cv::imread(imgBathPath + "33" + imgSuffix, 1);
	cv::Mat copy1IniImage;
	iniImg.copyTo(copy1IniImage);
	std::cout << "The size of the img is: " << iniImg.size() << std::endl << std::endl;

	cv::namedWindow("iniImg", 1);
	cv::imshow("iniImg", iniImg);

	//cv::cvtColor(iniImg, iniImg, CV_32F);//ע��ͼ��ı��룬���õ���CV_8U��CV_32F�������Ҫת�Ļ�Ҫʹ�������䣩����ס�������ֲᣬ���庯��֧�ֵı����ʽ
	if (iniImg.channels() == 3)
	{
		cv::cvtColor(iniImg, iniImg, CV_BGR2GRAY);
	}

	//�γ�Sobel��
	/*double dx[9] = {-1,0,1,-2,0,2,-1,0,1};
	cv::Mat kernelx(3,3,CV_32F,dx);*/				//���ָ�ֵҲ�У���Ҫע���������ͣ������8U,�Ǿ��������ˣ�������֮�֣���ӦΪ32F
	//cv::Mat kernelx = (cv::Mat_<double>(3, 3) << -1 / 6, 0, 1 / 6, -1 / 6, 0, 1 / 6, -1 / 6, 0, 1 / 6);
	cv::Mat kernelx = (cv::Mat_<double>(3, 3) << -1, 0, 1, -2, 0, 2, -1, 0, 1); //����double��float���⣬��������32F����ͼ���������
	cv::Mat kernely;
	cv::transpose(kernelx, kernely);

	//std::cout << "The Kernel x is: " << std::endl << ' ' << kernelx << std::endl << std::endl;
	//std::cout << "The Kernel y is: " << std::endl << ' ' << kernely << std::endl << std::endl;

	//����һ��ͼ��
	cv::Mat Ix, Iy;
	cv::filter2D(iniImg, Ix, CV_32F, kernelx, cv::Point(-1, -1));	//ע������ǧ��ҪΪ8U�ģ���Ȼ��û��������
	cv::filter2D(iniImg, Iy, CV_32F, kernely, cv::Point(-1, -1));
	std::cout << "The subsection of the iniImg are: " << std::endl << ' ' << iniImg(cv::Range(0, 20), cv::Range(0, 20)) << std::endl << std::endl;
	std::cout << "The subsection of the Ix are: " << std::endl << ' ' << Ix(cv::Range(0, 20), cv::Range(0, 20)) << std::endl << std::endl;
	//cv::Sobel(iniImg, Ix, CV_32F, 1, 0, 3);			//Sobel��ר�ŵ���������ͼ���һ�ס����ס����׵�΢��ͼ��ע����룩�������һ��������ӦcornerHariss
	//cv::Sobel(iniImg, Iy, CV_32F, 0, 1, 3);			//�е�ksize��ȡ3��5��7����������ʵ���ʹ��Sobel����ʱ����Ҫkernelx��kernely��ֻ���Sobel�Ĳ�������
	cv::namedWindow("Ix", 1);
	cv::imshow("Ix", Ix);
	cv::namedWindow("Iy", 1);
	cv::imshow("Iy", Iy);
	cv::waitKey(5000);
	

	//�������ͼ��
	cv::Mat Ix2, Iy2, Ixy;
	cv::multiply(Ix, Ix, Ix2);
	cv::multiply(Iy, Iy, Iy2);
	cv::multiply(Ix, Iy, Ixy);
	cv::namedWindow("Ix2", 1);
	cv::imshow("Ix2", Ix2);
	cv::namedWindow("Iy2", 1);
	cv::imshow("Iy2", Iy2);
	cv::waitKey(5000);

	//��˹��Ȩ����˹ģ������˹�˲���
	int ksizeBlur = 11;								//��˹�˲��˵Ĵ�С�ƺ��Լ������̫����
	double sigmax = 1;
	double sigmay = 1;
	cv::Mat A, B, C;
	cv::GaussianBlur(Ix2, A, cv::Size(ksizeBlur, ksizeBlur), sigmax, sigmay);
	cv::GaussianBlur(Iy2, B, cv::Size(ksizeBlur, ksizeBlur), sigmax, sigmay);
	cv::GaussianBlur(Ixy, C, cv::Size(ksizeBlur, ksizeBlur), sigmax, sigmay);
	cv::namedWindow("A", 1);
	cv::imshow("A", A);
	cv::namedWindow("B", 1);
	cv::imshow("B", B);
	cv::waitKey(5000);

	//������Ӧ��������R
	cv::Mat R, AB, C2, detM, traceM;
	cv::multiply(A, B, AB);
	cv::multiply(C, C, C2);
	cv::subtract(AB, C2, detM);
	double alpha = 0.04;
	cv::add(A, C, traceM);
	cv::subtract(detM, alpha*traceM.mul(traceM), R);
	cv::normalize(R, R, 1, 0, cv::NORM_MINMAX, CV_32F, cv::Mat()); //��һ����ע����룩����ȻRԪ��ֵ̫�󣬲����趨��Ӧֵ��ֵthreR���й��ˣ��Ǳ�Ҫ���������Ҫ��
	/*����ѡ��NORM_MINMAX����һ��ԭ���ǣ�R(x,y)-min(R)��/��max(R)-min(R)��*/
	//std::cout << "The subsection of the R are: " << std::endl << ' ' << R(cv::Range(180, 200), cv::Range(250, 270)) << std::endl << std::endl;

	//��ȡ�ǵ����꣺�����ͣ��ٶ�ֵ����Ȼ����λ��Ƚ����㣬�ٽ��зǼ���ֵ���ƣ����ѭ����ȡ����
	cv::Mat dilated;		//���ͣ�����������Ԫ�ض�ȥ�����ڵļ���ֵ��Ҳ��ֻ�м���ֵֵ���ֲ��䣬��������仯
	cv::Mat kernelDilate;
	//int ksizeDilate = 21;
	int ksizeDilate = 51;	//������Ĵ�С����Ҫ���ܴ�̶��Ͼ���������⵽�Ľǵ��������Խ�󣬼�⵽�Ľǵ���Խ�٣���ӦcornerHarris������blocksize������
	kernelDilate = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(ksizeDilate, ksizeDilate));	//��ȡ����������������ô��ȡ��
	cv::dilate(R, dilated, kernelDilate, cv::Point(-1, -1));		//we can input cv::Mat() if we use the default value of the third parameter
	cv::Mat localMax;
	cv::compare(R, dilated, localMax, CV_CMP_EQ);	//ͨ��������ǰͼ�����Ƚϵõ��������ڵļ���ֵ�㣨���ͺ�ֻ�и�����ļ���ֵ��ֵ���䣬������ֵ��ȣ���ȡ1������ȡ0��
	//std::cout << "The subsection of the localMax are: " << std::endl << ' ' << localMax(cv::Range(180, 200), cv::Range(250, 270)) << std::endl << std::endl;

	//�Ǽ���ֵ����
	double threR = 0.01; //therRΪ��Ӧֵ����ֵ��ֻ�д��������ֵ�ļ���ֵ����������Ľǵ㣬�������Ա�֤�ǵ�Ʒ�ʣ���ֵԽ�󣬽ǵ�Խ�٣����ǰ��û�н���R�����һ������ܲ����У��������Ҫ���й�һ��
	cv::Mat cornerTh, cornerMap;
	cv::threshold(R, cornerTh, threR, 255, cv::THRESH_BINARY);		//��ֵ����������ֵȡ255��С��ȡ0
	cornerTh.convertTo(cornerMap, CV_8U);							//ע�����ת�䣬����λ������ֻ֧��8U��ͼ��
	cv::bitwise_and(cornerMap, localMax, cornerMap);	//λ�����㣬ֵ��ͬ��ȡ1����ͬΪ0��ֻ��ͬʱ�������򼫴�ֵ�ʹ���R��ֵ�ĵ��������Ҫ��Ľǵ㣬˫��Ҫ�󣬿�������ȥ�����е�һ�أ�ֻ�Ǽ�С�˽ǵ�������ȣ�
	//std::cout << "The subsection of the cornerMap are: " << std::endl << ' ' << cornerMap(cv::Range(180, 200), cv::Range(250, 270)) << std::endl << std::endl;

	//ѭ����ȡ�ǵ����꣬����corners�ǵ�������ע�����
	std::vector<cv::Point2f> corners;
	std::vector<cv::Point2f> trackedCorners;
	for (int it_row = 0;it_row < cornerMap.rows;it_row++)		//.rows not row
	{
		uchar* plocalMax = cornerMap.ptr<uchar>(it_row);
		for (int it_col = 0;it_col < cornerMap.cols;it_col++)	//.cols not col
		{
			if (plocalMax[it_col])
			{
				corners.push_back(cv::Point2f((float)it_col, (float)it_row));
				cv::circle(copy1IniImage, cv::Point2f((float)it_col, (float)it_row), 2, cv::Scalar(0, 255, 0), -1, 8, 0);	//��ǽǵ�

				if ((it_col == 190 && it_row == 149) || (it_col == 179 && it_row == 188))
				//if ((it_col == 409 && it_row == 181) || (it_col == 331 && it_row == 256))
				{
					//�����˹������������ϵ��������ٵ�
					trackedCorners.push_back(cv::Point2f((float)it_col, (float)it_row));
					cv::circle(copy1IniImage, cv::Point2f((float)it_col, (float)it_row), 6, cv::Scalar(0, 0, 255), 2, 8, 0);
				}
			}
		}
	}

	std::cout << "The total numer of the detected Harris corner is: " << corners.size() << std::endl;
	std::cout << "The detected Harris corner are: " << std::endl << ' ' << corners << std::endl << std::endl;
	cv::namedWindow("Harris Result", 1);
	cv::imshow("Harris Result", copy1IniImage);
	cv::waitKey(2000);

	/********************************************************************************************/
	/*************************************** KLT Tracker ****************************************/
	/********************************************************************************************/
	cv::Mat preImg, nextImg, copy1NextImg;			//ǰһ��ͼ����һ֡ͼ���Լ���֡ͼ��Ŀ���
	cv::Mat nextIx, nextIy;							//����ͼ���x�����ݶ�ͼ��y�����ݶȷ���
	std::vector<cv::Point2f> preCorner, nextCorner;	//ǰһ֡ͼ���еĸ��ٵ�λ�ã���һ֡ͼ���еĸ��ٵ�λ�ã���ά��

	iniImg.copyTo(preImg);							//��ʼ֡��ֵ
	preCorner = trackedCorners;						//��ʼ���ٵ�λ�ø�ֵ

	int imgRow = iniImg.rows;						//ͼ�������
	int imgCol = iniImg.cols;						//ͼ�������

	float tempx_s, tempx_a, tempy_s, tempy_a;		//���ô��ڵ����������ޣ�������飬��ֹ���ڵ�ͼ���ⲿ��
	int sobelSize = 3;								//���ݶ�ͼ�������õ�Soble�˳ߴ�
	int nPoint = trackedCorners.size();				//���ٵ�ĸ���
	std::cout << "The number of tracked corners is: " << nPoint << std::endl << std::endl;

	std::stringstream Num;							//�ַ�����
	std::string imgNum, fullPath;					//ͼƬ��ţ�·��ȫ��
	int num = 34;									//��ȥ��ʼ��ͼƬ��ʣ���ͼƬ���д�34��ʼ���

	//int templateSize = 3;							//ģ��ʹ���ͼ��ĳߴ磨ģ��ֻ��֡ͼ����ٵ����ڵľ������򣬿ɰ����з�λ��ȡ��
	int templateSize = 10;
	int windowSize = 2 * templateSize + 1;			//ʵ��ģ��ʹ���ͼ��Ĵ�СΪ2*templateSize + 1��������ָ��֡ͼ����ٵ��������򣬲��ɰ����к���ȡ����Ϊ��������ת����������б��

	//if (templateSize % 2 != 1) templateSize++;	//����ģ��ߴ�

	cv::Mat matX, matY;								//���ڣ����mask�����ֱ�洢���ڵ��к��к�
	matX = matOperations::seqMatCol(2 * templateSize + 1, 2 * templateSize + 1);	//CV_32S���γ��кž��󣨴洢���ڵ��кţ�
	matY = matOperations::seqMatRow(2 * templateSize + 1, 2 * templateSize + 1);	//CV_32S���γ��кž��󣨴洢���ڵ��кţ�

	matX = matX - templateSize;						//ʹ�ô������ĵ�����Ϊ��0��0��
	matY = matY - templateSize;						//���������갴x��ˮƽ���ң�,y����ֱ���£�ȡ�������

	cv::Mat templateImg, nextWarpped, nextROIx, nextROIy;		//��֡ͼ������ȡ��ģ�壬��֡ͼ�񾭷���任��ȡ���Ĵ��ڣ��Լ�x���ݶȣ�y���ݶ�
	cv::Mat err(windowSize, windowSize, CV_32F);	//ǰ������ٵ�����Ҷ�ֵ֮���ģ��Ҷ�ֵ-���ڻҶ�ֵ��
	//cv::Mat JacobiW(2, 6, CV_32F);				//W(x;p)�ݶ�
	//cv::Mat Hessi(6, 6, CV_32F);					//Hessian���󣬸þ���Ĵ�С��P�Ĵ�С���������㷨P�Ĵ�СΪ6����HessianΪ6*6
	cv::Mat GradientXY;								//nextImg��x,y�����ݶȣ�1*2ά
	cv::Mat JacobiW;								//W(x;p)�ݶȣ��任������x,y����һ�׵���ֵ����Ӧ���Ĳ�������
	cv::Mat steepestDesc;							//�����½�ֵ����Ӧ���岽��������
	cv::Mat TsteepestDesc;							//steepestDesc��ת��
	cv::Mat Hessi;									//Hessian���󣨶�Ӧ�����������������þ���Ĵ�С��P�Ĵ�С���������㷨P�Ĵ�СΪ6����HessianΪ6*6
	cv::Mat invHessi;								//Hessian���������
	cv::Mat totalDesc;								//�½�ֵ����Ӧ���߲���������
	cv::Mat deltaP;									//��Ӧ�ڰ˲�������

	std::vector<cv::Point2f> cornerTrack1;			//�洢�����ٵ㣨1�ţ��Ĺ켣������ֻ���������㣬��ˣ�ֻ������vector����
	std::vector<cv::Point2f> cornerTrack2;			//�洢�����ٵ㣨2�ţ��Ĺ켣������ֻ���������㣬��ˣ�ֻ������vector����
	cv::Point2f newCorner;							//�����洢���ٵ��������

	double eps = 0.01;								//�ݶ��½�����ֹ���ȣ���ֹ������
	double iter;									//��������

	////���䣨warp���任��������ʼֵȡ���ٵ�ĳ�ʼ���꣩
	float P[2][6] = { {0,0,0,0,preCorner[0].x ,preCorner[0].y },{ 0,0,0,0,preCorner[1].x ,preCorner[1].y } }; 

	//������ø�˹�˲����ݶ�ͼ������Ҫʹ�����¸�˹��
	//cv::Mat DGaussX = matOperations::gaussianKernelDeriX(3.0);
	//cv::Mat DGaussY = matOperations::gaussianKernelDeriY(3.0);
	//std::cout << "The DGaussX is: " << std::endl << DGaussX << std::endl << std::endl;
	//std::cout << "The DGaussY is: " << std::endl << DGaussY << std::endl << std::endl;
	//cv::Mat flipDGaussX, flipDGaussY;
	//cv::flip(DGaussX, flipDGaussX, -1);
	//cv::flip(DGaussY, flipDGaussY, -1);

	//��������ͼ���֡������ѭ��
	for (num;num<88;num++)
	{
		//��ȡ��֡ͼ��
		Num << num;		
		Num >> imgNum;
		Num.clear();

		fullPath = imgBathPath + imgNum + imgSuffix;
		nextImg = cv::imread(fullPath, 1);
		nextImg.copyTo(copy1NextImg);

		//ͼ���ȡʧ�ܣ�����ѭ��
		if (nextImg.empty()) break;

		//��ȡͼƬ��Ϊ3ͨ����ת���ɵ�ͨ��
		if (nextImg.channels() == 3)	cv::cvtColor(nextImg, nextImg, CV_BGR2GRAY);

		//ÿ����һ֡ͼ����������и��ٵ��λ�ã����forѭ���Ĵ���ΪnPoint�����ٵ������
		for (int iter1 = 0;iter1 < nPoint;iter1++)
		{
			deltaP = cv::Mat::ones(6, 1, CV_32F);				//deltaP����ֵ
			iter = 0;

			tempx_s = preCorner[iter1].x - templateSize;
			tempx_a = preCorner[iter1].x + templateSize;
			tempy_s = preCorner[iter1].y - templateSize;
			tempy_a = preCorner[iter1].y + templateSize;

			if ((tempx_a >= 0) && (tempx_a < imgCol) && (tempy_s >= 0) && (tempy_a < imgRow))
			{
				//����ѭ��������ٵ�����֡ͼ���е�����
				while (cv::norm(deltaP) > eps)
				{
					iter++;
					if (iter > 80) break;							//������������80��û�ﵽ���ȣ���Ҳ�˳�

					cv::Mat transformMatrix = (cv::Mat_<float>(2, 3) << 1 + P[iter1][0], P[iter1][2], P[iter1][4], P[iter1][1], 1 + P[iter1][3], P[iter1][5]);

					//���ŵ�����tempy_s�Ȼ����С������������ΪС������Range���Զ���������ȡ��
					preImg(cv::Range(tempy_s, tempy_a + 1), cv::Range(tempx_s, tempx_a + 1)).copyTo(templateImg);

					//1.����֡ͼ����з���任���õ�����
					nextWarpped = warpping::warpTransform(nextImg, transformMatrix, matX, matY);	//����ͼ��ΪCV_32F������ת����ע��matX��matY��ҪŪ����
					//opencv2����Ҳ�з���任����AffineWarp�������������Ҫ�������Բ�ֵ

					//2.����ģ��ʹ��ڵĻҶȲ�
					templateImg.convertTo(templateImg, CV_32F);		//ת��CV_32F������Ҫ��CV_32F��nextWarpped����������ת�����
					//nextWarpped.convertTo(nextWarpped, CV_32F);		
					cv::subtract(templateImg, nextWarpped, err);	//֮���Բ��ٶ����ʱ��Ͷ����CV_32F������Ϊ��Щʱ�򣬸պ���������opencv�л��Զ�ת�����ͳ�CV_8U֮��ģ�����û�취ֻ��ÿ�ζ�ת

					//3.������ͼ���Ƚ���һ���ݶ�ͼ����㣬Ȼ����з���任�õ����ڵ�x��y���ݶ�ͼ
					//cv::Mat filterKernelx = (cv::Mat_<float>(1,3) << -1,0, 1);
					//cv::Mat filterKernely;
					//cv::transpose(filterKernelx, filterKernely);
					//cv::filter2D(nextImg, nextIx, CV_32F, filterKernelx);
					//cv::filter2D(nextImg, nextIy, CV_32F, filterKernely);

					//Sobel�������ݶ�ͼ��
					cv::Sobel(nextImg, nextIx, CV_32F, 1, 0, sobelSize);
					cv::Sobel(nextImg, nextIy, CV_32F, 0, 1, sobelSize);

					//��˹�˲����ݶ�ͼ��
					//cv::filter2D(nextImg, nextIx, CV_32F, flipDGaussX);
					//cv::filter2D(nextImg, nextIy, CV_32F, flipDGaussY);

					//std::cout << "The nextIx is: " << std::endl << ' ' << nextIx(cv::Range(100, 120), cv::Range(100, 120)) << std::endl << std::endl;
					//std::cout << "The nextIy is: " << std::endl << ' ' << nextIy(cv::Range(100, 120), cv::Range(100, 120)) << std::endl << std::endl;
					//cv::namedWindow("DX", 1);
					//cv::namedWindow("DY", 1);
					//cv::imshow("DX", nextIx);
					//cv::imshow("DY", nextIy);
					//cv::waitKey(10000);
					//cv::GaussianBlur(nextImg, nextIx, cv::Size(ksizeBlur + 8, ksizeBlur + 8), 1, 0);
					//cv::GaussianBlur(nextImg, nextIy, cv::Size(ksizeBlur + 8, ksizeBlur + 8), 0, 1);

					nextROIx = warpping::warpTransform(nextIx, transformMatrix, matX, matY);	//�����ΪCV_32F������ת��
					nextROIy = warpping::warpTransform(nextIy, transformMatrix, matX, matY);
					//nextROIx.convertTo(nextROIx, CV_32F);		
					//nextROIy.convertTo(nextROIy, CV_32F);

					//4.5.6.7.���Ǳ任������ſ˱Ⱦ��󣬼��������½�ͼ�񣬼��㺣�����󣬼��������½����ȣ����ڰ�ÿ�����ؼ��㣬���⼸���Ӻ���ѭ����
					Hessi = cv::Mat::zeros(6, 6, CV_32F);		//��ʼֵΪ0��
					totalDesc = cv::Mat::zeros(6, 1, CV_32F);	//��ʼֵΪ0����ά��Ϊ��6*1��*1

					matX.convertTo(matX, CV_32F);				//ת��ΪCV_32F��������㶼���������
					matY.convertTo(matY, CV_32F);

					//�����ذ������㣺�Ȱ��У������У�ѭ������Ϊģ��򴰿ڵ������ظ���
					for (int iter2 = 0;iter2 < 2 * templateSize + 1;iter2++)
					{
						float* pMatX = matX.ptr<float>(iter2);				//��ȡ�кž�����ָ��
						float* pMatY = matY.ptr<float>(iter2);				//��ȡ�кž�����ָ��
						float* pNextROIx = nextROIx.ptr<float>(iter2);		//��ȡ����x�ݶ�ͼ����ָ��
						float* pNextROIy = nextROIy.ptr<float>(iter2);		//��ȡ����y�ݶ�ͼ����ָ��

						float* pErr = err.ptr<float>(iter2);				//��ȡ���ͼ����ָ��

						for (int iter3 = 0;iter3 < 2 * templateSize + 1;iter3++)
						{
							GradientXY = (cv::Mat_<float>(1, 2) << pNextROIx[iter3], pNextROIy[iter3]);		//��x,y�����ش����ݶ�

							//4.���㣨x,y�����ش��ı任�����ſ˱Ⱦ���
							JacobiW = (cv::Mat_<float>(2, 6) << pMatX[iter3], 0, pMatY[iter3], 0, 1, 0,		
								0, pMatX[iter3], 0, pMatY[iter3], 0, 1);

							//5.���㣨x,y�����ش������½�ͼ��
							steepestDesc = GradientXY*JacobiW;					
							cv::transpose(steepestDesc, TsteepestDesc);			//ת��

							//6.������������˷�������.mul����multiply�������Ǿ���˷������ǵ��
							cv::add(Hessi, TsteepestDesc*steepestDesc, Hessi);	

							//7.�������½�ֵ
							cv::add(totalDesc, TsteepestDesc*pErr[iter3], totalDesc);
						}
					}

					//8.��deltaP
					cv::invert(Hessi, invHessi);
					deltaP = invHessi*totalDesc;

					//.9����P
					for (int iter4 = 0;iter4 < 6;iter4++)
					{
						float* pTdeltaP = deltaP.ptr<float>(iter4);		//deltaP��һ��6*1��������
						P[iter1][iter4] = P[iter1][iter4] + pTdeltaP[0];
					}
				}
			}
			
			std::cout << "The iter is: " << iter << std::endl << std::endl;
			nextCorner.push_back(cv::Point2f(P[iter1][4], P[iter1][5]));

			//�����ٵ�����������켣���飨�����ط�ͼ��
			if (iter1 == 0) cornerTrack1.push_back(nextCorner[iter1]);
			else cornerTrack2.push_back(nextCorner[iter1]);
		}

		//�жϸ��ٵ���������Ƿ���ͼ���ڣ������ж�Ϊ����
		if (nextCorner[0].x < 0 || nextCorner[0].x > imgCol || nextCorner[0].y < 0 || nextCorner[0].y > imgRow)
		{
			std::cout << "The first tracked corner is lost!!!" << std::endl;
			std::cout << "The first corner is: " << nextCorner[0].x << ',' << nextCorner[0].y << std::endl << std::endl;
			break;
		}
		if (nextCorner[1].x < 0 || nextCorner[1].x > imgCol || nextCorner[1].y < 0 || nextCorner[1].y > imgRow)
		{
			std::cout << "The second tracked corner is lost!!!" << std::endl;
			std::cout << "The second corner is: " << nextCorner[1].x << ',' << nextCorner[1].y << std::endl << std::endl;
			break;
		}

		cv::circle(copy1NextImg, nextCorner[0], 5, cv::Scalar(0, 255, 0), 2);
		cv::circle(copy1NextImg, nextCorner[1], 5, cv::Scalar(0, 255, 0), 2);

		//���¸��ٵ������
		preCorner = nextCorner;
		//�³�nextCorner����Ϊ�������ֵ�洢��ǰ��������
		nextCorner.clear();
		//��֡ͼ���Ϊ��֡ͼ�񣬽����´�ѭ��
		nextImg.copyTo(preImg);

		cv::namedWindow("Track Result", 1);
		cv::imshow("Track Result", copy1NextImg);
		cv::waitKey(10);
	}
	system("pause");
}