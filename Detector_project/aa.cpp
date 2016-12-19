#include "cv.h"
#include "highgui.h"
#include <stdio.h>

void detect_and_draw_objects(
							 IplImage* image,
							 CvHaarClassifierCascade* cascade,
							 int do_pyramids ){
	
	IplImage* small_image = image;
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* faces;
	int i;
	double scale = 1;
	
	/* if the flag is specified, down-scale the input image to get a
	performance boost w/o loosing quality (perhaps) */
	
	
	/* use the fastest variant */
	faces = 
		cvHaarDetectObjects( 
        small_image,
        cascade,
        storage,
        1.2,
        2,
        CV_HAAR_DO_CANNY_PRUNING
		);
	
	/* draw all the rectangles */
	for( i = 0; i < faces->total; i++ )
	{
		/* extract the rectanlges only */
		CvRect face_rect = *(CvRect*)cvGetSeqElem( faces, i);
		cvRectangle( image, cvPoint(face_rect.x*scale,face_rect.y*scale),
			cvPoint((face_rect.x+face_rect.width)*scale,
			(face_rect.y+face_rect.height)*scale),
			CV_RGB(255,0,0), 2 );
	}
	
	if( small_image != image )
		cvReleaseImage( &small_image );
	cvReleaseMemStorage( &storage );
}


/* takes image filename and cascade path from the command line */
int main()
{	 
	char filename[100];
	char haarname[100];	
	char savename[100];	

	printf("haar train xml name?\n");
	scanf("%s",haarname);
	printf("%s\n",haarname);

		
	printf("Test File name?\n");
	scanf("%s",filename);
	printf("%s\n",filename);

	
	//파일 오픈 
	IplImage* image;
	image = cvLoadImage( filename, 1 );	
	//////////////////////////////////////////////////////////////////////////
	IplImage *dst_image;
	dst_image = cvCloneImage(image);
		
	if(image==0)
	{
		printf("파일없음");
		return 0;
	}
	
	CvHaarClassifierCascade* cascade = (CvHaarClassifierCascade*)cvLoad( haarname );
	detect_and_draw_objects( dst_image, cascade, 1 );
	
	sprintf(savename,"./save_%s.jpg",filename);
	printf("save = %s\n", savename);
	cvSaveImage(savename, dst_image);
	
	cvNamedWindow( "test", 0 );
	cvShowImage( "test", dst_image );
	cvWaitKey(0);
	cvReleaseHaarClassifierCascade( &cascade );
	cvReleaseImage( &image );		
	cvReleaseImage( &dst_image );	
	
	


	return 0;
} 
