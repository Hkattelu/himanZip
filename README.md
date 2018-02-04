# himanZip
A command line file compressor that uses Huffman encoding.

<h3> How to use the C version </h3>
Start off by downloading the compressor. Once downloaded, make it with gcc. 
You can then use the binary on any file with read/write permissions.
Files will be compressed to type .hzip, and can be decompressed with the 
same binary by using the -d flag.

Compressing: ./himanZip file.txt
Decompressing: ./himanZip -d file.txt.hzip

<h3> Setting up the python version </h3>

To run the code you may have to first install a python package called bitstring.
The purpose of bitstring is simply to allow easier manipulation of bits in python
for the encoding and decoding.

You should be able to easily install the package with
	easy_install bitstring or
	pip install bitstring

If those don't work, then you can get it here: https://github.com/scott-griffiths/bitstring
Just download, unzip, and run
	python setup.py install

<h3> Running the python version </h3>

To encode a text file, run
	
	python HuffmanCodec.py --encode filename.txt

This will generate a new file called filename.huff which is simply the encoded file.

To decode the huff file, run

	python HuffmanCodec.py --decode filename.huff

This will restore the encoded file and call it filename-decoded.txt.