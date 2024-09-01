#include <stdio.h>
#include <stdlib.h>

typedef struct LINE LINE;
typedef struct LINES LINES;

typedef struct STATES STATES;

size_t get_length(const char *input);

int get_state(LINE *items, size_t index, size_t count); 
void print_line(size_t *states, LINE item, int state);
void printer(LINES *input);

LINE parse_line(const char *input);
LINES *parser(FILE *input);

int main(int argc, char **argv);

size_t get_length(const char *input) {
    size_t i = 0;
    for (; input[i] != '\0'; i++);
    return i;
}

#define BRANCH_MID "├─"
#define BRANCH_END "└─"
#define BRANCH_LINE "│   "
#define TAB "    "

#define CHUNK 10
#define CAPACITY 1024

struct LINE {
    size_t depth;
    char literal[CAPACITY];
    size_t length;
};
struct LINES {
    LINE *items;
    size_t count;
    size_t capacity;
};

struct STATES {
    size_t *items;
    size_t capacity;
};

int get_state(LINE *items, size_t index, size_t count) {
    for (size_t j = index + 1; j < count && items[index].depth <= items[j].depth; j++) {
        if (items[j].depth == items[index].depth) {
            return 0;
        }
    }
    return 1;
}
void print_line(size_t *states, LINE item, int state) {
    for (size_t j = 0; j < item.depth; j++) {
        printf("%s", states[j] ? BRANCH_LINE : TAB);
    }
    printf("%s %s\n", state ? BRANCH_END : BRANCH_MID, item.literal);
}
void printer(LINES *input) {
    STATES *states = calloc(1, sizeof(STATES));
    for (size_t i = 0; i < input->count; i++) {
        if (input->items[i].depth >= states->capacity) {
            states->capacity += CHUNK;
            states->items = realloc(states->items, states->capacity * sizeof(size_t));
            if (states->items == NULL) {
                fprintf(stderr, "Error: Memory allocation failed\n");
                exit(EXIT_FAILURE);
            }
        }
        if ((i == 0 && input->items[i].depth > 0) || (i > 0 && input->items[i].depth > input->items[i - 1].depth + 1) || (!input->items[i].length)) {
            printf("Error: Construction failed at the line %zu\n", i);
            exit(EXIT_FAILURE);
        }
        int state = get_state(input->items, i, input->count);
        print_line(states->items, input->items[i], state);
        states->items[input->items[i].depth] = !state;
    }
    free(states->items);
}

LINE parse_line(const char *input) {
    LINE line = {0};
    line.depth = 0;
    size_t i = 0;
    for (; input[i] == ' '; i++, line.depth++);
    size_t j = 0;
    for (; input[i] != '\n' && input[i] != '\0' && input[i] != '\r'; i++, j++) {
        if (j >= CAPACITY - 1) {
            fprintf(stderr, "Error: Line overflow");
            exit(EXIT_FAILURE);
        }
        line.literal[j] = input[i];
    }
    line.literal[j] = '\0';
    line.length = j;
    return line;
}
LINES *parser(FILE *input) {
    LINES *lines = calloc(1, sizeof(LINES));
    if (!lines) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    char line[CAPACITY];
    while (fgets(line, CAPACITY, input)) {
        if (lines->count >= lines->capacity) {
            lines->capacity += CHUNK;
            lines->items = realloc(lines->items, lines->capacity * sizeof(LINE));
            if (!lines->items) {
                fprintf(stderr, "Error: Memory allocation failed\n");
                exit(EXIT_FAILURE);
            }
        }
        lines->items[lines->count] = parse_line(line);
        lines->count++;
    }
    return lines;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input.txt>\n", argv[0]);
        return EXIT_FAILURE;
    }
    FILE *file = fopen(argv[1], "rb");
    if (!file) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }
    LINES *lines = parser(file);
    fclose(file);
    printer(lines);
    free(lines->items);
    free(lines);

    return EXIT_SUCCESS;
}