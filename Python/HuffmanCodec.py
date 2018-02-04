import sys
from HuffmanTree import HuffmanTreeNode
from bitstring import BitArray

def bitArrayToFile(string, fileName):
	"""Write the given bitstring into the file with the specified filename"""
	with open(fileName, 'wb') as file:
		string.tofile(file)

def stringToFile(string, fileName):
	"""Write the given string into the file with the specified filename"""
	with open(fileName, 'w') as file:
		file.write(string)

def getCharFrequencies(string):
	"""
	Produce a map of characters to the number of times they occur given
	a specified string of character
	"""
	freqs = {}
	for char in string:
		if char in freqs:
			# If the character is in the dictionary, add one to the count
			freqs[char] += 1
		else:
			# Otherwise, add it to the dictionary with a count of 1
			freqs[char] = 1
	return freqs

def encode(filename):
	"""Encode a given .txt file and return the encoded bit array"""
	# Read the file
	fileContent = ""
	with open(filename, 'r') as file:
		fileContent = file.read()

	# Get the frequences of each character
	frequencies = getCharFrequencies(fileContent)

	# Generate the Huffman Tree and the Code map
	Tree = HuffmanTreeNode.createFromFrequencies(frequencies)
	encodemap = Tree.getEncodeMap()

	# Encode the tree to a string, and then every character in the file
	# based on the code map
	encoding = Tree.toBitArray()
	for char in fileContent:
		encoding += encodemap[char]
	return encoding

def decode(filename):
	"""Decode a given .huff file and return the decoded string"""
	# Read the file
	fileContent = ""
	with open(filename, 'rb') as file:
		fileContent = BitArray(file.read())

	# Obtain the Huffman tree from the file
	Tree = HuffmanTreeNode().fromBitArray(fileContent)

	# Use the tree to decode the rest of the bytes
	text = ""
	while True:
		decodedChar = Tree.decodeChar(fileContent)
		if decodedChar is None:
			break
		text += decodedChar

	# Return the decoded text
	return text

def main(argv):
	# Check input arguments
	if (len(argv) < 3 or argv[1] == "-h"):
		print("Usage: python HuffmanCodec.py [--encode file.txt] [-decode file.huff]")
		return 
	elif (not argv[2].endswith(".txt") and not argv[2].endswith(".huff")):
		print("File must either be .txt to encode or .huff to decode")
		return 

	# Further check input arguments
	if (argv[1] == "--encode" and argv[2].endswith(".txt")):
		# Encode the text file, then write it to a huff file
		encoding = encode(argv[2])
		newFilename = argv[2][:-4] + ".huff"
		bitArrayToFile(encoding, newFilename)
	elif (argv[1] == "--decode" and argv[2].endswith(".huff")):
		# Decode the huff file, then write it to a text file
		decoding = decode(argv[2])
		newFilename = argv[2][:-5] + "-decoded.txt"
		stringToFile(decoding, newFilename)
	else:
		print("Usage: python HuffmanCodec.py [--encode file.txt] [-decode file.huff]")

if __name__ == "__main__":
	main(sys.argv)