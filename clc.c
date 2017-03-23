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
	int writestatus = fwrite(defaultMain, strlen(defaultMain), 1, fprimary);

	char cB, lastChar;
	pid_t compileChild, executeChild; 
	int compileStatus, executeStatus;
	int isDefs = 0, isInclude = 0;
	while(cB != EOF) {
		fseek(fprimary, -1, SEEK_END);
		printf(":>");
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
			if (isInclude){
				fwrite(&cB, 1, 1, fIncludes);
			}
			if (isDefs) {
				fwrite(&cB, 1, 1, fDefs);
			}
			if (!isInclude && !isDefs){
				fwrite(&cB, 1, 1, fprimary);
			}
		}
		isDefs = 0;
		isInclude = 0;
		fwrite("}", 1, 1, fprimary);
		// clean up and send to disk	
		fflush(fprimary); // flush in file stream
		fflush(fDefs); // flush in file stream
		fflush(fIncludes); // flush in file stream
		fflush(stdin);
		//fsync(fileno(fprimary)); // send file to disk
		compileChild = fork();
		if (compileChild == 0) {
			int status = execl("/usr/bin/clang", "/usr/bin/clang", "/tmp/clcrepl.c", "-o", "/tmp/clcrepl.out", (char *) 0);
			return 1;
		} else {
			wait(&compileStatus);
		}	
		executeChild = fork();
		if (executeChild == 0) {
			int returnstatus = execl("/tmp/clcrepl.out", "/tmp/clcrepl.out", (char *) 1);
			printf("in child 2\n");
			//execl("/bin/echo", "/bin/echo", "he", (char *) 0);
			perror("status: ");
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
// grab input line
	// test if line has errors with clang
	// if errors display to user and don't write 
	// if line is has assingment automaticully display assigment
	// if no errors write to file and execute
	// 
