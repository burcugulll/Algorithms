#include <stdio.h>
#include <stdlib.h>
//BURCU GÜL 20010011066

typedef struct HuffmanNode
{
    char data;
    unsigned freq;
    struct HuffmanNode *left, *right;
} HuffmanNode;

HuffmanNode* new_node(char data, unsigned freq)
{
    HuffmanNode* node = (HuffmanNode*)malloc(sizeof(HuffmanNode));
    node->data = data;
    node->freq = freq;
    node->left = node->right = NULL;
    return node;
}
// Min frekans bulma
int find_min(int freq[], int n)
{
    int min = 0;
    for (int i = 1; i < n; ++i)
    {
        if (freq[i] > 0 && freq[i] < freq[min])
        {
            min = i;
        }
    }
    return min;
}
// Huffman ağacını preorder yazdırma
void preorder_traversal(HuffmanNode* root, char codes[256][10], char temp[], int index)
{
    if (root != NULL)
    {
        printf("%c: %u\n", root->data, root->freq);

        temp[index] = '0';
        preorder_traversal(root->left, codes, temp, index + 1);

        temp[index] = '1';
        preorder_traversal(root->right, codes, temp, index + 1);

        if (root->left == NULL && root->right == NULL)
        {
            temp[index] = '\0';
            printf("%c: %s\n", root->data, temp);
            for (int i = 0; temp[i] != '\0'; ++i)
            {
                codes[(unsigned char)root->data][i] = temp[i];
            }
        }
    }
}
// Huffman ağacını oluşturma
HuffmanNode* build_huffman_tree(int freq[256])
{
    HuffmanNode *left, *right, *top;
    char codes[256][10];
    char temp[10];
    for (int i = 0; i < 256; ++i)
    {
        codes[i][0] = '\0';
    }
    for (int i = 0; i < 256; ++i)
    {
        if (freq[i] > 0)
        {
            printf("'%c' karakterinin frekansi: %d\n", i, freq[i]);
        }
    }
    int min1 = find_min(freq, 256);
    freq[min1] = 0;
    int min2 = find_min(freq, 256);
    left = new_node(min1, freq[min1]);
    right = new_node(min2, freq[min2]);
    top = new_node('$', freq[min1] + freq[min2]);
    top->left = left;
    top->right = right;
    freq[min1] = freq[min2] = 0;
    freq[top->data] = top->freq;

    for (int i = 2; i < 256; ++i)
    {
        if (freq[i] > 0)
        {
            right = top;
            top = new_node(i, freq[i]);
            top->left = left;
            top->right = right;
            freq[i] = 0;
            freq[top->data] = top->freq;
        }
    }
    printf("Olusturulan Huffman Agaci Durumu (preorder):\n");
    preorder_traversal(top, codes, temp, 0);
    return top;
}

// Metni Huffman kodlama tablosuna göre kodlama
void encode_text(FILE* input, HuffmanNode* root, FILE* output)
{
    char codes[256][10];
    int bitBuffer = 0;
    int bitCount = 0;
    char temp[10];
    preorder_traversal(root, codes, temp, 0);
    char ch;
    while ((ch = fgetc(input)) != EOF)
    {
        char *code = codes[(unsigned char)ch];

        for (int i = 0; code[i] != '\0'; ++i)
        {
            bitBuffer <<= 1;
            bitBuffer |= (code[i] == '1') ? 1 : 0;
            bitCount++;

            if (bitCount == 8)
            {
                fputc(bitBuffer, output);
                bitBuffer = 0;
                bitCount = 0;
            }
        }
    }
    if (bitCount > 0)
    {
        bitBuffer <<= (8 - bitCount);
        fputc(bitBuffer, output);
    }
}
// Sıkıştırılmış veriyi Huffman ağacı kullanarak orijinal metne dönüştürme
void decode_text(HuffmanNode* root, FILE* input, FILE* output)
{
    HuffmanNode* current = root;
    int bitBuffer;
    int bitCount = 0;

    while ((bitBuffer = fgetc(input)) != EOF)
    {
        for (int i = 0; i < 8; ++i)
        {
            int bit = (bitBuffer >> (7 - i)) & 1;
            if (bit == 0)
            {
                current = current->left;
            }
            else
            {
                current = current->right;
            }
            if (current->left == NULL && current->right == NULL)
            {
                fputc(current->data, output);
                current = root;
            }
        }
    }
}

int main()
{
    int freq[256] = {0};

    FILE* input_file = fopen("girdi.txt", "r");
    if (input_file == NULL)
    {
        fprintf(stderr, "Giris dosyasi acilamadi!\n");
        return 1;
    }
    printf("Girdi Dosyasi Icerigi:\n");
    char ch;
    while ((ch = fgetc(input_file)) != EOF)
    {
        printf("%c", ch);
    }
    printf("\n");
    printf("Girdi dosyasindaki frekanslar:\n");

    rewind(input_file);
    while ((ch = fgetc(input_file)) != EOF)
    {
        freq[(unsigned char)ch]++;
    }
    HuffmanNode* root = build_huffman_tree(freq);

    fclose(input_file);
    FILE* compressed_file = fopen("compressed.txt", "w");
    if (compressed_file == NULL)
    {
        fprintf(stderr, "compressed.txt dosyasi acilamadi!\n");
        return 1;
    }
    FILE* decoded_output_file = fopen("decoded.txt", "w");
    if (decoded_output_file == NULL)
    {
        fprintf(stderr, "Decoded dosya acilamadi!\n");
        return 1;
    }

    decode_text(root, compressed_file, decoded_output_file);
    printf("Compressed dosyasi:\n");
    for (int i = 0; i < 256; ++i)
    {
        if (freq[i] > 0)
        {
            fprintf(compressed_file, "%c%d", i, freq[i]);
            printf("%c%d", i, freq[i]);
        }
    }
    fclose(compressed_file);
    fclose(decoded_output_file);
    printf("\nDecoding tamamlandi.\n");
    printf("\n");

    for (int i = 0; i < 256; ++i)
    {
        if (freq[i] > 0)
        {
            printf("%c%d", i, freq[i]);
        }
    }
    printf("\n");
    return 0;
}
