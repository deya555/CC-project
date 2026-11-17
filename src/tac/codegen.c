
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codegen.h"

static int temp_count = 0;
static int label_count = 0;

char *newTemp()
{
    char *name = malloc(20);
    sprintf(name, "t%d", temp_count++);
    return name;
}

char *newLabel()
{
    char *name = malloc(20);
    sprintf(name, "L%d", label_count++);
    return name;
}

TACInstr *emit(TACInstructionType type,
               const char *result,
               const char *arg1,
               const char *arg2)
{
    TACInstr *instr = malloc(sizeof(TACInstr));

    if (instr == NULL)
        return NULL;

    instr->type = type;
    instr->result = result ? strdup(result) : NULL;
    instr->arg1 = arg1 ? strdup(arg1) : NULL;
    instr->arg2 = arg2 ? strdup(arg2) : NULL;
    instr->operator = NULL;
    instr->next = NULL;

    return instr;
}

void appendInstruction(TACInstr **head,
                       TACInstr **tail,
                       TACInstr *instr)
{
    if (instr == NULL)
        return;

    if (*head == NULL)
    {
        *head = instr;
        *tail = instr;
    }
    else
    {
        (*tail)->next = instr;
        *tail = instr;
    }
}

char *translateExpr(ASTNode *node, TACInstr **head, TACInstr **tail);

void translateNode(ASTNode *node, TACInstr **head, TACInstr **tail);

char *translateExpr(ASTNode *node, TACInstr **head, TACInstr **tail)
{
    if (node == NULL)
        return strdup("0");

    char buffer[40];

    switch (node->type)
    {
        case NODE_INT:
            sprintf(buffer, "%d", node->int_value);
            return strdup(buffer);

        case NODE_FLOAT:
            sprintf(buffer, "%g", node->float_value);
            return strdup(buffer);

        case NODE_BOOL:
            return strdup(node->int_value ? "true" : "false");

        case NODE_IDENTIFIER:
            return strdup(node->name);

        case NODE_BINARY:
        case NODE_UNARY:
        {
            char *left = translateExpr(node->left, head, tail);
            char *right = node->right
                        ? translateExpr(node->right, head, tail)
                        : NULL;

            char *temp = newTemp();
            TACInstructionType type = TAC_REL;

            if (strcmp(node->operator, "+") == 0)
                type = TAC_ADD;
            else if (strcmp(node->operator, "-") == 0)
                type = node->type == NODE_UNARY ? TAC_NEG : TAC_SUB;
            else if (strcmp(node->operator, "*") == 0)
                type = TAC_MUL;
            else if (strcmp(node->operator, "/") == 0)
                type = TAC_DIV;
            else if (strcmp(node->operator, "%") == 0)
                type = TAC_MOD;
            else if (strcmp(node->operator, "&&") == 0 ||
                     strcmp(node->operator, "||") == 0)
                type = TAC_LOGIC;
            else if (strcmp(node->operator, "!") == 0)
                type = TAC_NOT;

            TACInstr *instr = emit(type, temp, left, right);

if (instr != NULL)
{
    instr->operator = strdup(node->operator);
    appendInstruction(head, tail, instr);
}

return temp;
        }

        default:
            return strdup("0");
    }
}

void translateNode(ASTNode *node, TACInstr **head, TACInstr **tail)
{
    while (node != NULL)
    {
        switch (node->type)
        {
            case NODE_PROGRAM:
            case NODE_BLOCK:
                translateNode(node->body, head, tail);
                break;

            case NODE_DECLARATION:
                break;

            case NODE_ASSIGNMENT:
            {
                char *value = translateExpr(node->right, head, tail);

                appendInstruction(head, tail,
                    emit(TAC_ASSIGN, node->left->name, value, NULL));
                break;
            }

            case NODE_PRINT:
            {
                char *value = translateExpr(node->left, head, tail);

                appendInstruction(head, tail,
                    emit(TAC_PRINT, value, NULL, NULL));
                break;
            }

            case NODE_IF:
            {
                char *condition =
                    translateExpr(node->condition, head, tail);

                char *elseLabel = newLabel();
                char *endLabel = newLabel();

                appendInstruction(head, tail,
                    emit(TAC_IF_FALSE, elseLabel, condition, NULL));

                translateNode(node->body, head, tail);

                if (node->else_body != NULL)
                {
                    appendInstruction(head, tail,
                        emit(TAC_GOTO, endLabel, NULL, NULL));

                    appendInstruction(head, tail,
                        emit(TAC_LABEL, elseLabel, NULL, NULL));

                    translateNode(node->else_body, head, tail);

                    appendInstruction(head, tail,
                        emit(TAC_LABEL, endLabel, NULL, NULL));
                }
                else
                {
                    appendInstruction(head, tail,
                        emit(TAC_LABEL, elseLabel, NULL, NULL));
                }

                break;
            }

            case NODE_WHILE:
            {
                char *startLabel = newLabel();
                char *endLabel = newLabel();

                appendInstruction(head, tail,
                    emit(TAC_LABEL, startLabel, NULL, NULL));

                char *condition =
                    translateExpr(node->condition, head, tail);

                appendInstruction(head, tail,
                    emit(TAC_IF_FALSE, endLabel, condition, NULL));

                translateNode(node->body, head, tail);

                appendInstruction(head, tail,
                    emit(TAC_GOTO, startLabel, NULL, NULL));

                appendInstruction(head, tail,
                    emit(TAC_LABEL, endLabel, NULL, NULL));

                break;
            }

            default:
                break;
        }

        node = node->next;
    }
}

TACInstr *generateTAC(ASTNode *root)
{
    temp_count = 0;
    label_count = 0;

    TACInstr *head = NULL;
    TACInstr *tail = NULL;

    translateNode(root, &head, &tail);

    return head;
}

void printTAC(TACInstr *head)
{
    printf("\nTHREE-ADDRESS CODE (TAC)\n");
    printf("--------------------------------\n");

    while (head != NULL)
    {
        switch (head->type)
        {
            case TAC_ASSIGN:
                printf("%s = %s\n", head->result, head->arg1);
                break;

            case TAC_ADD:
                printf("%s = %s + %s\n", head->result, head->arg1, head->arg2);
                break;

            case TAC_SUB:
                printf("%s = %s - %s\n", head->result, head->arg1, head->arg2);
                break;

            case TAC_MUL:
                printf("%s = %s * %s\n", head->result, head->arg1, head->arg2);
                break;

            case TAC_DIV:
                printf("%s = %s / %s\n", head->result, head->arg1, head->arg2);
                break;

            case TAC_MOD:
                printf("%s = %s %% %s\n", head->result, head->arg1, head->arg2);
                break;

            case TAC_REL:
case TAC_LOGIC:
    printf("%s = %s %s %s\n",
           head->result,
           head->arg1,
           head->operator,
           head->arg2);
    break;

            case TAC_NEG:
                printf("%s = -%s\n", head->result, head->arg1);
                break;

            case TAC_NOT:
                printf("%s = !%s\n", head->result, head->arg1);
                break;

            case TAC_PRINT:
                printf("print %s\n", head->result);
                break;

            case TAC_LABEL:
                printf("%s:\n", head->result);
                break;

            case TAC_GOTO:
                printf("goto %s\n", head->result);
                break;

            case TAC_IF_FALSE:
                printf("ifFalse %s goto %s\n",
                       head->arg1, head->result);
                break;
        }

        head = head->next;
    }

    printf("--------------------------------\n");
}

void freeTAC(TACInstr *head)
{
    while (head != NULL)
    {
        TACInstr *temp = head;
        head = head->next;

        free(temp->operator);
        free(temp->result);
        free(temp->arg1);
        free(temp->arg2);
        free(temp);
    }
}
