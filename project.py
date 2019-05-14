INTEGER, PLUS, MINUS, MUL, DIV, LPAREN, RPAREN, EOF = (
    'INTEGER', 'PLUS', 'MINUS', 'MUL', 'DIV', '(', ')', 'EOF'
)

class Token(object):
    def __init__(self, type, value):
        self.type = type
        self.value = value

    def __str__(self):
        return 'Token({type}, {value})'.format(
            type=self.type,
            value=repr(self.value)
        )

    def __repr__(self):
        return self.__str__()


class Lexer(object):
    def __init__(self, text):
        self.text = text
        self.pos = 0
        self.current_char = self.text[self.pos]

    def error(self):
        raise Exception('Invalid character')

    def moveon(self):
        self.pos += 1
        if self.pos > len(self.text) - 1:
            self.current_char = None
        else:
            self.current_char = self.text[self.pos]

    def skip(self):
        while self.current_char is not None and self.current_char.isspace():
            self.moveon()

    def integer(self):
        result = ''
        while self.current_char is not None and self.current_char.isdigit():
            result += self.current_char
            self.moveon()
        return int(result)

    def next_token(self):
        while self.current_char is not None:

            if self.current_char.isspace():
                self.skip()
                continue

            if self.current_char.isdigit():
                return Token(INTEGER, self.integer())

            if self.current_char == '+':
                self.moveon()
                return Token(PLUS, '+')

            if self.current_char == '-':
                self.moveon()
                return Token(MINUS, '-')

            if self.current_char == '*':
                self.moveon()
                return Token(MUL, '*')

            if self.current_char == '/':
                self.moveon()
                return Token(DIV, '/')

            if self.current_char == '(':
                self.moveon()
                return Token(LPAREN, '(')

            if self.current_char == ')':
                self.moveon()
                return Token(RPAREN, ')')

            self.error()

        return Token(EOF, None)

class AST(object):
        pass

class BinOp(AST):
        def __init__(self, left, op, right):
            self.left = left
            self.token = self.op = op
            self.right = right

class Num(AST):
        def __init__(self, token):
            self.token = token
            self.value = token.value

class UnaryOp(AST):
        def __init__(self, op, expr):
            self.token = self.op = op
            self.expr = expr

class Parser(object):
        def __init__(self, lexer):
            self.lexer = lexer

            self.current_token = self.lexer.next_token()

        def error(self):
            raise Exception('Invalid syntax')

        def eat(self, token_type):
            if self.current_token.type == token_type:
                self.current_token = self.lexer.next_token()
            else:
                self.error()

        def factor(self):
            token = self.current_token
            if token.type == PLUS:
                self.eat(PLUS)
                node = UnaryOp(token, self.factor())
                return node
            elif token.type == MINUS:
                self.eat(MINUS)
                node = UnaryOp(token, self.factor())
                return node
            elif token.type == INTEGER:
                self.eat(INTEGER)
                return Num(token)
            elif token.type == LPAREN:
                self.eat(LPAREN)
                node = self.expr()
                self.eat(RPAREN)
                return node

        def term(self):
            node = self.factor()

            while self.current_token.type in (MUL, DIV):
                token = self.current_token
                if token.type == MUL:
                    self.eat(MUL)
                elif token.type == DIV:
                    self.eat(DIV)

                node = BinOp(left=node, op=token, right=self.factor())

            return node

        def expr(self):
            node = self.term()

            while self.current_token.type in (PLUS, MINUS):
                token = self.current_token
                if token.type == PLUS:
                    self.eat(PLUS)
                elif token.type == MINUS:
                    self.eat(MINUS)

                node = BinOp(left=node, op=token, right=self.term())

            return node

        def parse(self):
            node = self.expr()
            if self.current_token.type != EOF:
                self.error()
            return node

class NodeVisitor(object):
    def visit(self, node):
        method_name = 'visit_' + type(node).__name__
        visitor = getattr(self, method_name, self.generic_visit)
        return visitor(node)

    def generic_visit(self, node):
        raise Exception('No visit_{} method'.format(type(node).__name__))


class Interpreter(NodeVisitor):
    def __init__(self, parser):
        self.parser = parser

    def visit_BinOp(self, node):
        if node.op.type == PLUS:
            return self.visit(node.left) + self.visit(node.right)
        elif node.op.type == MINUS:
            return self.visit(node.left) - self.visit(node.right)
        elif node.op.type == MUL:
            return self.visit(node.left) * self.visit(node.right)
        elif node.op.type == DIV:
            return self.visit(node.left) // self.visit(node.right)

    def visit_Num(self, node):
        return node.value

    def visit_UnaryOp(self, node):
        op = node.op.type
        if op == PLUS:
            return +self.visit(node.expr)
        elif op == MINUS:
            return -self.visit(node.expr)

    def interpret(self):
        tree = self.parser.parse()
        if tree is None:
            return ''
        return self.visit(tree)


def main():
        while True:
            try:
                text="200+200*(--2++4)*-3--5/(1--5-8+4)"

                break
            except EOFError:
                print( "error.")
            if not text:
                continue

        lexer = Lexer(text)
        parser = Parser(lexer)
        interpreter = Interpreter(parser)
        result = interpreter.interpret()
        print(result)


if __name__ == '__main__':
    main()