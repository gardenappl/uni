from pathlib import Path
import stanza

text = Path("lab3.txt").read_text()

stanza.download("uk")
nlp = stanza.Pipeline("uk")
doc = nlp(text)

for entity in doc.entities:
    print(entity.text, entity.type)
