/************************************************************

   facerecognizer.cpp -

   $Author: ser1zw $

   Copyright (C) 2013 ser1zw

************************************************************/
#include <stdio.h>
#include "facerecognizer.h"
/*
 * Document-class: OpenCV::FaceRecognizer
 *
 */
__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_FACERECOGNIZER

VALUE rb_klass;

VALUE
rb_class()
{
  return rb_klass;
}

/*
 * call-seq:
 *   train(src, labels)
 *
 * Trains a FaceRecognizer with given data and associated labels.
 */
VALUE
rb_train(VALUE self, VALUE src, VALUE labels)
{
  Check_Type(src, T_ARRAY);
  Check_Type(labels, T_ARRAY);

  VALUE *src_ptr = RARRAY_PTR(src);
  int src_size = RARRAY_LEN(src);
  std::vector<cv::Mat> images;
  for (int i = 0; i < src_size; i++) {
    images.push_back(cv::Mat(CVMAT_WITH_CHECK(src_ptr[i])));
  }

  VALUE *labels_ptr = RARRAY_PTR(labels);
  int labels_size = RARRAY_LEN(labels);
  std::vector<int> local_labels;
  for (int i = 0; i < labels_size; i++) {
    local_labels.push_back(NUM2INT(labels_ptr[i]));
  }

  cv::FaceRecognizer *self_ptr = FACERECOGNIZER(self);
  try {
    self_ptr->train(images, local_labels);
  }
  catch (cv::Exception& e) {
    raise_cverror(e);
  }

  return Qnil;
}

/*
 * call-seq:
 *   predict(src)
 * 
 * Predicts a label and associated confidence (e.g. distance) for a given input image.
 */
VALUE
rb_predict(VALUE self, VALUE src)
{
  cv::Mat mat = cv::Mat(CVMAT_WITH_CHECK(src));
  cv::FaceRecognizer *self_ptr = FACERECOGNIZER(self);
  int label;
  try {
    label = self_ptr->predict(mat);
  }
  catch (cv::Exception& e) {
    raise_cverror(e);
  }

  return INT2NUM(label);
}

/*
 * call-seq:
 *   save(filename)
 *
 * Saves this model to a given filename, either as XML or YAML.
 */
VALUE
rb_save(VALUE self, VALUE filename)
{
  Check_Type(filename, T_STRING);
  cv::FaceRecognizer *self_ptr = FACERECOGNIZER(self);
  try {
    char* s = StringValueCStr(filename);
    self_ptr->save(std::string(s));
  }
  catch (cv::Exception& e) {
    raise_cverror(e);
  }

  return Qnil;
}

/*
 * call-seq:
 *   load(filename)
 *
 * Loads a FaceRecognizer and its model state.
 */
VALUE
rb_load(VALUE self, VALUE filename)
{
  Check_Type(filename, T_STRING);
  cv::FaceRecognizer *self_ptr = FACERECOGNIZER(self);
  try {
    char* s = StringValueCStr(filename);
    self_ptr->load(std::string(s));
  }
  catch (cv::Exception& e) {
    raise_cverror(e);
  }

  return Qnil;
}

void
define_ruby_class()
{
  if (rb_klass)
    return;
  /* 
   * opencv = rb_define_module("OpenCV");
   * 
   * note: this comment is used by rdoc.
   */
  VALUE opencv = rb_module_opencv();
  rb_klass = rb_define_class_under(opencv, "FaceRecognizer", rb_cObject);
  rb_define_method(rb_klass, "train", RUBY_METHOD_FUNC(rb_train), 2);
  rb_define_method(rb_klass, "predict", RUBY_METHOD_FUNC(rb_predict), 1);
  rb_define_method(rb_klass, "save", RUBY_METHOD_FUNC(rb_save), 1);
  rb_define_method(rb_klass, "load", RUBY_METHOD_FUNC(rb_load), 1);
}

__NAMESPACE_END_FACERECOGNIZER
__NAMESPACE_END_OPENCV
