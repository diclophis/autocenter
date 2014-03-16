#include <stdio.h>
#include <stdlib.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>


#define w 853
#define h 803
int levels = 3;
CvSeq* contours = 0;

static void on_trackbar(int pos)
{
    IplImage* cnt_img = cvCreateImage( cvSize(w,h), 8, 3 );
    CvSeq* _contours = contours;
    int _levels = levels - 3;
    (void)pos;

    if( _levels <= 0 ) // get to the nearest face to make it look more funny
        _contours = _contours->h_next->h_next->h_next;
    cvZero( cnt_img );
    cvDrawContours( cnt_img, _contours, CV_RGB(255,0,0), CV_RGB(0,255,0), _levels, 1, CV_AA, cvPoint(0,0) );
    cvShowImage( "contours", cnt_img );
    cvReleaseImage( &cnt_img );
}

static void findCComp( IplImage* img )
{
    int x, y, cidx = 1;
    IplImage* mask = cvCreateImage( cvSize(img->width+2, img->height+2), 8, 1 );
    cvZero(mask);
    cvRectangle( mask, cvPoint(0, 0), cvPoint(mask->width-1, mask->height-1),
                 cvScalarAll(1), 1, 8, 0 );

    for( y = 0; y < img->height; y++ )
        for( x = 0; x < img->width; x++ )
        {
            if( CV_IMAGE_ELEM(mask, uchar, y+1, x+1) != 0 )
                continue;
            cvFloodFill(img, cvPoint(x,y), cvScalarAll(cidx),
                        cvScalarAll(0), cvScalarAll(0), 0, 4, mask);
            cidx++;
        }
}


int main(int argc, char* argv[])
{
  CvMemStorage* storage = cvCreateMemStorage(0);
  IplImage* img_src = NULL;

  img_src=cvLoadImage(argv[1], 1);
  if(!img_src) printf("Could not load image file: %s\n", argv[1]);

  cvNamedWindow( "image", 1 );
  cvNamedWindow( "colored", 1 );

  cvShowImage( "image", img_src );

  CvSize imgSize = cvGetSize(img_src);

  IplImage* img = cvCreateImage( imgSize, img_src->depth, 1 );

  cvCvtColor( img_src, img, CV_BGR2GRAY );
  
  IplImage* img32f = cvCreateImage(imgSize, IPL_DEPTH_32F, 1 );
  IplImage* img32s = cvCreateImage( imgSize, IPL_DEPTH_32S, 1 );
  IplImage* img3 = cvCreateImage( imgSize, 8, 3 );
  cvConvert( img, img32f );
  findCComp( img32f );
  cvConvert( img32f, img32s );

  
  cvFindContours(img32s, storage, &contours, sizeof(CvContour),
                 CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0) );


//  cvFindContours(img32s, storage, &contours, sizeof(CvContour),
//                 CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0) );


  // comment this out if you do not want approximation
  if (contours != NULL) {
    contours = cvApproxPoly( contours, sizeof(CvContour), storage, CV_POLY_APPROX_DP, 3, 1 );

    cvNamedWindow("contours", 1 );
    cvCreateTrackbar("levels+3", "contours", &levels, 10, on_trackbar );
    {
        CvRNG rng = cvRNG(-1);

        CvSeq* tcontours = contours;
        cvCvtColor( img, img3, CV_GRAY2BGR );
        while( tcontours->h_next )
            tcontours = tcontours->h_next;

        for( ; tcontours != 0; tcontours = tcontours->h_prev )
        {
            CvScalar color;
            color.val[0] = cvRandInt(&rng) % 256;
            color.val[1] = cvRandInt(&rng) % 256;
            color.val[2] = cvRandInt(&rng) % 256;
            color.val[3] = cvRandInt(&rng) % 256;
            cvDrawContours(img3, tcontours, color, color, 0, -1, 1, cvPoint(0,0));
            if( tcontours->v_next )
            {
                color.val[0] = cvRandInt(&rng) % 256;
                color.val[1] = cvRandInt(&rng) % 256;
                color.val[2] = cvRandInt(&rng) % 256;
                color.val[3] = cvRandInt(&rng) % 256;
                cvDrawContours(img3, tcontours->v_next, color, color, 1, -1, 1, cvPoint(0,0));
            }
        }
    }

    cvShowImage( "colored", img3 );
    on_trackbar(0);
  }
  
  cvWaitKey(0);
  cvReleaseMemStorage( &storage );
  cvReleaseImage( &img );
  cvReleaseImage( &img32f );
  cvReleaseImage( &img32s );
  cvReleaseImage( &img3 );

  return 0;
}