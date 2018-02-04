from queue import PriorityQueue

class HuffmanQueue(PriorityQueue):
	"""
	Class that can be used as a priority queue to store Huffman
	Tree nodes with priority being the frequency of the node element
	"""
	def __init__(self):
		"""Initialize an empty priority queue"""
		PriorityQueue.__init__(self)
		self.size = 0

		# Use a count to compare priorities if two nodes
		# have the same number of character occurences. The
		# count should not matter because all we care is that the
		# occurences are the same, so we will make the count unique
		# by adding 1 to it each time we put into the queue
		self.count = 0

	def put(self, item):
		"""Put a node with a specified priority into the queue"""
		# Insert a tuple of the frequency, the count, and the node itself
		PriorityQueue.put(self, (item.elementCount, self.count, item))
		self.size += 1
		self.count += 1

	def get(self):
		# Only return the node because that is all we care about
		_, _, item = PriorityQueue.get(self)
		self.size -= 1
		return item

	def getSize(self):
		return self.size