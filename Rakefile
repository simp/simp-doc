#!/usr/bin/rake -T

require 'simp/rake'

class DocPkg < Simp::Rake::Pkg
  # We need to inject the SCL Python repos for RHEL6 here if necessary
  def mock_pre_check(chroot, *args)
    mock_cmd = super(chroot, *args)

    rh_version = %x(#{mock_cmd} -r #{chroot} -q --chroot 'cat /etc/redhat-release | cut -f3 -d" " | cut -f1 -d"."').chomp

    # This is super fragile
    if rh_version.to_i == 6
      python_repo = 'rhscl-python27-epel-6-x86_64'

      puts "NOTICE: You can ignore any errors relating to RPM commands that don't result in failure"
      %x(#{mock_cmd} -q -r #{chroot} --chroot 'rpmdb --rebuilddb')
      sh  %(#{mock_cmd} -q -r #{chroot} --chroot 'rpm --quiet -q yum') do |ok,res|
        unless ok
          %x(#{mock_cmd} -q -r #{chroot} --install yum)
        end
      end

      sh %(#{mock_cmd} -q -r #{chroot} --chroot 'rpm --quiet -q #{python_repo}') do |ok,res|
        unless ok
          %x(#{mock_cmd} -q -r #{chroot} --install 'https://www.softwarecollections.org/en/scls/rhscl/python27/epel-6-x86_64/download/#{python_repo}.noarch.rpm')
        end
      end

      sh %(#{mock_cmd} -q -r #{chroot} --chroot 'rpm --quiet -q python27') do |ok,res|
        unless ok
          # Fun Fact: Mock (sometimes) adds its default repos to /etc/yum/yum.conf and ignores anything in yum.repos.d
          puts %x(#{mock_cmd} -q -r #{chroot} --chroot 'cat /etc/yum.repos.d/#{python_repo}.repo >> /etc/yum/yum.conf && yum install -qy python27')
        end
      end
    end

    mock_cmd
  end

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
  t.clean_list << "#{t.base_dir}/sphinx_cache"

  t.exclude_list << 'dist'
  # Need to ignore any generated files from ERB's.
  #t.ignore_changes_list += find_erb_files.map{|x| x = "#{File.dirname(x)}/#{File.basename(x,'.erb')}".sub(/^\.\//,'')}

  Dir.glob('build/rake_helpers/*.rake').each do |helper|
    load helper
  end
end

namespace :docs do
  desc 'build HTML docs'
  task :html do
    extra_args = ''
    ### TODO: decide how we want this task to work
    ### version = File.open('build/simp-doc.spec','r').readlines.select{|x| x =~ /^%define simp_major_version/}.first.chomp.split(' ').last
    ### extra_args = "-t simp_#{version}" if version
    cmd = "sphinx-build -E -n #{extra_args} -b html -d sphinx_cache docs html"
    puts "== #{cmd}"
    %x(#{cmd} > /dev/null)
  end

  desc 'build HTML docs (single page)'
  task :singlehtml do
    extra_args = ''
    cmd = "sphinx-build -E -n #{extra_args} -b singlehtml -d sphinx_cache docs html-single"
    puts "== #{cmd}"
    %x(#{cmd} > /dev/null)
  end

  desc 'build PDF docs (SLOW)'
  task :pdf do
    extra_args = ''
    cmd = "sphinx-build -E -n #{extra_args} -b pdf -d sphinx_cache docs pdf"
    puts "== #{cmd}"
    %x(#{cmd} > /dev/null)
  end
end
