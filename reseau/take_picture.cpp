#include "opencv/highgui.h"
#include "opencv/cv.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;
using namespace cv;

#define IMAGE_SIZE 100

struct stat st = {0};

int nb_images = 0;

Mat detect_faces(Mat frame_origin, CascadeClassifier face_cascade, CascadeClassifier eyes_cascade);


int main(int argc, char const *argv[]) {

  printf("------------ Face Key -----------\n");
  printf("|\tPress ESCAP to finish\t|\n");
  printf("---------------------------------\n");

  VideoCapture capture;
  Mat frame;

  //Create and load the face and eyes classifier
  CascadeClassifier face_cascade;
  CascadeClassifier eyes_cascade;

  if(!face_cascade.load("./file/haarcascade_frontalface_alt.xml")){
    printf("\tError loading face cascade !\n");
    return -1;
  };
  if(!eyes_cascade.load("./file/haarcascade_eye_tree_eyeglasses.xml")){
    printf("\tError loading eyes cascade !\n");
    return -1;
  };

  //Open the video stream
  capture.open(-1);
  if (!capture.isOpened()){
    printf("\tError opening video capture !\n");
    return -1;
  }

  //Create and setup the windows
  namedWindow("Faces detection", WINDOW_NORMAL);
  moveWindow("Faces detection", 710, 0);
  resizeWindow("Faces detection", 640, 480);

  //Begin the image processing of the video stream
  while (capture.read(frame)){
    if(frame.empty()){
      printf("\tError while reading the video stream !");
      break;
    }

    //Detect the faces and the eyes using the 2 classifier
    Mat face_detected = detect_faces(frame, face_cascade, eyes_cascade);

    imshow("Faces detection", face_detected);

    int c = waitKey(10);
    if((char)c == 27)  //escape
      break;
  }

  destroyAllWindows();

  return nb_images;
}

/*
This fonction detect faces and eyes on frame_origin using face and eyes classifier
It return a image with the faces and eyes surrounded
*/
Mat detect_faces(Mat frame_origin, CascadeClassifier face_cascade, CascadeClassifier eyes_cascade){

  Mat frame ;
  frame_origin.copyTo(frame);

  std::vector<Rect> faces;
  Mat frame_gray;

  cvtColor(frame_origin, frame_gray, COLOR_BGR2GRAY);
  equalizeHist(frame_gray, frame_gray);

  //Face detection
  face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30));

  int i, j;
  for(i = 0; i < faces.size(); i++){

    Point face_center(faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2);

    //Save the image of the face in the folder in parameter
    Mat face_saved, face_saved_resized;
    frame_origin(faces[i]).copyTo(face_saved);
    resize(face_saved, face_saved_resized, cvSize(IMAGE_SIZE, IMAGE_SIZE));
    String file_name = "./client_x/image/";
    stringstream ss;
    ss << ++nb_images;
    file_name += ss.str();
    file_name += ".jpg";
    imwrite(file_name, face_saved_resized);


    ellipse(frame, face_center, Size(faces[i].width/2, faces[i].height/2), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);

    //Eyes detection
    Mat faceROI = frame_gray(faces[i]);
    std::vector<Rect> eyes;
    eyes_cascade.detectMultiScale(faceROI, eyes, 1.1, 2, 0 |CASCADE_SCALE_IMAGE, Size(30, 30));

    for(j = 0; j < eyes.size(); j++){
      Point eye_center(faces[i].x + eyes[j].x + eyes[j].width/2, faces[i].y + eyes[j].y + eyes[j].height/2);
      int radius = cvRound((eyes[j].width + eyes[j].height)*0.25);
      circle(frame, eye_center, radius, Scalar(255, 0, 0), 4, 8, 0);
    }
  }

  return frame;
}
