# meson.build

project('acmewm', 'c',
  version: '0.1',
  default_options: ['warning_level=2', 'buildtype=debug'],
  license: 'MIT'
)

# Find dependencies
wayland_scanner = find_program('wayland-scanner')
wlroots = dependency('wlroots-0.18')
wayland_protocols = dependency('wayland-protocols', required: true)
wayland_server= dependency('wayland-server', required: true)
wayland_client= dependency('wayland-client', required: true)

dep = [ wlroots, wayland_protocols, wayland_server, wayland_client]

# Create an include directory for the project
inc_dir = include_directories('include')

# Define the protocol XML files
protocols = [
  {
    'name': 'xdg-shell',
    'xml': 'protocols/xdg-shell.xml',
  }
]

generated_protocol_sources = []
generated_protocol_headers = []

# Generate the protocol headers
#foreach proto : protocols
  #server_header = custom_target(proto['name'] + '-server_header',
    #input: proto['xml'],
    #output: proto['name'] + '-server-protocol.h',
    #command: [wayland_scanner, '-server-header', '@INPUT@', '@OUTPUT@']
  #)
#
 #client_header = custom_target(proto['name'] + '-client_header',
    #input: proto['xml'],
    #output: proto['name'] + '-client-protocol.h',
    #command: [wayland_scanner, '-client-header', '@INPUT@', '@OUTPUT@']
  #)
#
  #private_code = custom_target(proto['name'] + '-code',
    #input: proto['xml'],
    #output: [proto['name'] + '-protocol.c'],
    #command: [wayland_scanner, 'private-code', '@INPUT@', '@OUTPUT@']
  #)
#
  #generated_protocol_sources += private_code
  #generated_protocol_headers += [server_header, client_header]
#endforeach

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

# Compile the source files for the window manager
executable('acmewm',
  sources: ['src/main.c', 'src/wm.c', 'src/output.c', 'src/surface.c', xdg_h, xdg_c],
# + generated_protocol_sources + generated_protocol_headers,
  include_directories: inc_dir,
  dependencies: dep,
  c_args: ['-DWLR_USE_UNSTABLE'],  # Enable unstable features for wlroots
  install: false
)

