import random

prenomFile = open("prenom.txt", "r")
prenoms = prenomFile.readlines()

for x in range(100):
	for y in range(100):
		name = str(x)+"-"+str(y)+".json"
		f = open(name, "w")
		strJSON = "{\n\"tile\": [\n"
		for row in range(24):
			strRow = "["
			for col in range(24):
				#create objezct
				objStr = "{"

				textureNumber = "0"
				r = random.random()*100
				if r < 1:
					textureNumber = "3"
				elif r < 6:
					textureNumber = "2"
				elif r < 12:
					textureNumber = "1"
				else:
					textureNumber = "0"

				objStr += "\"textureNumber\":"+textureNumber+","

				nbTot = 100
				first = random.randint(0,nbTot)
				nbTot -= first
				second = random.randint(0,nbTot)
				nbTot -= second
				third = nbTot
				objStr += "\"waterScale\":"+str(first)+","
				objStr += "\"woodScale\":"+str(second)+","
				objStr += "\"coalScale\":"+str(third)

				objStr += "}"
				strRow += objStr+","
			strRow = strRow[:-1]
			strRow += "],\n"
			if row == 23:
				strRow = strRow[:-2]
				strRow += "],\n"
			strJSON += strRow

		strJSON += "\"factory\": [\n\
		{\n\
		\"coo\": ["+str(random.randint(0,23))+","+str(random.randint(0,23))+"],\n\
		\"lvl\": "+str(random.randint(1,100))+",\n\
		\"owner\": \""+prenoms[random.randint(0,len(prenoms)-1)][:-3]+"\"\n\
		}\n\
		]\n\
		}"
		f.write(strJSON)
		f.close()