#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#define defaultMain \
"#include \"clcincludes.c\" \n\ 
#include \"clcdefinitions.c\" \n\
int main(int argvc, char* argv[]) {}"

int main(int argvc, char* argv[]) {
	FILE* fIncludes = fopen("/tmp/clcincludes.c", "w+");
	FILE* fDefs = fopen("/tmp/clcdefinitions.c", "w+");
	FILE* fprimary = fopen("/tmp/clcrepl.c","w+");
	FILE* ferrcheck = fopen("/tmp/clcerrcheck.c", "w+");
	int writestatus = fwrite(defaultMain, strlen(defaultMain), 1, fprimary);

	char cB, lastChar;
	pid_t compileChild, executeChild; 
	int compileStatus, executeStatus;
	int isDefs = 0, isInclude = 0;
	while(cB != EOF) {
		fseek(fprimary, -1, SEEK_END);
		printf("\n:>");
		while((cB = fgetc(stdin)) != '\n' || lastChar == '\\') {
			lastChar = cB;  
			if (cB == '#'){
				char lookAhead = fgetc(stdin);
				ungetc(lookAhead, stdin);
				if (lookAhead == 'i') {
					isInclude = 1;
				}
				if (lookAhead == 'd'){
					isDefs = 1;
				}
				if (!isDefs && !isInclude) {printf("huston we have a problem"); abort();}
			}
			fwrite(&cb, 1, 1, ferrcheck)
		}
		
		
		int status = execl("/usr/bin/clang", "/usr/bin/clang", "/tmp/clcerrcheck.c", "-o", "/tmp/clcrepl.out", (char *) 0);
		if (status != 1 {}
		if (isDefs) {
			fwrite("\n", 1, 1, fDefs);
		}
		if (isInclude){
			fwrite("\n", 1, 1, fIncludes); 	
		}
		if (!isInclude && !isDefs){
			fwrite("}", 1, 1, fprimary);
		}	
		isDefs = 0;
		isInclude = 0;
		fflush(fprimary); 
		fflush(fDefs);
		fflush(fIncludes);
		fflush(stdin);
		//fsync(fileno(fprimary)); // send file to disk uncomment if not writting to /tmp
		compileChild = fork();
		if (compileChild == 0) {
			int status = execl("/usr/bin/clang", "/usr/bin/clang", "/tmp/clcrepl.c", "-o", "/tmp/clcrepl.out", (char *) 0);
			return 1;
		} else {
			wait(&compileStatus);
		}	
		executeChild = fork();
		if (executeChild == 0) {
			int returnstatus = execl("/tmp/clcrepl.out", "/tmp/clcrepl.out", (char *) 0);
			return 1;
		} else {
			wait(&executeStatus);	
		}
	}
	close(fileno(fprimary));	// close file
	close(fileno(fDefs));
	close(fileno(fIncludes));
	return 0;
}
