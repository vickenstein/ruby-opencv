/************************************************************

   eigenfaces.cpp -

   $Author: ser1zw $

   Copyright (C) 2013 ser1zw

************************************************************/
#include <stdio.h>
#include "eigenfaces.h"
/*
 * Document-class: OpenCV::EigenFaces
 *
 */
__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_EIGENFACES

VALUE rb_klass;
std::map<long, cv::Ptr<cv::FaceRecognizer> > ptr_guard_map;

VALUE
rb_class()
{
  return rb_klass;
}

void
release_facerecognizer(void *ptr) {
  long key = (long)ptr;
  ptr_guard_map[key].release();
  ptr_guard_map.erase(key);
}

VALUE
rb_allocate(VALUE klass)
{
  return Data_Wrap_Struct(klass, 0, release_facerecognizer, NULL);
}

/* 
 * call-seq:
 *   EigenFaces.new(num_components=0, threshold=DBL_MAX)
 */
VALUE
rb_initialize(int argc, VALUE argv[], VALUE self)
{
  VALUE num_components_val, threshold_val;
  rb_scan_args(argc, argv, "02", &num_components_val, &threshold_val);

  int num_components = NIL_P(num_components_val) ? 0 : NUM2INT(num_components_val);
  double threshold = NIL_P(threshold_val) ? DBL_MAX : NUM2DBL(threshold_val);

  free(DATA_PTR(self));
  cv::Ptr<cv::FaceRecognizer> ptr = cv::createEigenFaceRecognizer(num_components, threshold);
  DATA_PTR(self) = ptr;

  long key = (long)(DATA_PTR(self));
  ptr_guard_map[key] = ptr; // To avoid cv::Ptr's GC

  return self;
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
  rb_klass = rb_define_class_under(opencv, "EigenFaces", rb_cObject);
  rb_define_alloc_func(rb_klass, rb_allocate);
  rb_define_private_method(rb_klass, "initialize", RUBY_METHOD_FUNC(rb_initialize), -1);
  rb_define_method(rb_klass, "train", RUBY_METHOD_FUNC(rb_train), 2);
  rb_define_method(rb_klass, "predict", RUBY_METHOD_FUNC(rb_predict), 1);
  rb_define_method(rb_klass, "save", RUBY_METHOD_FUNC(rb_save), 1);
  rb_define_method(rb_klass, "load", RUBY_METHOD_FUNC(rb_load), 1);
}

__NAMESPACE_END_EIGENFACES
__NAMESPACE_END_OPENCV

