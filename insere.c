#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bplus.h"

typedef struct _Endereco Endereco;

struct _Endereco
{
    char logradouro[72];
    char bairro[72];
    char cidade[72];
    char uf[72];
    char sigla[2];
    char cep[8];
    char lixo[2];
};

#define TAMANHO_CEP 8
#define ORDEM_ARVORE 100

int main()
{
    FILE *arquivo;
    Endereco endereco;
    BP_TREE *arvore;
    unsigned long posicao = 0;
    char chave[TAMANHO_CEP];

    arquivo = fopen("cep_ordenado.dat", "rb");

    if (arquivo == NULL)
    {
        printf("Nao foi possivel abrir o arquivo cep_ordenado.dat\n");
        return 1;
    }

    if (!bp_create_if_not_exists("indice.dat", TAMANHO_CEP, ORDEM_ARVORE))
    {
        printf("Nao foi possivel criar indice.dat. O arquivo pode ja existir.\n");
        fclose(arquivo);
        return 1;
    }

    arvore = bp_open("indice.dat");

    if (arvore == NULL)
    {
        printf("Nao foi possivel abrir a arvore B+\n");
        fclose(arquivo);
        return 1;
    }

    while (fread(&endereco, sizeof(Endereco), 1, arquivo) == 1)
    {
        memcpy(chave, endereco.cep, TAMANHO_CEP);

        if (!bp_insert(arvore, chave, posicao))
        {
            printf("Erro ao inserir o CEP no indice.\n");
            bp_close(arvore);
            fclose(arquivo);
            return 1;
        }

        posicao++;
    }

    if (ferror(arquivo))
    {
        printf("Erro durante a leitura do arquivo de CEPs.\n");
        bp_close(arvore);
        fclose(arquivo);
        return 1;
    }

    bp_close(arvore);
    fclose(arquivo);

    printf("Indice B+ gerado com %lu registros!\n", posicao);

    return 0;
}