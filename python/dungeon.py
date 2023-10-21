from math import floor
import random
import copy


random.seed()
MAPSIZE = 32
global pairId 
pairId = 0

class Leaf:
	def __init__(self, x, y, xend, yend, direction, depth, id):
		self.x = x
		self.y = y
		self.xend = xend
		self.yend = yend
		self.splitDirection = direction
		self.depth = depth
		self.pairId = id
	def xSize(self):
		return self.xend - self.x
	def ySize(self):
		return self.yend - self.y
	
class Corridor:
	def __init__(self, x, y, xend, yend):
		self.x = x
		self.y = y
		self.xend = xend
		self.yend = yend

def clamp(value, min, max):
	if (value < min):
		return min
	if (value > max):
		return max
	return value

def shrink(leaf):

	if (leaf.xSize() > 5):
		xmid = floor((leaf.xend + leaf.x) / 2)
		x = random.randint(leaf.x, xmid)
		xend = random.randint(xmid, leaf.xend)
		leaf.x = x
		leaf.xend = xend
	if (leaf.ySize() > 5):
		ymid = floor((leaf.yend + leaf.y) / 2)
		y = random.randint(leaf.y, ymid)
		yend = random.randint(ymid, leaf.yend)
		leaf.y = y
		leaf.yend = yend




def divide(leaf, depth, horizontal=True):
	global pairId
	pairId += 1
	x = leaf.x
	y = leaf.y
	xend = leaf.xend
	yend = leaf.yend

	if (leaf.xSize() > 5 and leaf.ySize() > 5):
		xmid = random.randint(x+2, xend -2)
		ymid = random.randint(y+2, yend -2)

		if (horizontal ):
			leaf1 = Leaf(leaf.x, leaf.y, xmid-1, leaf.yend, "h", depth, pairId)
			leaf2 = Leaf(xmid+1, leaf.y, leaf.xend, leaf.yend, "h", depth, pairId)
			return [leaf1, leaf2]
		elif ((not horizontal)):
			leaf1 = Leaf(leaf.x, leaf.y, leaf.xend, ymid-1, "v", depth, pairId)
			leaf2 = Leaf(leaf.x, ymid+1, leaf.xend, leaf.yend, "v", depth, pairId)
			return [leaf1, leaf2]
		else:
			return [leaf]
	else:
		return [leaf]


def generate(leaf, maxDepth, depth = 1):
	direction = random.choice([True, False])
	leaves = divide(leaf, depth, direction)
	roomLeaves = []
	leafTree = copy.deepcopy(leaves)
	if (depth < maxDepth):
		for dividedLeaf in leaves:
			newLeaves, newCompTree = generate(dividedLeaf, maxDepth, depth + 1)
			roomLeaves += newLeaves
			leafTree += newCompTree
	else:
		roomLeaves = leaves

	return roomLeaves, leafTree



map = [["##" for _ in range(MAPSIZE)] for _ in range(MAPSIZE)]

rootLeaf = Leaf(1,1,MAPSIZE-2, MAPSIZE-2, 0,  None, 0)

leaves, compTree = generate(rootLeaf, 5)
corridors = []


for i in range(len(compTree)):
	first = compTree[i]
	for j in range(len(compTree)):
		second = compTree[j]
		if (second.pairId == first.pairId and first != second):
			firstMidX = floor((first.x + first.xend) / 2)
			firstMidY = floor((first.y + first.yend) / 2)
			secondMidX = floor((second.x + second.xend) / 2)
			secondMidY = floor((second.y + second.yend) / 2)
			x = firstMidX
			y = firstMidY
			endx = secondMidX
			endy = secondMidY
			"""
			if (firstMidX > secondMidX):
				x = secondMidX
				endx = firstMidX
			if (firstMidY > secondMidY):
				y = secondMidY
				endy = firstMidY
			"""
			corridor = Corridor(x, y, endx, endy)
			corridors.append(corridor)
			break


for corridor in corridors:
	for y in range(corridor.y, corridor.yend+1):
		for x in range (corridor.x, corridor.xend+1):
			map[y][x] = ".."
			pass

for leaf in leaves:
	shrink(leaf)
	pass


for leaf in leaves:
	#if (leaf.depth == 3):
	for y in range(leaf.y, leaf.yend+1):
		for x in range (leaf.x, leaf.xend+1):
			map[y][x] = ".."
			pass

for y in range(MAPSIZE):
	for x in range(MAPSIZE):
		print(map[y][x], sep="", end="")
	print("")
