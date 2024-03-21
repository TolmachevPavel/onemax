#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
  // open output files
  FILE* file[argc];
  file[0] = stdout;
  for (int i = 1; i < argc; ++i)
    if (!(file[i] = fopen(argv[i], "w"))) {
      fprintf(stderr, "failed to open '%s': %s", argv[i], strerror(errno));
      exit(EXIT_FAILURE);
    }

  // copy input lines to output files
  char line[BUFSIZ];
  while(fgets(line, sizeof line, stdin))
    for (int i = 0; i < argc; ++i)
      if (fputs(line, file[i]) == EOF) {
        fprintf(stderr, "failed to write to '%s': %s", (i ? argv[i] : "stdout"),
                strerror(errno));
        exit(EXIT_FAILURE);
      }

  return !feof(stdin); // success on eof
}
