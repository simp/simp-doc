#!/usr/bin/rake -T
namespace :publican do

  verbose(false)

  require 'cgi'
  require 'erb'
  require 'find'
  require 'rexml/document'
  require 'simp/rpm'
  include REXML

  # The following two directories are for code that depends on the
  # rest of the build tree being in place. If these do not exist, a
  # warning will be issued.
  BUILD_DIR = File.expand_path(File.dirname(__FILE__) + '/../../../../build')
  MODULE_DIR = File.expand_path(File.dirname(__FILE__) + '/../../../puppet/modules')

  PUBLICAN_DIR = 'publican/'
  DOC_HEAD_DIR = 'build_docs/'
  DOC_BUILD_DIR = DOC_HEAD_DIR + "en-US/"
  DOC_TMP_HTML_DIR = DOC_BUILD_DIR + "html/"
  DOC_TMP_HTML_SINGLE_DIR = DOC_BUILD_DIR + "html-single/"
  DOC_FINAL_HTML_DIR = 'html/users_guide/'
  DOC_FINAL_HTML_SINGLE_DIR = 'html/users_guide/single/'

  desc <<-EOM
    Builds the SIMP user documentation for packaging

    This task does *not* build the PDF, the RPM does that.
  EOM
  task :build,[:spec_file, :product_number, :os_version, :cleanup, :verbose] do |t,args|
    args.with_defaults(:verbose => false)
    args.with_defaults(:cleanup => true)
    args.with_defaults(:os_version => 'STUB_OS_VERSION')

    fail "A spec file must be specified" if not args.spec_file

    verbose = args.verbose

    doc_product_number = Simp::RPM.get_info(args.spec_file)

    # Remove any previous results from running publican
    verbose and puts "Removing old publican build_docs directory to avoid conflict"
    if File.directory?("#{DOC_HEAD_DIR}") then FileUtils.rm_rf("#{DOC_HEAD_DIR}") end

    verbose and puts "Removing any previous HTML user guide documentation"
    if File.directory?("#{DOC_FINAL_HTML_DIR}") then FileUtils.rm_rf("#{DOC_FINAL_HTML_DIR}") end

    verbose and puts "Removing any previous HTML-single user guide documentation"
    if File.directory?("#{DOC_FINAL_HTML_SINGLE_DIR}") then FileUtils.rm_rf("#{DOC_FINAL_HTML_SINGLE_DIR}") end

    # Create directories for the finished results
    verbose and puts "Recreating the html and pdf directories for the final output"
    FileUtils.mkdir("#{DOC_FINAL_HTML_DIR}")
    FileUtils.mkdir("#{DOC_FINAL_HTML_SINGLE_DIR}")

    # Update the publican.cfg.
    fh = File.open("#{PUBLICAN_DIR}/publican.cfg",'w')
    fh.puts(ERB.new(File.read("#{PUBLICAN_DIR}/publican.cfg.erb"),nil,'-').result(binding))
    fh.close

    # Update the SIMP_Documentation.ent with the latest version number and
    # the RHEL release.
    fh = File.open("#{PUBLICAN_DIR}/en-US/SIMP_Documentation.ent",'w')
    fh.puts(ERB.new(File.read("#{PUBLICAN_DIR}/en-US/SIMP_Documentation.ent.erb"),nil,'-').result(binding))
    fh.close

    # Update the changelog from the template
    !File.readable?("Changelog.rst") and raise Error("Error: Could not find Changelog.rst!")

    # Have to split this into multiple files since Publican dies at 8192 bytes!
    changelog_txt = CGI.escapeHTML(File.read("Changelog.rst"))

    fh = File.open("#{PUBLICAN_DIR}/en-US/common/Changelog.xml",'w')
    fh.puts(ERB.new(File.read("#{PUBLICAN_DIR}/en-US/common/Changelog.xml.erb"),nil,'-').result(binding))
    fh.close

    build_rpm_source_appendix
    build_module_installed_appendix(doc_product_number)
  end # End of doc_build task

  desc "Generate the publican PDF"
  task :gendoc,[:version,:format,:lang] do |t,args|
    args.with_defaults(:format => 'pdf')
    args.with_defaults(:lang => 'en-US')

    Rake::Task['publican:build'].invoke(args.version)

    Dir.chdir(PUBLICAN_DIR) do
      sh %{publican build --formats=#{args.format} --langs=#{args.lang}}
    end

    FileUtils.mkdir('pdf') unless File.directory?('pdf')
    FileUtils.mv(Dir.glob("build_docs/#{args.lang}/pdf/*.pdf"),'pdf')
  end

  def build_module_installed_appendix(doc_product_number)
    stubbin_time = false
    if not File.directory?(MODULE_DIR) then
      $stderr.puts("Warning: Could not find the module directory '#{MODULE_DIR}'. Skipping installed module appendix")
      stubbin_time = true
    else
      # And now, another Appendix that holds the list of pupmod RPMs
      # installed, and not installed, by default.
      modules = {}

      module_default = YAML.load(IO.read('../build/package_metadata_defaults.yaml'))['optional']

      Dir.chdir(MODULE_DIR) do

        Dir.glob("*") do |mod_dir|
          specfile = "#{mod_dir}/build/pupmod-#{mod_dir}.spec"
          next if not File.exists?(specfile)

          # Set this to the default...by default.
          modules[mod_dir] = {
            :installed => module_default,
            :info      => Simp::RPM.get_info(specfile)
          }

          metadata = "#{mod_dir}/build/package_metadata.yaml"
          if File.exists?(metadata) then
            modules[mod_dir][:installed] = YAML.load(IO.read(metadata))['optional']
          end
        end

      end
    end

   fh = File.open("#{PUBLICAN_DIR}/en-US/user_guide/SIMP_Package_Data.xml",'w')
   fh.puts(ERB.new(File.read("#{PUBLICAN_DIR}/en-US/user_guide/SIMP_Package_Data.xml.erb"),nil,'-').result(binding))
   fh.close
  end

  def build_rpm_source_appendix
    # Go dig up the information for the RPMs and create a hash. Right
    # now, we don't have any way to figure out how to properly
    # differentiate between the distros at build time.
    #
    # This is all hard coded, which is bad, but meh...
    # TODO: Fix me
    rpm_info = {
      'Ext_RHEL' => {},
      'Ext_CentOS' => {},
      'Ext_Common' => {}
    }

    stubbin_time = false
    if not File.directory?(BUILD_DIR) then
      $stderr.puts("Warning: Could not find the build directory '#{BUILD_DIR}'. Skipping RPM source appendix")
      stubbin_time = true
    else
      Dir.chdir(BUILD_DIR) do
        rpm_info.keys.each do |dir|
          Find.find(dir) do |file|
            if file =~ /.rpm$/ and file !~ /src.rpm$/ then
              if File.exist?("#{BUILD_DIR}/#{file}.source") then
                rpm_info[dir][File.basename(file)] = File.read("#{BUILD_DIR}/#{file}.source").chomp
              else
                rpm_info[dir][File.basename(file)] = "Source Unknown - To be corrected"
              end
            end
          end
        end
      end
    end

    # Here's a lovely bunch of crap for dumping the RPM list into
    # the Appendix_RPM.xml
    fh = File.open("#{PUBLICAN_DIR}/en-US/security_conop/Appendix_RPM.xml",'w')
    fh.puts(ERB.new(File.read("#{PUBLICAN_DIR}/en-US/security_conop/Appendix_RPM.xml.erb"),nil,'-').result(binding))
    fh.close

  end

  def recursive_uptodate?(dest,src)
    raise(Exception,"Error: Cannot recursively check dates on non-existent file '#{src}'") unless File.exist?(src)
    return false unless File.exist?(dest)

    src_files = []
    dest_files = []

    Find.find(src) do |x|
      src_files << File.stat(x).mtime
    end
    Find.find(dest) do |x|
      dest_files << File.stat(x).mtime
    end

    return src_files.last == dest_files.last
  end

end
