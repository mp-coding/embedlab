libtlsf_inc = include_directories('.')

libtlsf = library(
    'tlsf',
    sources : 'tlsf.c',
    include_directories : libtlsf_inc)

libtlsf_dep = declare_dependency(link_with : libtlsf, include_directories : libtlsf_inc)
