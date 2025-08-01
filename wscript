from os.path import join
from waflib.extras.test_base import summary
from waflib.extras.symwaf2ic import get_toplevel_path


def depends(dep):
    dep('code-format')


def options(opt):
    opt.load('compiler_cxx')
    opt.load('gtest')
    opt.load("doxygen")


def configure(cfg):
    cfg.load('compiler_cxx')
    cfg.load('gtest')
    cfg.load("doxygen")


def build(bld):
    bld.install_files(
        dest='${PREFIX}/',
        files=bld.path.ant_glob('include/dapr/**/*.(h)'),
        name='dapr_header',
        relative_trick=True
    )

    bld(
        target='dapr_inc',
        export_includes='include',
        use='dapr_header',
    )

    bld.shlib(
        source=bld.path.ant_glob('src/**/*.cpp'),
        target='dapr',
        use='dapr_inc',
    )

    # unit tests
    bld(
        features='cxx cxxprogram gtest',
        source=bld.path.ant_glob('tests/**/*.cpp'),
        target='dapr_tests',
        use='dapr',
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
