project(
  'hyprbar',
  'cpp',
  version : '0.1',
  meson_version : '>= 1.3.0',
  default_options : ['warning_level=3', 'cpp_std=c++23'],
)

dependencies = [
  dependency('hyprland'),
  dependency('wayland-client'),
  dependency('wayland-protocols'),
  dependency('hyprlang', version: '>=0.6.0'),
  dependency('hyprutils', version: '>=0.2.4'),
  dependency('hyprtoolkit', version: '>=0.2.2'),
  dependency('pixman-1'),
  dependency('libdrm'),
  dependency('threads')
]

exe = executable(
  'hyprbar',
  'main.cpp',
  install : true,
  dependencies : dependencies,
)

test('basic', exe)
