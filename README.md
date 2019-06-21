# Install from source:
- $ python setup.py build
- $ python setup.py install
----
# Usage:
- compress & decompress:<br>
**python example:**
```python
#-*- coding:utf-8 -*-
import huffmanModule


def run_test():
    file_in = 'original_file.jpg'
    file_out = 'compressed_file.hum.bin'
    file_out_out = 'decompressed_file.hum.jpg'
    rtn = huffmanModule.compress(file_in, file_out)
    print('compress return:', rtn)
    rtn = huffmanModule.decompress(file_out, file_out_out)
    print('decompress return:', rtn)


if __name__ == '__main__':
    run_test()
```
# Upload PyPI
- $ python -m pip install --user --upgrade setuptools wheel twine
- $ python setup.py sdist bdist_wheel
- $ python -m twine upload --repository-url https://upload.pypi.org/legacy/ dist/*
----
# Upload TestPyPI
- $ python -m pip install --user --upgrade setuptools wheel twine
- $ python setup.py sdist bdist_wheel
- $ python -m twine upload --repository-url https://test.pypi.org/legacy/ dist/*
- $ python -m pip install --index-url https://test.pypi.org/simple/ --no-deps PycHuffman
