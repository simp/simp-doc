#!/usr/bin/rake -T

require 'simp/rake'

class DocPkg < Simp::Rake::Pkg
  def define_clean
    task :clean do
      find_erb_files.each do |erb|
        short_name = "#{File.dirname(erb)}/#{File.basename(erb,'.erb')}"
        if File.exist?(short_name) then
          rm(short_name)
        end
      end

      Rake::Task['rdoc:clean'].invoke
    end
  end

  def define_pkg_tar
    # First, we need to assemble the documentation.
    # This doesn't work properly under Ruby >= 1.9 and leaves cruft in the directories
    # We can try again when 'puppet strings' hits the ground
    if Gem::Version.new(RUBY_VERSION) < Gem::Version.new('1.9')
      Rake::Task['rdoc:build'].invoke
    end

#    Rake::Task['publican:build'].invoke(@spec_file)

    super
  end

  def find_erb_files(dir=@base_dir)
    to_ret = []
    Find.find(dir) do |erb|
      if erb =~ /\.erb$/ then
        to_ret << erb
      end
    end

    to_ret
  end
end

DocPkg.new( File.dirname( __FILE__ ) ) do |t|
  # Not sure this is right
  t.clean_list << "#{t.base_dir}/build_docs"
  t.clean_list << "#{t.base_dir}/html/user_guide/*"
  t.clean_list << "#{t.base_dir}/pdf"

  t.exclude_list << 'dist'
  t.exclude_list << 'build_docs'

  # Need to ignore any generated files from ERB's.
  #t.ignore_changes_list += find_erb_files.map{|x| x = "#{File.dirname(x)}/#{File.basename(x,'.erb')}".sub(/^\.\//,'')}

  Dir.glob('build/rake_helpers/*.rake').each do |helper|
    load helper
  end
end
