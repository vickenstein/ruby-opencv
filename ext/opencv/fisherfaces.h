/************************************************************

   fisherfaces.h

   $Author: ser1zw $

   Copyright (C) 2013 ser1zw

************************************************************/
#ifndef RUBY_OPENCV_FISHERFACES_H
#define RUBY_OPENCV_FISHERFACES_H

#include "opencv.h"

#define __NAMESPACE_BEGIN_FISHERFACES namespace cFisherFaces {
#define __NAMESPACE_END_FISHERFACES }

__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_FISHERFACES

VALUE rb_class();

void define_ruby_class();
VALUE rb_initialize(int argc, VALUE argv[], VALUE self);

__NAMESPACE_END_FISHERFACES
__NAMESPACE_END_OPENCV

#endif // RUBY_OPENCV_FISHERFACES_H

