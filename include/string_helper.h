#ifndef MONITOR_STRING_HELPER_H
#define MONITOR_STRING_HELPER_H

#include <string>
#include <vector>
#include <algorithm>

namespace StringHelper {
template <typename T>
T GetElement(std::string input, int index);

template <typename T>
std::vector<T> GetElements(std::string input, std::vector<int> indezes);

template <typename T>
inline std::vector<T> GetElements(std::string input, std::vector<int> indezes){
  if(input.empty()){
    throw std::invalid_argument("The provided string is empty.");
  }

  std::sort(indezes.begin(), indezes.end(), [](int element_one, int element_two){return element_one < element_two;});
  std::vector<T> elements;

  std::istringstream stream(input);
  std::string tmp;

  int element_index = 0;
  int indezes_index = 0;

  while(stream >> tmp){
      if(element_index == (indezes[indezes_index] - 1)){
        std::istringstream convert(tmp);
        T value;
        convert >> value;

        elements.push_back(value);
        indezes_index++;
      }

      element_index++;
  }

  return elements;
}

template <typename T>
inline T StringHelper::GetElement(std::string input, int index) {
  if(input.empty()){
    throw std::invalid_argument("The provided string is empty.");
  }

  if (index < 1) {
    throw std::runtime_error("Please provide an index greater than 0.");
  }

  // start counting at 1
  index -= 1;

  std::istringstream stream(input);
  std::string tmp;
  int tmp_index = 0;

  while (stream >> tmp) {
    if (tmp_index == index) {

      // create stream on one string element to be able to convert it to the template type
      std::istringstream convert(tmp);
      T value;
      convert >> value;

      return value;
    } else {
      tmp_index++;
    }
  }
}
}  // namespace StringHelper

#endif  // MONITOR_STRING_HELPER_H
