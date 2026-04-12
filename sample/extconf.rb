require 'mkmf'

$objs = %w[
  main
].map do |o|
  o + ".#{$OBJEXT}"
end

append_cppflags("-DUNIVERSAL_PARSER=1")

$INCFLAGS << " -I" << File.expand_path("../../ext/ruby-parser/include/", __FILE__)
$INCFLAGS << " -I" << File.expand_path("../../ext/ruby-parser/ruby/", __FILE__)
$INCFLAGS << " -I" << File.expand_path("../../ext/ruby-parser/ruby/internal", __FILE__)

create_makefile('sample')
