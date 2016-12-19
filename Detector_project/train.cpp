// Training.cpp
#include "CVFrameDlg.h"
#include "cv.h"
#include "highgui.h"
#include "cxcore.h"
#include "stdafx.h"
#include"CVFrame.h"
#include "CVFrameDlg.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <float.h>
#include<limits.h>
#include <time.h>
#include <ctype.h>
//#include"CVAPP.h"
#include"./api/HistoryEdit.h"
#include "ImageProc.h"
#include "IntegralImage.h"
#include "Setting.h"
#include"./api/FileInfo.h"
#include <algorithm>
#ifdef _DEBUG
#define newDEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif
// Loadingpositiveexamples(face)
voidCCVFrameDlg::LoadPrositiveExamples()
{
	CString dirPath,pathName, filename;
	CFileDialog dlg(TRUE, NULL,(LPCTSTR)m_openedFileName);
	chartitle1[]={"Open POSITIVE(face) ImageDirectory: Select oneimagewithin thedirectory"};
	dlg.m_ofn.lpstrTitle= title1;
	if (dlg.DoModal()== IDOK)
	{
		m_bOpenFile = TRUE;
		pathName = dlg.GetPathName();
		filename = dlg.GetFileName();
		dirPath= pathName.Left( pathName.GetLength()-filename.GetLength() );
	}
	CFileInfoArray fileArray;
	fileArray.AddDir(
		dirPath,
		// Directory
		"*.*",
		// Filemask (all files)
		FALSE,
		// Recurse subdirs
		CFileInfoArray::AP_SORTBYNAME| CFileInfoArray::AP_SORTASCENDING, // Sort by name andascending
		FALSE
		// Don't add entries for dirs
		);
	//char buff[128];
	m_cntTrainedImg= fileArray.GetSize();
	inttempSize = fileArray.GetSize() ;
	m_nPositiveImg= tempSize;
	for(intj=0; j < tempSize; j++){
		m_szPositiveImgPath[j].path = fileArray[j].GetFilePath();
		m_szPositiveImgPath[j].filename = fileArray[j].GetFileTitle();
		/*
		m_ImageProc->m_filename= m_szPositiveImgPath[j].path;
		m_ImageProc->InitImageBuff();
		// convertcolorto grayimage.
		IplImage* image= cvCreateImage(cvSize( m_ImageProc->m_img->width,m_ImageProc->m_img->height), IPL_DEPTH_8U, 1);
		image->origin = m_ImageProc->m_img->origin;
		cvCvtColor( m_ImageProc->m_img, image, CV_BGR2GRAY );
		sprintf(buff,"Image %s",m_szPositiveImgPath[j].filename);
		cvvNamedWindow( buff, 1 );
		cvvShowImage( buff, image);
		*/
	}
}
// LoadingNegativeexamples(face)
voidCCVFrameDlg::LoadNegativeExamples()
{
	CString dirPath,pathName, filename;
	CFileDialog dlg(TRUE, NULL,(LPCTSTR)m_openedFileName);
	chartitle1[]={"Open NEGATIVE(Non-face)ImageDirectory: Select oneimagewithin the directory"};
	dlg.m_ofn.lpstrTitle= title1;
	if (dlg.DoModal()== IDOK)
	{
		m_bOpenFile = TRUE;
		pathName = dlg.GetPathName();
		filename = dlg.GetFileName();
		dirPath= pathName.Left( pathName.GetLength()-filename.GetLength() );
	}
	CFileInfoArray fileArray;
	fileArray.AddDir(
		dirPath,
		// Directory
		"*.*",
		// Filemask (all files)
		FALSE,
		// Recurse subdirs
		CFileInfoArray::AP_SORTBYNAME| CFileInfoArray::AP_SORTASCENDING, // Sort by name andascending
		FALSE
		// Don't add entries for dirs
		);
	//char buff[128];
	m_cntTrainedImg= fileArray.GetSize();
	inttempSize = fileArray.GetSize() ;
	m_nNegativeImg = tempSize;
	for(intj=0; j < tempSize; j++){
		m_szNegativeImgPath[j].path = fileArray[j].GetFilePath();
		m_szNegativeImgPath[j].filename = fileArray[j].GetFileTitle();
	}
}
// Init positive images
voidCCVFrameDlg::InitPositiveImage()
{
	CString temp;
	for(inti=0; i < m_nPositiveImg ; i++){
		IplImage* tempImg;IplImage* tempImage2;
		tempImg= cvLoadImage(m_szPositiveImgPath[i].path,1); //load gray image
		tempImage2= cvCreateImage(cvSize( tempImg->width,tempImg->height), IPL_DEPTH_8U, 1);
		tempImage2->origin = tempImg->origin;
		cvCvtColor( tempImg, tempImage2, CV_BGR2GRAY );
		memset(m_ImgPositive[i].InputImage,0,WINDOW_WIDTH*WINDOW_HEIGHT);
		memset(m_ImgPositive[i].IntegrailImage,0,(WINDOW_WIDTH+1)*(WINDOW_HEIGHT+1));
		if( ((tempImage2->width)!=WINDOW_WIDTH)||((tempImage2->width)!=WINDOW_HEIGHT) ){
			AfxMessageBox("Imagesizeis notmatch!");
			return;
		}
		intwidth = tempImage2->width;
		int height = tempImage2->height;
		m_width= width;
		m_height = height;
		for(introw=0; row< height; row++){
			for(intcol=0; col< width; col++){
				m_ImgPositive[i].InputImage[width*(row)+(col)]= tempImage2->imageData[width*(row)+(col)];
				m_DlgImg[width*(row) + (col)] = m_ImgPositive[i].InputImage[width*(row) +(col)];
			}
		}
		Invalidate(FALSE);
		// AfxMessageBox("push to next");
		MakeIntegImage(m_ImgPositive[i].InputImage,m_ImgPositive[i].IntegrailImage,width,height);
		temp.Format("Positive TrainedFile: %sisformulated integralimage",m_szPositiveImgPath[i].filename);
		m_Log.AppendString(temp);
		cvReleaseImage(&tempImg );
	}
}
// Init Negative images
voidCCVFrameDlg::InitNegativeImage()
{
	CString temp;
	for(inti=0; i < m_nNegativeImg ; i++){
		IplImage* tempImg;IplImage* tempImage2;
		tempImg= cvLoadImage(m_szNegativeImgPath[i].path,1); //load grayimage
		tempImage2= cvCreateImage(cvSize( tempImg->width,tempImg->height), IPL_DEPTH_8U, 1);
		tempImage2->origin = tempImg->origin;
		cvCvtColor( tempImg, tempImage2, CV_BGR2GRAY );
		memset(m_ImgNegative[i].InputImage,0,WINDOW_WIDTH*WINDOW_HEIGHT);
		memset(m_ImgNegative[i].IntegrailImage,0,(WINDOW_WIDTH+1)*(WINDOW_HEIGHT+1));
		if( ((tempImage2->width)!=WINDOW_WIDTH)||((tempImage2->width)!=WINDOW_HEIGHT) ){
			AfxMessageBox("Imagesizeis notmatch!");
			return;
		}
		intwidth = tempImage2->width;
		int height = tempImage2->height;
		m_width= width;
		m_height = height;
		for(introw=0; row< height; row++){
			for(intcol=0; col< width; col++){
				m_ImgNegative[i].InputImage[width*(row)+(col)]= tempImage2->imageData[width*(row)+(col)];
				m_DlgImg[width*(row) + (col)] = m_ImgNegative[i].InputImage[width*(row) +(col)];
			}
		}
		Invalidate(FALSE);
		MakeIntegImage(m_ImgNegative[i].InputImage,m_ImgNegative[i].IntegrailImage,width,height);
		temp.Format("Negative TrainedFile:%s is formulatedintegralimage",m_szPositiveImgPath[i].filename);
		m_Log.AppendString(temp);
		cvReleaseImage(&tempImg );
	}
}
// BeginCascading Classifier
void CCVFrameDlg::BeginCascadeClassifier()
{
	double* F= new double[NODE_CASCADE];
	double* D= new double[NODE_CASCADE];
	int* n= newint[NODE_CASCADE];
	F[0] = 1.0;
	D[0] = 1.0;
	m_fMaxFP = 0.5f;
	for(intnNode= 0; nNode< NODE_CASCADE; nNode++){
		// initialize weight
		if(nNode == 0)
		{
			double wIniWeightPosi = 0.5/m_nPositiveImg;
			double wIniWeightNega = 0.5/m_nNegativeImg;
			// Paperdefine'i' for numberof examplesand 'j' fornumberof featureso i follow it.
			for(inti=0; i < m_nPositiveImg; i++ ){
				m_ImgPositive[i].weight = wIniWeightPosi;
			}
			for(intii=0; ii< m_nNegativeImg; ii++){
				m_ImgNegative[ii].weight = wIniWeightNega;
			}
		}
		nNode++;
		n[nNode] = 0;
		F[nNode] = F[nNode-1];
		F[nNode] = 1.0;
		while(F[nNode]>(m_fMaxFP)){
			n[nNode]++;
			// AdaBoost
			//m_pThread = AfxBeginThread(ThreadAdaFunc, this, THREAD_PRIORITY_NORMAL,0, 0);
			AdaBoost(n[nNode], nNode -1);
		}
	}
}
// Adaboost
voidCCVFrameDlg::AdaBoost(intn, intnNode)
{
	int* eachlabels = NULL;
	double* eachweight = NULL;
	double* errors1; double* errors2;
	double* value = new double[m_nPositiveImg+m_nNegativeImg];
	double* weakFeatureValue = new double[m_nPositiveImg+m_nNegativeImg];
	int* labels = newint[m_nPositiveImg+m_nNegativeImg];
	double* weights = new double[m_nPositiveImg+m_nNegativeImg];
	errors1 = new double[m_nPositiveImg+m_nNegativeImg];
	errors2 = new double[m_nPositiveImg+m_nNegativeImg];
	eachlabels = new int[m_nPositiveImg+m_nNegativeImg];
	eachweight = new double[m_nPositiveImg+m_nNegativeImg];
	CString strTemp;
	//int ntrainingPos = training.m_posSize;
	intcntLabel = 0;
	for(int i=0;i< m_nPositiveImg;i++)
	{
		weights[i]= m_ImgPositive[i].weight;
		labels[i] = 1;// positive
		cntLabel++;
	}
	for( i=0;i< m_nNegativeImg;i++)
	{
		weights[i]= m_ImgNegative[i].weight;
		labels[i] = 0;// negative
		cntLabel++;
	}
	double minerror;
	minerror = 1E100;
	// CWeakClassifierminsc= allFeatures.m_wData[0];
	/////////////////////////////////////////////////////////////////////////////
	//... weakClass[0] -> minsc!!!
	WeakClassifier minsc= weakClass[0];
	/////////////////////////////////////////////////////////////////////////////
	intsindex = 0;
	// number of generatedweakclass(feature)
	for(int ii=0;ii<m_total_weakclass;ii++)
	{
		double* min1; double* min2;
		double e;
		memcpy(eachlabels, labels, sizeof(int)*(m_nPositiveImg+m_nNegativeImg));
		memcpy(eachweight, weights, sizeof(double)*(m_nPositiveImg+m_nNegativeImg));
		intcntAllexample= 0;
		for(int j=0;j<m_nPositiveImg;j++){
			weakFeatureValue[j] = GetOneFeature(m_ImgPositive[j].IntegrailImage,0,0,&weakClass[ii]);
			cntAllexample++;
		}
		for( j=cntAllexample;j<m_nPositiveImg + cntAllexample;j++){
			weakFeatureValue[j] = GetOneFeature(m_ImgNegative[j].IntegrailImage,0,0,&weakClass[ii]);
		}
		memcpy(value,weakFeatureValue,sizeof(double)*(m_nPositiveImg+m_nNegativeImg));
		QuickSort(weakFeatureValue,
			eachlabels,
			eachweight,0,(m_nPositiveImg+m_nNegativeImg)-1 );
		// computeerrors1, suppose parityis 1, thatis f(x)<thresh ==> h(x)= 1
		e= 0.0;
		for(inti=0; i<(m_nPositiveImg+m_nNegativeImg); i++){
			if( eachlabels[i]== 1 ) // positiveex
				e += eachweight[i];
		}
		errors1[0] = e;
		for(i=1;i<(m_nPositiveImg+m_nNegativeImg);i++){
			errors1[i] = errors1[i-1];
			if(eachlabels[i-1] == 1)
				errors1[i] -= eachweight[i-1];
			//errors1[i] -= w_s[i];
			else
				errors1[i] += eachweight[i-1];
			//errors1[i] += w_s[i];
			if(errors1[i] < 0.000000000001){
				// skip this one
				errors1[i] = errors1[i-1];
			}
		}
		// computeerrors2, suppose parityis 0, thatis f(x)>thresh ==> h(x)= 1
		e= 0.0;
		for(i=0;i<(m_nPositiveImg+m_nNegativeImg);i++){
			if(eachlabels[i] == 0) //negative ex
				e += eachweight[i];
		}
		errors2[0] = e;
		for(i=1;i<(m_nPositiveImg+m_nNegativeImg);i++){
			errors2[i] = errors2[i-1];
			if(eachlabels[i-1] == 0)
				errors2[i] -= eachweight[i-1];
			//errors2[i] -= w_s[i];
			else
				errors2[i] += eachweight[i-1];
			//errors2[i] += w_s[i];
			if(errors2[i] < 0.000000000001){
				// skip this one
				errors2[i] = errors2[i-1];
			}
		}
		min1= std::min_element(errors1,errors1+(m_nPositiveImg+m_nNegativeImg));
		min2= std::min_element(errors2,errors2+(m_nPositiveImg+m_nNegativeImg));
		int pos1 = min1- errors1, pos2= min2- errors2;
		if( (*min1)<(*min2) ){
			weakClass[ii].parity = 1;
			weakClass[ii].error = (*min1);
			if(pos1==(m_nPositiveImg+m_nNegativeImg))
				weakClass[ii].thresh = weakFeatureValue[pos1] + (weakFeatureValue[pos1]-weakFeatureValue[pos1-1])/2.0;
			else if(pos1==0)
				weakClass[ii].thresh = weakFeatureValue[pos1] - (weakFeatureValue[pos1+1]-weakFeatureValue[pos1])/2.0;
			else
				weakClass[ii].thresh = weakFeatureValue[pos1] - (weakFeatureValue[pos1]-weakFeatureValue[pos1-1])/2.0;
			int cntTemp1=0; //c
			for(int a=0;a<m_nPositiveImg;a++){
				if(value[a]<weakClass[ii].thresh){
					cntTemp1++;
				}
			}
			weakClass[ii].detection_rate = (double)cntTemp1/(double)(m_nPositiveImg);
			cntTemp1=0;
			for(a=m_nPositiveImg;a<(m_nPositiveImg+m_nNegativeImg);a++){
				if(value[a]<weakClass[ii].thresh){
					cntTemp1++;
				}
			}
			weakClass[ii].false_positive_rate = (double)cntTemp1/(double)(m_nNegativeImg);
		}else{
			weakClass[ii].parity = -1;
			weakClass[ii].error = (*min2);
			if(pos2==(m_nPositiveImg+m_nNegativeImg))
				weakClass[ii].thresh = weakFeatureValue[pos2] + (weakFeatureValue[pos2]-weakFeatureValue[pos2-1])/2.0;
			else if(pos1==0)
				weakClass[ii].thresh = weakFeatureValue[pos2] - (weakFeatureValue[pos2+1]-weakFeatureValue[pos2])/2.0;
			else
				weakClass[ii].thresh = weakFeatureValue[pos2] - (weakFeatureValue[pos2]-weakFeatureValue[pos2-1])/2.0;
			int c=0;
			for(int a=0;a<m_nPositiveImg;a++){
				if(value[a]>=weakClass[ii].thresh){
					c++;
				}
			}
			weakClass[ii].detection_rate = (double)c/(double)(m_nPositiveImg);
			c=0;
			for(a=m_nPositiveImg;a<(m_nPositiveImg+m_nNegativeImg);a++){
				if(value[a]>=weakClass[ii].thresh){
					c++;
				}
			}
			weakClass[ii].false_positive_rate =(double)c/(double)(m_nNegativeImg);
		}
		if((weakClass[ii].error < minerror)&&(weakClass[ii].reference_count< REFCOUNT)){
			minerror = weakClass[ii].error;
			/////////////////////////////////////////////////////////////////////////////////
			minsc = weakClass[ii];
			/////////////////////////////////////////////////////////////////////////////////
			sindex = ii;
		}
		strTemp.Format("%d/%d(total) WeakClassifier hasbeen processed forallexamples", ii,m_total_weakclass);
		m_Log.AppendString(strTemp);
		if(m_bStopTheAdaboost){
			return;
		}
		inttemporaryvalue = 0;

	}
	
	weakClass[sindex].reference_count++;
	double beta;
	beta= minsc.error /(1.0-minsc.error);
	// updatetheweights
	int cntAllexample = 0;
	int tempValue = 0;
	for(int j=0;j<m_nPositiveImg;j++){
		double v;
		v= GetOneFeature(m_ImgPositive[j].IntegrailImage,0,0,&minsc);
		if(minsc.parity*v<minsc.parity*minsc.thresh){
			tempValue = 1;
		}else{
			tempValue = 0;
		}
		if(tempValue== m_ImgPositive[j].Label)
			m_ImgPositive[j].weight *= beta;
		cntAllexample++;
	}
	for( j=cntAllexample;j<m_nPositiveImg + cntAllexample;j++){
		double v;
		v= GetOneFeature(m_ImgNegative[j].IntegrailImage,0,0,&minsc);
		if(minsc.parity*v<minsc.parity*minsc.thresh){
			tempValue = 1;
		}else{
			tempValue = 0;
		}
		if(tempValue== m_ImgNegative[j].Label)
			m_ImgNegative[j].weight *= beta;
	}
	minsc.alpha=-log(beta);
	delete[] errors1; errors1 = NULL;
	delete[] errors2; errors2 = NULL;
	delete[] eachlabels;
	delete[] eachweight;
	delete[] labels;
	delete[] weights;
	delete[] weakFeatureValue;
	//return minsc;
	SingleMinStrongClass = minsc;
}



// Init and begin Trainimages
voidCCVFrameDlg::OnInitTrainImg()
{
	m_bStopTheAdaboost = FALSE;
	InitPossibleFeatures();
	LoadPrositiveExamples();
	LoadNegativeExamples();
	InitPositiveImage();
	InitNegativeImage();
	BeginCascadeClassifier();
	return;
}