require (File.dirname(__FILE__) + '/opencv/version')

if RUBY_PLATFORM =~ /mingw|mswin/
  major, minor, subminor = RUBY_VERSION.split('.')
  version_dir = (major.to_i >= 2) ? '2.0' : '1.9'
  begin
    require "#{version_dir}/opencv.so"
  rescue LoadError
    require 'opencv.so'
  end
else
  require 'opencv.so'
end
