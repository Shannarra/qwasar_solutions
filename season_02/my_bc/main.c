#include <stdio.h>  // i/o
#include <stdlib.h> // atoi
#include <ctype.h>  // isalnum
#include <string.h> // strlen
#include <assert.h> // assert

#define EXPRESSION_MAX_LENGTH 100
#define EVALUATION_ERROR_RESULT -132456789

#define ANSI_COLOR_RESET  "\033[0m"
#define ANSI_COLOR_YELLOW "\033[0;33m"
#define ANSI_COLOR_GREEN  "\x1b[32m"
#define ANSI_COLOR_RED    "\033[0;31m"

#define SIZE 100
char conversion_stack[100];
int conversion_stack_top = -1;

void conversion_stack_push(char x)
{
    conversion_stack[++conversion_stack_top] = x;
}

char conversion_stack_pop()
{
    return conversion_stack_top == -1 ? -1 : conversion_stack[conversion_stack_top--];
}

int operator_precedence(char x)
{
    if(x == '(')
        return 0;

    if(x == '+' || x == '-')
        return 1;

    if(x == '*' || x == '/')
        return 2;

    return 0;
}

int is_operator(char op) {
    return op == '+' || op == '-' || op == '/' || op == '*';
}

int infix_to_postfix(char exp[EXPRESSION_MAX_LENGTH], char post[EXPRESSION_MAX_LENGTH]) {
    char *e, x;
    int postfix_index = 0;

    e = exp;

    while(*e != '\0')                                              //WHILE IN THE STRING 
    {
        if(isalnum(*e))                                                     
            post[postfix_index++] = *e;                            //IF ITS A NUMBER ADD TO STACK
        else if(*e == '(')
            conversion_stack_push(*e);                             //OPENING BRACKET ADD TO OPERATORS STACK
        else if(*e == ')')                                         //IF CLOSING BRACKET FOUND POP ALL OPERATOR TILL OPENING BRACKET          
        {
            while((x = conversion_stack_pop()) != '('){
                post[postfix_index++] = x;
            }
        }
        else                                                //IF NOT ASY OF THE ABOVE THEN CHECK PRECIDENSE
        {                                                   //AND ADD TO STACK
            post[postfix_index++] = ' ';
            while(operator_precedence(conversion_stack[conversion_stack_top]) >= operator_precedence(*e))
            post[postfix_index++] = conversion_stack_pop();
            conversion_stack_push(*e);
        }
        e++;
    }

    while(conversion_stack_top != -1)                       //ADD ALL THE REST AT THE END OF THE LOOP
    {
        post[postfix_index++] = conversion_stack_pop();
    }
    return postfix_index;
}

int evaluation_stack[20];                                                   
int evaluation_stack_top = -1;

void evaluation_stack_push(int x)
{
    evaluation_stack[++evaluation_stack_top] = x;
}

int evaluation_stack_pop()
{
    return evaluation_stack[evaluation_stack_top--];
}

int validate_user_input(char* expr) {
    
    size_t len = strlen(expr);
    // CHECK IF THE USER INPUT IS CORRECT 
    // DON'T ALLOW SINGULAR NUMBER EXPRESSION
    // DON'T ALLOW OPERATOR FOLLOWED BY A ( OR )
    // DON'T ALLOW + AND - FOLLOWED BY * OR /
    // DON'T ALLOW TEXT CHARACTERS! -> new

    int numbers_count = 0, operators_count = 0;

    if (is_operator(expr[len-1])) return 1;

    for (size_t i = 0; i < len; ++i) {
        if (expr[i] == ' ') {
            fprintf(
                    stderr,
                    ANSI_COLOR_RED
                    "ERROR: Whitespace detected. Please, remove all whitespace characters from your expression.\n"
                    ANSI_COLOR_RESET
            );
            return 1;
        }

        if ((expr[i] >= 'a' && expr[i] <= 'z') || (expr[i] >= 'A' && expr[i] <= 'Z')) {
            fprintf(stderr,
                    ANSI_COLOR_RED
                    "ERROR: Detected non-alphanumeric character. Allowed characters are + - * / and numbers.\n"
                    ANSI_COLOR_RESET
            );
            return 1;
        }

        char curr = expr[i], next = expr[i+1];

        if (is_operator(curr) && is_operator(next)) {
            if (next == '/' || next == '*') return 1;
        }

        if (isalnum(curr)) {
            if (next == '(') return 1;
            numbers_count++;
        } else if (is_operator(curr)) {
            if (next == ')') return 1;
            if (curr == '*' && next == '-') return 1;
            operators_count++;
        }
    }

    return numbers_count < 2 || operators_count < 1;
}

int evaluate(char* infix) {
    char exp[EXPRESSION_MAX_LENGTH];                
    char *e;
    int n1,n2,n3;

    int postfix_length = infix_to_postfix(infix, exp);
    exp[postfix_length] = '\0';

    e = exp;
    while(*e != '\0')
    {
        if (*e == ' ') {
            e++;                                
            continue;
        }

        if(isalnum(*e))                                               // IF CURR CHARACTER IS A NUMBER THEN GET ALL OF IT 
        {
            char str[EXPRESSION_MAX_LENGTH] = {0};
            int idx = 0;
            str[idx++] = *e;

            while (isalnum(*(e+1))){
                str[idx++] = *(e+1);
                e++;
            }

            str[idx] = '\0';
            evaluation_stack_push(atoi(str));                       // ATOI TO INT STACK 
        }
        else
        {                                                           // IF OPERATOR FOUND POP 2 TOP NUMBERS AND EVALUATE 
            n1 = evaluation_stack_pop();
            n2 = evaluation_stack_pop();

            switch(*e)
            {
                case '+':
                {
                    n3 = n1 + n2;
                    break;
                }
                case '-':
                {
                    n3 = n2 - n1;
                    break;
                }
                case '*':
                {
                    n3 = n1 * n2;
                    break;
                }
                case '/':
                {
                    if (n1 == 0) {                                      //DONT ALLOW TO DIVIDE BY ZERO
                        fprintf(stderr,
                                ANSI_COLOR_RED
                                "ERROR: Attempted to divide by 0.\n"
                                ANSI_COLOR_RESET
                        );
                        return EVALUATION_ERROR_RESULT;
                    }
                    if (n2 == 0) {
                        fprintf(stderr,                                 //DONT ALLOW TO DIVIDE ZERO
                                ANSI_COLOR_RED
                                "ERROR: Dividing 0 by %d. This doesn't make sense!\n"
                                ANSI_COLOR_RESET,
                                n1
                        );
                        return EVALUATION_ERROR_RESULT;
                    }

                    n3 = n2 / n1;
                    break;
                }
            }
            evaluation_stack_push(n3);
        }
        e++;
    }
    return evaluation_stack_pop();
}

void test(char* expr, int res) {
    printf("\n---\n"ANSI_COLOR_YELLOW"Testing expression \"%s\""ANSI_COLOR_RESET"\n", expr);
    assert(evaluate(expr) == res);
    printf(ANSI_COLOR_GREEN"%s got the correct answer of %d"ANSI_COLOR_RESET"\n---\n", expr, res);
}

int automated_tests() {
    printf("Running automated tests:\n");

    test("12/(1+3)-9*6",         -51);
    test("-(-((-4)+-6))",        -10);
    test("11+3-7*2+1*4/2",       2);
    test("5*10-(8*6-15)+4*20/4", 37);
    test("7*9+3-6/2+2*2-11",     56);

    printf(ANSI_COLOR_GREEN"All automated tests ran with success! 😍\n");

    return 0;
}

int main(int argc, char** argv)
{
    if (argc == 1) {
        fprintf(stderr,
                ANSI_COLOR_RED
                "ERROR: You haven't provided an expression.\n"
                ANSI_COLOR_RESET
        );
        return automated_tests();
    } else if (argc > 2) {
        fprintf(stderr,
                ANSI_COLOR_RED
                "ERROR: Multiple arguments or expressions are not supported at the moment.\n"
                ANSI_COLOR_RESET
        );
        return automated_tests();
    }

    if (strlen(argv[1]) > EXPRESSION_MAX_LENGTH) {
        fprintf(stderr,
                "Expression is too long. Please, shorten it to maximum %d characters.\n",
                EXPRESSION_MAX_LENGTH
        );
        return -1;
    }

    if (validate_user_input(argv[1])) {
        fprintf(stderr,
                ANSI_COLOR_RED
                "ERROR: Invalid expression provided!\n"
                ANSI_COLOR_RESET
        );
        return -1;
    }

    int result = evaluate(argv[1]);
    if (result != EVALUATION_ERROR_RESULT)
        printf("Result: %d\n", evaluate(argv[1]));

    return 0;
}