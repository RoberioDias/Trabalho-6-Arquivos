#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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

int main(int argc, char **argv)
{
    BP_TREE *arvore;
    FILE *arquivo;
    Endereco endereco;
    unsigned long posicao;
    char chave[TAMANHO_CEP];
    int i;

    if (argc != 2)
    {
        printf("Forma de uso: %s CEP\n", argv[0]);
        return 1;
    }

    if (strlen(argv[1]) != TAMANHO_CEP)
    {
        printf("O CEP deve conter exatamente 8 digitos.\n");
        return 1;
    }

    for (i = 0; i < TAMANHO_CEP; i++)
    {
        if (!isdigit((unsigned char)argv[1][i]))
        {
            printf("O CEP deve conter apenas numeros.\n");
            return 1;
        }
    }

    memcpy(chave, argv[1], TAMANHO_CEP);

    arvore = bp_open("indice.dat");

    if (arvore == NULL)
    {
        printf("Nao foi possivel abrir indice.dat. Gere o indice primeiro.\n");
        return 1;
    }

    arquivo = fopen("cep_ordenado.dat", "rb");

    if (arquivo == NULL)
    {
        printf("Nao foi possivel abrir o arquivo cep_ordenado.dat\n");
        bp_close(arvore);
        return 1;
    }

    if (bp_find(arvore, chave, &posicao))
    {
        if (fseek(arquivo, (long)(posicao * sizeof(Endereco)), SEEK_SET) == 0
            && fread(&endereco, sizeof(Endereco), 1, arquivo) == 1)
        {
            printf("\nEndereco localizado:\n");
            printf("%.72s\n", endereco.logradouro);
            printf("%.72s\n", endereco.bairro);
            printf("%.72s\n", endereco.cidade);
            printf("%.72s\n", endereco.uf);
            printf("%.2s\n", endereco.sigla);
            printf("%.8s\n", endereco.cep);
        }
        else
        {
            printf("Erro ao acessar o registro do endereco.\n");
        }
    }
    else
    {
        printf("Nenhum endereco encontrado para este CEP.\n");
    }

    fclose(arquivo);
    bp_close(arvore);

    return 0;
}