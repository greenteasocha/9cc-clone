#include <9cc.h>

Token *new_token(TokenKind kind, Token *cur, char *str, int len)
{
    Token *tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->str = str;
    tok->len = len;
    cur->next = tok;
    return tok;
}

Token *tokenize(char *p)
{
    Token head;
    head.next = NULL;
    Token *cur = &head;

    while (*p)
    {
        if (isspace(*p))
        {
            p++;
            continue;
        }

        if (
            strncmp(p, "==", 2) == 0 || strncmp(p, "!=", 2) == 0 || strncmp(p, ">=", 2) == 0 || strncmp(p, "<=", 2) == 0)
        {
            cur = new_token(TK_RESERVED, cur, p++, 2);
            // 余分にpを進める
            p++;
            continue;
        }

        if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '(' || *p == ')' || *p == '<' || *p == '>')
        {
            cur = new_token(TK_RESERVED, cur, p++, 1);
            continue;
        }

        if (isdigit(*p))
        {
            cur = new_token(TK_NUM, cur, p, 0);
            char *q = p;
            cur->val = strtol(p, &p, 10);
            cur->len = q - p;
            continue;
        }

        error("トークナイズできません。");
    }

    new_token(TK_EOF, cur, p, 0);
    return head.next;
}

Node *new_node(NodeKind kind, Node *lhs, Node *rhs)
{
    Node *node = calloc(1, sizeof(Node));
    node->kind = kind;
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
}

// 個人メモ: numは終端記号なのでlhs, rhsがない
Node *new_node_num(int val)
{
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_NUM;
    node->val = val;
    return node;
}

void show_token(Token *token)
{
    printf("%d, %d, %s\n", token->kind, token->val, token->str);
    if (token->next != NULL)
    {
        show_token(token->next);
    }
}

// 次のトークンが期待している記号のときには、トークンを1つ読み進めて
// 真を返す。それ以外の場合には偽を返す。
bool consume(char *op)
{
    if (
        token->kind != TK_RESERVED || token->len != strlen(op) || memcmp(token->str, op, token->len))
    {
        return false;
    }
    token = token->next;
    return true;
}

void expect(char *op)
{
    if (
        token->kind != TK_RESERVED || token->len != strlen(op) || memcmp(token->str, op, token->len))
    {
        error("'%c'ではありません", op);
    }
    token = token->next;
}

int expect_number()
{
    if (token->kind != TK_NUM)
    {
        error("数ではありません。");
    }
    int val = token->val;
    token = token->next;
    return val;
}

bool at_eof()
{
    return token->kind == TK_EOF;
}

Node *expr();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *primary();

Node *expr()
{
    Node *node = equality();
    return node;
}

Node *equality()
{
    Node *node = relational();

    for (;;)
    {
        if (consume("=="))
        {
            node = new_node(ND_EQL, node, relational());
        }
        else if (consume("!="))
        {
            node = new_node(ND_NEQ, node, relational());
        }
        else
        {
            return node;
        }
    }
}

Node *relational()
{
    Node *node = add();

    for (;;)
    {
        if (consume("<="))
        {
            node = new_node(ND_LEQ, node, relational());
        }
        else if (consume("<"))
        {
            node = new_node(ND_LES, node, relational());
        }
        else if (consume(">="))
        {
            node = new_node(ND_LEQ, relational(), node);
        }
        else if (consume(">"))
        {
            node = new_node(ND_LES, relational(), node);
        }
        else
        {
            return node;
        }
    }
}

Node *add()
{
    Node *node = mul();

    for (;;)
    {
        if (consume("+"))
        {
            node = new_node(ND_ADD, node, mul());
        }
        else if (consume("-"))
        {
            node = new_node(ND_SUB, node, mul());
        }
        else
        {
            return node;
        }
    }
}

Node *mul()
{
    Node *node = unary();

    for (;;)
    {
        if (consume("*"))
        {
            node = new_node(ND_MUL, node, unary());
        }
        else if (consume("/"))
        {
            node = new_node(ND_DIV, node, unary());
        }
        else
        {
            return node;
        }
    }
}

Node *unary()
{
    if (consume("+"))
    {
        return primary();
    }
    else if (consume("-"))
    {
        return new_node(ND_SUB, new_node_num(0), primary());
    }

    return primary();
}

Node *primary()
{
    if (consume("("))
    {
        Node *node = expr();
        expect(")");
        return node;
    }

    return new_node_num(expect_number());
}
