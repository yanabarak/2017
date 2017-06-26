import nltk

class Analyzer():
    """Implements sentiment analysis."""

    positiveWords = []
    negativeWords = []
    def __init__(self, positives="positive-words.txt", negatives="negative-words.txt"):
    
        try:
            positiveW = open(positives, "r")
            for line in positiveW:
                Analyzer.positiveWords.append(line.replace('\n',''))
            negativeW = open(negatives, "r")
            for line in negativeW:
                Analyzer.negativeWords.append(line.replace('\n',''))
        finally:
            positiveW.close()
            negativeW.close()
        

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""
        score = 0
        wordCheck = text.split(' ')
        for word in wordCheck:
            for line in self.positiveWords:
                    if word.lower() == line:
                        score += 1
                        break
            for line in self.negativeWords:
                if word.lower() == line:
                    score -= 1
        return score
