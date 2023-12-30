require "fileutils"
require "rake/extensiontask"

COPY_TARGETS = %w[
  internal/array.h
  internal/compilers.h
  internal/imemo.h
  internal/parse.h
  internal/ruby_parser.h
  internal/serial.h
  internal/static_assert.h
  internal/vm.h
  internal.h
  node.c
  node.h
  parser_bits.h
  parser_node.h
  parser_st.c
  parser_st.h
  parser_value.h
  rubyparser.h
  st.c
  universal_parser.c  
]

namespace "build" do
  desc "build parse.c and parse.h from parse.y"
  task :parser do
    sh "bundle exec lrama -oext/ruby-parser/ruby/parse.c -Hext/ruby-parser/ruby/parse.h ext/ruby-parser/ruby/parse.tmp.y"
  end
end

namespace "setup" do
  desc "copy files from ruby/ruby"
  task :copy_files, [:ruby_dir] do |task, args|
    unless (ruby_dir = args[:ruby_dir])
      raise "ruby_dir is not specified"
    end

    dist = File.expand_path("../ext/ruby-parser/ruby", __FILE__)

    COPY_TARGETS.each do |target|
      FileUtils.cp File.join(ruby_dir, target), File.join(dist, target)
    end

    # "parse.tmp.y"
    id2token_path = File.join(ruby_dir, "tool/id2token.rb")
    parse_y_path = File.join(ruby_dir, "parse.y")
    parse_tmp_y_path = File.join(dist, "parse.tmp.y")
    sh "ruby #{id2token_path} #{parse_y_path} > #{parse_tmp_y_path}"

    # "id.h"
    generic_erb_path = File.join(ruby_dir, "tool/generic_erb.rb")
    id_h_tmpl_path = File.join(ruby_dir, "template/id.h.tmpl")
    id_h_path = File.join(dist, "id.h")
    sh "ruby #{generic_erb_path} --output=#{id_h_path} #{id_h_tmpl_path}"

    # "probes.h"
    probes_h_path = File.join(dist, "probes.h")
    File.open(probes_h_path, "w+") do |f|
      f << <<~SRC
        #define RUBY_DTRACE_PARSE_BEGIN_ENABLED() (0)
        #define RUBY_DTRACE_PARSE_BEGIN(arg0, arg1) (void)(arg0), (void)(arg1);
        #define RUBY_DTRACE_PARSE_END_ENABLED() (0)
        #define RUBY_DTRACE_PARSE_END(arg0, arg1) (void)(arg0), (void)(arg1);
      SRC
    end

    # "node_name.inc"
    node_name_path = File.join(ruby_dir, "tool/node_name.rb")
    rubyparser_h_path = File.join(ruby_dir, "rubyparser.h")
    node_name_inc_path = File.join(dist, "node_name.inc")
    sh "ruby -n #{node_name_path} < #{rubyparser_h_path} > #{node_name_inc_path}"

    # TODO: Generate "lex.c"
    #       At the moment, copy on local
  end
end

Rake::ExtensionTask.new("ruby-parser") do |ext|
  ext.ext_dir = "ext/ruby-parser/ruby"
end
