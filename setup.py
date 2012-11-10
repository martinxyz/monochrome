#!/usr/bin/env python3
from distutils.core import setup, Extension
from glob import glob

setup(ext_modules = [
        Extension("_monofast",
                  sources = ["monofast.i"],
                  depends = glob("*.hpp") + ["setup.py"],
                  swig_opts = ["-c++"],
                  undef_macros = ["NDEBUG"], # make assert() work
                  #extra_compile_args = ["-O3"],
                  extra_compile_args = ["-mtune=native"],
                  )
        ])
