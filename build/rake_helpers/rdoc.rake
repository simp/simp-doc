# Puppet doc specific tasks
namespace :rdoc do

  verbose(false)

  module_dir = File.expand_path(File.dirname(__FILE__) + '/../puppet/modules')
  basedir = File.expand_path(File.dirname(__FILE__) + '/../../')

  task :clean do
    FileUtils.rm_rf('html/developers_guide/rdoc')
  end

  desc <<-EOM
    Builds the SIMP API documentation.

    This is not yet incorporated into the PDF but will be in the future.
  EOM
  task :build do
    tmpdir = "/tmp/simp_puppetdoc_#{Time.now.hash}"
    if File.exists?(tmpdir) then
      FileUtils.rm_rf(tmpdir, :secure => true)
    end
    FileUtils.mkdir(tmpdir)

    # For this to work properly, some files need to stubbed
    Dir.chdir(tmpdir) do
      FileUtils.touch('config')
      FileUtils.mkdir('manifests')
      FileUtils.mkdir('modules')

      # Link in all of the modules.
      Dir.chdir('modules') do
        Dir.glob("#{module_dir}/*") do |dir|
          if File.directory?(dir) then
            FileUtils.mkdir(File.basename(dir))
            Dir.chdir(File.basename(dir)) do
              FileUtils.ln_s("#{dir}/lib",'lib') if File.exist?("#{dir}/lib")
              FileUtils.ln_s("#{dir}/manifests",'manifests') if File.exist?("#{dir}/manifests")
            end
          end
        end
      end

      begin
        sh %{puppet doc --all --mode rdoc --outputdir rdoc --modulepath modules --manifestdir manifests --config config}
      ensure
        if (Gem::Version.new(RUBY_VERSION) >= Gem::Version.new('1.9')) then
          # In Ruby > 1.8.7, crufty _pp.html files are left all over the place.
          sh %{find -L modules -type f \\( -name "*_pp.html" -o -name "*rb.html" \\) -exec rm \{\} \\;}
        end
      end

      FileUtils.cp_r("rdoc/","#{basedir}/html/developers_guide/")
    end

    # Remove empty cruft
    FileUtils.rm_rf("#{basedir}/html/developers_guide/rdoc/files/tmp")

    # Now, we have to go through and reassign everything to the real system file location.
    # No, this isn't portable, but most of this isn't so we're not going to
    # worry about purity right now.
    sh %{find #{basedir}/html/developers_guide/rdoc -name "*.html" -exec sed -i 's|\\(\\.\\./\\?\\)\\+/files/tmp/simp_puppetdoc_[[:digit:]]\\+\\(.*\\)_pp\\.html|file:///etc/puppet\\2.pp|g' {} \\;}
    sh %{find #{basedir}/html/developers_guide/rdoc -name "*.html" -exec sed -i 's|/tmp/simp_puppetdoc_[[:digit:]]\\+|/etc/puppet|g' {} \\;}

    FileUtils.rm_rf(tmpdir, :secure => true)
  end
end
