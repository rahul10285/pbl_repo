#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 100

// ===== COLOR MACROS =====
#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define YELLOW "\033[1;33m"
#define RESET "\033[0m"

// ===== STRUCTURES =====
struct User {
    char username[30];
    char password[30];
};

struct Cloth {
    int id;
    char name[50];
    char size[10];
    char owner[30];
};

struct Cloth clothes[MAX];
int clothCount = 0;

// ================= FILE FUNCTIONS =================
void loadClothes() {
    FILE *fp = fopen("clothes.txt", "r");
    if (!fp) return;

    while (fscanf(fp, "%d,%49[^,],%9[^,],%29[^\n]\n",
                  &clothes[clothCount].id,
                  clothes[clothCount].name,
                  clothes[clothCount].size,
                  clothes[clothCount].owner) == 4) {
        clothCount++;
    }
    fclose(fp);
}

void saveClothes() {
    FILE *fp = fopen("clothes.txt", "w");
    for (int i = 0; i < clothCount; i++) {
        fprintf(fp, "%d,%s,%s,%s\n",
                clothes[i].id,
                clothes[i].name,
                clothes[i].size,
                clothes[i].owner);
    }
    fclose(fp);
}

// ================= SIGNUP =================
void signup() {
    struct User u;
    FILE *fp = fopen("users.txt", "a");

    printf("\nEnter new username: ");
    scanf("%s", u.username);
    printf("Enter new password: ");
    scanf("%s", u.password);

    fprintf(fp, "%s %s\n", u.username, u.password);
    fclose(fp);

    printf(GREEN "✅ Signup successful!\n" RESET);
}

// ================= LOGIN =================
int login(char currentUser[]) {
    char user[30], pass[30];
    struct User u;
    FILE *fp = fopen("users.txt", "r");

    if (!fp) {
        printf(RED "No users found. Please signup first.\n" RESET);
        return 0;
    }

    printf("\nUsername: ");
    scanf("%s", user);
    printf("Password: ");
    scanf("%s", pass);

    while (fscanf(fp, "%s %s", u.username, u.password) == 2) {
        if (strcmp(user, u.username) == 0 &&
            strcmp(pass, u.password) == 0) {
            strcpy(currentUser, user);
            fclose(fp);
            printf(GREEN "✅ Login successful!\n" RESET);
            return 1;
        }
    }

    fclose(fp);
    printf(RED "❌ Invalid login!\n" RESET);
    return 0;
}

// ================= ADD CLOTH =================
void addCloth(char currentUser[]) {
    if (clothCount >= MAX) return;

    printf("\nEnter Cloth ID: ");
    scanf("%d", &clothes[clothCount].id);

    printf("Enter Cloth Name: ");
    scanf(" %[^\n]", clothes[clothCount].name);

    printf("Enter Size: ");
    scanf("%s", clothes[clothCount].size);

    strcpy(clothes[clothCount].owner, currentUser);

    clothCount++;
    saveClothes();

    printf(GREEN "✅ Cloth added successfully!\n" RESET);
}

// ================= VIEW =================
void viewClothes() {
    if (clothCount == 0) {
        printf(YELLOW "\nNo clothes available.\n" RESET);
        return;
    }

    printf(YELLOW "\n------ AVAILABLE CLOTHES ------\n" RESET);
    for (int i = 0; i < clothCount; i++) {
        printf("\nID: %d | %s | Size: %s | Owner: %s",
               clothes[i].id,
               clothes[i].name,
               clothes[i].size,
               clothes[i].owner);
    }
    printf("\n");
}

// ================= SEARCH =================
void searchBySize() {
    char size[10];
    int found = 0;

    printf("\nEnter size: ");
    scanf("%s", size);

    for (int i = 0; i < clothCount; i++) {
        if (strcmp(size, clothes[i].size) == 0) {
            printf("\nID:%d | %s | Owner:%s",
                   clothes[i].id,
                   clothes[i].name,
                   clothes[i].owner);
            found = 1;
        }
    }

    if (!found)
        printf(RED "\nNo matching clothes.\n" RESET);
}

// ================= SWAP =================
void requestSwap() {
    int id, found = 0;

    printf("\nEnter Cloth ID to swap: ");
    scanf("%d", &id);

    for (int i = 0; i < clothCount; i++) {
        if (clothes[i].id == id) {
            printf(GREEN "\n🔄 Swap request sent to %s!\n" RESET,
                   clothes[i].owner);
            found = 1;
            break;
        }
    }

    if (!found)
        printf(RED "Cloth not found.\n" RESET);
}

// ================= MAIN =================
int main() {
    int choice, authChoice;
    char currentUser[30];

    loadClothes();

    printf(YELLOW "\n==== CLOTHES SWAP APP ====\n" RESET);
    printf("1. Signup\n2. Login\nChoice: ");
    scanf("%d", &authChoice);

    if (authChoice == 1) signup();

    if (!login(currentUser)) return 0;

    do {
        printf(YELLOW "\n===== MENU =====\n" RESET);
        printf("1. Add Cloth\n");
        printf("2. View Clothes\n");
        printf("3. Search by Size\n");
        printf("4. Request Swap\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addCloth(currentUser); break;
            case 2: viewClothes(); break;
            case 3: searchBySize(); break;
            case 4: requestSwap(); break;
        }

    } while (choice != 5);

    printf(GREEN "\n👋 Thank you for using Clothes Swap App!\n" RESET);
    return 0;
}
