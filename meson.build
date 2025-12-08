project(
  'hyprbar',
  'cpp',
  version : '0.1',
  meson_version : '>= 1.3.0',
  default_options : ['warning_level=3', 'cpp_std=c++23'],
)

add_project_arguments('-DGLOBAL_DEFINE', '-Wall', language: 'cpp')

dependencies = [
  dependency('libudev'),
  dependency('hyprland'),
  dependency('wayland-client'),
  dependency('wayland-protocols'),
  dependency('hyprlang', version: '>=0.6.0'),
  dependency('hyprutils', version: '>=0.2.4'),
  dependency('hyprtoolkit', version: '>=0.2.2'),
  dependency('pixman-1'),
  dependency('libdrm'),
  dependency('threads'),
  dependency('fmt', version : ['>=8.1.1'], fallback : ['fmt', 'fmt_dep']),
  dependency('spdlog', version : ['>=1.15.2'], fallback : ['spdlog', 'spdlog_dep'], default_options : ['std_format=disabled', 'tests=disabled']),
  dependency('jsoncpp', version : ['>=1.9.2'], fallback : ['jsoncpp', 'jsoncpp_dep']),
]

exe = executable(
  'hyprbar',
  'src/main.cpp',
  'src/json.hpp',
  'src/IPC.hpp',
  'src/IPC.cpp',
  'src/elements/ActiveWindow.cpp',
  'src/elements/Battery.cpp',
  'src/elements/Workspaces.cpp',
  'src/elements/Clock.cpp',
  'src/Modules.cpp',
  'src/Hyprbar.cpp',
  install : true,
  dependencies : dependencies,
)

test('basic', exe)
