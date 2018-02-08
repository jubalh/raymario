project('raymario', 'c', version: '1.0',
        meson_version: '>= 0.39.1')

cc = meson.get_compiler('c')

# find dependencies
glfw_dep = dependency('glfw3')
gl_dep = dependency('gl')
openal_dep = dependency('openal')
m_dep = cc.find_library('m', required : false)
raylib_dep = cc.find_library('raylib', required : false)

prefix = get_option('prefix')

conf = configuration_data()
conf.set('PREFIX', prefix,
         description : 'Installation prefix.')
conf.set('RESOURCE_DIR', '"' + prefix + '/share/raymario/resources"',
         description : 'Resource directory contains fonts, images, sounds, maps. Resource files will be installed in $PREFIX/share/raymario/resources')

config_h = configure_file(output : 'config.h', configuration : conf)

source_c = [
  'src/main.c',
  'src/level.c',
  'src/screens/title_screen.c',
  'src/screens/loading_screen.c',
  'src/screens/gameplay_screen.c',
  'src/screens/win_screen.c',
  'src/screens/end_screen.c',
  'src/libraries/physac.c',
  config_h
]

# build executable
raymario = executable('raymario',
  source_c,
  dependencies : [ raylib_dep, glfw_dep, gl_dep, openal_dep, m_dep ],
  install : true)

# install resources directory to prefix/share/raymario
install_subdir('resources', install_dir : 'share/raymario')
