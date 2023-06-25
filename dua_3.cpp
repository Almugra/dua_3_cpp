#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

const int INF = 3000000;

std::vector<std::string> splitWhitespace(std::string s) {
  std::istringstream iss(s);
  return std::vector<std::string>{std::istream_iterator<std::string>{iss},
                                  std::istream_iterator<std::string>{}};
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    throw std::invalid_argument("one argument needed");
  }

  auto input = std::fstream(argv[1]);

  {
    std::string str;
    std::getline(input, str); // skip first line
  }

  std::string str;
  std::getline(input, str);
  int num_of_nodes = std::stoi(str.substr(3, str.size()));

  std::cout << "n = " << num_of_nodes << std::endl;

  auto matrix = std::vector<std::vector<int>>(
      num_of_nodes, std::vector<int>(num_of_nodes, INF));

  for (std::string line; std::getline(input, line);) {
    std::vector<std::string> line_split = splitWhitespace(line);
    int nr = std::stoi(line_split[0]);

    if (line_split.size() > 2) {
      auto adj_list =
          std::vector<std::string>(line_split.begin() + 2, line_split.end());

      for (std::string adj : adj_list) {
        auto w_pos = adj.find('w');
        auto node = std::stoi(adj.substr(0, w_pos));
        auto weight = std::stoi(adj.substr(w_pos + 1, adj.size()));
        matrix[nr][node] = weight;
      }
    }
  }

  for (int i = 0; i < num_of_nodes; i++) {
    matrix[i][i] = 0;
  }

  for (int k = 0; k < num_of_nodes; k++) {
    for (int i = 0; i < num_of_nodes; i++) {
      for (int j = 0; j < num_of_nodes; j++) {
        if (matrix[i][k] != INF && matrix[k][j] != INF) {
          matrix[i][j] = std::min(matrix[i][j], matrix[i][k] + matrix[k][j]);
        }
      }
    }
  }

  for (int x = 0; x < num_of_nodes; x++) {
    std::string line;
    for (int y = 0; y < num_of_nodes; y++) {
      if (matrix[x][y] != INF) {
        line += std::to_string(y) + 'w' + std::to_string(matrix[x][y]) + ' ';
      }
    }
    std::cout << x << " : " << line.substr(0, line.size() - 1) << std::endl;
    line.clear();
  }

  return 0;
}
