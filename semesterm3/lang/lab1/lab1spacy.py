# https://spacy.io

import spacy
# from spacy import displacy

nlp = spacy.load("uk_core_news_sm")
doc = nlp("Предметом вивчення синтаксичної науки є закони побудови зв’язного мовлення, яке постає в реченнєвих конструкціях.")

print(spacy.explain("VBZ"))

for token in doc:
    print(token.text, token.lemma_, token.pos_, token.tag_, token.dep_,
            token.shape_, token.is_alpha, token.is_stop, sep='\t')
