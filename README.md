# Install:
- $ python setup.py build
- $ python setup.py install
----
# Usage:
- compress & decompress:
python实现
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
