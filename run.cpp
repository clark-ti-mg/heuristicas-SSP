#include <dirent.h>
#include <cstdlib>
#include <string>
#include <fstream>

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>

/*
	Comandos para execuçao do programa no terminal do Ubuntu 24.04 LTS: 
	
	Parametro 0: nome do programa = ./run
	Parametro 1: arg[1] = nomeDir = nome do diretorio ./instancia/
	Parametro 2: quantidade de repetiçoes (1 = 1*10) = 1

	./run ./Tabela1/ 1
	./run ./Tabela2/ 1
	./run ./Tabela3/ 1
	./run ./Tabela4/ 1

	

*/

int main(int argc, char* argv[]) {
	if (argc != 3) {
        std::cerr << "Diretório com as instâncias não foi informado." << std::endl;
        exit (1);
    }
	std::string nomeDir = argv[1];
	int repeticoes = std::stoi(argv[2]);
	DIR *dir = 0;
    struct dirent *entrada = 0;
    unsigned char isFile = 0x8;

    dir = opendir (nomeDir.c_str());

    if (dir == 0) {
        std::cerr << "Nao foi possível abrir diretorio com as instâncias." << std::endl;
        exit (1);
    }
	// Pego todas as instâncias do diretório
	while ((entrada = readdir (dir))){
		if (entrada->d_type == isFile){
			// Executo o programa N vezes
			std::string xF; // = nomeDir + "solucoes/TS_SOLUCAO_"+entrada->d_name + "_10";
			const char * x; // = xF.c_str();
			FILE *TFile;// = fopen(x,"r");
			//if (!TFile){ // Ignorar as instancias já executadas
			for (int i=1;i<=repeticoes;++i) {
				std::stringstream convert;
				std::cout << i << " Execucao: " << nomeDir+entrada->d_name << std::endl;
				convert << i;
				std::string xF = nomeDir + "solucoes/TS_SOLUCAO_"+entrada->d_name +  "_" + convert.str();
				x = xF.c_str();
				std::ifstream existeInstancia;
				existeInstancia.open(xF);
			// if (!existeInstancia.is_open()){
			//TFile = fopen(x,"r");
			//if (!TFile){ // Ignorar as instancias já executadas
				std::string cmd = "./samplecode " + nomeDir + "solucoes/TS_SOLUCAO_"+entrada->d_name + "_" + convert.str()+ " <" + nomeDir+entrada->d_name; //+ " >"  + nomeDir + "solucoes/TS_SOLUCAO_"+entrada->d_name + "_" + convert.str();
				const char * c = cmd.c_str();
				int s = system(c);
			//   } else{
			//     existeInstancia.close();
			//     std::cout <<  "já executado. " << std::endl;
			//   }
			}
		//} else {
		//  std::cout << nomeDir + "solucoes/BRKGA_SOLUCAO_"+entrada->d_name + " já executado." << std::endl;
		//}
		}
	}
    closedir (dir);

    nomeDir+="solucoes/";
    std::cout << nomeDir;
	dir = opendir (nomeDir.c_str());
	// Gerando o acumulado
	std::ifstream file;
	std::ofstream fileR;
	std::string dadoN, dadoT, dadoC, dado, dado2, dado3, dado4, dado5, dado6, dado7, dado8, dado9, dado10, dado11, dado12, dado13,dado14,dado15,dado16;
	fileR.open(nomeDir+"TS_RESUMO.txt");
	while ((entrada = readdir (dir))){
        if (entrada->d_type == isFile){
			 std::string nomeArq = entrada->d_name;
			 if (nomeArq.compare("TS_RESUMO.txt")!=0){
				 file.open(nomeDir+nomeArq, std::ifstream::in);
				 file >> dadoN;
				 file >> dadoT;
				 file >> dadoC;
				 file >> dado;
				 file >> dado2;
                 file >> dado3;
                 file >> dado4;
				//  file >> dado5;
				//  file >> dado6;
				//  file >> dado7;
				//  file >> dado8;	
				//  file >> dado9;
				//  file >> dado10;
				//  file >> dado11;
				//  file >> dado12;
				//  file >> dado13;
				//  file >> dado14;
				//  file >> dado15;
				//  file >> dado16;	
				 fileR << entrada->d_name << " " << dadoN << " " << dadoT << " " << dadoC << " " << dado << " " << dado2 << " " << dado3 << " " << dado4 << std::endl; //<< dado4 << " ";
				//  fileR << dado5 << " " << dado6 << " " << dado7 << " " << dado8 << " " ;
				//  fileR << dado9 << " " << dado10 << " " << dado11 << " " << dado12 << " ";
				//  fileR << dado13 << " " << dado14 << " " << dado15 << " " << dado16 << " " << std::endl;
				 file.close();
			 }
		}
	}
	fileR.close();
	closedir(dir);
	return 0;
}//
