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
  rb_klass = rb_define_class_under(opencv, "EigenFaces", cFaceRecognizer::rb_class());
  rb_define_alloc_func(rb_klass, rb_allocate);
  rb_define_private_method(rb_klass, "initialize", RUBY_METHOD_FUNC(rb_initialize), -1);
}

__NAMESPACE_END_EIGENFACES
__NAMESPACE_END_OPENCV

