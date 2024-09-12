# SPDX-License-Identifier: MIT
project('acews-demo',
    ['c'],
    meson_version: '>=0.60.0',
    version: files('version.txt'),
)

acews_proj = subproject('acews')
bin2c_py = acews_proj.get_variable('bin2c_py')

server_private_public_pem = custom_target(
    output: ['server-private.pem', 'server-public.pem'],
    command: ['openssl', 'req', '-newkey', 'rsa:2048', '-days', '3650', '-nodes', '-x509', '-subj', '/C=US/ST=Denial/L=Springfield/O=Dis/CN=localhost', '-keyout', '@OUTPUT0@', '-out', '@OUTPUT1@'],
)

server_private_der = custom_target(
    input: server_private_public_pem[0],
    output: 'server-private.der',
    command: ['openssl', 'rsa', '-outform', 'der', '-in', '@INPUT@', '-out', '@OUTPUT@'],
)

server_public_der = custom_target(
    input: server_private_public_pem[1],
    output: 'server-public.der',
    command: ['openssl', 'x509', '-outform', 'der', '-in', '@INPUT@', '-out', '@OUTPUT@'],
)

server_private_der_c = custom_target(
    input: server_private_der,
    output: 'server-private-der.c',
    command: [bin2c_py, '@INPUT@', '@OUTPUT@'],
)

server_public_der_c = custom_target(
    input: server_public_der,
    output: 'server-public-der.c',
    command: [bin2c_py, '@INPUT@', '@OUTPUT@'],
)

depends = []
sources = [
    server_private_der_c,
    server_public_der_c,
]
subdir('src')

depends += [
    dependency('acews'),
]

acews_demo = executable('acews-demo',
    sources,
    dependencies: depends,
)

docs = acews_proj.get_variable('docs')

run_target('docs',
    depends: docs,
    command: ['true'],
)

run_target('browse',
    depends: docs,
    command: ['sensible-browser', 'file://' / docs.full_path() / 'html' / 'index.html'],
)

run_target('run',
    depends: acews_demo,
    command: [acews_demo],
)

run_target('debug',
    depends: acews_demo,
    command: ['gdb', '-ex', 'r', acews_demo],
)
