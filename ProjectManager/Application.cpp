#include <iostream>
#include <string>

int main(int argc, char const *argv[]) {
   if (argc < 2) {
      return 1;
   }

   std::string command = argv[1];
   if (command == "init") {
      std::cout << "This will generate a new project" << '\n';
   }

   return 0;
}
