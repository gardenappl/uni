import spacy
from pathlib import Path

text = Path("lab3.txt").read_text()

nlp = spacy.load("uk_core_news_sm")
doc = nlp(text)

for ent in doc.ents:
    print(ent)
    print(ent.text, ent.label_)
