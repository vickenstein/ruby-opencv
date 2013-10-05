#!/usr/bin/env ruby
# -*- mode: ruby; coding: utf-8-unix -*- 
require 'test/unit'
require 'opencv'
require 'date'
require File.expand_path(File.dirname(__FILE__)) + '/helper'

include OpenCV

# Tests for OpenCV::FisherFaces
class TestFisherFaces < OpenCVTestCase
  def setup
    @fisherfaces = FisherFaces.new

    @fisherfaces_trained = FisherFaces.new
    @images = [CvMat.load(FILENAME_LENA256x256, CV_LOAD_IMAGE_GRAYSCALE)] * 2
    @fisherfaces_trained.train(@images, [1, 2])
  end

  def test_initialize
    [FisherFaces.new, FisherFaces.new(1), FisherFaces.new(1, 99999)].each { |ff|
      assert_equal(FisherFaces, ff.class)
    }

    assert_raise(TypeError) {
      FisherFaces.new(DUMMY_OBJ)
    }

    assert_raise(TypeError) {
      FisherFaces.new(1, DUMMY_OBJ)
    }
  end

  def test_train
    assert_nil(@fisherfaces.train(@images, [1, 2]))

    assert_raise(TypeError) {
      @fisherfaces.train(DUMMY_OBJ, [1])
    }

    assert_raise(TypeError) {
      @fisherfaces.train(@images, DUMMY_OBJ)
    }
  end

  def test_predict
    label = 1
    assert_equal(1, @fisherfaces_trained.predict(@images[0]))

    assert_raise(TypeError) {
      @fisherfaces_trained.predict(DUMMY_OBJ)
    }
  end

  def test_save
    filename = "fisherfaces_save-#{DateTime.now.strftime('%Y%m%d%H%M%S')}.xml"
    begin
      @fisherfaces_trained.save(filename)
      assert(File.exist? filename)
    ensure
      File.delete filename
    end
    assert_raise(TypeError) {
      @fisherfaces_trained.save(DUMMY_OBJ)
    }
  end

  def test_load
    assert_nothing_raised {
      @fisherfaces.load('fisherfaces_save.xml')
    }
    assert_raise(TypeError) {
      @fisherfaces.load(DUMMY_OBJ)
    }
  end

  def test_name
    assert_equal('FaceRecognizer.Fisherfaces', @fisherfaces.name)
  end

  def test_get_mat
    mat = @fisherfaces_trained.get_mat('eigenvalues')
    assert_not_nil(mat)
    assert_equal(CvMat, mat.class)

    assert_raise(TypeError) {
      @fisherfaces_trained.get_mat(DUMMY_OBJ)
    }
  end
end

