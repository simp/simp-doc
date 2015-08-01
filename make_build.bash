# build reStructuredText documentation
sphinx-build -E -n -t simp_4 -b html -d _build_v4/sphinx_cache docs/ _build_v4/html
sphinx-build -E -n -t simp_5 -b html -d _build_v4/sphinx_cache docs/ _build_v5/html
firefox _build_v4/html/index.html &
firefox _build_v5/html/index.html &

# build the 3 sub-docs and a large one
sphinx-build -E -n -t simp_5 -b singlehtml -d _build_v5/sphinx_cache docs/user_guide _build_v5/html/user
sphinx-build -E -n -t simp_5 -b singlehtml -d _build_v5/sphinx_cache docs/installation_guide _build_v5/html/install
sphinx-build -E -n -t simp_5 -b singlehtml -d _build_v5/sphinx_cache docs/security_conop _build_v5/html/security
sphinx-build -E -n -t simp_5 -b singlehtml -d _build_v5/sphinx_cache docs _build_v5/html/manual

# build the 3 sub-docs and a large one
sphinx-build -E -n -t simp_4 -b singlehtml -d _build_v4/sphinx_cache docs/user_guide _build_v4/html/user
sphinx-build -E -n -t simp_4 -b singlehtml -d _build_v4/sphinx_cache docs/installation_guide _build_v4/html/install
sphinx-build -E -n -t simp_4 -b singlehtml -d _build_v4/sphinx_cache docs/security_conop _build_v4/html/security
sphinx-build -E -n -t simp_4 -b singlehtml -d _build_v4/sphinx_cache docs _build_v4/html/manual


