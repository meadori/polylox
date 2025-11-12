import re

RUN_LINE_RE = re.compile("^//\s+RUN(-[^:]+)?:(.+)$")
CHECK_LINE_RE = re.compile("^//\s+(CHECK)(-[^:]+)?:(.+)$")


def get_tag(name):
    if not name:
        return "DEFAULT"
    if name.startswith("-"):
        name = name[1:]
    return name


class Collector:
    def __init__(self, filename):
        self.filename = filename
        self.run_lines = []
        self.tagged_expextations = {}

    def __enter__(self):
        try:
            self.file = open(self.filename, "r")
            for line in self.file:
                match = RUN_LINE_RE.match(line)
                if match:
                    tag, run_line = get_tag(match.group(1)), match.group(2)
                    self.run_lines.append((tag, run_line))
                match = CHECK_LINE_RE.match(line)
                if match:
                    cmd, tag, check_line = (
                        match.group(1),
                        get_tag(match.group(2)),
                        match.group(3),
                    )
                    if tag not in self.tagged_expextations:
                        self.tagged_expextations[tag] = []
                    self.tagged_expextations[tag].append((cmd, check_line))

            return self
        except FileNotFoundError:
            raise

    def __exit__(self, exc_type, exc_value, traceback):
        if self.file:
            self.file.close()

        return False

    def runs(self):
        return self.run_lines

    def expextations(self, tag):
        return self.tagged_expextations[tag]
