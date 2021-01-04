#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

std::vector<char> buf;
std::vector<char> result;
int               layer = 0; // the indent layer
int               pos   = 0;
FILE*             src_fp;
FILE*             dest_fp;

void append(char ch)
{
    result.push_back(ch);
    ++pos;
}

void indent()
{
    for (int i = 0; i < (layer << 2); ++i) {
        append(' ');
    }
}

void handle_buf()
{
    int cur = 0;

    while (cur < buf.size()) {
        while (isspace(buf[cur]) && cur < buf.size()) ++cur; // skip spaces at the beginning
        if (cur == buf.size()) break;

        switch (buf[cur]) {
        case '{':
        case '[':
            indent();
            append(buf[cur++]);
            append('\n');
            ++layer;
            break;
        case '}':
        case ']':
            if (result[pos - 1] != '\n') append('\n');
            --layer;
            indent();
            append(buf[cur++]);
            break;
        case '"':
            if (result[pos - 2] != ':') {
                indent();
            }

            append(buf[cur++]);

            while (cur < buf.size() && buf[cur] != '"') {
                append(buf[cur++]);
            }

            if (buf[cur] != '"') {
                perror("double quotes are not paired.\n");
                exit(0);
            }

            append(buf[cur++]);

            break;
        case ',':
            append(buf[cur++]);
            append('\n');
            break;
        case ':':
            append(buf[cur++]);
            append(' ');
            break;
        default:
            if (isdigit(buf[cur])) {
                if (result[pos - 2] != ':') {
                    indent();
                }

                while (cur < buf.size() && isdigit(buf[cur])) {
                    append(buf[cur++]);
                }

                if (!isspace(buf[cur]) && buf[cur] != ',' && buf[cur] != ']' && buf[cur] != '}') {
                    fprintf(stderr, "syntax error around '%c'\n", buf[cur]);
                    exit(0);
                }
            }
        }
    }
    append('\n');
}

int main()
{
    // src_fp  = fopen("test.txt", "r");
    // dest_fp = fopen("out.txt", "w");
    src_fp  = stdin;  // read from stdin
    dest_fp = stdout; // output to stdout

    char line_buf[1024];
    char c;

    while ((c = fgetc(src_fp)) != EOF) {
        buf.push_back(c);
    }

    handle_buf();
    fputs(result.data(), dest_fp);

    fclose(src_fp);
    fclose(dest_fp);

    return 0;
}
