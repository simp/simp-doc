# build reStructuredText documentation
sphinx-build -n -t simp_4 -b html -d _build_v4/sphinx_cache docs/ _build_v4/html
sphinx-build -n -t simp_5 -b html -d _build_v4/sphinx_cache docs/ _build_v5/html
firefox _build_v4/html/index.html &
firefox _build_v5/html/index.html &
touch *rst */*rst */*/*rst */*/*/*rst */*/*/*/*rst

# build the 3 sub-docs and a large one
sphinx-build -n -t simp_5 -b singlehtml -d _build_v5/sphinx_cache docs/user_guide _build_v5/html/user
sphinx-build -n -t simp_5 -b singlehtml -d _build_v5/sphinx_cache docs/installation_guide _build_v5/html/install
sphinx-build -n -t simp_5 -b singlehtml -d _build_v5/sphinx_cache docs/security_conop _build_v5/html/security
touch *rst */*rst */*/*rst */*/*/*rst */*/*/*/*rst
sphinx-build -n -t simp_5 -b singlehtml -d _build_v5/sphinx_cache docs _build_v5/html/manual

touch *rst */*rst */*/*rst */*/*/*rst */*/*/*/*rst
# build the 3 sub-docs and a large one
sphinx-build -n -t simp_4 -b singlehtml -d _build_v4/sphinx_cache docs/user_guide _build_v4/html/user
sphinx-build -n -t simp_4 -b singlehtml -d _build_v4/sphinx_cache docs/installation_guide _build_v4/html/install
sphinx-build -n -t simp_4 -b singlehtml -d _build_v4/sphinx_cache docs/security_conop _build_v4/html/security
touch *rst */*rst */*/*rst */*/*/*rst */*/*/*/*rst
sphinx-build -n -t simp_4 -b singlehtml -d _build_v4/sphinx_cache docs _build_v4/html/manual


