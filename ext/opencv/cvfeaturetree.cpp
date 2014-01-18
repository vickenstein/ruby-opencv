/************************************************************

   cvfeaturetree.cpp -

   $Author: ser1zw $

   Copyright (C) 2011 ser1zw

************************************************************/
#include "cvfeaturetree.h"
/*
 * Document-class: OpenCV::CvFeatureTree
 */
__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVFEATURETREE

VALUE rb_klass;

VALUE
rb_class()
{
  return rb_klass;
}

void
mark_feature_tree(void *ptr)
{
  if (ptr) {
    VALUE desc = ((CvFeatureTreeWrap*)ptr)->desc;
    rb_gc_mark(desc);
  }
}

void
rb_release_feature_tree(void *ptr)
{
  if (ptr) {
    CvFeatureTree* ft = ((CvFeatureTreeWrap*)ptr)->feature_tree;
    cvReleaseFeatureTree(ft);
  }
}

VALUE
rb_allocate(VALUE klass)
{
  CvFeatureTreeWrap* ptr;
  return Data_Make_Struct(klass, CvFeatureTreeWrap, mark_feature_tree,
			  rb_release_feature_tree, ptr);
}

/*
 * Create a new kd-tree
 * @overload new(desc)
 *   @param desc [CvMat] Descriptors
 * @return [CvFeatureTree] self
 * @opencv_func cvCreateKDTree
 */
VALUE
rb_initialize(VALUE self, VALUE desc)
{
  CvMat* desc_mat = CVMAT_WITH_CHECK(desc);
  CvFeatureTreeWrap* self_ptr = (CvFeatureTreeWrap*)DATA_PTR(self);
  free(self_ptr);
  self_ptr = ALLOC(CvFeatureTreeWrap);
  try {
    self_ptr->feature_tree = cvCreateKDTree(desc_mat);
  }
  catch (cv::Exception& e) {
    raise_cverror(e);
  }
  self_ptr->desc = desc;
  return self;
}

/*
 * Find features from kd-tree
 * @overload find_features(desc, k, emax)
 *   @param desc [CvMat] m x d matrix of (row-)vectors to find the nearest neighbors of.
 *   @param k [Integer] The number of neighbors to find.
 *   @param emax [Integer] The maximum number of leaves to visit.
 * @return [Array] Array of [results, dist]
 *    - results: m x k set of row indices of matching vectors (referring to matrix passed to cvCreateFeatureTree). Contains -1 in some columns if fewer than k neighbors found.
 *    - dist: m x k matrix of distances to k nearest neighbors.
 * @opencv_func cvFindFeatures
 */
VALUE
rb_find_features(VALUE self, VALUE desc, VALUE k, VALUE emax)
{
  CvMat* desc_mat = CVMAT_WITH_CHECK(desc);
  int _k = NUM2INT(k);
  VALUE results = cCvMat::new_object(desc_mat->rows, _k, CV_32SC1);
  VALUE dist = cCvMat::new_object(desc_mat->rows, _k, CV_64FC1);
  try {
    cvFindFeatures(CVFEATURETREE(self), desc_mat, CVMAT(results), CVMAT(dist), _k, NUM2INT(emax));
  }
  catch (cv::Exception& e) {
    raise_cverror(e);
  }
  return rb_assoc_new(results, dist);
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
  rb_klass = rb_define_class_under(opencv, "CvFeatureTree", rb_cObject);
  rb_define_alloc_func(rb_klass, rb_allocate);
  rb_define_method(rb_klass, "initialize", RUBY_METHOD_FUNC(rb_initialize), 1);

  rb_define_method(rb_klass, "find_features", RUBY_METHOD_FUNC(rb_find_features), 3);
}

__NAMESPACE_END_OPENCV
__NAMESPACE_END_CVFEATURETREE

