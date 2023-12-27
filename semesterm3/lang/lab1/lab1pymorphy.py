import pymorphy3
import pprint

pp = pprint.PrettyPrinter()

morph = pymorphy3.MorphAnalyzer(lang="uk")
parse = morph.parse("Мати")
for p in parse:
    if "VERB" in p.tag:
        lex = p.lexeme
        for l in lex:
            print(l.word, l.tag)

lex = morph.parse("квапотливкий")[0].lexeme
for l in lex:
    print(l.word, l.tag)
