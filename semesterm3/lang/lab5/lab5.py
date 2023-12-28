import stanza
from pathlib import Path
import sys

if len(sys.argv) < 2:
    print("Usage: ./lab5.py FILENAME [--split-sentences]")
    exit(1)

text = Path(sys.argv[1]).read_text()
split_sentences = len(sys.argv) > 2 and sys.argv[2] == "--split-sentences"

nlp = stanza.Pipeline(lang='en', processors='tokenize,sentiment', tokenize_no_ssplit=(not split_sentences))
doc = nlp(text)
if split_sentences:
    for i, sentence in enumerate(doc.sentences):
        print("Sentence #%d -> %d" % (i, sentence.sentiment))
else:
    print("Text -> %d" % (doc.sentences[0].sentiment))
