#include <stdio.h>
#include <stdlib.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>


int main( int argc, char** argv )
{
  /// Load source image and convert it to gray
  //src = imread(argv[1], 1);
  IplImage* src=0; 
  src=cvLoadImage(argv[1], 1);
  if(!src) printf("Could not load image file: %s\n", argv[1]);

  /// Convert image to gray and blur it
  //cvtColor( src, src_gray, CV_BGR2GRAY );
  //blur( src_gray, src_gray, Size(3,3) );

  /// Create Window
  char* source_window = "Source";
  cvNamedWindow( source_window, CV_WINDOW_AUTOSIZE );
  cvShowImage( source_window, src );

  //createTrackbar( " Threshold:", "Source", &thresh, max_thresh, thresh_callback );
  //thresh_callback( 0, 0 );

  cvWaitKey(0);
  return(0);
}
