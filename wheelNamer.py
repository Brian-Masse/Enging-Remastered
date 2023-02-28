import os

files = os.listdir('dist/')
wheel_file = ""

for file in files:
    if "whl" in file: 
        wheel_file = file

index = wheel_file.find( "-" )

count = 0
index = 0

for i, char in enumerate(wheel_file):
    if char == "-":
        if count == 1:
            index = i
            break
        count += 1

substring = wheel_file[ 0:i ]

file_name = substring + "-py3-none-any.whl"

os.rename( "dist/" + wheel_file, "dist/" + file_name )
