#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

const int INF = 3000000;

using namespace std;

vector<string> splitWhitespace(string s) {
  istringstream iss(s);
  return vector<string>{istream_iterator<string>{iss},
                        istream_iterator<string>{}};
}

void setWeights(vector<vector<int>> &v, int n, basic_fstream<char> &input) {
  for (string line; getline(input, line);) {
    vector<string> parts = splitWhitespace(line);
    int nr = stoi(parts[0]);

    if (parts.size() > 2) {
      for (size_t i = 2; i < parts.size(); i++) {
        auto adj = parts[i];
        auto w_pos = adj.find('w');
        auto node = stoi(adj.substr(0, w_pos));
        auto weight = stoi(adj.substr(w_pos + 1, adj.size()));
        v[nr][node] = weight;
      }
    }
  }

  for (int i = 0; i < n; i++) {
    v[i][i] = 0;
  }
}

void floydWarshall(vector<vector<int>> &matrix, int n) {
  for (int k = 0; k < n; k++) {
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        if (matrix[i][k] != INF && matrix[k][j] != INF) {
          matrix[i][j] = min(matrix[i][j], matrix[i][k] + matrix[k][j]);
        }
      }
    }
  }
}

void writeMatrix(vector<vector<int>> matrix, int n) {
  for (int x = 0; x < n; x++) {
    string line;
    for (int y = 0; y < n; y++) {
      if (matrix[x][y] != INF) {
        line += to_string(y) + 'w' + to_string(matrix[x][y]) + ' ';
      }
    }
    cout << x << " : " << line.substr(0, line.size() - 1) << endl;
    line.clear();
  }
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    throw invalid_argument("one argument needed");
  }

  auto input = fstream(argv[1]);

  {
    string str;
    getline(input, str); // skip first line
  }

  string str;
  getline(input, str);
  int num_of_nodes = stoi(str.substr(3, str.size()));

  cout << "n = " << num_of_nodes << endl;

  auto matrix =
      vector<vector<int>>(num_of_nodes, vector<int>(num_of_nodes, INF));

  setWeights(matrix, num_of_nodes, input);

  floydWarshall(matrix, num_of_nodes);

  writeMatrix(matrix, num_of_nodes);

  return 0;
}
