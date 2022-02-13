# Setup the testing application to ensure the Torus works as intended.
incdir = include_directories('../src')
unit_test = executable(
    'unittests', 'test.cpp', include_directories : incdir
)

# Adds each Unit test to meson
test('[torus]', unit_test)
