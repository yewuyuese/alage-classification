#include <opencv2/opencv.hpp>
#include <fstream>
#include <iostream>
#include "enumfile.h"	//ע���޸��ļ��и�ʽ��׺��Ϊ��Ҫͳ�Ƶ��ļ���ʽ

using namespace std;
using namespace cv;

int calchist_train();	//ͳ������ѵ������ֱ��ͼ
int calchist_test();	//ͳ�����д�������ֱ��ͼ
int camphist(int Knn_K = 10);	//KNN�㷨�ó�����������������

//#define Knn_K 15	//KNN �㷨�е� K ֵ

int main()
{
	char choice = NULL;
	do
	{
		cout << "ͳ�ơ�ѵ��������ֱ��ͼ��" << endl;
		cout << "1	ͳ��" << endl;
		cout << "2	��ͳ��" << endl;
		cin >> choice;
	} while (choice != '1' && choice != '2');
	if (choice == '1')
	{
		calchist_train();
	}

	choice = NULL;
	do
	{
		cout << "ͳ�ơ�����������ֱ��ͼ��" << endl;
		cout << "1	ͳ��" << endl;
		cout << "2	��ͳ��" << endl;
		cin >> choice;
		
	} while (choice != '1' && choice != '2');
	if (choice == '1')
	{
		calchist_test();
	}

	choice = NULL;
	do
	{
		cout << "KNN�㷨�������������" << endl;
		cout << "1	����" << endl;
		cout << "2	������" << endl;
		cin >> choice;
	} while (choice != '1' && choice != '2');
	if (choice == '1')
	{
		for (int K = 6, classNum = 0; K <= 6; K++)

		{
			classNum = camphist(K);
			cout << "K = " << K << "	Good = " << classNum << endl;
		}
	}
	
	system("pause");
	return 0;
}

int calchist_train()
{
	cout << "�Ƿ�ͳ������ѵ������ֱ��ͼ��" << endl;
	//�����ļ���ͳ���ļ�����
	int fileNum = 0;	//��������
	fileNum = enumfile(_T("E:\\�������ʶ��\\SVM MULTI\\Train Samples"));	//ע���޸�ͷ�ļ�enumfile.h�е�ͼƬ��ʽ��׺��
	cout << "��֪�������������" << fileNum << endl;

	char imagename[200];	//ͼ��·��
	char histname[200];
	int imagenum;
	//------------��һ��ȡͼƬ������ֱ��ͼ���洢-------------------
	for (imagenum = 1; imagenum <= fileNum; imagenum++)
	{
		cout << imagenum << ".jpgֱ��ͼ������..." << endl;
		sprintf(imagename, "E:\\�������ʶ��\\SVM MULTI\\Train Samples\\%d.jpg", imagenum);
		Mat img = imread(imagename, 0);
		if (!img.data)
		{
			return -1;
		}
		imshow("����ͼ", img);
		/// �ָ��3����ͨ��ͼ�� ( R, G �� B )
		vector<Mat> rgb_planes;
		split(img, rgb_planes);

		/// �趨bin��Ŀ
		int histSize = 255;

		/// �趨ȡֵ��Χ ( R,G,B) )
		float range[] = { 0, 254 };
		const float* histRange = { range };

		bool uniform = true; bool accumulate = false;

		Mat r_hist;

		/// ����ֱ��ͼ:
		calcHist(&rgb_planes[0], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);

		// ����ֱ��ͼ����
		int hist_w = 400; int hist_h = 400;
		int bin_w = cvRound((double)hist_w / histSize);

		Mat histImage(hist_w, hist_h, CV_8UC3, Scalar(0, 0, 0));

		/// ��ֱ��ͼ��һ������Χ [ 0, histImage.rows ]
		normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

		/// ��ֱ��ͼ�����ϻ���ֱ��ͼ
		for (int i = 1; i < histSize; i++)
		{
			line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
				Point(bin_w*(i), hist_h - cvRound(r_hist.at<float>(i))),
				Scalar(0, 0, 255), 2, 8, 0);
		}

		sprintf(histname, "E:\\�������ʶ��\\SVM MULTI\\hist_train\\%d.jpg", imagenum);
		imwrite(histname, r_hist);
		sprintf(histname, "E:\\�������ʶ��\\SVM MULTI\\histImage_train\\%d.jpg", imagenum);
		imwrite(histname, histImage);
		/// ��ʾֱ��ͼ
		//namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE);
		//imshow("calcHist Demo", histImage);
		//double rb = compareHist(r_hist, r_hist, CV_COMP_CORREL);
		//cout << rb << endl;
	}
	cout << "���ͳ��ѵ������ֱ��ͼ" << endl;
	return 0;
}

int calchist_test()
{
	cout << "��ʼͳ�Ʋ�������ֱ��ͼ��" << endl;
	//�����ļ���ͳ���ļ�����
	int fileNum = 0;	//��������
	fileNum = enumfile(_T("E:\\�������ʶ��\\SVM MULTI\\Test Samples"));	//ע���޸�ͷ�ļ�enumfile.h�е�ͼƬ��ʽ��׺��
	cout << "��֪�������������" << fileNum << endl;

	char imagename[200];	//ͼ��·��
	char histname[200];
	int imagenum;
	//------------��һ��ȡͼƬ������ֱ��ͼ���洢-------------------
	for (imagenum = 1; imagenum <= fileNum; imagenum++)
	{
		cout << imagenum << ".jpgֱ��ͼ������..." << endl;
		sprintf(imagename, "E:\\�������ʶ��\\SVM MULTI\\Test Samples\\%d.jpg", imagenum);
		Mat img = imread(imagename, 0);
		if (!img.data)
		{
			return -1;
		}
		imshow("����ͼ", img);
		waitKey(100);
		/// �ָ��3����ͨ��ͼ�� ( R, G �� B )
		vector<Mat> rgb_planes;
		split(img, rgb_planes);

		/// �趨bin��Ŀ
		int histSize = 255;

		/// �趨ȡֵ��Χ ( R,G,B) )
		float range[] = { 0, 254 };
		const float* histRange = { range };

		bool uniform = true; bool accumulate = false;

		Mat r_hist;

		/// ����ֱ��ͼ:
		calcHist(&rgb_planes[0], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);

		// ����ֱ��ͼ����
		int hist_w = 400; int hist_h = 400;
		int bin_w = cvRound((double)hist_w / histSize);

		Mat histImage(hist_w, hist_h, CV_8UC3, Scalar(0, 0, 0));

		/// ��ֱ��ͼ��һ������Χ [ 0, histImage.rows ]
		normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

		/// ��ֱ��ͼ�����ϻ���ֱ��ͼ
		for (int i = 1; i < histSize; i++)
		{
			line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
				Point(bin_w*(i), hist_h - cvRound(r_hist.at<float>(i))),
				Scalar(0, 0, 255), 2, 8, 0);
		}

		sprintf(histname, "E:\\�������ʶ��\\SVM MULTI\\hist_test\\%d.jpg", imagenum);
		imwrite(histname, r_hist);
		sprintf(histname, "E:\\�������ʶ��\\SVM MULTI\\histImage_test\\%d.jpg", imagenum);
		imwrite(histname, histImage);
		/// ��ʾֱ��ͼ
		//namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE);
		//imshow("calcHist Demo", histImage);
		//double rr = compareHist(r_hist, r_hist, CV_COMP_CORREL);
		//cout << rr << endl;
	}
	cout << "���ͳ�Ʋ�������ֱ��ͼ" << endl;
	return 0;
}

int camphist(int Knn_K)
{
	cout << "�������������ѵ��������ֱ��ͼ���ƶ���..." << endl;

	int testNum, trainNum, classNum = 0;	//������������ѵ����������������ȷ������
	vector<int> classIndex(4,0);	//
	Mat imgTest, imgTrain;
	Mat histTest, histTrain;

	testNum = enumfile(_T("E:\\�������ʶ��\\SVM MULTI\\Test Samples"));	//ͳ�Ʋ�������ֱ��ͼ����---ע���޸�ͷ�ļ�enumfile.h�е�ͼƬ��ʽ��׺��
	cout << "��������ֱ��ͼ������" << testNum << endl;
	trainNum = enumfile(_T("E:\\�������ʶ��\\SVM MULTI\\Train Samples"));	//ͳ��ѵ������ֱ��ͼ����---ע���޸�ͷ�ļ�enumfile.h�е�ͼƬ��ʽ��׺��
	cout << "ѵ������ֱ��ͼ������" << trainNum << endl;

	vector<double> approxi;
	vector<int> ranking;
	double relation;
	char histTestPath[200];
	char histTrainPath[200];
	FileStorage file("approxi.yml", FileStorage::WRITE);

	for (int testIndex = 1; testIndex <= testNum; testIndex++)
	{
		approxi.clear();
		ranking.clear();
		for (auto i = classIndex.begin(); i != classIndex.end(); i++)
		{
			*i = 0;
			//cout << *i << "	" << endl;
		}
		sprintf(histTestPath, "E:\\�������ʶ��\\SVM MULTI\\Test Samples\\%d.jpg", testIndex);
		imgTest = imread(histTestPath,1);
		if (!imgTest.data)
		{
			return -1;
		}

		vector<Mat> rgb_planes;
		split(imgTest, rgb_planes);

		/// �趨bin��Ŀ
		int histSize = 255;

		/// �趨ȡֵ��Χ ( R,G,B) )
		float range[] = { 0, 254 };
		const float* histRange = { range };

		bool uniform = true; bool accumulate = false;

		/// ����ֱ��ͼ:
		calcHist(&rgb_planes[0], 1, 0, Mat(), histTest, 1, &histSize, &histRange, uniform, accumulate);

		// ����ֱ��ͼ����
		int hist_w = 255; int hist_h = 255;
		//int bin_w = cvRound((double)hist_w / histSize);

		Mat histImage(hist_w, hist_h, CV_8UC3, Scalar(0, 0, 0));

		/// ��ֱ��ͼ��һ������Χ [ 0, histImage.rows ]
		normalize(histTest, histTest, 0, histImage.rows, NORM_MINMAX, -1, Mat());

		//file << testIndex << ": ";
		for (int trainIndex = 1; trainIndex <= trainNum; trainIndex++)
		{
			sprintf(histTrainPath, "E:\\�������ʶ��\\SVM MULTI\\Train Samples\\%d.jpg", trainIndex);
			imgTrain = imread(histTrainPath, 0);
			if (!imgTrain.data)
			{
				return -1;
			}
			split(imgTrain,rgb_planes);
			calcHist(&rgb_planes[0], 1, 0, Mat(), histTrain, 1, &histSize, &histRange, uniform, accumulate);
			normalize(histTrain, histTrain, 0, histImage.rows, NORM_MINMAX, -1, Mat());

			relation = 0;
			relation = compareHist(histTest, histTrain, CV_COMP_CORREL);
			approxi.push_back(relation);
			ranking.push_back(trainIndex);
			//file << relation;
		}
		if (!(approxi.size() == ranking.size() &&approxi.size() == trainNum))
		{
			cout << "Error������������ѵ��������ֱ��ͼ���ƶȼ���ʧ��" << endl;
			return 0;
		}
		else
		{
			//cout << testIndex<< ".jpg ֱ��ͼ���ƶȼ������..." << endl;
		}

		//���ƶ��ɴ�С����ͬʱ���Ҳ��Ӧ����
		for (unsigned int i = 0; i < approxi.size()-1; i++)
		{
			for (unsigned int j = i + 1; j < approxi.size(); j++)
			{
				if (approxi[i] < approxi[j])
				{
					swap(approxi[i], approxi[j]);
					swap(ranking[i], ranking[j]);
				}
			}
		}

		//�ҳ����ƶ���ߵ�Knn_K��ѵ��������������
		for (int i = 0; i < Knn_K; i++)
		{
			if (ranking[i] <= 100)
				classIndex[1]++;
			else if (ranking[i] <= 200)
				classIndex[2]++;
			else if (ranking[i] <= 300)
				classIndex[3]++;
		}
		for (unsigned int i = 1; i < classIndex.size(); i++)
		{
			//cout << classIndex[i] << "	";
			if (classIndex[0] < classIndex[i])
				classIndex[0] = i;
		}
		//cout << endl << "-------------------------�������" << classIndex[0] << endl;
		
		if (testIndex <= 10)
			classNum += (classIndex[0] == 1);
		else if (testIndex <= 20)
			classNum += (classIndex[0] == 2);
		else if (testIndex <= 30)
			classNum += (classIndex[0] == 3);
	}
	file.release();
	//system("pause");
	return classNum;
}