# Install:
### $ python setup.py build
### $ python setup.py install

# Usage:
## compress & decompress:
##### >>> import huffmanModule
##### >>> 
##### >>> file_in = 'original_file.jpg'
##### >>> file_out = 'compressed_file.hum.bin'
##### >>> file_out_out = 'decompressed_file.hum.jpg'
##### >>> rtn = huffmanModule.compress(file_in, file_out)
##### >>> print('compress return:', rtn)
##### >>> rtn = huffmanModule.decompress(file_out, file_out_out)
##### >>> print('decompress return:', rtn)
