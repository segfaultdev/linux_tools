#include <sys/wait.h>
#include <strings.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

int shell_run(char *const argv[]) {
  int pid = fork();
  
  if (pid) {
    int wstatus;
    waitpid(pid, &wstatus, 0);
    
    return WEXITSTATUS(wstatus);
  }
  
  execvp(argv[0], argv);
  exit(-1);
}

int main(int argc, char *const argv[argc]) {
  if (argc == 1) {
    printf("%s <FILE_1> [FILE_2] [FILE_3] [...]\n", argv[0]);
    exit(-1);
  }
  
  char *version_argv[3] = {
    "id3tool",
    "-v",
    NULL,
  };
  
  if (shell_run(version_argv)) {
    printf("Error: id3tool not installed.\n");
    exit(-1);
  }
  
  char *format_argv[7] = {
    "id3tool",
    "-a", NULL,
    "-t", NULL,
    NULL,
    NULL,
  };
  
  int done_count = 0, skipped_count = 0, failed_count = 0;
  char *path = NULL;
  
  for (int i = 1; i < argc; i++) {
    if (path) {
      free(path);
    }
    
    path = strdup(argv[i]);
    
    char
      *artist, *title,
      *last_slash = strrchr(path, '/'),
      *last_dot = strrchr(path, '.')
    ;
    
    if (last_slash != NULL) {
      artist = last_slash + 1;
    } else {
      artist = path;
    }
    
    if (last_dot == NULL) {
      printf("SKIP '%s'\n", argv[i]);
      
      skipped_count++;
      continue;
    }
    
    if (strcasecmp(last_dot, ".mp3")) {
      printf("SKIP '%s'\n", argv[i]);
      
      skipped_count++;
      continue;
    }
    
    *last_dot = '\0';
    title = strstr(artist, " - ");
    
    if (title == NULL) {
      printf("SKIP '%s'\n", argv[i]);
      
      skipped_count++;
      continue;
    }
    
    *title = '\0', title += 3;
    
    format_argv[2] = artist;
    format_argv[4] = title;
    format_argv[5] = argv[i];
    
    printf("- '%s': '%s'\n", artist, title);
    
    if (shell_run(format_argv)) {
      printf("FAIL '%s'\n", argv[i]);
      
      failed_count++;
      continue;
    }
    
    printf("DONE '%s'\n", argv[i]);
    done_count++;
  }
  
  if (path) {
    free(path);
  }
  
  return failed_count;
}
