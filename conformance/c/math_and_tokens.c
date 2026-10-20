#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

int main(void) {
    printf("%.4f %.4f %.1f %.1f\n", sqrt(2.0), pow(2.0, 0.5), fabs(-3.5), floor(-2.5));
    printf("%.1f %.1f %.1f\n", ceil(2.1), round(2.5), round(-2.5));
    printf("%.4f %.4f\n", sin(M_PI / 6), cos(0.0));
    printf("%.3f %.3f\n", log(M_E), log10(1000.0));
    printf("%d %ld\n", abs(-9), labs(-123456L));
    char text[] = "alpha,beta,,gamma;delta";
    for (char *tok = strtok(text, ",;"); tok; tok = strtok(NULL, ",;")) printf("<%s>", tok);
    printf("\n");
    printf("%d %.2f %ld\n", atoi("42abc"), atof("3.75"), strtol("ff", NULL, 16));
    return 0;
}
