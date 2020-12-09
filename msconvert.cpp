#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <numeric>

typedef std::vector<std::vector<int>> matrix_t;

void PrintVector(std::vector<double> v) {
    for (auto i : v) {
        std::cout << i;
    }
    std::cout << std::endl;
}

int ReadSegsitesLine(std::istringstream &iss)
{
    iss.seekg(10);
    std::string n_segsites_str;
    iss >> n_segsites_str;
    int n_segsites = atoi(n_segsites_str.c_str());
    iss.clear();//clear any bits set
    iss.str(std::string());
    return n_segsites;
}

matrix_t ParseGenotypeMatrix(std::ifstream &infile, int n_positions, bool DEBUG)
{
    std::string line;
    std::vector<std::vector<int>> matrix{};
    int i=0;
    while (std::getline(infile, line)) {
        if (line.empty()) {break; }
        matrix.emplace_back(std::vector<int>{});
        for (int j=0; j<n_positions; j++) {
            matrix[i].emplace_back(line[j] - '0');
            if (DEBUG) {std::cout << matrix[i][j];}
        }
        if (DEBUG) {std::cout << std::endl;}
        i++;
    }
    return matrix;
}

std::vector<double> TokenizePositionsLine(std::istringstream &iss) {
    std::string token;
    std::vector<double> sites;
    while(std::getline(iss, token, ' ')) {
        if (token.compare("positions:") != 0) {
            sites.emplace_back(stod(token));
        }
    }
    iss.clear();//clear any bits set
    iss.str(std::string());
    return sites;
}

std::vector<int> FirstColumnOfMatrix(const matrix_t &matrix) 
{
    std::vector<int> firstcolum{};
    for (int i=0; i<matrix.size(); i++) {
        firstcolum.emplace_back(matrix[i][0]);
    }
    return firstcolum;
}

std::vector<int> NAccumulate(std::vector<int> vec, int N)
{
    int size = vec.size();
    if (size % N != 0) {
        std::cout << "N not a multiple of first column. Change N." << std::endl;
        return std::vector<int>{};
    }
    int part_size = size / N;
    std::vector<int> rv{};
    int k=0;
    for(int i=0; i<N; i++) {
        int start_index = i*part_size;
        int end_index = (i+1)*part_size;
        //std::cout << start_index << " " << end_index << std::endl;
        std::vector<int> part(vec.begin()+start_index, vec.begin()+end_index);
        rv.emplace_back(std::accumulate(part.begin(), part.end(), 0));
    }
    return rv;
}

void PrintOutput(std::vector<matrix_t> &gentype_matrices)
{
    for (int i=0; i<gentype_matrices.size(); i++) {
        std::vector<int> column = FirstColumnOfMatrix(gentype_matrices[i]);
        std::vector<int> n_sum = NAccumulate(column, 2);
        std::cout << i << "\t" <<n_sum[0] << "\t" << n_sum[1] << std::endl;
    }
}

int main(int argc, char* argv[]){
	if (argc != 2) {
		std::cout << "Usage: msconvert ms.outfile >> output.txt" << std::endl;
        return 1;
	}

    std::ifstream infile(argv[1]);
    std::string line;
    int nth_line = 0;
    bool DEBUG{false};
    std::istringstream iss;


    std::vector<matrix_t> genotype_matrices{};

    while (std::getline(infile, line))
    {
        if (line.empty()) { continue; } // skips empty lines
        if (nth_line == 0) { nth_line++; continue; } // skips first random number line
        
            
        // start of locus
        if (line.compare(0, 2, "//") == 0){
            if (DEBUG) std::cout << "[PARSED] "<< line << std::endl;
            
            // parse segsites: x line
            std::getline(infile, line);
            iss.str(line);
            int n_segsites = ReadSegsitesLine(iss);
            if (n_segsites == 0) { continue; } // continue to next locus
            
            // parse positions
            std::getline(infile, line);
            iss.str(line);
            std::vector<double> positions = TokenizePositionsLine(iss);
            int n_positions = positions.size();
            if (DEBUG) { PrintVector(positions); }
            
            // parse genotypes
            matrix_t matrix = ParseGenotypeMatrix(infile, n_positions, DEBUG);
            genotype_matrices.emplace_back(matrix);
                    
        }
    }

    PrintOutput(genotype_matrices);


}
