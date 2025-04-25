# meson.build

project('acmewm', 'c',
  version: '0.1',
  default_options: ['warning_level=2', 'buildtype=debug'],

)

# Find dependencies
wayland_scanner = find_program('wayland-scanner')
wlroots = dependency('wlroots-0.18')
wayland_protocols = dependency('wayland-protocols', required: true)
wayland_server= dependency('wayland-server', required: true)
wayland_client= dependency('wayland-client', required: true)

dep = [ wlroots, wayland_protocols, wayland_server, wayland_client]

inc_dir = include_directories('include')

xdg_protocol_xml = files('protocols/xdg-shell.xml')

xdg_h = custom_target('xdg-shell-protocol.h',
  input: xdg_protocol_xml,
  output: 'xdg-shell-protocol.h',
  command: ['wayland-scanner', 'client-header', '@INPUT@', '@OUTPUT@']
)

xdg_c = custom_target('xdg-shell-protocol.c',
  input: xdg_protocol_xml,
  output: 'xdg-shell-protocol.c',
  command: ['wayland-scanner', 'private-code', '@INPUT@', '@OUTPUT@']
)

cursor_shape_v1_protocol_xml = files('protocols/cursor-shape-v1.xml')

cursor_shape_h = custom_target('cursor-shape-protocol.h',
  input: cursor_shape_v1_protocol_xml,
  output: 'cursor-shape-v1-protocol.h',
  command: ['wayland-scanner', 'client-header', '@INPUT@', '@OUTPUT@']
)

cursor_shape_c = custom_target('cursor-shape-protocol.c',
  input: cursor_shape_v1_protocol_xml,
  output: 'cursor-shape-v1-protocol.c',
  command: ['wayland-scanner', 'private-code', '@INPUT@', '@OUTPUT@']
)

src = ['src/main.c', 'src/wm.c', 'src/output.c', 'src/surface.c', xdg_h, xdg_c, cursor_shape_h, cursor_shape_c]

args = ['-DWLR_USE_UNSTABLE']

executable('acmewm',
  sources: src,
  include_directories: inc_dir,
  dependencies: dep,
  c_args: args,
  install: false
)

