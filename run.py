file = input("File: ")
print()

import subprocess
result = subprocess.Popen(file)
print("Opening '{0}'...".format(file))
text = result.communicate()[0]
returncode = result.returncode
print("Done!")
print("Return code:", returncode)