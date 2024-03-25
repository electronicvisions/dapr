from os.path import join
from waflib.extras.test_base import summary
from waflib.extras.symwaf2ic import get_toplevel_path


def depends(dep):
    dep('code-format')
    dep('hate')


def options(opt):
    opt.load('compiler_cxx')
    opt.load('gtest')
    opt.load("doxygen")

    hopts = opt.add_option_group('dapr options')
    hopts.add_withoption('dapr-python-bindings', default=True,
                         help='Toggle the generation and build of dapr python bindings')
    opt.load('pytest')
    opt.load('pylint')
    opt.load('pycodestyle')

def configure(cfg):
    cfg.load('compiler_cxx')
    cfg.load('gtest')
    cfg.load("doxygen")

    if getattr(cfg.options, 'with_dapr_python_bindings', True):
        cfg.load('python')
        cfg.check_python_version()
        cfg.check_python_headers()
        cfg.load('genpybind')
        cfg.load('pytest')
        cfg.load('pylint')
        cfg.load('pycodestyle')

    cfg.env.with_dapr_python_bindings = cfg.options.with_dapr_python_bindings

    cfg.env.CXXFLAGS_DAPR = [
        '-fvisibility=hidden',
        '-fvisibility-inlines-hidden',
    ]
    cfg.env.LINKFLAGS_DAPR = [
        '-fvisibility=hidden',
        '-fvisibility-inlines-hidden',
    ]

def build(bld):
    bld.install_files(
        dest='${PREFIX}/',
        files=bld.path.ant_glob('include/dapr/**/*.(h)'),
        name='dapr_header',
        relative_trick=True
    )

    bld(
        target='dapr_inc',
        features='use',
        export_includes='include',
        use=['dapr_header', 'hate_inc'],
    )

    bld.shlib(
        source=bld.path.ant_glob('src/**/*.cpp'),
        target='dapr',
        use='dapr_inc',
        uselib='DAPR',
    )

    # unit tests
    bld(
        features='cxx cxxprogram gtest',
        source=bld.path.ant_glob('tests/**/*.cpp'),
        target='dapr_tests',
        use='dapr',
    )

    if bld.env.with_dapr_python_bindings:
        bld.shlib(
            source=bld.path.ant_glob('src/pydapr/*.cpp'),
            target='pydapr_cpp',
            use='dapr',
        )

        bld(
            target         = 'pydapr',
            features       = 'genpybind cxx cxxshlib pyext',
            source         = 'include/pydapr/pydapr.h',
            genpybind_tags = 'pydapr',
            use            = 'pydapr_cpp',
        )

        print(bld.path.ant_glob('pydapr/tests/*.py'))
        bld(
            name="pydapr_tests",
            tests=bld.path.ant_glob('tests/pydapr/*.py'),
            features='use pytest pylint pycodestyle',
            use='pydapr',
            install_path='${PREFIX}/bin',
            pylint_config=join(get_toplevel_path(), "code-format", "pylintrc"),
            pycodestyle_config=join(get_toplevel_path(), "code-format", "pycodestyle")
        )

    # documentation
    bld(
        target='doxygen_dapr',
        features='doxygen',
        doxyfile=bld.root.make_node(join(get_toplevel_path(), "code-format", "doxyfile")),
        doxy_inputs='include/dapr',
        install_path='doc/dapr',
        pars={
            "PROJECT_NAME": "\"Data Abstractions with Polymorphic Representation library\"",
            "INCLUDE_PATH": join(get_toplevel_path(), "dapr", "include"),
            "OUTPUT_DIRECTORY": join(get_toplevel_path(), "build", "dapr", "doc")
        },
    )

    bld.add_post_fun(summary)
