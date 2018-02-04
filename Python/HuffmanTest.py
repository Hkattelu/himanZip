import sys
from HuffmanTree import HuffmanTreeNode
import HuffmanCodec

# Test file to make sure that the huffman tree is running without errors
freqs = {'a':10, 'b':12, 'c':9, 'd':5, 'e':20}
Tree = HuffmanTreeNode.createFromFrequencies(freqs)
codeDict = Tree.getEncodeMap()
bitArray = Tree.toBitArray()
Tree = Tree.fromBitArray(bitArray)

encoding = HuffmanCodec.encode('example.txt')
HuffmanCodec.bitArrayToFile(encoding, 'example.huff')
print(encoding)
text = HuffmanCodec.decode('example.huff')
print(text)
HuffmanCodec.bitArrayToFile(encoding, 'example-decoded.txt')