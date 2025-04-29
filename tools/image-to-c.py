from sys import argv 

if len(argv) < 3:
  print(f"Usage: {argv[0]} filename output.h")
  
filename = argv[1]
output = argv[2]

arrayname = filename.split(".")[0].split("/")[1]

with open(filename, "rb") as f:
  data = [int(i) for i in f.read()]
  
with open(output, "w") as f:
  f.write(f"int {arrayname}_size = {len(data)};\n")
  f.write(f"const unsigned char ")
  f.write(arrayname)
  f.write(f"_image[{len(data)}]")
  f.write("={")
  for i in data:
    f.write(f"{i},")
  f.write("};")