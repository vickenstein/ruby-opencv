/************************************************************

   eigenfacerecognizer.h

   $Author: ser1zw $

   Copyright (C) 2013 ser1zw

************************************************************/
#ifndef RUBY_OPENCV_EIGENFACES_H
#define RUBY_OPENCV_EIGENFACES_H

#include "opencv.h"

#define __NAMESPACE_BEGIN_EIGENFACES namespace cEigenFaces {
#define __NAMESPACE_END_EIGENFACES }

__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_EIGENFACES

VALUE rb_class();

void define_ruby_class();
VALUE rb_allocate(VALUE klass);
VALUE rb_initialize(int argc, VALUE argv[], VALUE self);
VALUE rb_train(VALUE self, VALUE src, VALUE labels);

__NAMESPACE_END_EIGENFACES

inline cv::FaceRecognizer*
FACERECOGNIZER(VALUE object)
{
  cv::FaceRecognizer *ptr;
  Data_Get_Struct(object, cv::FaceRecognizer, ptr);
  return ptr;
}

__NAMESPACE_END_OPENCV

#endif // RUBY_OPENCV_EIGENFACES_H


