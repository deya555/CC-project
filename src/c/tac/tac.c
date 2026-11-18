
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tac.h"

#define MAX_TAC_INSTRUCTIONS 500
#define MAX_TAC_LENGTH 200

static char instructions[MAX_TAC_INSTRUCTIONS][MAX_TAC_LENGTH];
static int instruction_count = 0;
static int temp_count = 0;
static int label_count = 0;

/* Generate temporary variable */
static char *new_temp(void)
{
    static char temp[20];
    snprintf(temp, sizeof(temp), "t%d", ++temp_count);
    return temp;
}

/* Generate label */
static char *new_label(void)
{
    static char label[20];
    snprintf(label, sizeof(label), "L%d", ++label_count);
    return label;
}

/* Add TAC instruction */
static void emit(const char *instruction)
{
    if (instruction_count >= MAX_TAC_INSTRUCTIONS)
    {
        printf("TAC Error: Instruction limit exceeded.\n");
        return;
    }

    snprintf(instructions[instruction_count],
             MAX_TAC_LENGTH, "%s", instruction);

    instruction_count++;
}

/* Constant arithmetic operations */
static int fold_binary_operation(
    const char *op,
    double left,
    double right,
    double *result)
{
    if (strcmp(op, "+") == 0)
        *result = left + right;

    else if (strcmp(op, "-") == 0)
        *result = left - right;

    else if (strcmp(op, "*") == 0)
        *result = left * right;

    else if (strcmp(op, "/") == 0)
    {
        if (right == 0)
            return 0;

        *result = left / right;
    }

    else if (strcmp(op, "%") == 0)
    {
        if ((int)right == 0)
            return 0;

        *result = (int)left % (int)right;
    }

    else
        return 0;

    return 1;
}

/* Recursively evaluate constant expressions */
static int evaluate_constant(
    ASTNode *node,
    double *result,
    int *is_integer)
{
    if (node == NULL)
        return 0;

    if (node->type == AST_INTEGER)
    {
        *result = strtod(node->value, NULL);
        *is_integer = 1;
        return 1;
    }

    if (node->type == AST_FLOAT)
    {
        *result = strtod(node->value, NULL);
        *is_integer = 0;
        return 1;
    }

    if (node->type != AST_BINARY_EXPR)
        return 0;

    ASTNode *left = node->child;
    ASTNode *right = left ? left->next : NULL;

    double left_value, right_value;
    int left_integer, right_integer;

    if (!evaluate_constant(left, &left_value, &left_integer))
        return 0;

    if (!evaluate_constant(right, &right_value, &right_integer))
        return 0;

    if (strcmp(node->value, "%") == 0)
    {
        if (!left_integer || !right_integer ||
            (long long)right_value == 0)
            return 0;

        *result = (long long)left_value % (long long)right_value;
        *is_integer = 1;
        return 1;
    }

    if (strcmp(node->value, "/") == 0 &&
        left_integer && right_integer)
    {
        if ((long long)right_value == 0)
            return 0;

        *result = (long long)left_value / (long long)right_value;
        *is_integer = 1;
        return 1;
    }

    if (!fold_binary_operation(
            node->value, left_value, right_value, result))
        return 0;

    *is_integer = left_integer && right_integer;
    return 1;
}

/* Generate TAC for expressions */
static char *generate_expression(ASTNode *node)
{
    static char result_buffer[100];

    if (node == NULL)
        return "";

    double constant_value;
    int is_integer;

    /* Constant folding */
    if (evaluate_constant(node, &constant_value, &is_integer))
    {
        if (is_integer)
        {
            snprintf(result_buffer, sizeof(result_buffer),
                     "%lld", (long long)constant_value);
        }
        else
        {
            snprintf(result_buffer, sizeof(result_buffer),
                     "%g", constant_value);
        }

        return result_buffer;
    }

    /* Identifiers and literal values */
    if (node->type == AST_IDENTIFIER ||
        node->type == AST_INTEGER ||
        node->type == AST_FLOAT ||
        node->type == AST_STRING ||
        node->type == AST_CHAR)
    {
        return node->value;
    }

    /* Binary expressions */
    if (node->type == AST_BINARY_EXPR)
    {
        ASTNode *left = node->child;
        ASTNode *right = left ? left->next : NULL;

        if (left == NULL || right == NULL)
            return "";

        char left_copy[100];
        char right_copy[100];

        snprintf(left_copy, sizeof(left_copy), "%s",
                 generate_expression(left));

        snprintf(right_copy, sizeof(right_copy), "%s",
                 generate_expression(right));

        char temp[20];
        snprintf(temp, sizeof(temp), "%s", new_temp());

        char instruction[MAX_TAC_LENGTH];

        snprintf(instruction, sizeof(instruction),
                 "%s = %s %s %s",
                 temp, left_copy, node->value, right_copy);

        emit(instruction);

        snprintf(result_buffer, sizeof(result_buffer),
                 "%s", temp);

        return result_buffer;
    }

    /* Unary expressions: !, - and other unary operators */
    if (node->type == AST_UNARY_EXPR)
    {
        ASTNode *operand = node->child;

        if (operand == NULL)
            return "";

        char operand_copy[100];

        snprintf(operand_copy, sizeof(operand_copy), "%s",
                 generate_expression(operand));

        char temp[20];
        snprintf(temp, sizeof(temp), "%s", new_temp());

        char instruction[MAX_TAC_LENGTH];

        snprintf(instruction, sizeof(instruction),
                 "%s = %s%s",
                 temp, node->value, operand_copy);

        emit(instruction);

        snprintf(result_buffer, sizeof(result_buffer),
                 "%s", temp);

        return result_buffer;
    }

    return node->value ? node->value : "";
}

static void generate_statement(ASTNode *node);

/* Generate TAC for if and if-else */
static void generate_if(ASTNode *node)
{
    ASTNode *condition = node->child;
    ASTNode *then_branch = condition ? condition->next : NULL;
    ASTNode *else_branch = then_branch ? then_branch->next : NULL;

    if (condition == NULL || then_branch == NULL)
        return;

    char condition_copy[100];

    snprintf(condition_copy, sizeof(condition_copy), "%s",
             generate_expression(condition));

    char else_label[20];
    char end_label[20];

    snprintf(else_label, sizeof(else_label), "%s", new_label());
    snprintf(end_label, sizeof(end_label), "%s", new_label());

    char instruction[MAX_TAC_LENGTH];

    if (else_branch != NULL)
    {
        snprintf(instruction, sizeof(instruction),
                 "ifFalse %s goto %s",
                 condition_copy, else_label);
        emit(instruction);

        generate_statement(then_branch);

        snprintf(instruction, sizeof(instruction),
                 "goto %s", end_label);
        emit(instruction);

        snprintf(instruction, sizeof(instruction),
                 "%s:", else_label);
        emit(instruction);

        generate_statement(else_branch);

        snprintf(instruction, sizeof(instruction),
                 "%s:", end_label);
        emit(instruction);
    }
    else
    {
        snprintf(instruction, sizeof(instruction),
                 "ifFalse %s goto %s",
                 condition_copy, end_label);
        emit(instruction);

        generate_statement(then_branch);

        snprintf(instruction, sizeof(instruction),
                 "%s:", end_label);
        emit(instruction);
    }
}

/* Generate TAC for while loops */
static void generate_while(ASTNode *node)
{
    ASTNode *condition = node->child;
    ASTNode *body = condition ? condition->next : NULL;

    if (condition == NULL || body == NULL)
        return;

    char start_label[20];
    char end_label[20];

    snprintf(start_label, sizeof(start_label), "%s", new_label());
    snprintf(end_label, sizeof(end_label), "%s", new_label());

    char instruction[MAX_TAC_LENGTH];

    snprintf(instruction, sizeof(instruction),
             "%s:", start_label);
    emit(instruction);

    char condition_copy[100];

    snprintf(condition_copy, sizeof(condition_copy), "%s",
             generate_expression(condition));

    snprintf(instruction, sizeof(instruction),
             "ifFalse %s goto %s",
             condition_copy, end_label);
    emit(instruction);

    generate_statement(body);

    snprintf(instruction, sizeof(instruction),
             "goto %s", start_label);
    emit(instruction);

    snprintf(instruction, sizeof(instruction),
             "%s:", end_label);
    emit(instruction);
}

/* Generate TAC for statements */
static void generate_statement(ASTNode *node)
{
    if (node == NULL)
        return;

    switch (node->type)
    {
        case AST_PROGRAM:
        case AST_BLOCK:
        {
            ASTNode *child = node->child;

            while (child != NULL)
            {
                generate_statement(child);
                child = child->next;
            }
            break;
        }

        case AST_DECLARATION:
        {
            ASTNode *type_node = node->child;
            ASTNode *variables = type_node ? type_node->next : NULL;
            ASTNode *variable = variables ? variables->child : NULL;

            while (variable != NULL)
            {
                if (variable->type == AST_VARIABLE &&
                    variable->child != NULL)
                {
                    char value[100];

                    snprintf(value, sizeof(value), "%s",
                             generate_expression(variable->child));

                    char instruction[MAX_TAC_LENGTH];

                    snprintf(instruction, sizeof(instruction),
                             "%s = %s", variable->value, value);

                    emit(instruction);
                }

                variable = variable->next;
            }
            break;
        }

        case AST_ASSIGNMENT:
        {
            ASTNode *left = node->child;
            ASTNode *right = left ? left->next : NULL;

            if (left && right)
            {
                char value[100];

                snprintf(value, sizeof(value), "%s",
                         generate_expression(right));

                char instruction[MAX_TAC_LENGTH];

                snprintf(instruction, sizeof(instruction),
                         "%s = %s", left->value, value);

                emit(instruction);
            }
            break;
        }

                case AST_PRINT:
        {
            if (node->value != NULL &&
                strcmp(node->value, "print") == 0)
            {
                ASTNode *expression = node->child;

                if (expression != NULL)
                {
                    char value[100];

                    snprintf(value, sizeof(value), "%s",
                             generate_expression(expression));

                    char instruction[MAX_TAC_LENGTH];

                    snprintf(instruction, sizeof(instruction),
                             "print %s", value);

                    emit(instruction);
                }
            }
            else
            {
                /* Existing printf handling will be added later. */
            }

            break;
        }

        case AST_IF:
            generate_if(node);
            break;

        case AST_WHILE:
            generate_while(node);
            break;

        default:
        {
            ASTNode *child = node->child;

            while (child != NULL)
            {
                generate_statement(child);
                child = child->next;
            }
            break;
        }
    }
}

/* Check whether a name is a generated temporary */
static int is_temporary(const char *name)
{
    return name != NULL &&
           name[0] == 't' &&
           name[1] >= '0' &&
           name[1] <= '9';
}

/* Remove unused temporary assignments */
static void eliminate_dead_temporaries(void)
{
    int i, j;
    int changed;

    do
    {
        changed = 0;

        for (i = 0; i < instruction_count; i++)
        {
            char destination[50];
            char extra[50];

            if (sscanf(instructions[i], "%49s = %49s",
                       destination, extra) != 2)
                continue;

            if (!is_temporary(destination))
                continue;

            int used = 0;

            for (j = 0; j < instruction_count; j++)
            {
                if (i == j)
                    continue;

                if (strstr(instructions[j], destination) != NULL)
                {
                    used = 1;
                    break;
                }
            }

            if (!used)
            {
                for (j = i; j < instruction_count - 1; j++)
                {
                    strcpy(instructions[j], instructions[j + 1]);
                }

                instruction_count--;
                changed = 1;
                break;
            }
        }

    } while (changed);
}

/* Main TAC generation function */
void tac_generate(ASTNode *root)
{
    instruction_count = 0;
    temp_count = 0;
    label_count = 0;

    generate_statement(root);

    eliminate_dead_temporaries();
}

/* Print TAC */
void tac_print(void)
{
    int i;

    printf("\n========== THREE ADDRESS CODE ==========\n");

    for (i = 0; i < instruction_count; i++)
        printf("%s\n", instructions[i]);

    printf("========================================\n");
}

/* Reset TAC data */
void tac_free(void)
{
    instruction_count = 0;
    temp_count = 0;
    label_count = 0;
}
