/************************************************************

   facerecognizer.h

   $Author: ser1zw $

   Copyright (C) 2013 ser1zw

************************************************************/
#ifndef RUBY_OPENCV_FACERECOGNIZER_H
#define RUBY_OPENCV_FACERECOGNIZER_H

#include "opencv.h"

#define __NAMESPACE_BEGIN_FACERECOGNIZER namespace cFaceRecognizer {
#define __NAMESPACE_END_FACERECOGNIZER }

__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_FACERECOGNIZER

VALUE rb_class();

void define_ruby_class();
VALUE rb_train(VALUE self, VALUE src, VALUE labels);
VALUE rb_predict(VALUE self, VALUE src);
VALUE rb_save(VALUE self, VALUE filename);
VALUE rb_load(VALUE self, VALUE filename);

void guard_facerecognizer(void* data_ptr, cv::Ptr<cv::FaceRecognizer> ptr);
void release_facerecognizer(void *data_ptr);
VALUE allocate_facerecognizer(VALUE klass);

__NAMESPACE_END_FACERECOGNIZER

inline cv::FaceRecognizer*
FACERECOGNIZER(VALUE object)
{
  cv::FaceRecognizer *ptr;
  Data_Get_Struct(object, cv::FaceRecognizer, ptr);
  return ptr;
}

__NAMESPACE_END_OPENCV

#endif // RUBY_OPENCV_FACERECOGNIZER_H

