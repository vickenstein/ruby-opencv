/************************************************************

   lbph.cpp -

   $Author: ser1zw $

   Copyright (C) 2013 ser1zw

************************************************************/
#include <stdio.h>
#include "lbph.h"
/*
 * Document-class: OpenCV::LBPH
 *
 */
__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_LBPH

VALUE rb_klass;

VALUE
rb_class()
{
  return rb_klass;
}

/* 
 * call-seq:
 *   LBPH.new(radius=1, neighbors=8, grid_x=8, grid_y=8, threshold=DBL_MAX) -> cvmat
 */
VALUE
rb_initialize(int argc, VALUE argv[], VALUE self)
{
  VALUE radius_val, neighbors_val, grid_x_val, grid_y_val, threshold_val;
  rb_scan_args(argc, argv, "05", &radius_val, &neighbors_val, &grid_x_val, &grid_y_val, &threshold_val);

  int radius = NIL_P(radius_val) ? 1 : NUM2INT(radius_val);
  int neighbors = NIL_P(neighbors_val) ? 8 : NUM2INT(neighbors_val);
  int grid_x = NIL_P(grid_x_val) ? 8 : NUM2INT(grid_x_val);
  int grid_y = NIL_P(grid_y_val) ? 8 : NUM2INT(grid_y_val);
  double threshold = NIL_P(threshold_val) ? DBL_MAX : NUM2INT(threshold_val);

  free(DATA_PTR(self));
  cv::Ptr<cv::FaceRecognizer> ptr = cv::createLBPHFaceRecognizer(radius, neighbors, grid_x, grid_y, threshold);
  DATA_PTR(self) = ptr;

  cFaceRecognizer::guard_facerecognizer(DATA_PTR(self), ptr);

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
  rb_klass = rb_define_class_under(opencv, "LBPH", cFaceRecognizer::rb_class());
  rb_define_alloc_func(rb_klass, cFaceRecognizer::allocate_facerecognizer);
  rb_define_private_method(rb_klass, "initialize", RUBY_METHOD_FUNC(rb_initialize), -1);
}

__NAMESPACE_END_LBPH
__NAMESPACE_END_OPENCV

