#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <string>
#include <fstream>
#include <iostream>

namespace Crimson {
   static std::string read_file(const std::string& fileName) {
      std::ifstream stream(fileName);

      std::string line;
      std::string result;

      if (stream.is_open()) {
         while (std::getline(stream, line)) {
            result += line + '\n';
         }
         stream.close();
      } else {
         std::cerr << "Unable to open file: " << fileName << '\n';
      }

      return result;
   }
}

#endif /* end of include guard: FILE_UTILS_H */
