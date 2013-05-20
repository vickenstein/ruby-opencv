#!/usr/bin/env ruby
# -*- mode: ruby; coding: utf-8-unix -*- 
require 'test/unit'
require 'opencv'
require 'date'
require File.expand_path(File.dirname(__FILE__)) + '/helper'

include OpenCV

# Tests for OpenCV::EigenFaces
class TestEigenFaces < OpenCVTestCase
  def setup
    @eigenfaces = EigenFaces.new
  end

  def test_initialize
    [EigenFaces.new, EigenFaces.new(1), EigenFaces.new(1, 99999)].each { |ef|
      assert_equal(EigenFaces, ef.class)
    }

    assert_raise(TypeError) {
      EigenFaces.new(DUMMY_OBJ)
    }

    assert_raise(TypeError) {
      EigenFaces.new(1, DUMMY_OBJ)
    }
  end

  def test_train
    img = CvMat.load(FILENAME_LENA256x256, CV_LOAD_IMAGE_GRAYSCALE)
    assert_nil(@eigenfaces.train([img], [1]))

    assert_raise(TypeError) {
      @eigenfaces.train(DUMMY_OBJ, [1])
    }

    assert_raise(TypeError) {
      @eigenfaces.train([img], DUMMY_OBJ)
    }
  end

  def test_predict
    img = CvMat.load(FILENAME_LENA256x256, CV_LOAD_IMAGE_GRAYSCALE)
    label = 1
    @eigenfaces.train([img], [label])
    assert_equal(label, @eigenfaces.predict(img))

    assert_raise(TypeError) {
      @eigenfaces.predict(DUMMY_OBJ)
    }
  end

  def test_save
    img = CvMat.load(FILENAME_LENA256x256, CV_LOAD_IMAGE_GRAYSCALE)
    label = 1
    @eigenfaces.train([img], [label])
    filename = "eigenfaces_save-#{DateTime.now.strftime('%Y%m%d%H%M%S')}.xml"
    begin
      @eigenfaces.save(filename)
      assert(File.exist? filename)
    ensure
      File.delete filename
    end
  end

  def test_load
    assert_nothing_raised {
      @eigenfaces.load('eigenfaces_save.xml')
    }
    assert_raise(TypeError) {
      @eigenfaces.load(DUMMY_OBJ)
    }
  end
end

