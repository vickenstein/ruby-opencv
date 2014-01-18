/************************************************************

  gui.cpp -

  $Author: lsxi $

  Copyright (C) 2005-2006 Masakazu Yonekura

************************************************************/
#include "gui.h"
/*
 * Document-module: OpenCV::GUI
 */
__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_GUI
    
VALUE rb_module;

VALUE
rb_module_GUI()
{
  return rb_module;
}

/*
 * Waits for a pressed key.
 *
 * @overload wait_key(delay = 0)
 *   @param delay [Integer] Delay in milliseconds. 0 is the special value that means "forever".
 * @return [Number] The code of the pressed key or <tt>nil</tt> if no key was pressed
 *   before the specified time had elapsed.
 * @opencv_func cvWaitKey
 */
VALUE
rb_wait_key(int argc, VALUE *argv, VALUE self)
{
  VALUE delay;
  rb_scan_args(argc, argv, "01", &delay);
  int keycode = 0;
  try {
    keycode = cvWaitKey(IF_INT(delay, 0));
  }
  catch (cv::Exception& e) {
    raise_cverror(e);
  }
  return (keycode < 0) ? Qnil : INT2NUM(keycode);
}

void
init_ruby_module()
{
#if 0
  // For documentation using YARD
  VALUE opencv = rb_define_module("OpenCV");
#endif

  if (rb_module)
    return;
  /* 
   * opencv = rb_define_module("OpenCV");      
   *
   * note: this comment is used by rdoc.
   */
  VALUE opencv = rb_module_opencv();
  rb_module = rb_define_module_under(opencv, "GUI");
  rb_define_singleton_method(rb_module, "wait_key", RUBY_METHOD_FUNC(rb_wait_key), -1);
}

__NAMESPACE_END_GUI
__NAMESPACE_END_OPENCV

