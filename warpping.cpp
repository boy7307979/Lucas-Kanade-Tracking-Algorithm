#pragma once
#include<opencv2\opencv.hpp>
#include "warpping.h"

cv::Mat warpping::warpTransform(cv::Mat img, cv::Mat transMatrix, cv::Mat x, cv::Mat y)
{
	//**********************************************************
	//������Ϊ����任����������ľ���typeΪCV_32F
	//���룺	img��			Ϊ����ԭʼͼ��type����
	//			transMatrix��	Ϊ�任����
	//			x��				Ϊģ����о��󣬶�Ӧimg�е��к�
	//			y��				Ϊģ����о��󣬶�Ӧimg�е��к�
	//**********************************************************

	if (img.depth() != 5)		img.convertTo(img, CV_32F);	//����������������ת�������������Ϊ�䶨���ָ��Ϊfloat���͵�
	int cols = img.cols;
	int rows = img.rows;

	cv::Mat Tlocalx, Tlocaly;		//
	//cv::Mat Tlocalx(rows_x, rows_x, CV_32F);
	//cv::Mat Tlocaly(rows_x, rows_x, CV_32F);

	float M11 = transMatrix.at<float>(0, 0);
	float M12 = transMatrix.at<float>(0, 1);
	float M13 = transMatrix.at<float>(0, 2);
	float M21 = transMatrix.at<float>(1, 0);
	float M22 = transMatrix.at<float>(1, 1);
	float M23 = transMatrix.at<float>(1, 2);

	cv::addWeighted(x, M11, y, M12, M13, Tlocalx);
	cv::addWeighted(x, M21, y, M22, M23, Tlocaly);
	
	//�����M,x,y��Ԫ�ؿ������������������û��ǿ��ָ��M,x,y�����ͣ���opencv��Ĭ����ΪCV_8U��
	//����������֮���Tlocal��ΪCV_8U���ͣ����Զ����ݲ�������Ĳ����������ͣ�
	//��ý����н��뵽���������MatͳһΪCV_32F��������û�����������
	if (Tlocalx.depth() != 5)		Tlocalx.convertTo(Tlocalx, CV_32F);
	if (Tlocaly.depth() != 5)		Tlocaly.convertTo(Tlocaly, CV_32F);
		
	cv::Mat xBas0, xBas1, yBas0, yBas1;
	
	xBas0 = floor(Tlocalx, cols - 1, 0);	//���ǶԻҶ����������ȡ�������ǻҶ�ֵ
	yBas0 = floor(Tlocaly, rows - 1, 0);

	int basex00, basey00, basex01, basey01, basex10, basey10, basex11, basey11;

	cv::Mat xCoe, yCoe;

	xBas0.convertTo(xBas0, CV_32F);		//ע�⣬floor��������CV_32S��������subtract��Ҫ��CV_32F���������������ǰת������Ȼ����
	yBas0.convertTo(yBas0, CV_32F);

	cv::subtract(Tlocalx, xBas0, xCoe);
	cv::subtract(Tlocaly, yBas0, yCoe);

	//˫���Բ�ֵϵ����ע�������ǵ�˲��Ǿ���˷���
	cv::Mat perc00 = (1.f - xCoe).mul(1.f - yCoe);		
	//cv::Mat perc1 = (1 - xCoe).mul(yCoe);
	//cv::Mat perc2 = xCoe.mul(1 - yCoe);
	cv::Mat perc01 = xCoe.mul(1.f - yCoe);
	cv::Mat perc10 = (1.f - xCoe).mul(yCoe);
	cv::Mat perc11 = xCoe.mul(yCoe);

	cv::Mat outM(x.size(), CV_32F);
	float* poutM;
	float intensity00, intensity01, intensity10, intensity11;
	float coe00, coe01, coe10, coe11;
	float* pxBas0;
	float* pyBas0;

	float* pImg1;
	float* pImg2;

	float* pPerc00;
	float* pPerc01;
	float* pPerc10;
	float* pPerc11;

	int windowRow = yBas0.rows;
	int windowCol = xBas0.rows;

	//��������任֮�󣬴��ڻ�������ת������ֱ����Range��ȥ���У���Ҫ���ݱ任֮�󴰿ڵ�ÿ��Ԫ���и������굽��֡ͼ���в�����ȡ��Ӧ�Ҷ�ֵ
	for (int iter1 = 0;iter1 < windowRow;iter1++)
	{
		pxBas0 = xBas0.ptr<float>(iter1);
		pyBas0 = yBas0.ptr<float>(iter1);

		pPerc00 = perc00.ptr<float>(iter1);
		pPerc01 = perc01.ptr<float>(iter1);
		pPerc10 = perc10.ptr<float>(iter1);
		pPerc11 = perc11.ptr<float>(iter1);

		poutM = outM.ptr<float>(iter1);
		for (int iter2 = 0;iter2 < windowCol;iter2++)
		{
			basex00 = pxBas0[iter2];
			basey00 = pyBas0[iter2];
			basex01 = basex00 + 1;
			basey01 = basey00;
			basex10 = basex00;
			basey10 = basey00 + 1;
			basex11 = basex00 + 1;
			basey11 = basey00 + 1;

			pImg1 = img.ptr<float>(basey00);
			pImg2 = img.ptr<float>(basey10);

			intensity00 = pImg1[basex00];
			intensity01 = pImg1[basex01];
			intensity10 = pImg2[basex10];
			intensity11 = pImg2[basex11];

			poutM[iter2] = pPerc00[iter2] * intensity00 + pPerc01[iter2] * intensity01 + pPerc10[iter2] * intensity10 + pPerc11[iter2] * intensity11;
		}
	}

	return outM;
}

cv::Mat warpping::floor(cv::Mat M)
{
	//**********************************************************
	//��Mat�������ͽ�������ȡ��������CV_32S
	//���룺	M��ΪCV_32F Mat����Ҫȡ���ľ���
	//**********************************************************

	cv::Mat outM(M.size(), CV_32S);
	//cv::Mat outM;
	int cols = M.cols;
	int rows = M.rows;
	for (int iter1 = 0;iter1 < rows;iter1++)
	{
		float* pM = M.ptr<float>(iter1);		//����������������Ϊ32F
		int* poutM = outM.ptr<int>(iter1);
		for (int iter2 = 0;iter2 < cols;iter2++)
		{
			poutM[iter2] = cvFloor(pM[iter2]);
		}
	}
	return outM;
}

cv::Mat warpping::floor(cv::Mat M, int max, int min)
{
	//**********************************************************
	//��Mat�������ͽ�������ȡ����ͬʱ���������޼�⣬����CV_32S
	//���룺	M��ΪCV_32F Mat����Ҫȡ���ľ���
	//**********************************************************

	cv::Mat outM(M.size(), CV_32S);
	//cv::Mat outM;
	int cols = M.cols;
	int rows = M.rows;
	for (int iter1 = 0;iter1 < rows;iter1++)
	{
		float* pM = M.ptr<float>(iter1);	//����������������Ϊ32F
		int* poutM = outM.ptr<int>(iter1);
		for (int iter2 = 0;iter2 < cols;iter2++)
		{
			poutM[iter2] = cvFloor(pM[iter2]);
			if (poutM[iter2]<min || poutM[iter2]>max)
			{
				poutM[iter2] = 0;	//�����������ޣ�����Ϊ0ֵ
			}
		}
	}
	return outM;
}

cv::Mat warpping::ceil(cv::Mat M)
{
	//**********************************************************
	//��Mat�������ͽ�������ȡ��������CV_32S
	//���룺	M��ΪCV_32F Mat����Ҫȡ���ľ���
	//**********************************************************

	cv::Mat outM(M.size(), CV_32S);
	//cv::Mat outM;
	int cols = M.cols;
	int rows = M.rows;
	for (int iter1 = 0;iter1 < rows;iter1++)
	{
		float* pM = M.ptr<float>(iter1);
		int* poutM = outM.ptr<int>(iter1);
		for (int iter2 = 0;iter2 < cols;iter2++)
		{
			poutM[iter2] = cvCeil(pM[iter2]);
		}
	}
	return outM;
}

cv::Mat warpping::ceil(cv::Mat M, int max, int min)
{
	//**********************************************************
	//��Mat�������ͽ�������ȡ����ͬʱ���������޼�⣬����CV_32S
	//���룺	M��ΪCV_32F Mat����Ҫȡ���ľ���
	//**********************************************************

	cv::Mat outM(M.size(), CV_32S);
	//cv::Mat outM;
	int cols = M.cols;
	int rows = M.rows;
	for (int iter1 = 0;iter1 < rows;iter1++)
	{
		float* pM = M.ptr<float>(iter1);
		int* poutM = outM.ptr<int>(iter1);
		for (int iter2 = 0;iter2 < cols;iter2++)
		{
			poutM[iter2] = cvCeil(pM[iter2]);
			if (poutM[iter2]<min || poutM[iter2]>max)
			{
				poutM[iter2] = 0;	//�����������ޣ�����Ϊ0ֵ
			}
		}
	}
	return outM;
}