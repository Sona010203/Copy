#include <iostream>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cerrno>

int main(int argc, char **argv){

	// checking number of arguments
	if(argc < 3){
		std::cerr << "Too few arguments\n";
		return 1;
	}

	// getting source and destination paths
	const char* source = argv[1];
	const char* destination = argv[2];

	//checking if sourse and destination are different
	if (strcmp(source, destination) == 0) {
		std::cerr << "Sourse and destination files are the same\n";
		return 1;
	}

	// opening source file
	int source_fd = open(source, O_RDONLY);

	// checking if file was opened successfully
	if(source_fd < 0){
		exit(errno);
	}

	// creating destination file if it doesn't exist
	int destination_fd = open(destination, O_CREAT | O_EXCL | O_WRONLY, 0644);

	// checking if file already exists
	if(destination_fd < 0){

		if(errno == EEXIST) {

			std::cout << "File " << destination << " already exists. To rewrite enter y else n : ";
			
			char answer;
			std::cin >> answer;

			if(answer != 'y'&& answer != 'Y') {
				exit(errno);
			}

			//opening the file if user wants to rewrite it
			else {
				destination_fd = open(destination, O_CREAT | O_WRONLY, 0644);

				//checking if it was opened successfully
				if(destination_fd < 0) {
					exit(errno);
				}
			}

		}
	}

	// creating buffer
	char buffer[4096];

	while(true){

		int count = read(source_fd, buffer, 4096);

		if(count == 0){
			break;
		}

		if(count < 0){
			return 1;
		}

		int write_count = write(destination_fd, buffer, count);

		if(write_count < 0){
			exit(errno);
		}

	}

	//closing source and destination files
	if(close(source_fd) != 0) {
		exit(errno);
	}

	if(close(destination_fd) != 0) {
		exit(errno);
	}
 
	return 0;
}
