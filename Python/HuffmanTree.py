from HuffmanQueue import HuffmanQueue
from struct import *
from bitstring import BitArray

# Constant for the number of bits to encode at once
CHAR_LENGTH = 8

class HuffmanTreeNode:
	"""Class that represents a node in a Huffman Tree"""

	def __init__(self):
	  """
	  Initialize an empty Huffman Tree for converting elements into shorter 
	  bit-wise representations
	  """ 
	  self.element = None
	  self.elementCount = 0
	  self.left = None
	  self.right = None

	@staticmethod
	def createFromFrequencies(frequencies):
		"""
		Constructs a Huffman Tree from a given dictionary mapping an element
		to the number of times it occurs
		"""
		# Create a Huffman tree node for each element in the frequences 
		# and insert each into a priority queue (based on frequency of that element)
		pQueue = HuffmanQueue()
		for key in frequencies.keys():
			node = HuffmanTreeNode()
			node.element = key
			node.elementCount = frequencies[key]
			pQueue.put(node)

		# Repeat the Tree building process until there is only one node left in the 
		# priority queue of huffman tree nodes
		while pQueue.getSize() > 1:
			# Take the two most frequent elements
			leftNode = pQueue.get()
			rightNode = pQueue.get()

			# Create a special element with frequency as the sum of the previous two
			newNode = HuffmanTreeNode()
			newNode.elementCount = leftNode.elementCount + rightNode.elementCount

			# Make it the parent of the two elements, then insert it back
			newNode.left = leftNode
			newNode.right = rightNode
			pQueue.put(newNode)

		# The last remaining element will be the root of the tree
		return pQueue.get()

	def getEncodeMap(self):
		"""
		Return a dictionary repesentation of this huffman tree mapping every element
		in the tree to it's associated bit-array encoding
		"""
		codeDict = self._getEncodeMap({}, BitArray())
		codeDict.pop(None)
		return codeDict

	def _getEncodeMap(self, codeDict, encoding):
		"""Helper method for getCodeMap."""

		# Traversing left means a 0 bit goes into the encoding.
		if self.left is not None:
			codeDict = self.left._getEncodeMap(codeDict, encoding + BitArray('0b0'))

		# Insert current encoding into the dictionary
		codeDict[self.element] = encoding

		# Traversing right means a 1 bit goes into the encoding. 
		if self.right is not None:
			codeDict = self.right._getEncodeMap(codeDict, encoding + BitArray('0b1'))
		return codeDict

	def decodeChar(self, bitArray):
		"""Returns a character given a bit-array based on this tree's encodings"""

		# Traverse down the tree until a leaf node with an element is reached
		node = self
		while node is not None and node.element is None:
			# If the sequence ended prematurely then return None
			if (len(bitArray) > 0):
				if bitArray[0]:
					# Encountering a 1 bit means go left 
					node = node.right
				else :
					# Encountering a 1 bit means go right
					node = node.left
				del bitArray[0]
			else:
				return None

		# Return the element at the leaf node
		return node.element

	def toBitArray(self):
		"""Return a bit-array representation of this Huffman Tree"""
		return self._toBitArray(BitArray())

	def _toBitArray(self, bitArray):
		"""Helper method for toBitArray"""

		if self.left is None and self.right is None:
			# Leaf nodes are preceded by a '1'
			# Encode the actual character to a bit-array
			bitArray += BitArray('0b1')

			# Format the character to a size 8 bit-array
			bitArray += BitArray(format(ord(self.element), '#010b'))
		else:
			# Non-leaves are preceded by a '0'
			bitArray += BitArray('0b0')

		if self.left is not None:
			# Convert the left subtree
			bitArray = self.left._toBitArray(bitArray)
		if self.right is not None:
			# Convert the right subtree
			bitArray = self.right._toBitArray(bitArray)
		return bitArray

	def fromBitArray(self, huffmanTreeString):
		"""
		Construct this Huffman Tree based on a given bit-string representation
		of a Huffman Tree
		"""
		return self._fromBitArray(huffmanTreeString)

	def _fromBitArray(self, huffmanTreeString):
		"""Helper method for fromBitArray"""
		newNode = HuffmanTreeNode()
		if huffmanTreeString[0]:
			# Encountering a 1 means the next CHAR_LENGTH bytes are an element
			del huffmanTreeString[0]
			newNode.element = huffmanTreeString[0:CHAR_LENGTH].bytes.decode()
			del huffmanTreeString[0:CHAR_LENGTH]
		else:
			del huffmanTreeString[0]
			# Encountering a 0 means the two subtrees are next
			newNode.left = self._fromBitArray(huffmanTreeString)
			newNode.right = self._fromBitArray(huffmanTreeString)
		return newNode

	def toString(self):
		"""Return a human-readable string encoding of this Huffman Tree"""
		return self._toString("")

	def _toString(self, string):
		"""Helper method for toString"""
		if self.left is not None:
			string = self.left._toString(string)
		string += str(self.element) + " " + str(self.elementCount) + "\n"
		if self.right is not None:
			string = self.right._toString(string)
		return string
