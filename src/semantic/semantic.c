
#include <stdio.h>
#include <string.h>
#include "semantic.h"

/* Return the line number when available */
int getLine(ASTNode *node)
{
    return node->line;
}

/* Display semantic errors */
void semanticError(ASTNode *node, const char *message)
{
    if (getLine(node) > 0)
        printf("Semantic error at line %d: %s\n",
               getLine(node), message);
    else
        printf("Semantic error: %s\n", message);
}

/* Check whether a type is numeric */
int isNumeric(DataType type)
{
    return type == TYPE_INT || type == TYPE_FLOAT;
}

/* Check whether a value can be assigned to a variable */
int compatibleTypes(DataType target, DataType source)
{
    if (target == source)
        return 1;

    /* An integer can be assigned to a float */
    if (target == TYPE_FLOAT && source == TYPE_INT)
        return 1;

    return 0;
}

/* Determine the type of an expression */
int expressionType(ASTNode *node,
                   SymbolTable *table,
                   int *errors)
{
    if (node == NULL)
        return -1;

    switch (node->type)
    {
        case NODE_INT:
            return TYPE_INT;

        case NODE_FLOAT:
            return TYPE_FLOAT;

        case NODE_BOOL:
            return TYPE_BOOL;

        case NODE_IDENTIFIER:
        {
            Symbol *symbol = lookupSymbol(table, node->name);

            if (symbol == NULL)
            {
                char message[256];

                snprintf(message, sizeof(message),
                         "Variable '%s' is not declared.",
                         node->name);

                semanticError(node, message);
                (*errors)++;
                return -1;
            }

            return symbol->type;
        }

        case NODE_UNARY:
        {
            int operandType =
                expressionType(node->left, table, errors);

            if (operandType == -1)
                return -1;

            if (strcmp(node->operator, "!") == 0)
            {
                if (operandType != TYPE_BOOL)
                {
                    semanticError(node,
                        "Logical NOT requires a boolean operand.");
                    (*errors)++;
                    return -1;
                }

                return TYPE_BOOL;
            }

            if (strcmp(node->operator, "-") == 0)
            {
                if (!isNumeric(operandType))
                {
                    semanticError(node,
                        "Unary minus requires a numeric operand.");
                    (*errors)++;
                    return -1;
                }

                return operandType;
            }

            return -1;
        }

        case NODE_BINARY:
        {
            int leftType =
                expressionType(node->left, table, errors);

            int rightType =
                expressionType(node->right, table, errors);

            if (leftType == -1 || rightType == -1)
                return -1;

            char *op = node->operator;

            if (strcmp(op, "+") == 0 ||
                strcmp(op, "-") == 0 ||
                strcmp(op, "*") == 0 ||
                strcmp(op, "/") == 0)
            {
                if (!isNumeric(leftType) ||
                    !isNumeric(rightType))
                {
                    semanticError(node,
                        "Arithmetic operators require numeric operands.");
                    (*errors)++;
                    return -1;
                }

                return (leftType == TYPE_FLOAT ||
                        rightType == TYPE_FLOAT)
                       ? TYPE_FLOAT : TYPE_INT;
            }

            if (strcmp(op, "%") == 0)
            {
                if (leftType != TYPE_INT ||
                    rightType != TYPE_INT)
                {
                    semanticError(node,
                        "Modulo operator requires integer operands.");
                    (*errors)++;
                    return -1;
                }

                return TYPE_INT;
            }

            if (strcmp(op, "<") == 0 ||
                strcmp(op, ">") == 0 ||
                strcmp(op, "<=") == 0 ||
                strcmp(op, ">=") == 0)
            {
                if (!isNumeric(leftType) ||
                    !isNumeric(rightType))
                {
                    semanticError(node,
                        "Relational operators require numeric operands.");
                    (*errors)++;
                    return -1;
                }

                return TYPE_BOOL;
            }

            if (strcmp(op, "==") == 0 ||
                strcmp(op, "!=") == 0)
            {
                if (leftType != rightType &&
                    !(isNumeric(leftType) &&
                      isNumeric(rightType)))
                {
                    semanticError(node,
                        "Equality operators require compatible operands.");
                    (*errors)++;
                    return -1;
                }

                return TYPE_BOOL;
            }

            if (strcmp(op, "&&") == 0 ||
                strcmp(op, "||") == 0)
            {
                if (leftType != TYPE_BOOL ||
                    rightType != TYPE_BOOL)
                {
                    semanticError(node,
                        "Logical operators require boolean operands.");
                    (*errors)++;
                    return -1;
                }

                return TYPE_BOOL;
            }

            return -1;
        }

        default:
            return -1;
    }
}

/* Analyze one statement */
void analyzeStatement(ASTNode *node,
                      SymbolTable *table,
                      int *errors)
{
    if (node == NULL)
        return;

    switch (node->type)
    {
        case NODE_DECLARATION:
        {
            DataType type;

            if (strcmp(node->declared_type, "int") == 0)
                type = TYPE_INT;
            else if (strcmp(node->declared_type, "float") == 0)
                type = TYPE_FLOAT;
            else
                type = TYPE_BOOL;

            if (!insertSymbol(table, node->name,
                              type, getLine(node)))
            {
                char message[256];

                snprintf(message, sizeof(message),
                         "Variable '%s' is already declared in this scope.",
                         node->name);

                semanticError(node, message);
                (*errors)++;
            }

            break;
        }

        case NODE_ASSIGNMENT:
        {
            ASTNode *identifier = node->left;

            Symbol *symbol =
                lookupSymbol(table, identifier->name);

            int valueType =
                expressionType(node->right, table, errors);

            if (symbol == NULL)
            {
                char message[256];

                snprintf(message, sizeof(message),
                         "Variable '%s' is not declared.",
                         identifier->name);

                semanticError(node, message);
                (*errors)++;
            }
            else if (valueType != -1 &&
                     !compatibleTypes(symbol->type, valueType))
            {
                semanticError(node,
                    "Assignment type mismatch.");
                (*errors)++;
            }

            break;
        }

        case NODE_PRINT:
            expressionType(node->left, table, errors);
            break;

        case NODE_IF:
        case NODE_WHILE:
        {
            int conditionType =
                expressionType(node->condition, table, errors);

            if (conditionType != -1 &&
                conditionType != TYPE_BOOL)
            {
                semanticError(node,
                    "Condition must have a boolean value.");
                (*errors)++;
            }

            analyzeStatement(node->body, table, errors);
            analyzeStatement(node->else_body, table, errors);
            break;
        }

        case NODE_BLOCK:
            enterScope(table);

            {
                ASTNode *current = node->body;

                while (current != NULL)
                {
                    analyzeStatement(current, table, errors);
                    current = current->next;
                }
            }

            exitScope(table);
            break;

        default:
            break;
    }
}

/* Main semantic analysis function */
int analyzeSemantics(ASTNode *root, SymbolTable *table)
{
    int errors = 0;

    printf("\nSEMANTIC ANALYSIS\n");
    printf("----------------------------------------\n");

    if (root == NULL)
        return 0;

    ASTNode *current = root->body;

    while (current != NULL)
    {
        analyzeStatement(current, table, &errors);
        current = current->next;
    }

    if (errors == 0)
        printf("No semantic errors found!\n");
    else
        printf("Total semantic errors: %d\n", errors);

    return errors;
}
