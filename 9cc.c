#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <9cc.h>

// // トークンの種類
// typedef enum
// {
//     TK_RESERVED, // 記号
//     TK_NUM,      // 整数トークン
//     TK_EOF,      // 入力の終わりを表すトークン
// } TokenKind;

// typedef struct Token Token;

// // トークン型
// struct Token
// {
//     TokenKind kind; // トークンの型
//     Token *next;    // 次の入力トークン
//     int val;        // kindがTK_NUMの場合、その数値
//     char *str;      // トークン文字列
//     int len;        // トークンの長さ
// };

// 現在着目しているトークン
Token *token;

// // エラーを報告するための関数
// // printfと同じ引数を取る
// void error(char *fmt, ...)
// {

//     va_list ap;
//     va_start(ap, fmt);
//     vfprintf(stderr, fmt, ap);
//     fprintf(stderr, "\n");
//     exit(1);
// }

// // 抽象構文木のノードの種類
// typedef enum
// {
//     ND_ADD, // +
//     ND_SUB, // -
//     ND_MUL, // *
//     ND_DIV, // /
//     ND_EQL, // ==
//     ND_NEQ, // !=
//     ND_LES, // <
//     ND_LEQ, // <=
//     // ND_GRT, >,  両辺を入れ替えたLESで代用
//     // ND_GEQ, >=, 両辺を入れ替えたLEQで代用
//     ND_NUM, // 整数
// } NodeKind;

// typedef struct Node Node;

// // 抽象構文木のノードの型
// // 個人メモ: 二分木なので子ノードは配列ではなくlhs, rhsを取る
// struct Node
// {
//     NodeKind kind; // ノードの型
//     Node *lhs;     // 左辺
//     Node *rhs;     // 右辺
//     int val;       // kindがND_NUMの場合のみ使う
// };

// Node *new_node(NodeKind kind, Node *lhs, Node *rhs)
// {
//     Node *node = calloc(1, sizeof(Node));
//     node->kind = kind;
//     node->lhs = lhs;
//     node->rhs = rhs;
//     return node;
// }

// // 個人メモ: numは終端記号なのでlhs, rhsがない
// Node *new_node_num(int val)
// {
//     Node *node = calloc(1, sizeof(Node));
//     node->kind = ND_NUM;
//     node->val = val;
//     return node;
// }

// void show_token(Token *token)
// {
//     printf("%d, %d, %s\n", token->kind, token->val, token->str);
//     if (token->next != NULL)
//     {
//         show_token(token->next);
//     }
// }

// // 次のトークンが期待している記号のときには、トークンを1つ読み進めて
// // 真を返す。それ以外の場合には偽を返す。
// bool consume(char *op)
// {
//     if (
//         token->kind != TK_RESERVED || token->len != strlen(op) || memcmp(token->str, op, token->len))
//     {
//         return false;
//     }
//     token = token->next;
//     return true;
// }

// void expect(char *op)
// {
//     if (
//         token->kind != TK_RESERVED || token->len != strlen(op) || memcmp(token->str, op, token->len))
//     {
//         error("'%c'ではありません", op);
//     }
//     token = token->next;
// }

// int expect_number()
// {
//     if (token->kind != TK_NUM)
//     {
//         error("数ではありません。");
//     }
//     int val = token->val;
//     token = token->next;
//     return val;
// }

// bool at_eof()
// {
//     return token->kind == TK_EOF;
// }

// Node *expr();
// Node *equality();
// Node *relational();
// Node *add();
// Node *mul();
// Node *unary();
// Node *primary();

// Node *expr()
// {
//     Node *node = equality();
//     return node;
// }

// Node *equality()
// {
//     Node *node = relational();

//     for (;;)
//     {
//         if (consume("=="))
//         {
//             node = new_node(ND_EQL, node, relational());
//         }
//         else if (consume("!="))
//         {
//             node = new_node(ND_NEQ, node, relational());
//         }
//         else
//         {
//             return node;
//         }
//     }
// }

// Node *relational()
// {
//     Node *node = add();

//     for (;;)
//     {
//         if (consume("<="))
//         {
//             node = new_node(ND_LEQ, node, relational());
//         }
//         else if (consume("<"))
//         {
//             node = new_node(ND_LES, node, relational());
//         }
//         else if (consume(">="))
//         {
//             node = new_node(ND_LEQ, relational(), node);
//         }
//         else if (consume(">"))
//         {
//             node = new_node(ND_LES, relational(), node);
//         }
//         else
//         {
//             return node;
//         }
//     }
// }

// Node *add()
// {
//     Node *node = mul();

//     for (;;)
//     {
//         if (consume("+"))
//         {
//             node = new_node(ND_ADD, node, mul());
//         }
//         else if (consume("-"))
//         {
//             node = new_node(ND_SUB, node, mul());
//         }
//         else
//         {
//             return node;
//         }
//     }
// }

// Node *mul()
// {
//     Node *node = unary();

//     for (;;)
//     {
//         if (consume("*"))
//         {
//             node = new_node(ND_MUL, node, unary());
//         }
//         else if (consume("/"))
//         {
//             node = new_node(ND_DIV, node, unary());
//         }
//         else
//         {
//             return node;
//         }
//     }
// }

// Node *unary()
// {
//     if (consume("+"))
//     {
//         return primary();
//     }
//     else if (consume("-"))
//     {
//         return new_node(ND_SUB, new_node_num(0), primary());
//     }

//     return primary();
// }

// Node *primary()
// {
//     if (consume("("))
//     {
//         Node *node = expr();
//         expect(")");
//         return node;
//     }

//     return new_node_num(expect_number());
// }

// Token *new_token(TokenKind kind, Token *cur, char *str, int len)
// {
//     Token *tok = calloc(1, sizeof(Token));
//     tok->kind = kind;
//     tok->str = str;
//     tok->len = len;
//     cur->next = tok;
//     return tok;
// }

// void gen(Node *node)
// {
//     if (node->kind == ND_NUM)
//     {
//         printf("  push %d\n", node->val);
//         return;
//     }

//     gen(node->lhs);
//     gen(node->rhs);

//     printf("  pop rdi\n");
//     printf("  pop rax\n");

//     switch (node->kind)
//     {
//     case ND_ADD:
//         printf("  add rax, rdi\n");
//         break;
//     case ND_SUB:
//         printf("  sub rax, rdi\n");
//         break;
//     case ND_MUL:
//         printf("  imul rax, rdi\n");
//         break;
//     case ND_DIV:
//         printf("  cqo\n");
//         printf("  idiv rdi\n");
//         break;
//     case ND_EQL:
//         printf("  cmp rdi, rax\n");
//         printf("  sete al\n");
//         printf("  movzb rax, al\n");
//         break;
//     case ND_NEQ:
//         printf("  cmp rdi, rax\n");
//         printf("  setne al\n");
//         printf("  movzb rax, al\n");
//         break;
//     case ND_LES:
//         printf("  cmp rdi, rax\n");
//         printf("  setl al\n");
//         printf("  movzb rax, al\n");
//         break;
//     case ND_LEQ:
//         printf("  cmp rdi, rax\n");
//         printf("  setle al\n");
//         printf("  movzb rax, al\n");
//         break;
//     }

//     printf("  push rax\n");
// }

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        error("引数の個数が正しくありません");
        return 1;
    }

    char *user_input = argv[1];
    token = tokenize(user_input);

    // show_token(token);
    Node *node = expr();

    // アセンブリの前半部分を出力
    printf(".intel_syntax noprefix\n");
    printf(".globl main\n");
    printf("main:\n");

    // 抽象構文木を下りながらコード生成
    gen(node);

    // スタックトップに式全体の値が残っているはずなので
    // それをRAXにロードして関数からの返り値とする
    printf("  pop rax\n");
    printf("  ret\n");
    return 0;
    // Token *next = token->next;
    // char *val = (&next).str;
    // printf("%s", token->val);
    // printf("%s", token->next->val);
    // アセンブリの前半部分を出力
    // printf(".intel_syntax noprefix\n");
    // printf(".globl main\n");
    // printf("main:\n");

    // // 式の最初は数でなければならないので、それをチェックして
    // // 最初のmov命令を出力
    // printf("  mov rax, %d\n", expect_number());

    // while(!at_eof()) {
    //     // printf("%s, %s\n", token->kind,"a");
    //     // token=token->next;
    //     // continue;
    //     if (consume('+')) {
    //         printf("  add rax, %d\n", expect_number());
    //         continue;
    //     }

    //     expect('-');
    //     printf("  sub rax, %d\n", expect_number());
    // }

    // printf("  ret\n");
    // return 0;
}
