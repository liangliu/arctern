from distutils.core import setup, Extension
import os
import numpy as np
import pyarrow as pa

# MOD = "zilliz_gis"

real_path = os.path.realpath(__file__)
dir_path = os.path.dirname(real_path)

from distutils.core import setup
from Cython.Build import cythonize


gis_core_modules = cythonize("./cython/zilliz_gis_core.pyx", compiler_directives = {'language_level': 3})

for ext in gis_core_modules:
    # The Numpy C headers are currently required
    ext.include_dirs.append(np.get_include())
    ext.include_dirs.append(pa.get_include())
    ext.include_dirs.append(dir_path + "/../../core/src/gis")
    ext.include_dirs.append(dir_path + "/../../core/src/render")
    ext.libraries.extend(pa.get_libraries())
    ext.library_dirs.extend(pa.get_library_dirs())

    if os.name == 'posix':
        ext.extra_compile_args.append('-std=c++11')

    # Try uncommenting the following line on Linux
    # if you get weird linker errors or runtime crashes
    ext.define_macros.append(("_GLIBCXX_USE_CXX11_ABI", "0"))


setup(
    name = "zilliz_gis",
    py_modules = ['register'],
    ext_modules=cythonize(gis_core_modules)
)

