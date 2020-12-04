#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>

void PrintVector(std::vector<double> v) {
    for (auto i : v) {
        std::cout << i;
    }
    std::cout << std::endl;
}

std::ifstream infile("ms.out");


int main()
{

	int line_num = 0;
	std::string line;
	std::string data;
	int n_individuals = 4;

	while (std::getline(infile, line))
	{
    		if (line.empty()) continue;
            std::istringstream iss;

            // parse random numbers
            if (line_num == 0){
                iss.str(line);
                int a, b, c;
                if (!(iss >> a >> b >> c)) { break; }


            } else {
                if (line.compare(0, 2, "//") == 0){
                    std::cout << "[PARSED] "<< line << std::endl;

                    // parse segsites:
                    std::getline(infile, line);
                    iss.str(line);
                    std::string segsites;
                    int n_sites;
                    iss >> segsites;
                    iss >> n_sites;
                    std::cout << "[PARSED] " << segsites << " " << n_sites << std::endl;
                    iss.str("");



                    // parse positions;
                    std::getline(infile, line);
                    std::istringstream ss(line);
                    std::string token;
                    std::vector<double> sites;
                    while(std::getline(ss, token, ' ')) {
                        if (token.compare("positions:") != 0)
                            sites.emplace_back(stod(token));
                    }
                    std::cout << "[PARSED] positions: ";
                    PrintVector(sites);



                    // parse genotype
                    std::cout << "[PARSED] Genotype matrix: " << std::endl;;
                    std::vector<std::vector<int>> matrix{};
                    for (int i=0; i<n_individuals; i++) {
                        std::getline(infile, line);
                        matrix.emplace_back(std::vector<int>{});
                        std::cout << "[PARSED] ";
                        for (int j=0; j<n_sites; j++) {
                            matrix[i].emplace_back(line[j] - '0');
                            std::cout << matrix[i][j];
                        }
                        std::cout << std::endl;
                    }


                    std::cout << "Press Enter to Continue";
                    std::cin.ignore();
                }
            }
            line_num++;


        }
	return 0;
}
