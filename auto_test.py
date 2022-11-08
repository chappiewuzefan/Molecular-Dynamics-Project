import os
import math
from subprocess import PIPE, Popen

RELATIVE_TOLERANCE = 1e-6
ZERO_ISH = 1e-50


def get_sample_output(N=3, R=1, dt=0.001, i=10):
    return open(f"auto_test_outputs/{N}_{R}_{dt}_{i}.cor").read()


def run_code(version, N=3, R=1, dt=0.001, i=10):
    if version == "Python":
        return os.popen(f"python3 md.py {N} {R} {dt} {i}").read()
    return os.popen(f"./md_{version} {N} {R} {dt} {i}").read()


def compare_outputs(output, correct, n):
    out_lines = output.split("\n")
    cor_lines = correct.split("\n")

    for i, (out, cor) in enumerate(list(zip(out_lines, cor_lines))[6 : 6 + n]):
        diff = any(
            math.isnan(float(o))
            or (
                abs(float(o)) > ZERO_ISH
                if abs(float(c)) < ZERO_ISH
                else abs((float(o) - float(c)) / float(c)) > RELATIVE_TOLERANCE
            )
            for o, c in zip(out.split(), cor.split())
        )
        if diff:
            return (
                f"First difference on line {i}\n"
                + f"Output:  {out.strip()}\n"
                + f"Correct: {cor.strip()}\n"
                + (
                    f"Next out: {out_lines[7+i].strip()}\nNext cor: {cor_lines[7+i].strip()}"
                    if i < n - 1
                    else ""
                )
            )

    return None


def main():
    print("=== Compiling ===")
    os.popen("make clean").read()
    make_out, make_err = Popen(
        "make", shell=True, stdout=PIPE, stderr=PIPE
    ).communicate()

    if "error" in str(make_err):
        print("Error while running 'make':")
        print("STDOUT:")
        print(make_out.decode())
        print("\nSTDERR:")
        print(make_err.decode())
        exit()

    warnings = (str(make_out) + str(make_err)).count("warning")
    if warnings:
        print(f"Make ran with ~{warnings} warnings")

    tests = [
        {"N": 3, "R": 1, "dt": 0.001, "i": 10},
        {"N": 5, "R": 3, "dt": 0.001, "i": 100},
    ]

    for imp in ["Python", "AoS", "SoA", "Opt"]:
        print(f"\n=== Testing {imp} implementation ===")
        for test in tests:
            correct_output = get_sample_output(**test)
            output = run_code(imp, **test)
            if not output:
                print(f"No {imp} implementation found.")
                break
            with open(
                f"auto_test_outputs/{imp}_"
                + "_".join(str(test[x]) for x in ["N", "R", "dt", "i"])
                + ".out",
                "w",
            ) as out_file:
                out_file.write(output)

            if compare_outputs(output, correct_output, n=test["i"]):
                print(f"Difference on test {test}")
                break
        else:
            print(f"{imp} looks good to me!")


if __name__ == "__main__":
    main()
