/************************************************************

   algorithm.cpp -

   $Author: ser1zw $

   Copyright (C) 2013 ser1zw

************************************************************/
#include <stdio.h>
#include "algorithm.h"
/*
 * Document-class: OpenCV::Algorithm
 *
 */
__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_ALGORITHM

VALUE rb_klass;

VALUE
rb_class()
{
  return rb_klass;
}

VALUE
rb_set_int(VALUE self, VALUE parameter, VALUE value)
{
  Check_Type(parameter, T_STRING);
  try {
    ALGORITHM(self)->setInt(StringValueCStr(parameter), NUM2INT(value));
  }
  catch (cv::Exception& e) {
    raise_cverror(e);
  }

  return Qnil;
}

VALUE
rb_set_double(VALUE self, VALUE parameter, VALUE value)
{
  Check_Type(parameter, T_STRING);
  try {
    ALGORITHM(self)->setDouble(StringValueCStr(parameter), NUM2DBL(value));
  }
  catch (cv::Exception& e) {
    raise_cverror(e);
  }

  return Qnil;
}

VALUE
rb_set_bool(VALUE self, VALUE parameter, VALUE value)
{
  Check_Type(parameter, T_STRING);
  try {
    bool val = TRUE_OR_FALSE(value) ? true : false;
    ALGORITHM(self)->setBool(StringValueCStr(parameter), val);
  }
  catch (cv::Exception& e) {
    raise_cverror(e);
  }

  return Qnil;
}

VALUE
rb_set_string(VALUE self, VALUE parameter, VALUE value)
{
  Check_Type(parameter, T_STRING);
  Check_Type(value, T_STRING);
  try {
    ALGORITHM(self)->setString(StringValueCStr(parameter), StringValueCStr(value));
  }
  catch (cv::Exception& e) {
    raise_cverror(e);
  }

  return Qnil;
}

VALUE
rb_set_mat(VALUE self, VALUE parameter, VALUE value)
{
  Check_Type(parameter, T_STRING);
  try {
    CvMat* val = CVMAT_WITH_CHECK(value);
    cv::Mat mat(val);
    ALGORITHM(self)->setMat(StringValueCStr(parameter), mat);
  }
  catch (cv::Exception& e) {
    raise_cverror(e);
  }

  return Qnil;
}

VALUE
rb_set_matvector(VALUE self, VALUE parameter, VALUE value)
{
  Check_Type(parameter, T_STRING);
  Check_Type(value, T_ARRAY);
  try {
    long len = RARRAY_LEN(value);
    VALUE* value_ptr = RARRAY_PTR(value);
    std::vector<cv::Mat> mat_vector;
    for (int i = 0; i < len; i++) {
      CvMat* val = CVMAT_WITH_CHECK(value_ptr[i]);
      cv::Mat mat(val);
      mat_vector.push_back(mat);
    }
    ALGORITHM(self)->setMatVector(StringValueCStr(parameter), mat_vector);
  }
  catch (cv::Exception& e) {
    raise_cverror(e);
  }

  return Qnil;
}

VALUE
rb_set_algorithm(VALUE self, VALUE parameter, VALUE value)
{
  Check_Type(parameter, T_STRING);
  try {
    ALGORITHM(self)->setAlgorithm(StringValueCStr(parameter), ALGORITHM(value));
  }
  catch (cv::Exception& e) {
    raise_cverror(e);
  }

  return Qnil;
}


VALUE
rb_get_int(VALUE self, VALUE parameter)
{
  Check_Type(parameter, T_STRING);
  int value = 0;
  try {
    value = ALGORITHM(self)->getInt(StringValueCStr(parameter));
  }
  catch (cv::Exception& e) {
    raise_cverror(e);
  }

  return INT2NUM(value);
}

VALUE
rb_get_double(VALUE self, VALUE parameter)
{
  Check_Type(parameter, T_STRING);
  double value = 0.0;
  try {
    value = ALGORITHM(self)->getDouble(StringValueCStr(parameter));
  }
  catch (cv::Exception& e) {
    raise_cverror(e);
  }

  return DBL2NUM(value);
}

VALUE
rb_get_bool(VALUE self, VALUE parameter)
{
  Check_Type(parameter, T_STRING);
  bool value = false;
  try {
    value = ALGORITHM(self)->getBool(StringValueCStr(parameter));
  }
  catch (cv::Exception& e) {
    raise_cverror(e);
  }

  return value ? Qtrue : Qfalse;
}

VALUE
rb_get_string(VALUE self, VALUE parameter)
{
  Check_Type(parameter, T_STRING);
  std::string value = "";
  try {
    value = ALGORITHM(self)->getString(StringValueCStr(parameter));
  }
  catch (cv::Exception& e) {
    raise_cverror(e);
  }

  return rb_str_new_cstr(value.c_str());
}

VALUE
rb_get_mat(VALUE self, VALUE parameter)
{
  Check_Type(parameter, T_STRING);
  VALUE mat = Qnil;
  try {
    cv::Mat value = ALGORITHM(self)->getMat(StringValueCStr(parameter));
    cv::Size size = value.size();
    mat = cCvMat::new_object(size.height, size.width, value.type());
    cv::Mat dst(CVMAT(mat));
    value.copyTo(dst);
  }
  catch (cv::Exception& e) {
    raise_cverror(e);
  }
  return mat;
}

VALUE
rb_get_matvector(VALUE self, VALUE parameter)
{
  Check_Type(parameter, T_STRING);
  VALUE array = Qnil;
  try {
    std::vector<cv::Mat> value = ALGORITHM(self)->getMatVector(StringValueCStr(parameter));
    int len = value.size();
    array = rb_ary_new2(len);
    for (int i = 0; i < len; i++) {
      cv::Mat m = value[i];
      cv::Size size = m.size();
      VALUE mat = cCvMat::new_object(size.height, size.width, m.type());
      cv::Mat dst(CVMAT(mat));
      m.copyTo(dst);
      rb_ary_store(array, i, mat);
    }
  }
  catch (cv::Exception& e) {
    raise_cverror(e);
  }
  return array;
}

VALUE
rb_name(VALUE self)
{
  VALUE name = Qnil;
  try {
    name = rb_str_new_cstr(ALGORITHM(self)->name().c_str());
  }
  catch (cv::Exception& e) {
    raise_cverror(e);
  }
  return name;
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
  /* 
   * opencv = rb_define_module("OpenCV");
   * 
   * note: this comment is used by rdoc.
   */
  VALUE opencv = rb_module_opencv();
  rb_klass = rb_define_class_under(opencv, "Algorithm", rb_cObject);
  rb_define_method(rb_klass, "set_int", RUBY_METHOD_FUNC(rb_set_int), 2);
  rb_define_method(rb_klass, "set_double", RUBY_METHOD_FUNC(rb_set_double), 2);
  rb_define_method(rb_klass, "set_bool", RUBY_METHOD_FUNC(rb_set_bool), 2);
  rb_define_method(rb_klass, "set_string", RUBY_METHOD_FUNC(rb_set_string), 2);
  rb_define_method(rb_klass, "set_mat", RUBY_METHOD_FUNC(rb_set_mat), 2);
  rb_define_method(rb_klass, "set_matvector", RUBY_METHOD_FUNC(rb_set_matvector), 2);
  rb_define_method(rb_klass, "set_algorithm", RUBY_METHOD_FUNC(rb_set_algorithm), 2);

  rb_define_method(rb_klass, "get_int", RUBY_METHOD_FUNC(rb_get_int), 1);
  rb_define_method(rb_klass, "get_double", RUBY_METHOD_FUNC(rb_get_double), 1);
  rb_define_method(rb_klass, "get_bool", RUBY_METHOD_FUNC(rb_get_bool), 1);
  rb_define_method(rb_klass, "get_string", RUBY_METHOD_FUNC(rb_get_string), 1);
  rb_define_method(rb_klass, "get_mat", RUBY_METHOD_FUNC(rb_get_mat), 1);
  rb_define_method(rb_klass, "get_matvector", RUBY_METHOD_FUNC(rb_get_matvector), 1);

  rb_define_method(rb_klass, "name", RUBY_METHOD_FUNC(rb_name), 0);
}

__NAMESPACE_END_ALGORITM
__NAMESPACE_END_OPENCV

