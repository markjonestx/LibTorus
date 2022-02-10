project(
  'libtorus', 'cpp',
  version: '0.1.0',
  meson_version: '>= 0.50.0',
  default_options:
    [ 'warning_level=3',
      'cpp_std=gnu++20',
      'optimization=2'
    ],
)

# Setup document generation
doxygen = find_program('doxygen', required : false)
if doxygen.found()
  message('Doxygen found')
  run_target('docs', command : [doxygen, meson.source_root() + '/Doxyfile'])
else
  warning('Documentation is disabled without Doxygen')
endif



subdir('src')
subdir('test')
