class Matcher:
    def __init__(self, text):
        self.text = text
        self.line_number = 0

    def match(self, tag, cmd, expextation):
        if cmd == "CHECK":
            while self.line_number < len(self.text):
                line = self.text[self.line_number]
                self.line_number += 1
                if line.strip() == expextation.strip():
                    return True
        return False
