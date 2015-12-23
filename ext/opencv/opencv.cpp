/************************************************************

  opencv.cpp -

  $Author: lsxi $

  Copyright (C) 2005-2008 Masakazu Yonekura

************************************************************/
/*
 * Document-module: OpenCV
 *
 * =What is OpenCV?
 *
 * OpenCV is "Open Source Computer Vision Library".
 * OpenCV is developed by Intel and many opensource developers.
 * This library include many useful function for computer vision, such as object-detection.
 *
 * OpenCV is developed at
 * sourceforge.net[http://sourceforge.net/projects/opencvlibrary]
 *
 * =What is Ruby/OpenCV?
 * Ruby/OpenCV is manual Wrapper of OpenCV (not use SWIG).
 * This library seem to be *Ruby*.
 * * object-oriented
 * * support Garbage Collection by Ruby
 * Ruby/OpenCV is developed
 * http://rubyforge.org/projects/opencv  (Official)
 * and
 * http://blueruby.mydns.jp/pages/opencv (Japanese)
 *
 * =How to install
 * 
 * Show INSTALL
 *
 * =How to generate this documentation?
 * This document created by rdoc.
 * If you have Ruby 1.8 or later, you might use rdoc command.
 * for example
 *   > cd opencv
 *   > rdoc
 * and show "./doc/index.html"
 */
#include "opencv.h"

__NAMESPACE_BEGIN_OPENCV


/*
 * Hashtable for protect from GC
 */
st_table *root_table = st_init_numtable();

/*
 * Mark root object. (protect from GC)
 */
void
mark_root_object(void *ptr)
{
  VALUE value;
  if (ptr && st_lookup(root_table, (st_data_t)ptr, (st_data_t*)&value)) {
    rb_gc_mark(value);
  }
}

/*
 * Look-up Root root object.
 */
VALUE
lookup_root_object(void *ptr)
{
  VALUE value = 0;
  if (ptr)
    st_lookup(root_table, (st_data_t)ptr, (st_data_t*)&value);
  return value;
}

/*
 * Register root object.
 */
void
register_root_object(void *ptr, VALUE root)
{
  st_insert(root_table, (st_data_t)ptr, (st_data_t)root);
}

/*
 * Delete mark symbol from hashtable only, not free memory.
 */
void
unregister_object(void *ptr)
{
  st_delete(root_table, (st_data_t*)&ptr, 0);
}

/*
 * Delete mark symbol from hash table, then free memory.
 */
void
free_object(void *ptr)
{
  if (ptr) {
    unregister_object(ptr);
    try {
      cvFree(&ptr);
    }
    catch (cv::Exception& e) {
      raise_cverror(e);
    }
  }
}

/*
 * Release OpenCV specific structure(i.e CvMat, IplImage..) from memory and delete from hashtable.
 */
void
release_object(void *ptr)
{
  if (ptr) {
    unregister_object(ptr);
    try {
      cvRelease(&ptr);
    }
    catch (cv::Exception& e) {
      raise_cverror(e);
    }
  }
}

/*
 * Release IplConvKernel object from memory and delete from hashtable.
 */
void
release_iplconvkernel_object(void *ptr)
{
  if (ptr) {
    unregister_object(ptr);
    try {
      cvReleaseStructuringElement((IplConvKernel**)(&ptr));
    }
    catch (cv::Exception& e) {
      raise_cverror(e);
    }
  }
}

VALUE rb_module;
VALUE rb_opencv_constants;

VALUE
rb_module_opencv()
{
  return rb_module;
}

void
init_ruby_module()
{
  if (rb_module)
    return;
  rb_module = rb_define_module("OpenCV");  
  
  /* define version */

  const char* CV_VERSION = '2.4.3';
  const int CV_MAJOR_VERSION = 2;
  const int CV_MINOR_VERSION = 4;
  const int CV_SUBMINOR_VERSION = 3;
  const int CV_VERSION_EPOCH = 2;
  const int CV_VERSION_MAJOR = 4;
  const int CV_VERSION_MINOR = 3;
  const int CV_VERSION_REVISION = 0;

  /* OpenCV version */
  rb_define_const(rb_module, "CV_VERSION", rb_str_new2(CV_VERSION));
  rb_define_const(rb_module, "CV_MAJOR_VERSION", INT2FIX(CV_MAJOR_VERSION));
  rb_define_const(rb_module, "CV_MINOR_VERSION", INT2FIX(CV_MINOR_VERSION));
  rb_define_const(rb_module, "CV_SUBMINOR_VERSION", INT2FIX(CV_SUBMINOR_VERSION));

  rb_define_const(rb_module, "CV_VERSION_EPOCH", INT2FIX(CV_VERSION_EPOCH));
  rb_define_const(rb_module, "CV_VERSION_MAJOR", INT2FIX(CV_VERSION_MAJOR));
  rb_define_const(rb_module, "CV_VERSION_MINOR", INT2FIX(CV_VERSION_MINOR));
  rb_define_const(rb_module, "CV_VERSION_REVISION", INT2FIX(CV_VERSION_REVISION));

  /* 0: 8bit unsigned */
  rb_define_const(rb_module, "CV_8U", INT2FIX(CV_8U));
  /* 1: 8bit signed */
  rb_define_const(rb_module, "CV_8S", INT2FIX(CV_8S));
  /* 2: 16bit unsigned */
  rb_define_const(rb_module, "CV_16U", INT2FIX(CV_16U));
  /* 3: 16bit signed */
  rb_define_const(rb_module, "CV_16S", INT2FIX(CV_16S));
  /* 4: 32bit signed */
  rb_define_const(rb_module, "CV_32S", INT2FIX(CV_32S));
  /* 5: 32bit floating-point */
  rb_define_const(rb_module, "CV_32F", INT2FIX(CV_32F));
  /* 6: 64bit floating-point */
  rb_define_const(rb_module, "CV_64F", INT2FIX(CV_64F));

  /* Other depth */
  rb_define_const(rb_module, "CV_8UC1", INT2FIX(CV_8UC1));
  rb_define_const(rb_module, "CV_8UC2", INT2FIX(CV_8UC2));
  rb_define_const(rb_module, "CV_8UC3", INT2FIX(CV_8UC3));
  rb_define_const(rb_module, "CV_8UC4", INT2FIX(CV_8UC4));
  rb_define_const(rb_module, "CV_8SC1", INT2FIX(CV_8SC1));
  rb_define_const(rb_module, "CV_8SC2", INT2FIX(CV_8SC2));
  rb_define_const(rb_module, "CV_8SC3", INT2FIX(CV_8SC3));
  rb_define_const(rb_module, "CV_8SC4", INT2FIX(CV_8SC4));
  rb_define_const(rb_module, "CV_16UC1", INT2FIX(CV_16UC1));
  rb_define_const(rb_module, "CV_16UC2", INT2FIX(CV_16UC2));
  rb_define_const(rb_module, "CV_16UC3", INT2FIX(CV_16UC3));
  rb_define_const(rb_module, "CV_16UC4", INT2FIX(CV_16UC4));
  rb_define_const(rb_module, "CV_16SC1", INT2FIX(CV_16SC1));
  rb_define_const(rb_module, "CV_16SC2", INT2FIX(CV_16SC2));
  rb_define_const(rb_module, "CV_16SC3", INT2FIX(CV_16SC3));
  rb_define_const(rb_module, "CV_16SC4", INT2FIX(CV_16SC4));
  rb_define_const(rb_module, "CV_32SC1", INT2FIX(CV_32SC1));
  rb_define_const(rb_module, "CV_32SC2", INT2FIX(CV_32SC2));
  rb_define_const(rb_module, "CV_32SC3", INT2FIX(CV_32SC3));
  rb_define_const(rb_module, "CV_32SC4", INT2FIX(CV_32SC4));
  rb_define_const(rb_module, "CV_32FC1", INT2FIX(CV_32FC1));
  rb_define_const(rb_module, "CV_32FC2", INT2FIX(CV_32FC2));
  rb_define_const(rb_module, "CV_32FC3", INT2FIX(CV_32FC3));
  rb_define_const(rb_module, "CV_32FC4", INT2FIX(CV_32FC4));
  rb_define_const(rb_module, "CV_64FC1", INT2FIX(CV_64FC1));
  rb_define_const(rb_module, "CV_64FC2", INT2FIX(CV_64FC2));
  rb_define_const(rb_module, "CV_64FC3", INT2FIX(CV_64FC3));
  rb_define_const(rb_module, "CV_64FC4", INT2FIX(CV_64FC4));

  /* Color types of loaded images */
  rb_define_const(rb_module, "CV_LOAD_IMAGE_UNCHANGED", INT2FIX(CV_LOAD_IMAGE_UNCHANGED));
  rb_define_const(rb_module, "CV_LOAD_IMAGE_GRAYSCALE", INT2FIX(CV_LOAD_IMAGE_GRAYSCALE));
  rb_define_const(rb_module, "CV_LOAD_IMAGE_COLOR", INT2FIX(CV_LOAD_IMAGE_COLOR));
  rb_define_const(rb_module, "CV_LOAD_IMAGE_ANYDEPTH", INT2FIX(CV_LOAD_IMAGE_ANYDEPTH));
  rb_define_const(rb_module, "CV_LOAD_IMAGE_ANYCOLOR", INT2FIX(CV_LOAD_IMAGE_ANYCOLOR));

  /* Format-specific save parameters */
  rb_define_const(rb_module, "CV_IMWRITE_JPEG_QUALITY", INT2FIX(CV_IMWRITE_JPEG_QUALITY));
  rb_define_const(rb_module, "CV_IMWRITE_PNG_COMPRESSION", INT2FIX(CV_IMWRITE_PNG_COMPRESSION));
  rb_define_const(rb_module, "CV_IMWRITE_PNG_STRATEGY", INT2FIX(CV_IMWRITE_PNG_STRATEGY));
  rb_define_const(rb_module, "CV_IMWRITE_PNG_STRATEGY_DEFAULT", INT2FIX(CV_IMWRITE_PNG_STRATEGY_DEFAULT));
  rb_define_const(rb_module, "CV_IMWRITE_PNG_STRATEGY_FILTERED", INT2FIX(CV_IMWRITE_PNG_STRATEGY_FILTERED));
  rb_define_const(rb_module, "CV_IMWRITE_PNG_STRATEGY_HUFFMAN_ONLY", INT2FIX(CV_IMWRITE_PNG_STRATEGY_HUFFMAN_ONLY));
  rb_define_const(rb_module, "CV_IMWRITE_PNG_STRATEGY_RLE", INT2FIX(CV_IMWRITE_PNG_STRATEGY_RLE));
  rb_define_const(rb_module, "CV_IMWRITE_PNG_STRATEGY_FIXED", INT2FIX(CV_IMWRITE_PNG_STRATEGY_FIXED));
  rb_define_const(rb_module, "CV_IMWRITE_PXM_BINARY", INT2FIX(CV_IMWRITE_PXM_BINARY));

  /* Types of morphological operations */
  rb_define_const(rb_module, "CV_MOP_OPEN", INT2FIX(CV_MOP_OPEN));
  rb_define_const(rb_module, "CV_MOP_CLOSE", INT2FIX(CV_MOP_CLOSE));
  rb_define_const(rb_module, "CV_MOP_GRADIENT", INT2FIX(CV_MOP_GRADIENT));
  rb_define_const(rb_module, "CV_MOP_TOPHAT", INT2FIX(CV_MOP_TOPHAT));
  rb_define_const(rb_module, "CV_MOP_BLACKHAT", INT2FIX(CV_MOP_BLACKHAT));

  /* Shape of the structuring elements */
  rb_define_const(rb_module, "CV_SHAPE_RECT", INT2FIX(CV_SHAPE_RECT));
  rb_define_const(rb_module, "CV_SHAPE_CROSS", INT2FIX(CV_SHAPE_CROSS));
  rb_define_const(rb_module, "CV_SHAPE_ELLIPSE", INT2FIX(CV_SHAPE_ELLIPSE));
  rb_define_const(rb_module, "CV_SHAPE_CUSTOM", INT2FIX(CV_SHAPE_CUSTOM));

  /* Types of the smoothing */
  rb_define_const(rb_module, "CV_BLUR_NO_SCALE", INT2FIX(CV_BLUR_NO_SCALE));
  rb_define_const(rb_module, "CV_BLUR", INT2FIX(CV_BLUR));
  rb_define_const(rb_module, "CV_GAUSSIAN", INT2FIX(CV_GAUSSIAN));
  rb_define_const(rb_module, "CV_MEDIAN", INT2FIX(CV_MEDIAN));
  rb_define_const(rb_module, "CV_BILATERAL", INT2FIX(CV_BILATERAL));

  /* Thresholding types */
  rb_define_const(rb_module, "CV_THRESH_BINARY", INT2FIX(CV_THRESH_BINARY));
  rb_define_const(rb_module, "CV_THRESH_BINARY_INV", INT2FIX(CV_THRESH_BINARY_INV));
  rb_define_const(rb_module, "CV_THRESH_TRUNC", INT2FIX(CV_THRESH_TRUNC));
  rb_define_const(rb_module, "CV_THRESH_TOZERO", INT2FIX(CV_THRESH_TOZERO));
  rb_define_const(rb_module, "CV_THRESH_TOZERO_INV", INT2FIX(CV_THRESH_TOZERO_INV));
  rb_define_const(rb_module, "CV_THRESH_OTSU", INT2FIX(CV_THRESH_OTSU));

  /* Adaptive methods */
  rb_define_const(rb_module, "CV_ADAPTIVE_THRESH_MEAN_C", INT2FIX(CV_ADAPTIVE_THRESH_MEAN_C));
  rb_define_const(rb_module, "CV_ADAPTIVE_THRESH_GAUSSIAN_C", INT2FIX(CV_ADAPTIVE_THRESH_GAUSSIAN_C));

  /* Border type */
  rb_define_const(rb_module, "IPL_BORDER_CONSTANT", INT2FIX(IPL_BORDER_CONSTANT));
  rb_define_const(rb_module, "IPL_BORDER_REPLICATE", INT2FIX(IPL_BORDER_REPLICATE));
  
  /* Retrieval mode */
  rb_define_const(rb_module, "CV_RETR_EXTERNAL", INT2FIX(CV_RETR_EXTERNAL));
  rb_define_const(rb_module, "CV_RETR_LIST", INT2FIX(CV_RETR_LIST));
  rb_define_const(rb_module, "CV_RETR_CCOMP", INT2FIX(CV_RETR_CCOMP));
  rb_define_const(rb_module, "CV_RETR_TREE", INT2FIX(CV_RETR_TREE));
  
  /* Approximation method */
  rb_define_const(rb_module, "CV_CHAIN_CODE", INT2FIX(CV_CHAIN_CODE));
  rb_define_const(rb_module, "CV_CHAIN_APPROX_NONE", INT2FIX(CV_CHAIN_APPROX_NONE));
  rb_define_const(rb_module, "CV_CHAIN_APPROX_SIMPLE", INT2FIX(CV_CHAIN_APPROX_SIMPLE));
  rb_define_const(rb_module, "CV_CHAIN_APPROX_TC89_L1", INT2FIX(CV_CHAIN_APPROX_TC89_L1));
  rb_define_const(rb_module, "CV_CHAIN_APPROX_TC89_KCOS", INT2FIX(CV_CHAIN_APPROX_TC89_KCOS));
  rb_define_const(rb_module, "CV_LINK_RUNS", INT2FIX(CV_LINK_RUNS));

  /* Termination criteria for iterative algorithms */
  rb_define_const(rb_module, "CV_TERMCRIT_ITER", INT2FIX(CV_TERMCRIT_ITER));
  rb_define_const(rb_module, "CV_TERMCRIT_NUMBER", INT2FIX(CV_TERMCRIT_NUMBER));
  rb_define_const(rb_module, "CV_TERMCRIT_EPS", INT2FIX(CV_TERMCRIT_EPS));

  /* Hough transform method */
  rb_define_const(rb_module, "CV_HOUGH_STANDARD", INT2FIX(CV_HOUGH_STANDARD));
  rb_define_const(rb_module, "CV_HOUGH_PROBABILISTIC", INT2FIX(CV_HOUGH_PROBABILISTIC));
  rb_define_const(rb_module, "CV_HOUGH_MULTI_SCALE", INT2FIX(CV_HOUGH_MULTI_SCALE));
  rb_define_const(rb_module, "CV_HOUGH_GRADIENT", INT2FIX(CV_HOUGH_GRADIENT));

  /* Inpaint method */
  rb_define_const(rb_module, "CV_INPAINT_NS", INT2FIX(CV_INPAINT_NS));
  rb_define_const(rb_module, "CV_INPAINT_TELEA", INT2FIX(CV_INPAINT_TELEA));

  /* Match template method */
  rb_define_const(rb_module, "CV_TM_SQDIFF", INT2FIX(CV_TM_SQDIFF));
  rb_define_const(rb_module, "CV_TM_SQDIFF_NORMED", INT2FIX(CV_TM_SQDIFF_NORMED));
  rb_define_const(rb_module, "CV_TM_CCORR", INT2FIX(CV_TM_CCORR));
  rb_define_const(rb_module, "CV_TM_CCORR_NORMED", INT2FIX(CV_TM_CCORR_NORMED));
  rb_define_const(rb_module, "CV_TM_CCOEFF", INT2FIX(CV_TM_CCOEFF));
  rb_define_const(rb_module, "CV_TM_CCOEFF_NORMED", INT2FIX(CV_TM_CCOEFF_NORMED));

  /* Comparison method */
  rb_define_const(rb_module, "CV_CONTOURS_MATCH_I1", INT2FIX(CV_CONTOURS_MATCH_I1));
  rb_define_const(rb_module, "CV_CONTOURS_MATCH_I2", INT2FIX(CV_CONTOURS_MATCH_I2));
  rb_define_const(rb_module, "CV_CONTOURS_MATCH_I3", INT2FIX(CV_CONTOURS_MATCH_I3));

  /* Fundamental matrix computing methods */
  rb_define_const(rb_module, "CV_FM_7POINT", INT2FIX(CV_FM_7POINT));
  rb_define_const(rb_module, "CV_FM_8POINT", INT2FIX(CV_FM_8POINT));
  rb_define_const(rb_module, "CV_FM_RANSAC", INT2FIX(CV_FM_RANSAC));
  rb_define_const(rb_module, "CV_FM_LMEDS", INT2FIX(CV_FM_LMEDS));

  /* Flags of window */
  rb_define_const(rb_module, "CV_WINDOW_AUTOSIZE", INT2FIX(CV_WINDOW_AUTOSIZE));
  rb_define_const(rb_module, "CV_WINDOW_NORMAL", INT2FIX(CV_WINDOW_NORMAL));
  rb_define_const(rb_module, "CV_WINDOW_OPENGL", INT2FIX(CV_WINDOW_OPENGL));

  /* Object detection mode */
  rb_define_const(rb_module, "CV_HAAR_DO_CANNY_PRUNING", INT2FIX(CV_HAAR_DO_CANNY_PRUNING));

  /* Interpolation methods */
  rb_define_const(rb_module, "CV_INTER_NN", INT2FIX(CV_INTER_NN));
  rb_define_const(rb_module, "CV_INTER_LINEAR", INT2FIX(CV_INTER_LINEAR));
  rb_define_const(rb_module, "CV_INTER_AREA", INT2FIX(CV_INTER_AREA));
  rb_define_const(rb_module, "CV_INTER_CUBIC", INT2FIX(CV_INTER_CUBIC));
  rb_define_const(rb_module, "CV_INTER_LANCZOS4", INT2FIX(CV_INTER_LANCZOS4));

  /* Warp affine optional flags */
  rb_define_const(rb_module, "CV_WARP_FILL_OUTLIERS", INT2FIX(CV_WARP_FILL_OUTLIERS));
  rb_define_const(rb_module, "CV_WARP_INVERSE_MAP", INT2FIX(CV_WARP_INVERSE_MAP));

  /* SVD optional flags */
  rb_define_const(rb_module, "CV_SVD_MODIFY_A", INT2FIX(CV_SVD_MODIFY_A));
  rb_define_const(rb_module, "CV_SVD_U_T", INT2FIX(CV_SVD_U_T));
  rb_define_const(rb_module, "CV_SVD_V_T", INT2FIX(CV_SVD_V_T));

  /* Norm types */
  rb_define_const(rb_module, "CV_NORM_INF", INT2FIX(cv::NORM_INF));
  rb_define_const(rb_module, "CV_NORM_L1", INT2FIX(cv::NORM_L1));
  rb_define_const(rb_module, "CV_NORM_L2", INT2FIX(cv::NORM_L2));
  rb_define_const(rb_module, "CV_NORM_MINMAX", INT2FIX(cv::NORM_MINMAX));

  /* Histogram representation format */
  rb_define_const(rb_module, "CV_HIST_ARRAY", INT2FIX(CV_HIST_ARRAY));
  rb_define_const(rb_module, "CV_HIST_SPARSE", INT2FIX(CV_HIST_SPARSE));
  rb_define_const(rb_module, "CV_HIST_TREE", INT2FIX(CV_HIST_TREE));
  rb_define_const(rb_module, "CV_HIST_UNIFORM", INT2FIX(CV_HIST_UNIFORM));

  /* Histogram comparison method */
  rb_define_const(rb_module, "CV_COMP_CORREL", INT2FIX(CV_COMP_CORREL));
  rb_define_const(rb_module, "CV_COMP_CHISQR", INT2FIX(CV_COMP_CHISQR));
  rb_define_const(rb_module, "CV_COMP_INTERSECT", INT2FIX(CV_COMP_INTERSECT));
  rb_define_const(rb_module, "CV_COMP_BHATTACHARYYA", INT2FIX(CV_COMP_BHATTACHARYYA));

  /* DFT and DCT flags */
  rb_define_const(rb_module, "CV_DXT_FORWARD", INT2FIX(CV_DXT_FORWARD));
  rb_define_const(rb_module, "CV_DXT_INVERSE", INT2FIX(CV_DXT_INVERSE));
  rb_define_const(rb_module, "CV_DXT_SCALE", INT2FIX(CV_DXT_SCALE));
  rb_define_const(rb_module, "CV_DXT_INV_SCALE", INT2FIX(CV_DXT_INV_SCALE));
  rb_define_const(rb_module, "CV_DXT_INVERSE_SCALE", INT2FIX(CV_DXT_INVERSE_SCALE));
  rb_define_const(rb_module, "CV_DXT_ROWS", INT2FIX(CV_DXT_ROWS));

  /* FindChessboardCorners flags */
  rb_define_const(rb_module, "CV_CALIB_CB_ADAPTIVE_THRESH", INT2FIX(CV_CALIB_CB_ADAPTIVE_THRESH));
  rb_define_const(rb_module, "CV_CALIB_CB_NORMALIZE_IMAGE", INT2FIX(CV_CALIB_CB_NORMALIZE_IMAGE));
  rb_define_const(rb_module, "CV_CALIB_CB_FILTER_QUADS", INT2FIX(CV_CALIB_CB_FILTER_QUADS));
  rb_define_const(rb_module, "CV_CALIB_CB_FAST_CHECK", INT2FIX(CV_CALIB_CB_FAST_CHECK));

  /* Color map for cv::applyColorMap */
  rb_define_const(rb_module, "COLORMAP_AUTUMN", INT2FIX(cv::COLORMAP_AUTUMN));
  rb_define_const(rb_module, "COLORMAP_BONE", INT2FIX(cv::COLORMAP_BONE));
  rb_define_const(rb_module, "COLORMAP_JET", INT2FIX(cv::COLORMAP_JET));
  rb_define_const(rb_module, "COLORMAP_WINTER", INT2FIX(cv::COLORMAP_WINTER));
  rb_define_const(rb_module, "COLORMAP_RAINBOW", INT2FIX(cv::COLORMAP_RAINBOW));
  rb_define_const(rb_module, "COLORMAP_OCEAN", INT2FIX(cv::COLORMAP_OCEAN));
  rb_define_const(rb_module, "COLORMAP_SUMMER", INT2FIX(cv::COLORMAP_SUMMER));
  rb_define_const(rb_module, "COLORMAP_SPRING", INT2FIX(cv::COLORMAP_SPRING));
  rb_define_const(rb_module, "COLORMAP_COOL", INT2FIX(cv::COLORMAP_COOL));
  rb_define_const(rb_module, "COLORMAP_HSV", INT2FIX(cv::COLORMAP_HSV));
  rb_define_const(rb_module, "COLORMAP_PINK", INT2FIX(cv::COLORMAP_PINK));
  rb_define_const(rb_module, "COLORMAP_HOT", INT2FIX(cv::COLORMAP_HOT));

  /* Sequence types */
  rb_define_const(rb_module, "CV_SEQ_ELTYPE_BITS", INT2FIX(CV_SEQ_ELTYPE_BITS));
  rb_define_const(rb_module, "CV_SEQ_ELTYPE_MASK", INT2FIX(CV_SEQ_ELTYPE_MASK));
  rb_define_const(rb_module, "CV_SEQ_ELTYPE_POINT", INT2FIX(CV_SEQ_ELTYPE_POINT));
  rb_define_const(rb_module, "CV_SEQ_ELTYPE_CODE", INT2FIX(CV_SEQ_ELTYPE_CODE));
  rb_define_const(rb_module, "CV_SEQ_ELTYPE_GENERIC", INT2FIX(CV_SEQ_ELTYPE_GENERIC));
  rb_define_const(rb_module, "CV_SEQ_ELTYPE_PTR", INT2FIX(CV_SEQ_ELTYPE_PTR));
  rb_define_const(rb_module, "CV_SEQ_ELTYPE_PPOINT", INT2FIX(CV_SEQ_ELTYPE_PPOINT));
  rb_define_const(rb_module, "CV_SEQ_ELTYPE_INDEX", INT2FIX(CV_SEQ_ELTYPE_INDEX));
  rb_define_const(rb_module, "CV_SEQ_ELTYPE_GRAPH_EDGE", INT2FIX(CV_SEQ_ELTYPE_GRAPH_EDGE));
  rb_define_const(rb_module, "CV_SEQ_ELTYPE_GRAPH_VERTEX", INT2FIX(CV_SEQ_ELTYPE_GRAPH_VERTEX));
  rb_define_const(rb_module, "CV_SEQ_ELTYPE_TRIAN_ATR", INT2FIX(CV_SEQ_ELTYPE_TRIAN_ATR));
  rb_define_const(rb_module, "CV_SEQ_ELTYPE_CONNECTED_COMP", INT2FIX(CV_SEQ_ELTYPE_CONNECTED_COMP));
  rb_define_const(rb_module, "CV_SEQ_ELTYPE_POINT3D", INT2FIX(CV_SEQ_ELTYPE_POINT3D));
  rb_define_const(rb_module, "CV_SEQ_KIND_BITS", INT2FIX(CV_SEQ_KIND_BITS));
  rb_define_const(rb_module, "CV_SEQ_KIND_MASK", INT2FIX(CV_SEQ_KIND_MASK));
  rb_define_const(rb_module, "CV_SEQ_KIND_GENERIC", INT2FIX(CV_SEQ_KIND_GENERIC));
  rb_define_const(rb_module, "CV_SEQ_KIND_CURVE", INT2FIX(CV_SEQ_KIND_CURVE));
  rb_define_const(rb_module, "CV_SEQ_KIND_BIN_TREE", INT2FIX(CV_SEQ_KIND_BIN_TREE));
  rb_define_const(rb_module, "CV_SEQ_KIND_GRAPH", INT2FIX(CV_SEQ_KIND_GRAPH));
  rb_define_const(rb_module, "CV_SEQ_KIND_SUBDIV2D", INT2FIX(CV_SEQ_KIND_SUBDIV2D));
  rb_define_const(rb_module, "CV_SEQ_FLAG_SHIFT", INT2FIX(CV_SEQ_FLAG_SHIFT));
  rb_define_const(rb_module, "CV_SEQ_FLAG_CLOSED", INT2FIX(CV_SEQ_FLAG_CLOSED));
  rb_define_const(rb_module, "CV_SEQ_FLAG_SIMPLE", INT2FIX(CV_SEQ_FLAG_SIMPLE));
  rb_define_const(rb_module, "CV_SEQ_FLAG_CONVEX", INT2FIX(CV_SEQ_FLAG_CONVEX));
  rb_define_const(rb_module, "CV_SEQ_FLAG_HOLE", INT2FIX(CV_SEQ_FLAG_HOLE));
  rb_define_const(rb_module, "CV_GRAPH_FLAG_ORIENTED", INT2FIX(CV_GRAPH_FLAG_ORIENTED));
  rb_define_const(rb_module, "CV_GRAPH", INT2FIX(CV_GRAPH));
  rb_define_const(rb_module, "CV_ORIENTED_GRAPH", INT2FIX(CV_ORIENTED_GRAPH));
  rb_define_const(rb_module, "CV_SEQ_POINT_SET", INT2FIX(CV_SEQ_POINT_SET));
  rb_define_const(rb_module, "CV_SEQ_POINT3D_SET", INT2FIX(CV_SEQ_POINT3D_SET));
  rb_define_const(rb_module, "CV_SEQ_POLYLINE", INT2FIX(CV_SEQ_POLYLINE));
  rb_define_const(rb_module, "CV_SEQ_POLYGON", INT2FIX(CV_SEQ_POLYGON));
  rb_define_const(rb_module, "CV_SEQ_CONTOUR", INT2FIX(CV_SEQ_CONTOUR));
  rb_define_const(rb_module, "CV_SEQ_SIMPLE_POLYGON", INT2FIX(CV_SEQ_SIMPLE_POLYGON));
  rb_define_const(rb_module, "CV_SEQ_CHAIN", INT2FIX(CV_SEQ_CHAIN));
  rb_define_const(rb_module, "CV_SEQ_CHAIN_CONTOUR", INT2FIX(CV_SEQ_CHAIN_CONTOUR));
  rb_define_const(rb_module, "CV_SEQ_POLYGON_TREE", INT2FIX(CV_SEQ_POLYGON_TREE));
  rb_define_const(rb_module, "CV_SEQ_CONNECTED_COMP", INT2FIX(CV_SEQ_CONNECTED_COMP));
  rb_define_const(rb_module, "CV_SEQ_INDEX", INT2FIX(CV_SEQ_INDEX));

  VALUE inversion_method = rb_hash_new();
  /* {:lu, :svd, :svd_sym(:svd_symmetric)}: Inversion method */
  rb_define_const(rb_module, "INVERSION_METHOD", inversion_method);
  REGISTER_HASH(inversion_method, "lu", CV_LU);
  REGISTER_HASH(inversion_method, "svd", CV_SVD);
  REGISTER_HASH(inversion_method, "svd_sym", CV_SVD_SYM);
  REGISTER_HASH(inversion_method, "svd_symmetric", CV_SVD_SYM);
    
  VALUE homography_calc_method = rb_hash_new();
  /* {:all, :ransac, :lmeds}: Methods used to computed homography matrix */
  rb_define_const(rb_module, "HOMOGRAPHY_CALC_METHOD", homography_calc_method);
  REGISTER_HASH(homography_calc_method, "all", 0);
  REGISTER_HASH(homography_calc_method, "ransac", CV_RANSAC);
  REGISTER_HASH(homography_calc_method, "lmeds", CV_LMEDS);

  VALUE depth = rb_hash_new();
  /* {:cv8u, :cv8s, :cv16u, :cv16s, :cv32s, :cv32f, :cv64f}: Depth of each pixel. */
  rb_define_const(rb_module, "DEPTH", depth);
  REGISTER_HASH(depth, "cv8u", CV_8U);
  REGISTER_HASH(depth, "cv8s", CV_8S);
  REGISTER_HASH(depth, "cv16u", CV_16U);
  REGISTER_HASH(depth, "cv16s", CV_16S);
  REGISTER_HASH(depth, "cv32s", CV_32S);
  REGISTER_HASH(depth, "cv32f", CV_32F);
  REGISTER_HASH(depth, "cv64f", CV_64F);
  
  VALUE connectivity = rb_hash_new();
  /* {:aa(:anti_alias)}: Determined by the closeness of pixel values */
  rb_define_const(rb_module, "CONNECTIVITY", connectivity);
  REGISTER_HASH(connectivity, "aa", CV_AA);
  REGISTER_HASH(connectivity, "anti_alias", CV_AA);

  VALUE structuring_element_shape = rb_hash_new();
  /* {:rect, :cross, :ellipse, :custom}: Shape of the structuring elements */
  rb_define_const(rb_module, "STRUCTURING_ELEMENT_SHAPE", structuring_element_shape);
  REGISTER_HASH(structuring_element_shape, "rect", CV_SHAPE_RECT);
  REGISTER_HASH(structuring_element_shape, "cross", CV_SHAPE_CROSS);
  REGISTER_HASH(structuring_element_shape, "ellipse", CV_SHAPE_ELLIPSE);
  REGISTER_HASH(structuring_element_shape, "custom", CV_SHAPE_CUSTOM);

  VALUE retrieval_mode = rb_hash_new();
  /* {:external, :list, :ccomp, :tree}: Retrieval mode */
  rb_define_const(rb_module, "RETRIEVAL_MODE", retrieval_mode);
  REGISTER_HASH(retrieval_mode, "external", CV_RETR_EXTERNAL);
  REGISTER_HASH(retrieval_mode, "list", CV_RETR_LIST);
  REGISTER_HASH(retrieval_mode, "ccomp", CV_RETR_CCOMP);
  REGISTER_HASH(retrieval_mode, "tree", CV_RETR_TREE);

  VALUE approx_chain_method = rb_hash_new();
  /* {:code, :approx_none, :approx_simple, :apporx_tc89_11, :approx_tc89_kcos}: Approximation method */
  rb_define_const(rb_module, "APPROX_CHAIN_METHOD", approx_chain_method);
  REGISTER_HASH(approx_chain_method, "code", CV_CHAIN_CODE);
  REGISTER_HASH(approx_chain_method, "approx_none", CV_CHAIN_APPROX_NONE);
  REGISTER_HASH(approx_chain_method, "approx_simple", CV_CHAIN_APPROX_SIMPLE);
  REGISTER_HASH(approx_chain_method, "approx_tc89_l1", CV_CHAIN_APPROX_TC89_L1);
  REGISTER_HASH(approx_chain_method, "approx_tc89_kcos", CV_CHAIN_APPROX_TC89_KCOS);
  
  VALUE approx_poly_method = rb_hash_new();
  /* {:dp}: Approximation method (polygon) */
  rb_define_const(rb_module, "APPROX_POLY_METHOD", approx_poly_method);
  REGISTER_HASH(approx_poly_method, "dp", CV_POLY_APPROX_DP);

  VALUE match_template_method = rb_hash_new();
  /* {:sqdiff, :sqdiff_normed, :ccorr, :ccorr_normed, :ccoeff, :ccoeff_normed}: Match template method */
  rb_define_const(rb_module, "MATCH_TEMPLATE_METHOD", match_template_method);
  REGISTER_HASH(match_template_method, "sqdiff", CV_TM_SQDIFF);
  REGISTER_HASH(match_template_method, "sqdiff_normed", CV_TM_SQDIFF_NORMED);
  REGISTER_HASH(match_template_method, "ccorr", CV_TM_CCORR);
  REGISTER_HASH(match_template_method, "ccorr_normed", CV_TM_CCORR_NORMED);
  REGISTER_HASH(match_template_method, "ccoeff", CV_TM_CCOEFF);
  REGISTER_HASH(match_template_method, "ccoeff_normed", CV_TM_CCOEFF_NORMED);

  VALUE morphological_operation = rb_hash_new();
  /* {:open, :close, :gradient, :tophat, :blackhat}: Types of morphological operations */
  rb_define_const(rb_module, "MORPHOLOGICAL_OPERATION", morphological_operation);
  REGISTER_HASH(morphological_operation, "open", CV_MOP_OPEN);
  REGISTER_HASH(morphological_operation, "close", CV_MOP_CLOSE);
  REGISTER_HASH(morphological_operation, "gradient", CV_MOP_GRADIENT);
  REGISTER_HASH(morphological_operation, "tophat", CV_MOP_TOPHAT);
  REGISTER_HASH(morphological_operation, "blackhat", CV_MOP_BLACKHAT);

  VALUE smoothing_type = rb_hash_new();
  /* {:blur_no_scale, :blur, :gaussian, :median, :bilateral}: Types of smoothing */
  rb_define_const(rb_module, "SMOOTHING_TYPE", smoothing_type);
  REGISTER_HASH(smoothing_type, "blur_no_scale", CV_BLUR_NO_SCALE);
  REGISTER_HASH(smoothing_type, "blur", CV_BLUR);
  REGISTER_HASH(smoothing_type, "gaussian", CV_GAUSSIAN);
  REGISTER_HASH(smoothing_type, "median", CV_MEDIAN);
  REGISTER_HASH(smoothing_type, "bilateral", CV_BILATERAL);

  VALUE adaptive_method = rb_hash_new();
  /* {:mean_c, :gaussian_c}: Adaptive thresholding algorithm */
  rb_define_const(rb_module, "ADAPTIVE_METHOD", adaptive_method);
  REGISTER_HASH(adaptive_method, "mean_c", CV_ADAPTIVE_THRESH_MEAN_C);
  REGISTER_HASH(adaptive_method, "gaussian_c", CV_ADAPTIVE_THRESH_GAUSSIAN_C);

  VALUE threshold_type = rb_hash_new();
  /* {:binary, :binary_inv, :trunc, :tozero, :tozero_inv, :otsu} : Thresholding types */
  rb_define_const(rb_module, "THRESHOLD_TYPE", threshold_type);
  REGISTER_HASH(threshold_type, "binary", CV_THRESH_BINARY);
  REGISTER_HASH(threshold_type, "binary_inv", CV_THRESH_BINARY_INV);
  REGISTER_HASH(threshold_type, "trunc", CV_THRESH_TRUNC);
  REGISTER_HASH(threshold_type, "tozero", CV_THRESH_TOZERO);
  REGISTER_HASH(threshold_type, "tozero_inv", CV_THRESH_TOZERO_INV);
  REGISTER_HASH(threshold_type, "otsu", CV_THRESH_OTSU);

  VALUE hough_transform_method = rb_hash_new();
  /* {:standard, :probabilistic, :multi_scale} : Hough transform method */
  rb_define_const(rb_module, "HOUGH_TRANSFORM_METHOD", hough_transform_method);
  REGISTER_HASH(hough_transform_method, "standard", CV_HOUGH_STANDARD);
  REGISTER_HASH(hough_transform_method, "probabilistic", CV_HOUGH_PROBABILISTIC);
  REGISTER_HASH(hough_transform_method, "multi_scale", CV_HOUGH_MULTI_SCALE);
  REGISTER_HASH(hough_transform_method, "gradient", CV_HOUGH_GRADIENT);

  VALUE inpaint_method = rb_hash_new();
  /* {:ns, :telea} : Inpaint method */
  rb_define_const(rb_module, "INPAINT_METHOD", inpaint_method);
  REGISTER_HASH(inpaint_method, "ns", CV_INPAINT_NS);
  REGISTER_HASH(inpaint_method, "telea", CV_INPAINT_TELEA);

  VALUE comparison_method = rb_hash_new();
  /* Comparison method */
  rb_define_const(rb_module, "COMPARISON_METHOD", comparison_method);
  REGISTER_HASH(comparison_method, "i1", CV_CONTOURS_MATCH_I1);
  REGISTER_HASH(comparison_method, "i2", CV_CONTOURS_MATCH_I2);
  REGISTER_HASH(comparison_method, "i3", CV_CONTOURS_MATCH_I3);

  /* color convert methods */
  rb_define_module_function(rb_module, "BGR2BGRA", RUBY_METHOD_FUNC(rb_BGR2BGRA), 1);
  rb_define_module_function(rb_module, "RGB2RGBA", RUBY_METHOD_FUNC(rb_RGB2RGBA), 1);
  rb_define_module_function(rb_module, "BGRA2BGR", RUBY_METHOD_FUNC(rb_BGRA2BGR), 1);
  rb_define_module_function(rb_module, "RGBA2RGB", RUBY_METHOD_FUNC(rb_RGBA2RGB), 1);
  rb_define_module_function(rb_module, "BGR2RGBA", RUBY_METHOD_FUNC(rb_BGR2RGBA), 1);
  rb_define_module_function(rb_module, "RGB2BGRA", RUBY_METHOD_FUNC(rb_RGB2BGRA), 1);
  rb_define_module_function(rb_module, "RGBA2BGR", RUBY_METHOD_FUNC(rb_RGBA2BGR), 1);
  rb_define_module_function(rb_module, "BGRA2RGB", RUBY_METHOD_FUNC(rb_BGRA2RGB), 1);
  rb_define_module_function(rb_module, "BGR2RGB", RUBY_METHOD_FUNC(rb_BGR2RGB), 1);
  rb_define_module_function(rb_module, "RGB2BGR", RUBY_METHOD_FUNC(rb_RGB2BGR), 1);
  rb_define_module_function(rb_module, "BGRA2RGBA", RUBY_METHOD_FUNC(rb_BGRA2RGBA), 1);
  rb_define_module_function(rb_module, "RGBA2BGRA", RUBY_METHOD_FUNC(rb_RGBA2BGRA), 1);
  rb_define_module_function(rb_module, "BGR2GRAY", RUBY_METHOD_FUNC(rb_BGR2GRAY), 1);
  rb_define_module_function(rb_module, "RGB2GRAY", RUBY_METHOD_FUNC(rb_RGB2GRAY), 1);
  rb_define_module_function(rb_module, "GRAY2BGR", RUBY_METHOD_FUNC(rb_GRAY2BGR), 1);
  rb_define_module_function(rb_module, "GRAY2RGB", RUBY_METHOD_FUNC(rb_GRAY2RGB), 1);
  rb_define_module_function(rb_module, "GRAY2BGRA", RUBY_METHOD_FUNC(rb_GRAY2BGRA), 1);
  rb_define_module_function(rb_module, "GRAY2RGBA", RUBY_METHOD_FUNC(rb_GRAY2RGBA), 1);
  rb_define_module_function(rb_module, "BGRA2GRAY", RUBY_METHOD_FUNC(rb_BGRA2GRAY), 1);
  rb_define_module_function(rb_module, "RGBA2GRAY", RUBY_METHOD_FUNC(rb_RGBA2GRAY), 1);
  rb_define_module_function(rb_module, "BGR2BGR565", RUBY_METHOD_FUNC(rb_BGR2BGR565), 1);
  rb_define_module_function(rb_module, "RGB2BGR565", RUBY_METHOD_FUNC(rb_RGB2BGR565), 1);
  rb_define_module_function(rb_module, "BGR5652BGR", RUBY_METHOD_FUNC(rb_BGR5652BGR), 1);
  rb_define_module_function(rb_module, "BGR5652RGB", RUBY_METHOD_FUNC(rb_BGR5652RGB), 1);
  rb_define_module_function(rb_module, "BGRA2BGR565", RUBY_METHOD_FUNC(rb_BGRA2BGR565), 1);
  rb_define_module_function(rb_module, "RGBA2BGR565", RUBY_METHOD_FUNC(rb_RGBA2BGR565), 1);
  rb_define_module_function(rb_module, "BGR5652BGRA", RUBY_METHOD_FUNC(rb_BGR5652BGRA), 1);
  rb_define_module_function(rb_module, "BGR5652RGBA", RUBY_METHOD_FUNC(rb_BGR5652RGBA), 1);
  rb_define_module_function(rb_module, "GRAY2BGR565", RUBY_METHOD_FUNC(rb_GRAY2BGR565), 1);
  rb_define_module_function(rb_module, "BGR5652GRAY", RUBY_METHOD_FUNC(rb_BGR5652GRAY), 1);
  rb_define_module_function(rb_module, "BGR2BGR555", RUBY_METHOD_FUNC(rb_BGR2BGR555), 1);
  rb_define_module_function(rb_module, "RGB2BGR555", RUBY_METHOD_FUNC(rb_RGB2BGR555), 1);
  rb_define_module_function(rb_module, "BGR5552BGR", RUBY_METHOD_FUNC(rb_BGR5552BGR), 1);
  rb_define_module_function(rb_module, "BGR5552RGB", RUBY_METHOD_FUNC(rb_BGR5552RGB), 1);
  rb_define_module_function(rb_module, "BGRA2BGR555", RUBY_METHOD_FUNC(rb_BGRA2BGR555), 1);
  rb_define_module_function(rb_module, "RGBA2BGR555", RUBY_METHOD_FUNC(rb_RGBA2BGR555), 1);
  rb_define_module_function(rb_module, "BGR5552BGRA", RUBY_METHOD_FUNC(rb_BGR5552BGRA), 1);
  rb_define_module_function(rb_module, "BGR5552RGBA", RUBY_METHOD_FUNC(rb_BGR5552RGBA), 1);
  rb_define_module_function(rb_module, "GRAY2BGR555", RUBY_METHOD_FUNC(rb_GRAY2BGR555), 1);
  rb_define_module_function(rb_module, "BGR5552GRAY", RUBY_METHOD_FUNC(rb_BGR5552GRAY), 1);
  rb_define_module_function(rb_module, "BGR2XYZ", RUBY_METHOD_FUNC(rb_BGR2XYZ), 1);
  rb_define_module_function(rb_module, "RGB2XYZ", RUBY_METHOD_FUNC(rb_RGB2XYZ), 1);
  rb_define_module_function(rb_module, "XYZ2BGR", RUBY_METHOD_FUNC(rb_XYZ2BGR), 1);
  rb_define_module_function(rb_module, "XYZ2RGB", RUBY_METHOD_FUNC(rb_XYZ2RGB), 1);
  rb_define_module_function(rb_module, "BGR2YCrCb", RUBY_METHOD_FUNC(rb_BGR2YCrCb), 1);
  rb_define_module_function(rb_module, "RGB2YCrCb", RUBY_METHOD_FUNC(rb_RGB2YCrCb), 1);
  rb_define_module_function(rb_module, "YCrCb2BGR", RUBY_METHOD_FUNC(rb_YCrCb2BGR), 1);
  rb_define_module_function(rb_module, "YCrCb2RGB", RUBY_METHOD_FUNC(rb_YCrCb2RGB), 1);
  rb_define_module_function(rb_module, "BGR2HSV", RUBY_METHOD_FUNC(rb_BGR2HSV), 1);
  rb_define_module_function(rb_module, "RGB2HSV", RUBY_METHOD_FUNC(rb_RGB2HSV), 1);
  rb_define_module_function(rb_module, "BGR2Lab", RUBY_METHOD_FUNC(rb_BGR2Lab), 1);
  rb_define_module_function(rb_module, "RGB2Lab", RUBY_METHOD_FUNC(rb_RGB2Lab), 1);
  rb_define_module_function(rb_module, "BayerBG2BGR", RUBY_METHOD_FUNC(rb_BayerBG2BGR), 1);
  rb_define_module_function(rb_module, "BayerGB2BGR", RUBY_METHOD_FUNC(rb_BayerGB2BGR), 1);
  rb_define_module_function(rb_module, "BayerRG2BGR", RUBY_METHOD_FUNC(rb_BayerRG2BGR), 1);
  rb_define_module_function(rb_module, "BayerGR2BGR", RUBY_METHOD_FUNC(rb_BayerGR2BGR), 1);
  rb_define_module_function(rb_module, "BayerBG2RGB", RUBY_METHOD_FUNC(rb_BayerBG2RGB), 1);
  rb_define_module_function(rb_module, "BayerGB2RGB", RUBY_METHOD_FUNC(rb_BayerGB2RGB), 1);
  rb_define_module_function(rb_module, "BayerRG2RGB", RUBY_METHOD_FUNC(rb_BayerRG2RGB), 1);
  rb_define_module_function(rb_module, "BayerGR2RGB", RUBY_METHOD_FUNC(rb_BayerGR2RGB), 1);
  rb_define_module_function(rb_module, "BGR2Luv", RUBY_METHOD_FUNC(rb_BGR2Luv), 1);
  rb_define_module_function(rb_module, "RGB2Luv", RUBY_METHOD_FUNC(rb_RGB2Luv), 1);
  rb_define_module_function(rb_module, "BGR2HLS", RUBY_METHOD_FUNC(rb_BGR2HLS), 1);
  rb_define_module_function(rb_module, "RGB2HLS", RUBY_METHOD_FUNC(rb_RGB2HLS), 1);
  rb_define_module_function(rb_module, "HSV2BGR", RUBY_METHOD_FUNC(rb_HSV2BGR), 1);
  rb_define_module_function(rb_module, "HSV2RGB", RUBY_METHOD_FUNC(rb_HSV2RGB), 1);
  rb_define_module_function(rb_module, "Lab2BGR", RUBY_METHOD_FUNC(rb_Lab2BGR), 1);
  rb_define_module_function(rb_module, "Lab2RGB", RUBY_METHOD_FUNC(rb_Lab2RGB), 1);
  rb_define_module_function(rb_module, "Luv2BGR", RUBY_METHOD_FUNC(rb_Luv2BGR), 1);
  rb_define_module_function(rb_module, "Luv2RGB", RUBY_METHOD_FUNC(rb_Luv2RGB), 1);
  rb_define_module_function(rb_module, "HLS2BGR", RUBY_METHOD_FUNC(rb_HLS2BGR), 1);
  rb_define_module_function(rb_module, "HLS2RGB", RUBY_METHOD_FUNC(rb_HLS2RGB), 1);

  rb_define_module_function(rb_module, "build_information", RUBY_METHOD_FUNC(rb_build_information), 0);
}

#define CREATE_CVTCOLOR_FUNC(rb_func_name, c_const_name, src_cn, dest_cn) \
  VALUE rb_func_name(VALUE klass, VALUE image)				\
  {									\
    VALUE dest = Qnil;							\
    CvArr* img_ptr = CVARR(image);					\
    try {								\
      int type = cvGetElemType(img_ptr);				\
      if (CV_MAT_CN(type) != src_cn)					\
	rb_raise(rb_eArgError, "argument 1 should be %d-channel.", src_cn); \
      dest = cCvMat::new_mat_kind_object(cvGetSize(img_ptr), image, CV_MAT_DEPTH(type), dest_cn); \
      cvCvtColor(img_ptr, CVARR(dest), c_const_name);			\
    }									\
    catch (cv::Exception& e) {						\
      raise_cverror(e);							\
    }									\
    return dest;							\
  }

CREATE_CVTCOLOR_FUNC(rb_BGR2BGRA, CV_BGR2BGRA, 3, 4);
CREATE_CVTCOLOR_FUNC(rb_RGB2RGBA, CV_RGB2RGBA, 3, 4);
CREATE_CVTCOLOR_FUNC(rb_BGRA2BGR, CV_BGRA2BGR, 4, 3);
CREATE_CVTCOLOR_FUNC(rb_RGBA2RGB, CV_RGBA2RGB, 4, 3);
CREATE_CVTCOLOR_FUNC(rb_BGR2RGBA, CV_BGR2RGBA, 3, 4);
CREATE_CVTCOLOR_FUNC(rb_RGB2BGRA, CV_RGB2BGRA, 3, 4);
CREATE_CVTCOLOR_FUNC(rb_RGBA2BGR, CV_RGBA2BGR, 4, 3);
CREATE_CVTCOLOR_FUNC(rb_BGRA2RGB, CV_BGRA2RGB, 4, 3);
CREATE_CVTCOLOR_FUNC(rb_BGR2RGB, CV_BGR2RGB, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_RGB2BGR, CV_RGB2BGR, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_BGRA2RGBA, CV_BGRA2RGBA, 4, 4);
CREATE_CVTCOLOR_FUNC(rb_RGBA2BGRA, CV_RGBA2BGRA, 4, 4);
CREATE_CVTCOLOR_FUNC(rb_BGR2GRAY, CV_BGR2GRAY, 3, 1);
CREATE_CVTCOLOR_FUNC(rb_RGB2GRAY, CV_RGB2GRAY, 3, 1);
CREATE_CVTCOLOR_FUNC(rb_GRAY2BGR, CV_GRAY2BGR, 1, 3);
CREATE_CVTCOLOR_FUNC(rb_GRAY2RGB, CV_GRAY2RGB, 1, 3);
CREATE_CVTCOLOR_FUNC(rb_GRAY2BGRA, CV_GRAY2BGRA, 1, 4);
CREATE_CVTCOLOR_FUNC(rb_GRAY2RGBA, CV_GRAY2RGBA, 1, 4);
CREATE_CVTCOLOR_FUNC(rb_BGRA2GRAY, CV_BGRA2GRAY, 4, 1);
CREATE_CVTCOLOR_FUNC(rb_RGBA2GRAY, CV_RGBA2GRAY, 4, 1);
CREATE_CVTCOLOR_FUNC(rb_BGR2BGR565, CV_BGR2BGR565, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_RGB2BGR565, CV_RGB2BGR565, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_BGR5652BGR, CV_BGR5652BGR, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_BGR5652RGB, CV_BGR5652RGB, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_BGRA2BGR565, CV_BGRA2BGR565, 4, 3);
CREATE_CVTCOLOR_FUNC(rb_RGBA2BGR565, CV_RGBA2BGR565, 4, 3);
CREATE_CVTCOLOR_FUNC(rb_BGR5652BGRA, CV_BGR5652BGRA, 3, 4);
CREATE_CVTCOLOR_FUNC(rb_BGR5652RGBA, CV_BGR5652RGBA, 3, 4);
CREATE_CVTCOLOR_FUNC(rb_GRAY2BGR565, CV_GRAY2BGR565, 1, 3);
CREATE_CVTCOLOR_FUNC(rb_BGR5652GRAY, CV_BGR5652GRAY, 3, 1);
CREATE_CVTCOLOR_FUNC(rb_BGR2BGR555, CV_BGR2BGR555, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_RGB2BGR555, CV_RGB2BGR555, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_BGR5552BGR, CV_BGR5552BGR, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_BGR5552RGB, CV_BGR5552RGB, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_BGRA2BGR555, CV_BGRA2BGR555, 4, 3);
CREATE_CVTCOLOR_FUNC(rb_RGBA2BGR555, CV_RGBA2BGR555, 4, 3);
CREATE_CVTCOLOR_FUNC(rb_BGR5552BGRA, CV_BGR5552BGRA, 3, 4);
CREATE_CVTCOLOR_FUNC(rb_BGR5552RGBA, CV_BGR5552RGBA, 3, 4);
CREATE_CVTCOLOR_FUNC(rb_GRAY2BGR555, CV_GRAY2BGR555, 1, 3);
CREATE_CVTCOLOR_FUNC(rb_BGR5552GRAY, CV_BGR5552GRAY, 3, 1);
CREATE_CVTCOLOR_FUNC(rb_BGR2XYZ, CV_BGR2XYZ, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_RGB2XYZ, CV_RGB2XYZ, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_XYZ2BGR, CV_XYZ2BGR, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_XYZ2RGB, CV_XYZ2RGB, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_BGR2YCrCb, CV_BGR2YCrCb, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_RGB2YCrCb, CV_RGB2YCrCb, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_YCrCb2BGR, CV_YCrCb2BGR, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_YCrCb2RGB, CV_YCrCb2RGB, 0, 3);
CREATE_CVTCOLOR_FUNC(rb_BGR2HSV, CV_BGR2HSV, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_RGB2HSV, CV_RGB2HSV, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_BGR2Lab, CV_BGR2Lab, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_RGB2Lab, CV_RGB2Lab, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_BayerBG2BGR, CV_BayerBG2BGR, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_BayerGB2BGR, CV_BayerGB2BGR, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_BayerRG2BGR, CV_BayerRG2BGR, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_BayerGR2BGR, CV_BayerGR2BGR, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_BayerBG2RGB, CV_BayerBG2RGB, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_BayerGB2RGB, CV_BayerGB2RGB, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_BayerRG2RGB, CV_BayerRG2RGB, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_BayerGR2RGB, CV_BayerGR2RGB, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_BGR2Luv, CV_BGR2Luv, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_RGB2Luv, CV_RGB2Luv, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_BGR2HLS, CV_BGR2HLS, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_RGB2HLS, CV_RGB2HLS, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_HSV2BGR, CV_HSV2BGR, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_HSV2RGB, CV_HSV2RGB, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_Lab2BGR, CV_Lab2BGR, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_Lab2RGB, CV_Lab2RGB, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_Luv2BGR, CV_Luv2BGR, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_Luv2RGB, CV_Luv2RGB, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_HLS2BGR, CV_HLS2BGR, 3, 3);
CREATE_CVTCOLOR_FUNC(rb_HLS2RGB, CV_HLS2RGB, 3, 3);

VALUE
rb_build_information(VALUE klass)
{
  const char* ptr = cv::getBuildInformation().c_str();
  return rb_str_new(ptr, strlen(ptr));
}


int
error_callback(int status, const char *function_name, const char *error_message,
	       const char *file_name, int line, void *user_data)
{
  // dummy 
  return 0;
}

__NAMESPACE_END_OPENCV

extern "C" {
  void
  Init_opencv()
  {
    cvRedirectError((CvErrorCallback)mOpenCV::error_callback);

    mOpenCV::init_ruby_module();
    
    mOpenCV::cCvError::init_ruby_class();
    mOpenCV::cCvPoint::init_ruby_class();
    mOpenCV::cCvPoint2D32f::init_ruby_class();
    mOpenCV::cCvPoint3D32f::init_ruby_class();
    mOpenCV::cCvSize::init_ruby_class();
    mOpenCV::cCvSize2D32f::init_ruby_class();
    mOpenCV::cCvRect::init_ruby_class();
    mOpenCV::cCvScalar::init_ruby_class();
    mOpenCV::cCvSlice::init_ruby_class();
    mOpenCV::cCvTermCriteria::init_ruby_class();
    mOpenCV::cCvBox2D::init_ruby_class();
    mOpenCV::cCvFont::init_ruby_class();
    mOpenCV::cIplConvKernel::init_ruby_class();
    mOpenCV::cCvMoments::init_ruby_class();
    mOpenCV::cCvHuMoments::init_ruby_class();
    mOpenCV::cCvConvexityDefect::init_ruby_class();

    mOpenCV::cCvSURFPoint::init_ruby_class();
    mOpenCV::cCvSURFParams::init_ruby_class();
    
    mOpenCV::cCvMemStorage::init_ruby_class();

    mOpenCV::cCvSeq::init_ruby_class();
    mOpenCV::mCurve::init_ruby_module();
    mOpenCV::mPointSet::init_ruby_module();
    mOpenCV::cCvChain::init_ruby_class();
    mOpenCV::cCvContour::init_ruby_class();
    mOpenCV::cCvContourTree::init_ruby_class();

    mOpenCV::cCvMat::init_ruby_class();
    mOpenCV::cIplImage::init_ruby_class();
    mOpenCV::cCvHistogram::init_ruby_class();
    mOpenCV::cCvCapture::init_ruby_class();
    mOpenCV::cCvVideoWriter::init_ruby_class();

    mOpenCV::cCvLine::init_ruby_class();
    mOpenCV::cCvTwoPoints::init_ruby_class();
    mOpenCV::cCvCircle32f::init_ruby_class();

    mOpenCV::cCvFeatureTree::init_ruby_class();

    mOpenCV::cCvConnectedComp::init_ruby_class();
    mOpenCV::cCvAvgComp::init_ruby_class();
    mOpenCV::cCvHaarClassifierCascade::init_ruby_class();

    mOpenCV::cAlgorithm::init_ruby_class();
    mOpenCV::cFaceRecognizer::init_ruby_class();
    mOpenCV::cEigenFaces::init_ruby_class();
    mOpenCV::cFisherFaces::init_ruby_class();
    mOpenCV::cLBPH::init_ruby_class();

    mOpenCV::mGUI::init_ruby_module();
    mOpenCV::mGUI::cWindow::init_ruby_class();
    mOpenCV::mGUI::cTrackbar::init_ruby_class();
    mOpenCV::mGUI::cMouseEvent::init_ruby_class();

#ifdef HAVE_ML_H
    /* feature support.
       mOpenCV::mMachineLearning::init_ruby_module();
    */
#endif

#ifdef HAVE_OPENCV2_NONFREE_NONFREE_HPP
    cv::initModule_nonfree();
#endif
  }
}
