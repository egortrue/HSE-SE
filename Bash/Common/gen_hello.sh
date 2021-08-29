#!/bin/bash
main2=gen_hello.cpp
touch $main2

#####################

### Cpp-code ###
echo "#include <iostream>" > $main2
echo "#include <cstring>" >> $main2
echo "int main() {" >> $main2
echo "char code[] = " >> $main2

## Shell-code ##
echo "  \"#!/bin/bash\n\"" >> $main2
echo "  \"main1=hello.cpp\n\"" >> $main2
echo "  \"touch \$main1\n\n\"" >> $main2

# Cpp-code #
echo "  \"echo \\\"#include <iostream>\\\" > \$main1\n\"" >> $main2
echo "  \"echo \\\"int main() {\\\" >> \$main1\n\"" >> $main2
echo "  \"echo \\\"  std::cout << \\\\\\\"hello, world!\\\\\\\" << std::endl;\\\" >> \$main1\n\"" >> $main2
echo "  \"echo -e \\\"  return 0;\\\\\\\n}\\\" >> \$main1\n\n\"" >> $main2

## Shell-code ##
echo "  \"g++ \$main1 -o hello.exe\n\"" >> $main2
echo "  \"./hello.exe\n\";" >> $main2

### Cpp-code ###
echo "char command[500];" >> $main2
echo "strcat(command, \"touch hello.sh; echo '\");" >> $main2
echo "strcat(command, code);" >> $main2
echo "strcat(command, \"' > hello.sh; bash hello.sh\");" >> $main2

echo "std::system(command);" >> $main2
echo -e "return 0;\n}" >> $main2

#####################

g++ $main2 -o gen_hello.exe
./gen_hello.exe

