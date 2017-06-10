project('raymario', 'c', version: '1.0',
        meson_version: '>= 0.39.1')

cc = meson.get_compiler('c')

glfw_dep = dependency('glfw3')
gl_dep = dependency('gl')
openal_dep = dependency('openal')
m_dep = cc.find_library('m', required : false)
raylib_dep = cc.find_library('raylib', required : false)

source_c = [
  'src/main.c',
  'src/level.c',
  'src/title_screen.c',
  'src/loading_screen.c',
  'src/gameplay_screen.c',
  'src/win_screen.c',
  'src/end_screen.c'
]

raymario = executable('raymario',
  source_c,
  dependencies : [ raylib_dep, glfw_dep, gl_dep, openal_dep, m_dep ],
  install : true)
