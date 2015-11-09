#include <opencv2/opencv.hpp>
#include <fstream>
#include <iostream>
#include "enumfile.h"	//注意修改文件中格式后缀名为需要统计的文件格式

using namespace std;
using namespace cv;

int calchist_train();	//统计所有训练样本直方图
int calchist_test();	//统计所有待测样本直方图
int camphist(int Knn_K = 10);	//KNN算法得出测试样本所属的类

//#define Knn_K 15	//KNN 算法中的 K 值

int main()
{
	char choice = NULL;
	do
	{
		cout << "统计“训练样本”直方图？" << endl;
		cout << "1	统计" << endl;
		cout << "2	不统计" << endl;
		cin >> choice;
	} while (choice != '1' && choice != '2');
	if (choice == '1')
	{
		calchist_train();
	}

	choice = NULL;
	do
	{
		cout << "统计“测试样本”直方图？" << endl;
		cout << "1	统计" << endl;
		cout << "2	不统计" << endl;
		cin >> choice;
		
	} while (choice != '1' && choice != '2');
	if (choice == '1')
	{
		calchist_test();
	}

	choice = NULL;
	do
	{
		cout << "KNN算法分类测试样本？" << endl;
		cout << "1	分类" << endl;
		cout << "2	不分类" << endl;
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
	cout << "是否统计所有训练样本直方图？" << endl;
	//遍历文件，统计文件总数
	int fileNum = 0;	//样本总数
	fileNum = enumfile(_T("E:\\藻类分类识别\\SVM MULTI\\Train Samples"));	//注意修改头文件enumfile.h中的图片格式后缀名
	cout << "已知类别样本总数：" << fileNum << endl;

	char imagename[200];	//图像路径
	char histname[200];
	int imagenum;
	//------------逐一读取图片，计算直方图并存储-------------------
	for (imagenum = 1; imagenum <= fileNum; imagenum++)
	{
		cout << imagenum << ".jpg直方图计算中..." << endl;
		sprintf(imagename, "E:\\藻类分类识别\\SVM MULTI\\Train Samples\\%d.jpg", imagenum);
		Mat img = imread(imagename, 0);
		if (!img.data)
		{
			return -1;
		}
		imshow("样本图", img);
		/// 分割成3个单通道图像 ( R, G 和 B )
		vector<Mat> rgb_planes;
		split(img, rgb_planes);

		/// 设定bin数目
		int histSize = 255;

		/// 设定取值范围 ( R,G,B) )
		float range[] = { 0, 254 };
		const float* histRange = { range };

		bool uniform = true; bool accumulate = false;

		Mat r_hist;

		/// 计算直方图:
		calcHist(&rgb_planes[0], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);

		// 创建直方图画布
		int hist_w = 400; int hist_h = 400;
		int bin_w = cvRound((double)hist_w / histSize);

		Mat histImage(hist_w, hist_h, CV_8UC3, Scalar(0, 0, 0));

		/// 将直方图归一化到范围 [ 0, histImage.rows ]
		normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

		/// 在直方图画布上画出直方图
		for (int i = 1; i < histSize; i++)
		{
			line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
				Point(bin_w*(i), hist_h - cvRound(r_hist.at<float>(i))),
				Scalar(0, 0, 255), 2, 8, 0);
		}

		sprintf(histname, "E:\\藻类分类识别\\SVM MULTI\\hist_train\\%d.jpg", imagenum);
		imwrite(histname, r_hist);
		sprintf(histname, "E:\\藻类分类识别\\SVM MULTI\\histImage_train\\%d.jpg", imagenum);
		imwrite(histname, histImage);
		/// 显示直方图
		//namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE);
		//imshow("calcHist Demo", histImage);
		//double rb = compareHist(r_hist, r_hist, CV_COMP_CORREL);
		//cout << rb << endl;
	}
	cout << "完成统计训练样本直方图" << endl;
	return 0;
}

int calchist_test()
{
	cout << "开始统计测试样本直方图：" << endl;
	//遍历文件，统计文件总数
	int fileNum = 0;	//样本总数
	fileNum = enumfile(_T("E:\\藻类分类识别\\SVM MULTI\\Test Samples"));	//注意修改头文件enumfile.h中的图片格式后缀名
	cout << "已知类别样本总数：" << fileNum << endl;

	char imagename[200];	//图像路径
	char histname[200];
	int imagenum;
	//------------逐一读取图片，计算直方图并存储-------------------
	for (imagenum = 1; imagenum <= fileNum; imagenum++)
	{
		cout << imagenum << ".jpg直方图计算中..." << endl;
		sprintf(imagename, "E:\\藻类分类识别\\SVM MULTI\\Test Samples\\%d.jpg", imagenum);
		Mat img = imread(imagename, 0);
		if (!img.data)
		{
			return -1;
		}
		imshow("样本图", img);
		waitKey(100);
		/// 分割成3个单通道图像 ( R, G 和 B )
		vector<Mat> rgb_planes;
		split(img, rgb_planes);

		/// 设定bin数目
		int histSize = 255;

		/// 设定取值范围 ( R,G,B) )
		float range[] = { 0, 254 };
		const float* histRange = { range };

		bool uniform = true; bool accumulate = false;

		Mat r_hist;

		/// 计算直方图:
		calcHist(&rgb_planes[0], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);

		// 创建直方图画布
		int hist_w = 400; int hist_h = 400;
		int bin_w = cvRound((double)hist_w / histSize);

		Mat histImage(hist_w, hist_h, CV_8UC3, Scalar(0, 0, 0));

		/// 将直方图归一化到范围 [ 0, histImage.rows ]
		normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

		/// 在直方图画布上画出直方图
		for (int i = 1; i < histSize; i++)
		{
			line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
				Point(bin_w*(i), hist_h - cvRound(r_hist.at<float>(i))),
				Scalar(0, 0, 255), 2, 8, 0);
		}

		sprintf(histname, "E:\\藻类分类识别\\SVM MULTI\\hist_test\\%d.jpg", imagenum);
		imwrite(histname, r_hist);
		sprintf(histname, "E:\\藻类分类识别\\SVM MULTI\\histImage_test\\%d.jpg", imagenum);
		imwrite(histname, histImage);
		/// 显示直方图
		//namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE);
		//imshow("calcHist Demo", histImage);
		//double rr = compareHist(r_hist, r_hist, CV_COMP_CORREL);
		//cout << rr << endl;
	}
	cout << "完成统计测试样本直方图" << endl;
	return 0;
}

int camphist(int Knn_K)
{
	cout << "计算测试样本与训练样本的直方图近似度中..." << endl;

	int testNum, trainNum, classNum = 0;	//测试样本数，训练样本数，分类正确样本数
	vector<int> classIndex(4,0);	//
	Mat imgTest, imgTrain;
	Mat histTest, histTrain;

	testNum = enumfile(_T("E:\\藻类分类识别\\SVM MULTI\\Test Samples"));	//统计测试样本直方图总数---注意修改头文件enumfile.h中的图片格式后缀名
	cout << "测试样本直方图总数：" << testNum << endl;
	trainNum = enumfile(_T("E:\\藻类分类识别\\SVM MULTI\\Train Samples"));	//统计训练样本直方图总数---注意修改头文件enumfile.h中的图片格式后缀名
	cout << "训练样本直方图总数：" << trainNum << endl;

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
		sprintf(histTestPath, "E:\\藻类分类识别\\SVM MULTI\\Test Samples\\%d.jpg", testIndex);
		imgTest = imread(histTestPath,1);
		if (!imgTest.data)
		{
			return -1;
		}

		vector<Mat> rgb_planes;
		split(imgTest, rgb_planes);

		/// 设定bin数目
		int histSize = 255;

		/// 设定取值范围 ( R,G,B) )
		float range[] = { 0, 254 };
		const float* histRange = { range };

		bool uniform = true; bool accumulate = false;

		/// 计算直方图:
		calcHist(&rgb_planes[0], 1, 0, Mat(), histTest, 1, &histSize, &histRange, uniform, accumulate);

		// 创建直方图画布
		int hist_w = 255; int hist_h = 255;
		//int bin_w = cvRound((double)hist_w / histSize);

		Mat histImage(hist_w, hist_h, CV_8UC3, Scalar(0, 0, 0));

		/// 将直方图归一化到范围 [ 0, histImage.rows ]
		normalize(histTest, histTest, 0, histImage.rows, NORM_MINMAX, -1, Mat());

		//file << testIndex << ": ";
		for (int trainIndex = 1; trainIndex <= trainNum; trainIndex++)
		{
			sprintf(histTrainPath, "E:\\藻类分类识别\\SVM MULTI\\Train Samples\\%d.jpg", trainIndex);
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
			cout << "Error：测试样本与训练样本的直方图相似度计算失败" << endl;
			return 0;
		}
		else
		{
			//cout << testIndex<< ".jpg 直方图近似度计算完成..." << endl;
		}

		//相似度由大到小排序，同时序号也对应排序
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

		//找出相似度最高的Knn_K个训练样本中最多的类
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
		//cout << endl << "-------------------------所属类别：" << classIndex[0] << endl;
		
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