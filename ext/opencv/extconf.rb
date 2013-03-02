#!/usr/bin/env ruby
require "mkmf"

def cv_version_suffix(incdir)
  major, minor, subminor = nil, nil, nil
  open("#{incdir}/opencv2/core/version.hpp", 'r') { |f|
    f.read.lines.each { |line|
      major = $1.to_s if line =~ /\A#define\s+CV_MAJOR_VERSION\s+(\d+)\s*\Z/
      minor = $1.to_s if line =~ /\A#define\s+CV_MINOR_VERSION\s+(\d+)\s*\Z/
      subminor = $1.to_s if line =~ /\A#define\s+CV_SUBMINOR_VERSION\s+(\d+)\s*\Z/
    }
  }
  major + minor + subminor
end

incdir, libdir = dir_config("opencv", "/usr/local/include", "/usr/local/lib")
dir_config("libxml2", "/usr/include", "/usr/lib")

opencv_headers = ["opencv2/core/core_c.h", "opencv2/core/core.hpp", "opencv2/imgproc/imgproc_c.h",
                  "opencv2/imgproc/imgproc.hpp", "opencv2/video/tracking.hpp", "opencv2/features2d/features2d.hpp",
                  "opencv2/flann/flann.hpp", "opencv2/calib3d/calib3d.hpp", "opencv2/objdetect/objdetect.hpp",
                  "opencv2/legacy/compat.hpp", "opencv2/legacy/legacy.hpp", "opencv2/highgui/highgui_c.h",
                  "opencv2/highgui/highgui.hpp", "opencv2/photo/photo.hpp", "opencv2/nonfree/nonfree.hpp"]

opencv_libraries = ["opencv_calib3d", "opencv_contrib", "opencv_core", "opencv_features2d",
                    "opencv_flann", "opencv_gpu", "opencv_highgui", "opencv_imgproc",
                    "opencv_legacy", "opencv_ml", "opencv_objdetect", "opencv_video",
                    "opencv_photo", "opencv_nonfree"]


puts ">> Check the required libraries..."
case CONFIG["arch"]
when /mswin32/
  suffix = cv_version_suffix(incdir)
  opencv_libraries.map! {|lib| lib + suffix }
  have_library("msvcrt")
  opencv_libraries.each {|lib|
    raise "#{lib}.lib not found." unless have_library(lib)
  }
  $CFLAGS << ' /EHsc'
when /mingw32/
  suffix = cv_version_suffix(incdir)
  opencv_libraries.map! {|lib| lib + suffix }
  have_library("msvcrt")
  opencv_libraries.each {|lib|
    raise "lib#{lib} not found." unless have_library(lib)
  }
else
  opencv_libraries.each {|lib|
    raise "lib#{lib} not found." unless have_library(lib)
  }
  have_library("stdc++")
end

# Check the required headers
puts ">> Check the required headers..."
opencv_headers.each {|header|
  unless have_header(header)
    if CONFIG["arch"] =~ /mswin32/ and File.exists? "#{incdir}/#{header}"
      # In mswin32, have_header('opencv2/nonfree/nonfree.hpp') fails because of a syntax problem.
      warn "warning: #{header} found but `have_header` failed."
      $defs << "-DHAVE_#{header.tr_cpp}"
    else
      raise "#{header} not found."
    end
  end
}
have_header("stdarg.h")

# Quick fix for 1.8.7
$CFLAGS << " -I#{File.dirname(__FILE__)}/ext/opencv"

# Create Makefile
create_makefile('opencv')

