#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "tac.h"

/* ---------- Loop label stack (Step 1) ---------- */
#define CPP_TAC_MAX_LOOP_DEPTH 100

static char loop_break_labels[CPP_TAC_MAX_LOOP_DEPTH][CPP_TAC_MAX_LENGTH];
static char loop_continue_labels[CPP_TAC_MAX_LOOP_DEPTH][CPP_TAC_MAX_LENGTH];

static int loop_depth = 0;

static int push_loop(
    CPPTAC *tac,
    const char *break_label,
    const char *continue_label
) {
    if (loop_depth >= CPP_TAC_MAX_LOOP_DEPTH) {
        tac->had_error = 1;
        return 0;
    }

    snprintf(
        loop_break_labels[loop_depth],
        CPP_TAC_MAX_LENGTH,
        "%s",
        break_label
    );

    snprintf(
        loop_continue_labels[loop_depth],
        CPP_TAC_MAX_LENGTH,
        "%s",
        continue_label
    );

    loop_depth++;
    return 1;
}

static void pop_loop(void) {
    if (loop_depth > 0)
        loop_depth--;
}

/* Generate temporary variable names */
static void new_temp(CPPTAC *tac, char *buffer, size_t size) {
    tac->temporary_count++;

    snprintf(
        buffer,
        size,
        "t%d",
        tac->temporary_count
    );
}

/* Generate label names */
static void new_label(CPPTAC *tac, char *buffer, size_t size) {
    tac->label_count++;

    snprintf(
        buffer,
        size,
        "L%d",
        tac->label_count
    );
}

/* Add an instruction */
static void emit(CPPTAC *tac, const char *format, ...) {
    if (tac->instruction_count >= CPP_TAC_MAX_INSTRUCTIONS) {
        tac->had_error = 1;
        return;
    }

    va_list args;
    va_start(args, format);

    vsnprintf(
        tac->instructions[tac->instruction_count],
        CPP_TAC_MAX_LENGTH,
        format,
        args
    );

    va_end(args);

    tac->instruction_count++;
}

/* Forward declarations */
static void generate_statement(CPPTAC *tac, CPPASTNode *node);

static void generate_expression(
    CPPTAC *tac,
    CPPASTNode *node,
    char *result,
    size_t result_size
);

/* ---------- Expression Generation ---------- */

static void generate_expression(
    CPPTAC *tac,
    CPPASTNode *node,
    char *result,
    size_t result_size
) {
    if (!node) {
        snprintf(result, result_size, "0");
        return;
    }

    char left[CPP_TAC_MAX_LENGTH];
    char right[CPP_TAC_MAX_LENGTH];
    char temp[CPP_TAC_MAX_LENGTH];

    switch (node->type) {

        case AST_LITERAL:
        case AST_IDENTIFIER:
            snprintf(result, result_size, "%s", node->value);
            break;

        case AST_UNARY_EXPRESSION:
            generate_expression(
                tac,
                node->left,
                left,
                sizeof(left)
            );

            new_temp(tac, temp, sizeof(temp));

            emit(
                tac,
                "%s = %s%s",
                temp,
                node->value,
                left
            );

            snprintf(result, result_size, "%s", temp);
            break;

        case AST_BINARY_EXPRESSION:
            generate_expression(
                tac,
                node->left,
                left,
                sizeof(left)
            );

            generate_expression(
                tac,
                node->right,
                right,
                sizeof(right)
            );

            new_temp(tac, temp, sizeof(temp));

            emit(
                tac,
                "%s = %s %s %s",
                temp,
                left,
                node->value,
                right
            );

            snprintf(result, result_size, "%s", temp);
            break;

        case AST_ASSIGNMENT:
            generate_expression(
                tac,
                node->right,
                right,
                sizeof(right)
            );

            if (node->left &&
                node->left->type == AST_IDENTIFIER) {

                emit(
                    tac,
                    "%s = %s",
                    node->left->value,
                    right
                );

                snprintf(
                    result,
                    result_size,
                    "%s",
                    node->left->value
                );
            } else {
                tac->had_error = 1;
                snprintf(result, result_size, "0");
            }
            break;

        default:
            tac->had_error = 1;
            snprintf(result, result_size, "0");
            break;
    }
}

/* ---------- Statement Generation ---------- */

static void generate_statement(CPPTAC *tac, CPPASTNode *node) {
    if (!node)
        return;

    char result[CPP_TAC_MAX_LENGTH];
    char condition[CPP_TAC_MAX_LENGTH];

    switch (node->type) {

        case AST_PROGRAM:
        case AST_BLOCK:
            for (CPPASTNode *child = node->left;
                 child;
                 child = child->next) {

                generate_statement(tac, child);
            }
            break;

        case AST_VARIABLE_DECLARATION:
            if (node->left && node->right) {
                generate_expression(
                    tac,
                    node->right,
                    result,
                    sizeof(result)
                );

                emit(
                    tac,
                    "%s = %s",
                    node->left->value,
                    result
                );
            }
            break;

        case AST_EXPRESSION_STATEMENT:
            generate_expression(
                tac,
                node->left,
                result,
                sizeof(result)
            );
            break;

        case AST_ASSIGNMENT:
        case AST_BINARY_EXPRESSION:
        case AST_UNARY_EXPRESSION:
            generate_expression(
                tac,
                node,
                result,
                sizeof(result)
            );
            break;

        case AST_IF: {
            char false_label[CPP_TAC_MAX_LENGTH];
            char end_label[CPP_TAC_MAX_LENGTH];

            generate_expression(
                tac,
                node->left,
                condition,
                sizeof(condition)
            );

            new_label(tac, false_label, sizeof(false_label));

            emit(
                tac,
                "ifFalse %s goto %s",
                condition,
                false_label
            );

            generate_statement(tac, node->right);

            if (node->third) {
                new_label(tac, end_label, sizeof(end_label));

                emit(tac, "goto %s", end_label);
                emit(tac, "%s:", false_label);

                generate_statement(tac, node->third);

                emit(tac, "%s:", end_label);
            } else {
                emit(tac, "%s:", false_label);
            }

            break;
        }

        /* ---------- Step 2: patched AST_WHILE ---------- */
        case AST_WHILE: {
            char start_label[CPP_TAC_MAX_LENGTH];
            char end_label[CPP_TAC_MAX_LENGTH];

            new_label(tac, start_label, sizeof(start_label));
            new_label(tac, end_label, sizeof(end_label));

            emit(tac, "%s:", start_label);

            generate_expression(
                tac,
                node->left,
                condition,
                sizeof(condition)
            );

            emit(
                tac,
                "ifFalse %s goto %s",
                condition,
                end_label
            );

            if (!push_loop(tac, end_label, start_label))
                break;

            generate_statement(tac, node->right);

            pop_loop();

            emit(tac, "goto %s", start_label);
            emit(tac, "%s:", end_label);

            break;
        }

        case AST_RETURN:
            if (node->left) {
                generate_expression(
                    tac,
                    node->left,
                    result,
                    sizeof(result)
                );

                emit(tac, "return %s", result);
            } else {
                emit(tac, "return");
            }
            break;

        /* ---------- Step 3: patched break / continue ---------- */
        case AST_BREAK:
            if (loop_depth > 0) {
                emit(
                    tac,
                    "goto %s",
                    loop_break_labels[loop_depth - 1]
                );
            } else {
                tac->had_error = 1;
            }
            break;

        case AST_CONTINUE:
            if (loop_depth > 0) {
                emit(
                    tac,
                    "goto %s",
                    loop_continue_labels[loop_depth - 1]
                );
            } else {
                tac->had_error = 1;
            }
            break;

        case AST_EMPTY:
            break;

        default:
            tac->had_error = 1;
            break;
    }
}

/* ---------- Public API ---------- */

void cpp_tac_init(CPPTAC *tac) {
    if (!tac)
        return;

    memset(tac, 0, sizeof(*tac));

    loop_depth = 0;   /* ---------- Step 4 ---------- */
}

int cpp_tac_generate(CPPTAC *tac, CPPASTNode *root) {
    if (!tac || !root)
        return 0;

    generate_statement(tac, root);

    return tac->had_error ? 0 : 1;
}

void cpp_tac_print(CPPTAC *tac) {
    if (!tac)
        return;

    printf("\n========== THREE ADDRESS CODE ==========\n");

    for (int i = 0; i < tac->instruction_count; i++) {
        printf("%s\n", tac->instructions[i]);
    }

    printf("\nTemporary variables generated: %d\n",
           tac->temporary_count);
}