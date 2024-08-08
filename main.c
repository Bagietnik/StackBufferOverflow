#include <stdio.h>
#include <string.h>

void access_granted() {
    printf("Access granted!\n");
}

void check_password(char *input) {
    const char correct_password[] = "secret";
    if (strcmp(input, correct_password) == 0){
        access_granted();
    } else {
        printf("Wrong password!\n");
    }
}

void vulnerable_function() {
    char buffer[10];
    printf("Enter the password: ");
    scanf("%s", buffer);
    check_password(buffer);
}

int main() {
    vulnerable_function();
    return 0;
}

