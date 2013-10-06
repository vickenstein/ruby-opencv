/************************************************************

   lbph.h

   $Author: ser1zw $

   Copyright (C) 2013 ser1zw

************************************************************/
#ifndef RUBY_OPENCV_LBPH_H
#define RUBY_OPENCV_LBPH_H

#include "opencv.h"

#define __NAMESPACE_BEGIN_LBPH namespace cLBPH {
#define __NAMESPACE_END_LBPH }

__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_LBPH

VALUE rb_class();

void define_ruby_class();
VALUE rb_initialize(int argc, VALUE argv[], VALUE self);

__NAMESPACE_END_LBPH
__NAMESPACE_END_OPENCV

#endif // RUBY_OPENCV_LBPH_H

