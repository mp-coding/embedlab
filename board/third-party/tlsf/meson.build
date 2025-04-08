inc = include_directories('.')

tlsf = library(
    'tlsf',
    sources : 'tlsf.c',
    include_directories : inc)

tlsf_dep = declare_dependency(link_with : tlsf, include_directories : inc)
