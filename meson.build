
project('SDL Project', 'cpp', version: '0.0.1', default_options: ['cpp_std=c++23'])

subdir('shaders')
subdir('assets')

project_description = 'A SDL3 application'

project_source = [

    'main.cpp',
    'main_helper.cpp',
    'shader.cpp',
    'mesh.cpp',
    'texture.cpp',
    'transform.cpp',
    'camera.cpp',
    'entity.cpp'

]

build_args = []

project_dependencies = [

    dependency('sdl3', version: '>=3.2.0'),
    dependency('glesv2', version: '>=3.1'),

]

executable(meson.project_name(), project_source, dependencies: project_dependencies)