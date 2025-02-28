#ifndef HEURISTICS_H
#define HEURISTICS_H
#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <string.h>
#include <bitset>
#include <random>
#include <ctime>
#include <ratio>
#include <string>
#include <set>
#include <stack>
#include "KTNS.h"
#define MAX_INT 2147483647

vector<int> escolhe_maisProximo(int inicial, int tarefas) {
    vector<int> perm;
    vector<int> permAux;
    set<int> resto; // todas as tarefas que ainda não foram pra solução
    
    int diff = MAX_INT;
    int diffAux;
    int tam = 2;

    perm.assign(tarefas, -1); 
    perm[0] = inicial; // coloca a inicial no início da solução
    
    // coloca todas as tarefas no resto ( nenhuma foi escolhida ainda )
    for(int i = 0; i < tarefas; i++)
        resto.insert(i);

    // tira a tarefa inicial das tarefa que ainda vão entrar
    resto.erase(inicial);
    
    // enquanto existis tarefas a serem alocadas
    while(resto.size() > 0) {
        diff = MAX_INT;
        permAux.assign(tam, -1);

        /*
            caso não exista nenhuma tarefa na permutação auxiliar, coloca a tarefa inicial nela
        */
        if (tam == 2) 
            permAux[0] = inicial;
        else {
            // copia a melhor solução parcial antiga 
            for(int i = 0; i < tam; i++) {
                permAux[i] = perm[i];
            }
        }
        int saiu = -1;
        // pilha de candidatos de melhor tarefa para entrar, caso existam tarefas com o mesmo valor de troca
        stack<int> candidatos;
        for(set<int>::iterator it = resto.begin(); it != resto.end(); it++) {
            int aux = permAux[tam-1];
            permAux[tam-1] = *it;
            diffAux = KTNS(permAux);

            if(diffAux < diff) {
                diff = diffAux;
                while (candidatos.size() > 0)
                    candidatos.pop();
                candidatos.push(*it);
            } else if (diffAux == diff) {
                candidatos.push(*it);    
            } else {
                permAux[tam-1] = aux;
            }
        }
        
        // resolver empates aleatoriamente     
        random_device rd;
        uniform_int_distribution<int> dist(0, candidatos.size()-1);
        int p = dist(rd); 
        
        for(int i = 0; i < p; i++) {
            candidatos.pop();
        }
        
        // pega o candidato escolhido
        saiu = candidatos.top();

        // coloca ele no final da solução parcial
        permAux[tam-1] = saiu;
        perm = permAux; 
        resto.erase(saiu); // remove ele das tarefas que ainda vão entrar 
        tam++;
    }
    
    return perm;

}

vector<int> makeDomino(int inicial,int tarefas) {
    vector<int> permAux;
    vector<int> permAuxFinal;
    vector<int> permAuxInicio;

    set<int> jogador1;
    set<int> jogador2;
    set<int> taf;

    // set<int>::iterator posJog1;
    // set<int>::iterator posJog2;

    int posJog1;
    int posJog2;


    int tamPerm = 2;
    int trocasInicioJog1;
    int trocasFinalJog1;
    int trocasInicioJog2;
    int trocasFinalJog2;
    int aux1, aux2;

    permAux.assign(tarefas, -1);
    permAux[0] = inicial;

    for(int i = 0; i < tarefas; i++)
        taf.insert(i);

    taf.erase(inicial);

    for(int i = 0; i < tarefas/2; i++) {
        random_device rd;
        uniform_int_distribution<int> dist(0, taf.size()-1);
        int p = dist(rd); 
        while(jogador1.find(p) != jogador1.end()) {
            p = dist(rd);
        }
        jogador1.insert(p);
        taf.erase(p);
    }

    jogador1.erase(inicial);

    while(taf.size() > 0) {
        set<int>::iterator i = taf.begin();
        jogador2.insert(*i);
        taf.erase(*i);
    }

    jogador2.erase(inicial);

    // cada jogador ve se é melhor colocar a peça no começo ou no final
    while(tamPerm <= tarefas) {
        bool pula1 = false, pula2 = false;

        if(jogador1.size() == 0) {
            pula1 = true;
            aux1 = MAX_INT;
        }
    
        if(jogador2.size() == 0) {
            pula2 = true;
            aux2 = MAX_INT;
        }

        if (!pula1)
            posJog1 = -1;

        if (!pula2)
            posJog2 = -1;

        permAuxInicio.assign(tamPerm, -1);
        permAuxFinal.assign(tamPerm, -1);

        // puxar todas as tarefas pra frente
        for(int i = tamPerm-1; i > 0; i--) {
            permAuxInicio[i] = permAux[i-1];
        }

        for(int i = 0; i < tamPerm; i++)
            permAuxFinal[i] = permAux[i];

        // TAREFA DO JOGADOR 1
        bool frente = false;
        if (!pula1) {
            // Jogada do Player 1
            for(set<int>::iterator it = jogador1.begin(); it != jogador1.end(); it++) {
                permAuxInicio[0] = *it;
                permAuxFinal[tamPerm-1] = *it;
                trocasInicioJog1 = KTNS(permAuxInicio);
                trocasFinalJog1  = KTNS(permAuxFinal);

                if (trocasInicioJog1 < trocasFinalJog1) {
                    frente = true;
                    posJog1 = *it;
                } else if (trocasFinalJog1 < trocasInicioJog1) {
                    frente = false;          
                    posJog1 = *it;
                } else {
                    frente = true;
                    posJog1 = *it;
                }
                
            }
            // fim jogada do p1
        }
        if (frente) {
            permAuxInicio[0] = posJog1;
            permAux = permAuxInicio;
        } else {
            permAuxFinal[tamPerm-1] = posJog1;
            permAux = permAuxFinal;
        }

        if (!pula1)
            tamPerm++;



        permAuxInicio.assign(tamPerm, -1);
        permAuxFinal.assign(tamPerm, -1);

        // puxar todas as tarefas pra frente
        for(int i = tamPerm-1; i > 0; i--) {
            permAuxInicio[i] = permAux[i-1];
        }

        for(int i = 0; i < tamPerm; i++)
            permAuxFinal[i] = permAux[i];

        // TAREFA DO JOGADOR 2
        frente = false;
        if (!pula2) {
            // Jogada do Player 2
            for(set<int>::iterator it = jogador2.begin(); it != jogador2.end(); it++) {
                permAuxInicio[0] = *it;
                permAuxFinal[tamPerm-1] = *it;
                trocasInicioJog2 = KTNS(permAuxInicio);
                trocasFinalJog2  = KTNS(permAuxFinal);
                
                if (trocasInicioJog2 < trocasFinalJog2) {
                    frente = true;
                    posJog2 = *it;
                } else if (trocasFinalJog2 < trocasInicioJog2) {
                    frente = false;
                    posJog2 = *it;
                } else {
                    frente = true;
                    posJog2 = *it;
                }
                // fim jogada do p2

            }
            
            
        }

        if (frente) {
            permAuxInicio[0] = posJog2;
            permAux = permAuxInicio;
        } else {
            permAuxFinal[tamPerm-1] = posJog2;
            permAux = permAuxFinal;
        }

        if (!pula1) {
            jogador1.erase(posJog1);
        }
        
        if (!pula2) {
            jogador2.erase(posJog2);
        }
        tamPerm++;
    }

    return permAux;
}

vector<int>domino(int n){
    vector<int> permFinal;
    permFinal.assign(n, -1);
    vector<int> permAux;
    int diffAux;
    int diff = MAX_INT;

    for(int i = 0; i < n; i++) {
        vector<int> aux = makeDomino(i,n);
        diffAux = KTNS(aux);
        if (diffAux < diff) {
            permFinal = aux;
            diff = diffAux;
        }
    }
    return permFinal;
}

vector<int>maisProximo(int n){
    vector<int> final;
    final.assign(n, -1);
    int diff = MAX_INT;
    int diffAux;
    vector<int> permAux;
    for(int i = 0; i < n; i++) {
        permAux = escolhe_maisProximo(i,n);
        diffAux = KTNS(permAux);
        if (diffAux < diff) {
            diff = diffAux;
            final = permAux;
        }
    }
    return final;
}

vector<int> escolheSub(int inicial, int n) {
    vector<int> perm;
    vector<int> permAuxFinal;
    vector<int> permAuxInicio;
    set<int> resto;
    
    int diff = MAX_INT;
    int diffFinal;
    int diffInicio;
    int tam = 2;

    bool inicio;

    perm.assign(n, -1);    
    perm[0] = inicial;

    for(int i = 0; i < n; i++)
        resto.insert(i);

    resto.erase(inicial);
    
    while(resto.size() > 0) {
        diff = MAX_INT;
        permAuxFinal.assign(tam, -1);
        permAuxInicio.assign(tam, -1);

        if (tam == 2) 
            permAuxFinal[0] = inicial;
        else {
            for(int i = 0; i < tam; i++) {
                permAuxFinal[i] = perm[i];
            }
        }
        int saiu = -1;
        stack<int> candidatos;
        for(set<int>::iterator it = resto.begin(); it != resto.end(); it++) {
            permAuxFinal[tam-1] = *it;

            for(int i = tam-1; i > 0; i--) {
                permAuxInicio[i] = permAuxFinal[i-1];
            }
            permAuxInicio[0] = *it;

            diffFinal = KTNS(permAuxFinal);

            diffInicio = KTNS(permAuxInicio);

            if(diffFinal < diff && diffFinal < diffInicio) {
                while(candidatos.size() > 0)
                    candidatos.pop();
                candidatos.push(*it);

                diff = diffFinal;
                inicio = false;
            } else if (diffInicio < diff && diffInicio < diffFinal) {
                while(candidatos.size() > 0)
                    candidatos.pop();
                candidatos.push(*it);

                diff = diffInicio;
                inicio = true;
            } else if (diffInicio == diffFinal && diffFinal < diff ) {
                random_device r;
                uniform_int_distribution<int> d(0, 1);
                int aleatorio = d(r);
                if (aleatorio == 0) {
                    inicio = true;
                    diff = diffInicio;
                } else {
                    inicio = false;
                    diff = diffFinal;
                }    
                candidatos.push(*it);
            } 


        }
        
        // resolver empates aleatoriamente     
        random_device rd;
        uniform_int_distribution<int> dist(0, candidatos.size()-1);
        int p = dist(rd); 

        for(int i = 0; i < p; i++) {
            candidatos.pop();
        }
        
        saiu = candidatos.top();
        candidatos.pop();

        if (inicio) {
            for(int i = tam-1; i > 0; i--)
                permAuxFinal[i] = permAuxFinal[i-1];
            permAuxFinal[0] = saiu;
        } else {
            permAuxFinal[tam-1] = saiu;
        }

        perm = permAuxFinal;
        resto.erase(saiu);
        tam++;
    }
    return perm;
}
vector<int>sub(int n){
    vector<int> final;
    final.assign(n, -1);

    int diff = MAX_INT;
    int diffAux;
    vector<int> permAux;
    for(int i = 0; i < n; i++) {
        permAux = escolheSub(i,n);
        diffAux = KTNS(permAux);

        if (diffAux < diff) {
            diff = diffAux;
            final = permAux;
        }
    }
    return final;
}

int diferenca(int t1, int t2, int n) {
    int diff = 0;
    extern vector<vector<int>> matrixFerramentas;

    for(int i = 0; i < n; i++) {
        if(matrixFerramentas[i][t1] != matrixFerramentas[i][t2]) 
            diff++;
    }

    return diff;
}

void insercaoBarata_H(int tafI, int tafJ, int n, vector<int> &permAux) {
    /*
        s^k(i j) = d(i k) + d(k j) - d(i j)
        colocar a tarefa no meio das outras tarefas (seguindo a equação acima) e
        idenficar qual gera o menor número de trocas
    */

    int diff = MAX_INT;
    int tam = 2;

    vector<int> permIK(2);
    vector<int> permKJ(2);
    vector<int> permIJ(2);

    int trocasIK = MAX_INT;
    int trocasKJ = MAX_INT;
    int trocasIJ = MAX_INT;
    int equacao;
    int saiu;
    int posSaiu;

    set<int> resto;
    stack<int> candidatos;
    stack<int> posCand;

    for(int i = 0; i < n; i++) {
        resto.insert(i);
    }

    // tirar as tarefas que já estão começando
    for(int i = 0; i < tam; i++) {

        set<int>::iterator it = resto.find(permAux[i]);

        if(it != resto.end())
            resto.erase(*it);

    }

    resto.erase(tafI);
    resto.erase(tafJ);

    // s^k(i j) = d(i k) + d(k j) - d(i j)
    saiu = -1;

    permIK[0] = permAux[tafI];
    permKJ[1] = permAux[tafJ];

    while (tam < n) {
        permIJ[0] = permAux[tam-2];
        permIJ[1] = permAux[tam-1];

        permIK[0] = permAux[tam-2];

        permKJ[1] = permAux[tam-1];
        trocasIJ = KTNS(permIJ);

        diff = MAX_INT;
        // a tarefa a ser alocada é testada entre todas as tarefas que ja estão na solução parcial
        for(set<int>::iterator it = resto.begin(); it != resto.end();it++){
            permIK[1] = *it;
            permKJ[0] = *it;

            // cout << "Permutacao i k j: " << permIK[0] << " " << permIK[1] << " " << permKJ[1] << endl;

            // diferença entre as tarefas I K
            // trocasIK = KTNS(permIK);
            trocasIK = diferenca(permIK[0], permIK[1], n);

            // diferença entre as tarefas K J
            // trocasKJ = KTNS(permKJ);
            trocasKJ = diferenca(permKJ[0], permKJ[1],n);

            // cout << "Trocas I K: " << trocasIK << endl;
            // cout << "Trocas K J: " << trocasKJ << endl;

            // s^k(i j) = d(i k) + d(k j) - d(i j)
            equacao = trocasIK + trocasKJ - trocasIJ;
            if(equacao < diff) {
                //cout << "Melhora " << " Tarefa: " << *it << " Posicao: " << i << endl;
                diff = equacao;
                while(candidatos.size() > 0)
                    candidatos.pop();

                while(posCand.size() > 0)
                    posCand.pop();

                posCand.push(tam-1);
                candidatos.push(*it);
            } if (equacao == diff) {
                posCand.push(tam-1);
                candidatos.push(*it);
            }
        }

        random_device rd;
        uniform_int_distribution<int> dist(0, candidatos.size()-1);
        int p = dist(rd);

        for(int i = 0; i < p; i++) {
            candidatos.pop();
            posCand.pop();
        }

        saiu    = candidatos.top();
        posSaiu = posCand.top();

        for(int i = tam; i > posSaiu; i--) {
            permAux[i] = permAux[i-1];
        }

        permAux[posSaiu] = saiu;
        // cout << "Tarefa que entrou: " << candidatos.top() << endl;
        // cout << "Posicao que entrou: " << posCand.top() << endl;
        // cout << "Permutacao auxiliar: " << endl;
        // for(int i = 0; i < permAux.size(); i++) 
        //     cout << permAux[i] << " ";
        // cout << endl;

        resto.erase(saiu);
        
        while(candidatos.size() > 0)
            candidatos.pop();

        while(posCand.size() > 0)
            posCand.pop();

        tam++;
    }
    
    

}

vector<int> insercaoB(int n){
    int i = 0;
    int j = 1;
    int trocasAux;
    int diff = MAX_INT;
    vector<int> permFinal;
    vector<int> permAux;
    permFinal.assign(n, -1);
    permAux.assign(n, -1);
    
    int tam = 2;

    while(j < n) {
        // a heuristica começa com a combinação de duas tarefas
        permAux[0] = i;
        permAux[1] = j;
        insercaoBarata_H(i, j, n, permAux);

        trocasAux = KTNS(permAux);

        if(diff == MAX_INT) {
            permFinal = permAux;
            diff = trocasAux;
        } else {
            if (trocasAux < diff) {
                diff = trocasAux;
                permFinal = permAux;
            }
        }

        permAux.assign(n, -1);
        tam = 2;

        i++;
        j++;
        
    }
    return permFinal;
}




#endif