#!/usr/bin/env ruby
# -*- mode: ruby; coding: utf-8-unix -*- 
require 'test/unit'
require 'opencv'
require 'date'
require File.expand_path(File.dirname(__FILE__)) + '/helper'

include OpenCV

# Tests for OpenCV::LBPH
class TestLBPH < OpenCVTestCase
  def setup
    @lbph = LBPH.new

    @lbph_trained = LBPH.new
    @images = [CvMat.load(FILENAME_LENA256x256, CV_LOAD_IMAGE_GRAYSCALE)] * 2
    @lbph_trained.train(@images, [1, 1])
  end

  def test_initialize
    [LBPH.new, LBPH.new(1), LBPH.new(1, 2, 3, 4, 5.0)].each { |lbph|
      assert_equal(LBPH, lbph.class)
    }

    assert_raise(TypeError) {
      LBPH.new(DUMMY_OBJ)
    }

    assert_raise(TypeError) {
      LBPH.new(1, DUMMY_OBJ)
    }

    assert_raise(TypeError) {
      LBPH.new(1, 2, DUMMY_OBJ)
    }

    assert_raise(TypeError) {
      LBPH.new(1, 2, 3, DUMMY_OBJ)
    }

    assert_raise(TypeError) {
      LBPH.new(1, 2, 3, 4, DUMMY_OBJ)
    }
  end

  def test_train
    assert_nil(@lbph.train(@images, [1, 1]))

    assert_raise(TypeError) {
      @lbph.train(DUMMY_OBJ, [1, 1])
    }

    assert_raise(TypeError) {
      @lbph.train(@images, DUMMY_OBJ)
    }
  end

  def test_predict
    assert_equal(1, @lbph_trained.predict(@images[0]))

    assert_raise(TypeError) {
      @lbph_trained.predict(DUMMY_OBJ)
    }
  end

  def test_save
    filename = "lbph_save-#{DateTime.now.strftime('%Y%m%d%H%M%S')}.xml"
    begin
      @lbph_trained.save(filename)
      assert(File.exist? filename)
    ensure
      File.delete filename
    end
    assert_raise(TypeError) {
      @lbph_trained.save(DUMMY_OBJ)
    }
  end

  def test_load
    assert_nothing_raised {
      @lbph.load('lbph_save.xml')
    }
    assert_raise(TypeError) {
      @lbph.load(DUMMY_OBJ)
    }
  end

  def test_name
    assert_equal('FaceRecognizer.LBPH', @lbph.name)
  end
end

