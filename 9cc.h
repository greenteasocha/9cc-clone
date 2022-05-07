#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// トークンの種類
typedef enum
{
    TK_RESERVED, // 記号
    TK_NUM,      // 整数トークン
    TK_EOF,      // 入力の終わりを表すトークン
} TokenKind;

typedef struct Token Token;

// トークン型
struct Token
{
    TokenKind kind; // トークンの型
    Token *next;    // 次の入力トークン
    int val;        // kindがTK_NUMの場合、その数値
    char *str;      // トークン文字列
    int len;        // トークンの長さ
};

extern Token *token;

// エラーを報告するための関数
// printfと同じ引数を取る
void error(char *fmt, ...);

// 抽象構文木のノードの種類
typedef enum
{
    ND_ADD, // +
    ND_SUB, // -
    ND_MUL, // *
    ND_DIV, // /
    ND_EQL, // ==
    ND_NEQ, // !=
    ND_LES, // <
    ND_LEQ, // <=
    // ND_GRT, >,  両辺を入れ替えたLESで代用
    // ND_GEQ, >=, 両辺を入れ替えたLEQで代用
    ND_NUM, // 整数
} NodeKind;

typedef struct Node Node;

// 抽象構文木のノードの型
// 個人メモ: 二分木なので子ノードは配列ではなくlhs, rhsを取る
struct Node
{
    NodeKind kind; // ノードの型
    Node *lhs;     // 左辺
    Node *rhs;     // 右辺
    int val;       // kindがND_NUMの場合のみ使う
};

Token *tokenize(char *p);
Node *expr();
void gen(Node *node);
