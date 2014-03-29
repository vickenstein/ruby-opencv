/************************************************************

   cvline.cpp -

   $Author: lsxi $

   Copyright (C) 2005-2006 Masakazu Yonekura

************************************************************/
#include "cvline.h"
/*
 * Document-class: OpenCV::CvLine
 *
 * Line parameters represented by a two-element (rho, theta)
 * for CvMat#hough_lines
 */
__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVLINE

VALUE rb_klass;

VALUE
rb_class()
{
  return rb_klass;
}

VALUE
rb_allocate(VALUE klass)
{
  CvLine *ptr;
  return Data_Make_Struct(klass, CvLine, 0, -1, ptr);
}

/*
 * Returns distance from the coordinate origin (0, 0)
 * @overload rho
 * @return [Number] Distance from the coordinate origin
 */
VALUE
rb_rho(VALUE self)
{
  return rb_float_new(CVLINE(self)->rho);
}

/*
 * Set distance from the coordinate origin (0, 0)
 * @overload rho=(value)
 * @param value [Number] Distance from the coordinate origin
 */
VALUE
rb_set_rho(VALUE self, VALUE rho)
{
  CVLINE(self)->rho = NUM2DBL(rho);
  return self;
}

/*
 * Returns line rotation angle in radians
 * @overload theta
 * @return [Number] Line rotation angle in radians
 */
VALUE
rb_theta(VALUE self)
{
  return rb_float_new(CVLINE(self)->theta);
}

/*
 * Set line rotation angle in radians
 * @overload theta=(value)
 * @param value [Number] Line rotation angle
 */
VALUE
rb_set_theta(VALUE self, VALUE theta)
{
  CVLINE(self)->theta = NUM2DBL(theta);
  return self;
}

/*
 * Returns value of rho, theta
 * @overload [](index)
 * @param index [Integer] Index
 * @return [Number] If index = 0, returns rho, else if index = 1, returns theta.
 */
VALUE
rb_aref(VALUE self, VALUE index)
{
  switch (NUM2INT(index)) {
  case 0:
    return DBL2NUM(CVLINE(self)->rho);
    break;
  case 1:
    return DBL2NUM(CVLINE(self)->theta);
    break;
  default:
    rb_raise(rb_eIndexError, "index should be 0...2");
    break;
  }
  return Qnil;
}

/*
 * Set value of rho, theta
 * @overload []=(index, value)
 * @param index [Integer] Index
 * @param value [Number] Value
 * @return [Number] If index = 0, set rho, else if index = 1, set theta.
 */
VALUE
rb_aset(VALUE self, VALUE index, VALUE value)
{
  switch (NUM2INT(index)) {
  case 0:
    CVLINE(self)->rho = NUM2DBL(value);
    break;
  case 1:
    CVLINE(self)->theta = NUM2DBL(value);
    break;
  default:
    rb_raise(rb_eIndexError, "index should be 0...2");
    break;
  }
  return value;
}

VALUE
new_object(CvLine line)
{
  VALUE object = rb_allocate(rb_klass);
  *CVLINE(object) = line;
  return object;
}

void
init_ruby_class()
{
#if 0
  // For documentation using YARD
  VALUE opencv = rb_define_module("OpenCV");
#endif

  if (rb_klass)
    return;
  VALUE opencv = rb_module_opencv();

  rb_klass = rb_define_class_under(opencv, "CvLine", rb_cObject);
  rb_define_alloc_func(rb_klass, rb_allocate);
  rb_define_method(rb_klass, "rho", RUBY_METHOD_FUNC(rb_rho), 0);
  rb_define_method(rb_klass, "rho=", RUBY_METHOD_FUNC(rb_set_rho), 1);
  rb_define_method(rb_klass, "theta", RUBY_METHOD_FUNC(rb_theta), 0);
  rb_define_method(rb_klass, "theta=", RUBY_METHOD_FUNC(rb_set_theta), 1);
  rb_define_method(rb_klass, "[]", RUBY_METHOD_FUNC(rb_aref), 1);
  rb_define_method(rb_klass, "[]=", RUBY_METHOD_FUNC(rb_aset), 2);
}

__NAMESPACE_END_CVLINE
__NAMESPACE_END_OPENCV
