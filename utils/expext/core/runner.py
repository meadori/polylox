import subprocess


def run_shell_command(command):
    try:
        result = subprocess.run(
            command, shell=True, capture_output=True, text=True, check=False
        )

        return (result.returncode, result.stdout.strip(), result.stderr.strip())

    except FileNotFoundError:
        return (127, "", f"Error: Command '{command}' not found.")
    except Exception as e:
        return (1, "", f"An unexpected error occurred: {e}")


class Runner:
    def __init__(self, run_line):
        self.run_line = run_line

    def run(self):
        code, stdout, stderr = run_shell_command(self.run_line)
        if not code:
            return stdout.split('\n')
