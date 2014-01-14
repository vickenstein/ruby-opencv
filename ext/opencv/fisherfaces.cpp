/************************************************************

   fisherfaces.cpp -

   $Author: ser1zw $

   Copyright (C) 2013 ser1zw

************************************************************/
#include <stdio.h>
#include "fisherfaces.h"
/*
 * Document-class: OpenCV::FisherFaces
 *
 */
__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_FISHERFACES

VALUE rb_klass;

VALUE
rb_class()
{
  return rb_klass;
}

/* 
 * call-seq:
 *   FisherFaces.new(num_components=0, threshold=DBL_MAX)
 */
VALUE
rb_initialize(int argc, VALUE argv[], VALUE self)
{
  VALUE num_components_val, threshold_val;
  rb_scan_args(argc, argv, "02", &num_components_val, &threshold_val);

  int num_components = NIL_P(num_components_val) ? 0 : NUM2INT(num_components_val);
  double threshold = NIL_P(threshold_val) ? DBL_MAX : NUM2DBL(threshold_val);

  free(DATA_PTR(self));
  cv::Ptr<cv::FaceRecognizer> ptr = cv::createFisherFaceRecognizer(num_components, threshold);
  DATA_PTR(self) = ptr;

  cFaceRecognizer::guard_facerecognizer(DATA_PTR(self), ptr);

  return self;
}

void
init_ruby_class()
{
  if (rb_klass)
    return;
  /* 
   * opencv = rb_define_module("OpenCV");
   * 
   * note: this comment is used by rdoc.
   */
  VALUE opencv = rb_module_opencv();
  rb_klass = rb_define_class_under(opencv, "FisherFaces", cFaceRecognizer::rb_class());
  rb_define_alloc_func(rb_klass, cFaceRecognizer::allocate_facerecognizer);
  rb_define_private_method(rb_klass, "initialize", RUBY_METHOD_FUNC(rb_initialize), -1);
}

__NAMESPACE_END_FISHERFACES
__NAMESPACE_END_OPENCV

