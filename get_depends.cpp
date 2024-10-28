// spkg - Sourim Package Manager
// Copyright (C) 2024 r2team
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <getopt.h>

std::vector<std::vector<std::string>> output{};
std::vector<std::vector<std::string>> output_temp{};
std::vector<std::vector<std::string>> indexes{};
std::vector<std::vector<std::string>> versions_installed{};

std::vector<std::string> indexes_list{};
std::vector<std::string> packages_list{};

std::string root;

void usage(char *program) {
    fprintf(stderr, "usage: %s [-i INDEX_PATH] [-p PACKAGE] [-r ROOT_PATH] [a ARCH]", program);
    exit(EXIT_FAILURE);
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}
std::vector<std::string> findVal(std::vector<std::vector<std::string>> array, size_t index, std::string value) {
    std::vector<std::string> oarr{};
    for (std::vector<std::string> elem : array) {
        if (elem[index] == value)
            oarr = elem;
    }
    return oarr;
} 

void get_depends(std::string package) {
    std::vector<std::string> element = findVal(indexes, 2, package);

    for (size_t i = 6; i < element.size(); i++)
        get_depends(element[i]);

    std::vector<std::string> infover = findVal(versions_installed, 0, package);
    if (element.size() < 4)
        return;
    if (!infover.size() || (infover.size() == 2 && infover[1] != (element[3] + "_" + element[4])))
        output_temp.push_back(element);
}

int main(int argc, char **argv) {
    if (argc < 3)
        usage(argv[0]);

    int opt;
    while ((opt = getopt(argc, argv, "i:p:r:a:")) != -1) {
        switch (opt) {
            case 'i':
                indexes_list.push_back(std::string(optarg));
                break;
            case 'p':
                packages_list.push_back(std::string(optarg));
                break;
            case 'r':
                root = std::string(optarg);
                break;
            case '?':
                usage(argv[0]);
                break;
        }
    }

    // Load Version:
    std::ifstream version(root + "/var/spkg/pkgs");
    if (version.good()) {
        for (std::string line; std::getline(version, line);) {
            if (!line.empty())
                versions_installed.push_back(split(line, ' '));
        }
    }
    version.close(); 

    // Load Indexes:
    for (auto s : indexes_list) {
        std::vector<std::string> element = split(s, '=');
        std::string index_path = root + "/etc/spkg.d/repos/" + element[0];
        std::ifstream index(index_path);

        if (index.fail()) {
            fprintf(stderr, "Not found index file %s\n", index_path.c_str());
            exit(EXIT_FAILURE);
        }
        for (std::string line; std::getline(index, line); ) {
            indexes.push_back(split(element[0] + " " + element[1] + " " + line, ' '));
        }
    }

    for (std::string package : packages_list)
        get_depends(package);

    for (auto index : output_temp) {
        auto elem = findVal(output, 2, index[2]);

        if (!elem.size())
            output.push_back(index);
    }   
    for (auto index : output) {
        std::cout << index[0] + "/" + index[2] + " " + index[1] + " " + index[3] + " " + index[4] + " " + index[5] << std::endl;
    }
}