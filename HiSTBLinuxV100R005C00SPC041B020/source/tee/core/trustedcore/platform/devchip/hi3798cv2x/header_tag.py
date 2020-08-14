#!/usr/bin/env python

from __future__ import print_function
import struct
import os
import sys

header_tag = 'TRSTCORE'
header_tag_size = 8

def align_image(img_name):
	img_size = os.path.getsize(img_name)
	print ("The size of image file is {0}".format(img_size))

	mod = img_size % 64
	if mod:
		img_size += 64 - mod

	with open(img_name, 'ab+') as img:
		img.seek(0,2)
		img.truncate(img_size)

	img.close()
	img_size = os.path.getsize(img_name)
	print ("After align, the size of image file is {0}".format(img_size))

def add_header_tag(src_file, dst_file):
  with open(dst_file, 'wb') as f:
    z = struct.pack('<8s',header_tag)
    f.write(z)
    f.seek(header_tag_size)

    inf = open(src_file, 'rb')
    f.write(inf.read())
    inf.close()
    f.close()

if __name__ == '__main__':
  src_file = sys.argv[1]
  dst_file = sys.argv[2]
  add_header_tag(src_file, dst_file)
  align_image(dst_file)
