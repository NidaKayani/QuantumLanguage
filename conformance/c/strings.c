#include <stdio.h>
#include <string.h>
#include <ctype.h>

void reverse(char *s) {
    int n = strlen(s);
    for (int i = 0; i < n / 2; i++) {
        char t = s[i];
        s[i] = s[n - 1 - i];
        s[n - 1 - i] = t;
    }
}

int main(void) {
    char s[50] = "Hello";
    char t[50];
    strcpy(t, s);
    strcat(s, ", World");
    printf("%s (%zu) %s\n", s, strlen(s), t);
    printf("%d %d %d\n", strcmp("abc", "abd") < 0, strcmp("b", "a") > 0, strcmp(t, "Hello"));
    char *p = strchr(s, 'W');
    printf("%s\n", p);
    char *q = strstr(s, "lo, ");
    printf("%s\n", q);
    strncpy(t, "Quantum", 3);
    t[3] = '\0';
    printf("%s\n", t);
    reverse(s);
    printf("%s\n", s);
    char up[20] = "mixed Case 123";
    for (int i = 0; up[i]; i++) up[i] = toupper(up[i]);
    printf("%s\n", up);
    int digits = 0, alphas = 0, spaces = 0;
    const char *text = "a1 b2 c3!";
    for (const char *c = text; *c; c++) {
        if (isdigit(*c)) digits++;
        else if (isalpha(*c)) alphas++;
        else if (isspace(*c)) spaces++;
    }
    printf("%d %d %d\n", digits, alphas, spaces);
    char buf[64];
    sprintf(buf, "%s-%d-%.1f", "id", 7, 2.5);
    puts(buf);
    printf("%d\n", (int)strlen(""));
    return 0;
}
