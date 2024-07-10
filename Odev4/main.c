#include <stdio.h>
#include <stdlib.h>
//BURCU GÜL
//20010011066
// Red-Black Tree düğüm yapısı
typedef struct Node {
    int data;
    char color; // 'k' kırmızı, 's' siyah
    struct Node *left, *right, *parent;
} Node;

Node* root = NULL;

// Yeni düğüm oluşturma
Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->color = 'k'; // Yeni düğümler başlangıçta kırmızıdır
    newNode->left = newNode->right = newNode->parent = NULL;
    return newNode;
}

// Sol döndürme
void rotateLeft(Node** root, Node* pt) {
    Node* pt_right = pt->right;

    pt->right = pt_right->left;

    if (pt->right != NULL)
        pt->right->parent = pt;

    pt_right->parent = pt->parent;

    if (pt->parent == NULL)
        *root = pt_right;

    else if (pt == pt->parent->left)
        pt->parent->left = pt_right;

    else
        pt->parent->right = pt_right;

    pt_right->left = pt;
    pt->parent = pt_right;
}

// Sağ döndürme
void rotateRight(Node** root, Node* pt) {
    Node* pt_left = pt->left;

    pt->left = pt_left->right;

    if (pt->left != NULL)
        pt->left->parent = pt;

    pt_left->parent = pt->parent;

    if (pt->parent == NULL)
        *root = pt_left;

    else if (pt == pt->parent->left)
        pt->parent->left = pt_left;

    else
        pt->parent->right = pt_left;

    pt_left->right = pt;
    pt->parent = pt_left;
}

// Red-Black Tree fix-up
void fixViolation(Node** root, Node* pt) {
    Node* parent_pt = NULL;
    Node* grand_parent_pt = NULL;

    while ((pt != *root) && (pt->color != 's') && (pt->parent->color == 'k')) {
        parent_pt = pt->parent;
        grand_parent_pt = pt->parent->parent;

        // Parent düğümü sol çocuksa
        if (parent_pt == grand_parent_pt->left) {
            Node* uncle_pt = grand_parent_pt->right;

            // Uncle düğümü kırmızı ise sadece renk değiştir
            if (uncle_pt != NULL && uncle_pt->color == 'k') {
                grand_parent_pt->color = 'k';
                parent_pt->color = 's';
                uncle_pt->color = 's';
                pt = grand_parent_pt;
            }
            else {
                // PT sağ çocuksa sol döndür
                if (pt == parent_pt->right) {
                    rotateLeft(root, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }

                // PT sol çocuksa sağ döndür
                rotateRight(root, grand_parent_pt);
                char temp = parent_pt->color;
                parent_pt->color = grand_parent_pt->color;
                grand_parent_pt->color = temp;
                pt = parent_pt;
            }
        }
        // Parent düğümü sağ çocuksa
        else {
            Node* uncle_pt = grand_parent_pt->left;

            // Uncle düğümü kırmızı ise sadece renk değiştir
            if ((uncle_pt != NULL) && (uncle_pt->color == 'k')) {
                grand_parent_pt->color = 'k';
                parent_pt->color = 's';
                uncle_pt->color = 's';
                pt = grand_parent_pt;
            }
            else {
                // PT sol çocuksa sağ döndür
                if (pt == parent_pt->left) {
                    rotateRight(root, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }

                // PT sağ çocuksa sol döndür
                rotateLeft(root, grand_parent_pt);
                char temp = parent_pt->color;
                parent_pt->color = grand_parent_pt->color;
                grand_parent_pt->color = temp;
                pt = parent_pt;
            }
        }
    }

    (*root)->color = 's';
}

// Ağaca düğüm ekleme
void insert(const int data) {
    Node* newNode = createNode(data);
    if (root == NULL) {
        newNode->color = 's'; // Kök düğüm siyah olur
        root = newNode;
    }
    else {
        Node* temp = root;
        while (1) {
            if (data < temp->data) {
                if (temp->left == NULL) {
                    temp->left = newNode;
                    newNode->parent = temp;
                    break;
                }
                else {
                    temp = temp->left;
                }
            }
            else if (data > temp->data) {
                if (temp->right == NULL) {
                    temp->right = newNode;
                    newNode->parent = temp;
                    break;
                }
                else {
                    temp = temp->right;
                }
            }
            else {

                return;
            }
        }
        fixViolation(&root, newNode);
    }
}

// Minimum değeri bulma
Node* minValueNode(Node* node) {
    Node* ptr = node;

    while (ptr->left != NULL)
        ptr = ptr->left;

    return ptr;
}

// Silme fix-up
void fixDelete(Node** root, Node* x) {
    while (x != *root && x->color == 's') {
        if (x == x->parent->left) {
            Node* sibling = x->parent->right;

            if (sibling->color == 'k') {
                sibling->color = 's';
                x->parent->color = 'k';
                rotateLeft(root, x->parent);
                sibling = x->parent->right;
            }

            if ((sibling->left == NULL || sibling->left->color == 's') &&
                (sibling->right == NULL || sibling->right->color == 's')) {
                sibling->color = 'k';
                x = x->parent;
            }
            else {
                if (sibling->right == NULL || sibling->right->color == 's') {
                    if (sibling->left != NULL)
                        sibling->left->color = 's';
                    sibling->color = 'k';
                    rotateRight(root, sibling);
                    sibling = x->parent->right;
                }

                sibling->color = x->parent->color;
                x->parent->color = 's';
                if (sibling->right != NULL)
                    sibling->right->color = 's';
                rotateLeft(root, x->parent);
                x = *root;
            }
        }
        else {
            Node* sibling = x->parent->left;

            if (sibling->color == 'k') {
                sibling->color = 's';
                x->parent->color = 'k';
                rotateRight(root, x->parent);
                sibling = x->parent->left;
            }

            if ((sibling->left == NULL || sibling->left->color == 's') &&
                (sibling->right == NULL || sibling->right->color == 's')) {
                sibling->color = 'k';
                x = x->parent;
            }
            else {
                if (sibling->left == NULL || sibling->left->color == 's') {
                    if (sibling->right != NULL)
                        sibling->right->color = 's';
                    sibling->color = 'k';
                    rotateLeft(root, sibling);
                    sibling = x->parent->left;
                }

                sibling->color = x->parent->color;
                x->parent->color = 's';
                if (sibling->left != NULL)
                    sibling->left->color = 's';
                rotateRight(root, x->parent);
                x = *root;
            }
        }
    }
    x->color = 's';
}

// Ağacın düğümünü silme
void deleteNode(Node** root, Node* node) {
    Node* z = node;
    Node* y = z;
    Node* x = NULL;
    char y_original_color = y->color;

    if (z->left == NULL) {
        x = z->right;
        if (z->parent == NULL)
            *root = x;
        else if (z == z->parent->left)
            z->parent->left = x;
        else
            z->parent->right = x;
        if (x != NULL)
            x->parent = z->parent;
    }
    else if (z->right == NULL) {
        x = z->left;
        if (z->parent == NULL)
            *root = x;
        else if (z == z->parent->left)
            z->parent->left = x;
        else
            z->parent->right = x;
        if (x != NULL)
            x->parent = z->parent;
    }
    else {
        y = minValueNode(z->right);
        y_original_color = y->color;
        x = y->right;
        if (y->parent == z) {
            if (x != NULL)
                x->parent = y;
        }
        else {
            if (x != NULL)
                x->parent = y->parent;
            y->parent->left = x;
            y->right = z->right;
            z->right->parent = y;
        }
        if (z->parent == NULL)
            *root = y;
        else if (z == z->parent->left)
            z->parent->left = y;
        else
            z->parent->right = y;
        y->parent = z->parent;
        y->color = z->color;
        y->left = z->left;
        y->left->parent = y;
    }

    free(z);

    if (y_original_color == 's' && x != NULL)
        fixDelete(root, x);
}

// Belirli bir değeri bulma
Node* search(Node* root, int data) {
    while (root != NULL && data != root->data) {
        if (data < root->data)
            root = root->left;
        else
            root = root->right;
    }
    return root;
}

// Ağacı alt alta yazdırma
void printTree(Node* root, int space) {
    if (root == NULL)
        return;

    space += 10;

    printTree(root->right, space);

    printf("\n");
    for (int i = 10; i < space; i++)
        printf(" ");
    printf("%d%c\n", root->data, root->color);

    printTree(root->left, space);
}

// Menü
void menu() {
    int choice, data;
    while (1) {
        printf("1. Ekle\n");
        printf("2. Sil\n");
        printf("3. Yazdir\n");
        printf("4. Cikis\n");
        printf("Seciminiz: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Eklemek istediginiz sayiyi girin: ");
                scanf("%d", &data);
                insert(data);
                printTree(root, 0);
                break;
            case 2:
                printf("Silmek istediginiz sayiyi girin: ");
                scanf("%d", &data);
                Node* node = search(root, data);
                if (node != NULL) {
                    deleteNode(&root, node);
                    printTree(root, 0);
                }
                else
                    printf("Sayi agacta bulunamadi.\n");
                break;
            case 3:
                printTree(root, 0);
                break;
            case 4:
                exit(0);
            default:
                printf("Gecersiz secim!\n");
        }
    }
}

int main() {
    menu();
    return 0;
}

