import sys, os


def main():
    if len(sys.argv) < 2:
        print("expecting filename as first argument")
        exit(1)

    filename = sys.argv[1]
    with open(filename, "r") as f:
        file_contents = f.read()

    line_dict = dict()
    cullstop = "hit left or right"
    file_contents = file_contents.split("\n")
    line_no = 0
    lines_num = len(file_contents)
    new_file_strings = []
    while line_no < lines_num:
        line = file_contents[line_no]
        line_dict[line] = line_no
        line_no += 1

        if line.startswith(cullstop) or line_no >= lines_num:
            for line, _ in sorted(line_dict.items(), key=lambda item: item[1]):
                new_file_strings.append(line)
            line_dict = dict()

    basename, ext = os.path.splitext(filename)
    new_filename = basename + "_culled" + ext
    with open(new_filename, "w") as f:
        f.write("\n".join(new_file_strings))

if __name__ == "__main__":
    main()
