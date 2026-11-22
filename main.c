#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static int read_number(const char *prompt, double *value)
{
    char line[128];
    char *end;

    fputs(prompt, stdout);
    if (fgets(line, sizeof line, stdin) == NULL) {
        return 0;
    }

    errno = 0;
    double parsed = strtod(line, &end);
    if (end == line || errno == ERANGE || !isfinite(parsed)) {
        return -1;
    }

    while (isspace((unsigned char)*end)) {
        ++end;
    }
    if (*end != '\0') {
        return -1;
    }

    *value = parsed;
    return 1;
}

static int read_operator(char *operation)
{
    char line[32];
    char *cursor;

    fputs("Choose an operation (+, -, *, /): ", stdout);
    if (fgets(line, sizeof line, stdin) == NULL) {
        return 0;
    }

    cursor = line;
    while (isspace((unsigned char)*cursor)) {
        ++cursor;
    }
    if (*cursor == '\0') {
        return -1;
    }

    *operation = *cursor++;
    while (isspace((unsigned char)*cursor)) {
        ++cursor;
    }
    return *cursor == '\0' ? 1 : -1;
}

int main(void)
{
    double left;
    double right;
    double result;
    char operation;

    int status = read_number("Enter the first number: ", &left);
    if (status != 1) {
        fputs(status == 0 ? "Input ended.\n" : "Please enter a valid finite number.\n", stderr);
        return 1;
    }

    status = read_operator(&operation);
    if (status != 1) {
        fputs(status == 0 ? "Input ended.\n" : "Please enter one operation.\n", stderr);
        return 1;
    }

    status = read_number("Enter the second number: ", &right);
    if (status != 1) {
        fputs(status == 0 ? "Input ended.\n" : "Please enter a valid finite number.\n", stderr);
        return 1;
    }

    switch (operation) {
    case '+':
        result = left + right;
        break;
    case '-':
        result = left - right;
        break;
    case '*':
        result = left * right;
        break;
    case '/':
        if (right == 0.0) {
            fputs("Cannot divide by zero.\n", stderr);
            return 1;
        }
        result = left / right;
        break;
    default:
        fputs("Unknown operation. Use +, -, *, or /.\n", stderr);
        return 1;
    }

    if (!isfinite(result)) {
        fputs("The result is outside the supported numeric range.\n", stderr);
        return 1;
    }

    printf("Result: %.10g\n", result);
    return 0;
}
