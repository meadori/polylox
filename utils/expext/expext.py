#!/usr/bin/python

import sys
from utils.expext.core.collector import Collector
from utils.expext.core.runner import Runner
from utils.expext.core.matcher import Matcher

def main():
    filename = sys.argv[1]
    with Collector(filename) as collector:
        for tag, run_line in collector.runs():
            runner = Runner(run_line)
            output = runner.run()
            matcher = Matcher(output)
            try:
                for cmd, expextation in collector.expextations(tag):
                    if not matcher.match(tag, cmd, expextation):
                        print(f"Failed to match: '{tag}:{expextation}'.")
                        sys.exit(1)
            except KeyError as ke:
                print(f"No checks are in place for {ke}.")
                sys.exit(1)

if __name__ == "__main__":
    main()
