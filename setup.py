from distutils.core import setup, Extension

setup(
    name='pyhuffman', 
    version='1.0', 
    ext_modules=[Extension('pyhuffman', ['huffman.c','huffmanWrapper.c'])]
)
