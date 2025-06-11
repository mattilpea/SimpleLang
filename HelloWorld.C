#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_VARS 100

typedef struct {
    char name[32];
    int value;
} Variable;

Variable vars[MAX_VARS];
int var_count = 0;

int get_var_value(const char* name) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(vars[i].name, name) == 0)
            return vars[i].value;
    }
    printf("Error: Undefined variable '%s'\n", name);
    exit(1);
}

void set_var_value(const char* name, int value) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(vars[i].name, name) == 0) {
            vars[i].value = value;
            return;
        }
    }
    strcpy(vars[var_count].name, name);
    vars[var_count].value = value;
    var_count++;
}

int eval_expr(char* expr) {
    // support simple: value, value + value, var + value, etc.
    char* token = strtok(expr, "+");
    int total = 0;
    while (token) {
        while (isspace(*token)) token++;
        if (isalpha(token[0])) {
            total += get_var_value(token);
        } else {
            total += atoi(token);
        }
        token = strtok(NULL, "+");
    }
    return total;
}

void interpret_line(char* line) {
    if (strncmp(line, "print", 5) == 0) {
        char* var = line + 6;
        while (isspace(*var)) var++;
        printf("%d\n", get_var_value(var));
    } else {
        // handle assignment: x = 3 + 4
        char* eq = strchr(line, '=');
        if (!eq) {
            printf("Error: invalid statement\n");
            return;
        }

        *eq = '\0';
        char* name = line;
        char* expr = eq + 1;

        while (isspace(*name)) name++;
        while (isspace(*(name + strlen(name) - 1))) name[strlen(name) - 1] = '\0';

        int value = eval_expr(expr);
        set_var_value(name, value);
    }
}

int main() {
    char line[256];
    printf("SimpleLang > Type 'exit' to quit.\n");
    while (1) {
        printf("> ");
        fgets(line, sizeof(line), stdin);
        line[strcspn(line, "\n")] = '\0';
        if (strncmp(line, "exit", 4) == 0)
            break;
        interpret_line(line);
    }
    return 0;
}
