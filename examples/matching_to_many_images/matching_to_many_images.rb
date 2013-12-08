require 'opencv'
require 'benchmark'
include OpenCV

data = File.dirname(__FILE__)

query = CvMat.load File.join(data, 'query.png'), CV_LOAD_IMAGE_GRAYSCALE
image_files = ['1.png', '2.png', '3.png'].map{|f| File.join(data, 'train', f)}
images = image_files.map{|f| CvMat.load f, CV_LOAD_IMAGE_GRAYSCALE}


matchs = query.match_descriptors(images)

match_index, count = matchs.max_by {|image_index, count| count}

puts "max match: #{image_files[match_index]}"
