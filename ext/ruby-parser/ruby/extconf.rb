require 'mkmf'

# lex.c is included from parse.c then not include it to $objs
$objs = %w[
  node
  parse
  parser_st
  ruby_parser
  ../helper
].map do |o|
  o + ".#{$OBJEXT}"
end

append_cppflags("-DUNIVERSAL_PARSER=1")

$INCFLAGS << " -I" << File.expand_path("../../../../", __FILE__)
$INCFLAGS << " -I" << File.expand_path("../../include", __FILE__)
$INCFLAGS << " -I" << File.expand_path("../ruby", __FILE__)

create_makefile('ruby_parser')
