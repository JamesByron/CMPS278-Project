import sys
location  = "output/" + sys.argv[1]
locationC = "output/" + sys.argv[1] + "_control"
f = open(location)
fc = open(locationC)

labels = []
test = []
testCount = 0
control = []
controlCount = 0

for line in f:
	if "Using " in line:
		testCount += 1
	if " = " in line:
		arr = line.split()
		labels.append(arr[5])
	elif "SortTime" in line:
		arr = line.split()
		test.append(arr[1])
f.close()

tempLabels = []
for line in fc:
	if "Using " in line:
		controlCount += 1
	if " = " in line:
		arr = line.split()
		tempLabels.append(arr[5])
	elif "SortTime" in line:
		arr = line.split()
		control.append(arr[1])
fc.close()

assert(len(labels) == len(tempLabels))
assert(testCount == controlCount)
assert(len(test) == len(control))

rows = int(len(test)/testCount)
testOutput = []
controlOutput = []

for i in range(rows):
	testOutput.append([labels[i], 0, sys.maxsize, 0])
	controlOutput.append([labels[i], 0, sys.maxsize, 0])

for i in range(len(test)):
	testOutput[i%rows][1] += float(test[i]) / testCount
	controlOutput[i%rows][1] += float(control[i]) / controlCount
	if testOutput[i%rows][2] > int(test[i]):
		testOutput[i%rows][2] = int(test[i])
	if testOutput[i%rows][3] < int(test[i]):
		testOutput[i%rows][3] = int(test[i])
	if controlOutput[i%rows][2] > int(control[i]):
		controlOutput[i%rows][2] = int(control[i])
	if controlOutput[i%rows][3] < int(control[i]):
		controlOutput[i%rows][3] = int(control[i])

#print(testOutput)
outputString = ""
for i in range(len(testOutput)):
	for j in range(len(testOutput[i])):
		if j == 1:
			outputString += str(round(testOutput[i][j], 1))
		else:
			outputString += str(testOutput[i][j])
		if j < len(testOutput[i])-1:
			outputString += "\t"
	outputString += "\n"
testFile = open(location+".tsv","w")
testFile.write(outputString)
testFile.close()

outputString = ""
for i in range(len(controlOutput)):
	for j in range(len(controlOutput[i])):
		if j == 1:
			outputString += str(round(controlOutput[i][j], 1))
		else:
			outputString += str(controlOutput[i][j])
		if j < len(controlOutput[i])-1:
			outputString += "\t"
	outputString += "\n"

controlFile = open(locationC+".tsv","w")
controlFile.write(outputString)
controlFile.close()