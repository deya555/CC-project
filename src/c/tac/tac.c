
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tac.h"

#define MAX_TAC_INSTRUCTIONS 500
#define MAX_TAC_LENGTH 200

static char instructions[MAX_TAC_INSTRUCTIONS][MAX_TAC_LENGTH];
static int instruction_count = 0;
static int temp_count = 0;

static char *new_temp(void)
{
    static char temp[20];

    temp_count++;
    sprintf(temp, "t%d", temp_count);

    return temp;
}

static void emit(const char *instruction)
{
    if (instruction_count >= MAX_TAC_INSTRUCTIONS)
    {
        printf("TAC Error: Instruction limit exceeded.\n");
        return;
    }

    strcpy(instructions[instruction_count], instruction);
    instruction_count++;
}

static char *generate_expression(ASTNode *node)
{
    if (node == NULL)
        return "";

    if (node->type == AST_INTEGER ||
        node->type == AST_FLOAT ||
        node->type == AST_IDENTIFIER)
    {
        return node->value;
    }

    if (node->type == AST_BINARY_EXPR)
    {
        ASTNode *left = node->child;
        ASTNode *right = left ? left->next : NULL;

        char *left_value = generate_expression(left);
        char left_copy[100];

        strcpy(left_copy, left_value);

        char *right_value = generate_expression(right);
        char right_copy[100];

        strcpy(right_copy, right_value);

        char *temp = new_temp();

        char instruction[MAX_TAC_LENGTH];

        sprintf(instruction, "%s = %s %s %s",
                temp,
                left_copy,
                node->value,
                right_copy);

        emit(instruction);

        return temp;
    }

    return node->value ? node->value : "";
}

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
                    char *value = generate_expression(variable->child);

                    char instruction[MAX_TAC_LENGTH];

                    sprintf(instruction, "%s = %s",
                            variable->value, value);

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
                char *value = generate_expression(right);

                char instruction[MAX_TAC_LENGTH];

                sprintf(instruction, "%s = %s",
                        left->value, value);

                emit(instruction);
            }
            break;
        }

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

void tac_generate(ASTNode *root)
{
    instruction_count = 0;
    temp_count = 0;

    generate_statement(root);
}

void tac_print(void)
{
    int i;

    printf("\n========== THREE ADDRESS CODE ==========\n");

    for (i = 0; i < instruction_count; i++)
    {
        printf("%s\n", instructions[i]);
    }

    printf("========================================\n");
}

void tac_free(void)
{
    instruction_count = 0;
    temp_count = 0;
}
