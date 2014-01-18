require 'yard'

YARD::Tags::Library.define_tag('OpenCV function', :opencv_func)
YARD::Tags::Library.visible_tags.place(:opencv_func).before(:abstract)

