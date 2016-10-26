gem_sources   = ENV.key?('SIMP_GEM_SERVERS') ? ENV['SIMP_GEM_SERVERS'].split(/[, ]+/) : ['https://rubygems.org']
gem_sources.each { |gem_source| source gem_source }

gem 'rake', ENV.fetch('RAKE_VERSION', '~> 10')
gem 'simp-rake-helpers', ENV.fetch('SIMP_RAKE_HELPERS_VERSION', '~> 2.5')
gem 'puppet',  ENV.fetch('PUPPET_VERSION', '~>3')
