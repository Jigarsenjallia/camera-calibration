#include<cv.h>
#include<highgui.h>
#include<iostream>
#include<string>

using namespace std;

  //global variables
 int numberofboards=3;
 int width=3, height=3;
 int squarenumbers=width*height;
	//count the number of detected corners inside the chess boards
	int corner_count=0;


//matrix to store image x,y cordinate of the squares projected in camera views.
        CvMat* image_points =cvCreateMat( numberofboards*squarenumbers, 2, CV_32FC1 );
//matix to store object x,y,z cordiante of the squares in the model cordiante space
        CvMat* object_points = cvCreateMat( numberofboards*squarenumbers, 3, CV_32FC1 );

        //matrix to store the count of corner points in the images
        CvMat* point_counts	= cvCreateMat( numberofboards, 1, CV_32SC1 );
//matrix to store intrinsic parameters
        CvMat* intrinsic_matrix	= cvCreateMat( 3, 3, CV_32FC1 );
		CvMat* distortion_coeffs	= cvCreateMat( 4, 1,CV_32FC1 );

	//cvNamedWindow( "Calibration2" );
	//cvNamedWindow( "Calibration3" );
//size of the selected squares.
	CvSize boardsize = cvSize(width,height ); //size of the image
//matrix to store 2D corner points.
	CvPoint2D32f *corners = new CvPoint2D32f [squarenumbers];

	IplImage * image;
	int i=0;
	IplImage *img =0;

	void webcamimage()
	{
		     
//capture the image from the webcam
				CvCapture * capture =cvCaptureFromCAM(0);


				if(!cvGrabFrame(capture))
				{
				cout<<"could not grab a frame"<<endl;
				cout<<"error cannot capture";
				}
				//store the image in the datatype
				img=cvRetrieveFrame(capture);

				string filename="grab.jpg";

				//save the image.
				if(!cvSaveImage(filename.c_str(),img))
				{
				cout<<"could not save the imgae";

				}

			//img=cvLoadImage("grab10.jpg",0);
			//function to show the image.
			  cvShowImage( "Calibration1", img);

			  int x =cvWaitKey();

			      cvReleaseCapture(&capture);

	}



	void findcorners()
	{	image = cvCreateImage(cvGetSize(img),IPL_DEPTH_8U, 1 );
		//IplImage *image = cvCreateImage(boardsize,IPL_DEPTH_8U, 1 );
		//find chess board corners i.e. black and white cross-section.
		int cornerdata= cvFindChessboardCorners(image,boardsize,corners,
			&corner_count, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS | CV_CALIB_CB_NORMALIZE_IMAGE);
		if(cornerdata)
		{
		cvDrawChessboardCorners(image, boardsize, corners, corner_count, cornerdata );
		//cvShowImage("calibration2", img);
		cout<<"corenrs detected"<<endl;
		}
		else
		{
			cout<<"corners not detected"<<endl;
		}
		// assign put the data of objects and images of all the boards
		for(int j=0;j<squarenumbers;j++)
		{
			                CV_MAT_ELEM( *image_points, float, i, 0 ) = corners[j].x;
							CV_MAT_ELEM( *image_points, float, i, 1 ) = corners[j].y;
							CV_MAT_ELEM( *object_points, float, i, 0 ) = j/width;
							CV_MAT_ELEM( *object_points, float, i, 1 ) = j%width;
							CV_MAT_ELEM( *object_points, float, i, 2 ) = 0.0f;

							cout<<corners[j].x<<endl;
		}


		CV_MAT_ELEM( *point_counts, int, i, 0 ) = squarenumbers;


	}


	void calibratecamera()
	{
		//intialize the intrinsic matrix
		    CV_MAT_ELEM( *intrinsic_matrix, float, 0, 0 ) = 1.0;
			CV_MAT_ELEM( *intrinsic_matrix, float, 1, 1 ) = 1.0;

		     cvCalibrateCamera2( object_points, image_points, point_counts, cvGetSize(image),
			intrinsic_matrix, distortion_coeffs, NULL, NULL, CV_CALIB_FIX_ASPECT_RATIO );

		int t;
		//print the intrinsic parameters
		     for(int r=0;r<intrinsic_matrix->rows;r++)
			for(int c=0;c<intrinsic_matrix->cols;c++)
		     {
				t = cvmGet(intrinsic_matrix,r,c);
				cout<<t<<endl;
		     }

	}





int main(int argc, char **argv)
{

	int n= atoi(argv[1]);
	cout<<n<<endl;
	numberofboards=n;
	cvNamedWindow( "Calibration1" );
//this is the loop for different board image.

	for (i=0;i<numberofboards;i++)
				{
		webcamimage();
		findcorners();

	} //end of for loop.
	cvReleaseImage(&img);
	calibratecamera();
        IplImage* map1 = cvCreateImage( cvGetSize( image ), IPL_DEPTH_32F, 1 );
     	IplImage* map2 = cvCreateImage( cvGetSize( image ), IPL_DEPTH_32F, 1 );
     	cvInitUndistortMap( intrinsic_matrix, distortion_coeffs, map1, map2 );
     	IplImage* temp;
     	temp = cvCloneImage( image );
     	cvRemap(temp, image, map1, map2); // undistort image

     	// Run the camera to the screen, now showing the raw and undistorted image
     	cvNamedWindow( "Undistort" );


cvReleaseImage(&temp);
cvReleaseImage(&image);

return 0;
}








/*
how to run this program through the terminal.
g++ calibration.cpp -I /usr/include/opencv -L /usr/lib -lm -lcv -lhighgui -lcvaux

or
g++ `pkg-config --cflags opencv` calibration.cpp  `pkg-config --libs opencv`


*/
