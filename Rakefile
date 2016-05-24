#!/usr/bin/rake -T

require 'simp/rake'
require 'yaml'
require 'find'

task 'munge:prep' do
  @doc_spec_file = 'build/simp-doc.spec'

  if File.exist?(@doc_spec_file) && !@doc_spec_file_content
    @doc_spec_file_content = File.read(@doc_spec_file)
  end

  # This is done to make sure that we have something to build from!
  default_simp_version = '5.1.X'

  tmpspec = Tempfile.new('docspec')

  begin
    specfile = '../../src/build/simp.spec'

    unless File.exist?(specfile)
      require 'open-uri'

      simp_version = ENV['SIMP_VERSION']

      unless simp_version
        simp_version = default_simp_version
      end

      spec_url = "https://raw.githubusercontent.com/simp/simp-core/#{simp_version}/src/build/simp.spec"
      begin
        open(spec_url) do |specfile|
          tmpspec.write(specfile.read)
        end
      rescue Exception
        raise(Error, "Could not find a valid spec file at #{spec_url}, check your SIMP_VERSION environment setting!")
      end

      specfile = tmpspec.path
    end

    specinfo = Simp::RPM.get_info(specfile)

    simp_version = specinfo[:version]
    simp_release = specinfo[:release]

    %x(sed -i s/__VERSION__/#{simp_version}/ #{@doc_spec_file})
    %x(sed -i s/__RELEASE__/#{simp_release}/ #{@doc_spec_file})
  ensure
    tmpspec.close
    tmpspec.unlink
  end
end

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
  t.clean_list << "#{t.base_dir}/html"
  t.clean_list << "#{t.base_dir}/html-single"
  t.clean_list << "#{t.base_dir}/pdf"
  t.clean_list << "#{t.base_dir}/sphinx_cache"
  t.clean_list << "#{t.base_dir}/docs/dynamic"

  t.exclude_list << 'dist'
  # Need to ignore any generated files from ERB's.
  #t.ignore_changes_list += find_erb_files.map{|x| x = "#{File.dirname(x)}/#{File.basename(x,'.erb')}".sub(/^\.\//,'')}

  Dir.glob('build/rake_helpers/*.rake').each do |helper|
    load helper
  end
end

def process_rpm_yaml(rel)
  fail("Must pass release to 'process_rpm_yaml'") unless rel

  rpm_data = Dir.glob("../../build/yum_data/SIMP*#{rel}*/packages.yaml")

  data = ['Not Found,Unknown']
  unless rpm_data.empty?
    data = YAML.load_file(rpm_data.sort_by{|filename| File.mtime(filename)}.last)
    data = data.values.map{|x| x = x[:rpm_name] + ',' + x[:source]}
  end

  fh = File.open(File.join('docs','security_conop','RPM_Lists',%(#{rel}.csv)),'w')
  fh.puts(data.join("\n"))
  fh.sync
  fh.close
end

namespace :docs do
  namespace :rpm do
    desc 'Update the RPM lists'
    task :external do
      ['RHEL','CentOS'].each do |rel|
        process_rpm_yaml(rel)
      end
    end

    desc 'Update the SIMP RPM list'
    task :simp do
      simp_version = Simp::RPM.get_info('build/simp-doc.spec')[:version]
      default_data = ['Unknown,Unknown,Unknown']
      collected_data = []

      if File.directory?('../../src/build')
        default_metadata = YAML.load_file('../../src/build/package_metadata_defaults.yaml')

        Find.find('../../') do |path|
          path_basename = File.basename(path)

          # Ignore hidden directories
          unless path_basename == '..'
            Find.prune if path_basename[0].chr == '.'
          end

          # Ignore spec tests
          Find.prune if path_basename == 'spec'

          # Only Directories
          Find.prune unless File.directory?(path)
          # Ignore symlinks (this may be redundant on some systems)
          Find.prune if File.symlink?(path)

          build_dir = File.join(path,'build')
          if File.directory?(build_dir)
            Dir.chdir(path) do
              # Update the metadata for this RPM
              rpm_metadata = default_metadata.dup
              if File.exist?('build/package_metadata.yaml')
                rpm_metadata.merge!(YAML.load_file('build/package_metadata.yaml'))
              end

              valid_rpm = false
              Array(rpm_metadata['valid_versions']).each do |version_regex|
                if Regexp.new("^#{version_regex}$").match(simp_version)
                  valid_rpm = true
                  break
                end
              end

              if valid_rpm
                # Use the real RPMs if we have them
                rpms = Dir.glob('dist/*.rpm')
                rpms.delete_if{|x| x =~ /\.src\.rpm$/}
                if rpms.empty?
                  Dir.glob('build/*.spec').each do |rpm_spec|
                    pkginfo = Simp::RPM.get_info(rpm_spec)
                    pkginfo[:metadata] = rpm_metadata
                    collected_data << pkginfo
                  end
                else
                  rpms.each do |rpm|
                    pkginfo = Simp::RPM.get_info(rpm)
                    pkginfo[:metadata] = rpm_metadata
                    collected_data << pkginfo
                  end
                end
              end
            end
          end
        end
      end

      if collected_data.empty?
        collected_data = default_data
      else
        # Create the necessary CSV format
        collected_data.sort_by!{|x| x[:name]}
        collected_data.map!{|x| x = [x[:name],x[:full_version],!x[:metadata]['optional']].join(',')}
      end

      fh = File.open(File.join('docs','user_guide','SIMP_RPM_List.csv'),'w')
      # Highlight those items that are always there
      fh.puts(collected_data.join("\n").gsub(',true',',**true**'))
      fh.sync
      fh.close
    end
  end

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
    cmd = "sphinx-build -T -E -n #{extra_args} -b pdf -d sphinx_cache docs pdf"
    puts "== #{cmd}"
    %x(#{cmd} > /dev/null)
  end

  desc 'run a local web server to view HTML docs on http://localhost:5000'
  task :server, [:port] do |_t, args|
    port = args.to_hash.fetch(:port, 5000)
    puts "running web server on http://localhost:#{port}"
    %x(ruby -run -e httpd html/ -p #{port})
 end
end

# We want to prep for build if possible, but not when running `rake -T`, etc...
Rake.application.tasks.select{|task| task.name.start_with?('docs:', 'pkg:')}.each do |task|
  task.enhance ['munge:prep'] do
    # Restore the original spec file
    File.open(@doc_spec_file, 'w'){ |fh| fh.write(@doc_spec_file_content) }
  end
end
