#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

#define STRING_LENGTH 20
#define PASS_LENGTH 20
#define URL_LENGTH 50

#define NO_IMAGE -1

typedef struct {
	char username[STRING_LENGTH];
	char pass_hash[PASS_LENGTH];
	time_t registered_time;
	time_t birth_date;

	long first_image;
} User;

typedef struct {
	uint32_t id;
	char url[URL_LENGTH];

	char uploader_username[STRING_LENGTH];

	long next_image;
} Image;

typedef struct UserIndex {
	char username[20];
	long data_pos;
	struct UserIndex* next; 
} UserIndex;


UserIndex* user_index = NULL;


void insert_user_index(char* username, size_t data_pos) {
	if (user_index == NULL) {
		user_index = malloc(sizeof(UserIndex));
		strncpy(user_index->username, username, STRING_LENGTH);
		user_index->data_pos = data_pos;
		user_index->next = NULL;
		return;
	}
	UserIndex* node = user_index;
	UserIndex** node_ptr = &user_index;
	while (node != NULL && strcmp(node->username, username) < 0) {
		node_ptr = &node->next;
		node = node->next;
	}
	*node_ptr = malloc(sizeof(UserIndex));
	strncpy((*node_ptr)->username, username, STRING_LENGTH);
	(*node_ptr)->data_pos = data_pos;
	(*node_ptr)->next = node;
}


void print_user(User* user) {
	printf(user->username);
	printf("\n\tPassword hash: %s\n", user->pass_hash);
	printf("\tBirth: %s", ctime(&user->birth_date));
	printf("\tRegistered on %s", ctime(&user->registered_time));
	printf("\t(First image: %d)\n", user->first_image);
}

void print_user_index() {
	printf("----start index----\n");
	UserIndex* node = user_index;
	while (node) {
		printf(node->username);
		printf("\n\t%d\n", node->data_pos);
		node = node->next;
	}
	printf("----end index----\n");
}

void write_user_index() {
	FILE *output = fopen("user.ind", "wb");
	if (output == NULL) {
		perror("Error opening user.ind");
		exit(1);
	}
	UserIndex* node = user_index;

	while (node) {
		//write first two fields
		fwrite(node, STRING_LENGTH + sizeof(size_t), 1, output);
		node = node->next;
	}
	fclose(output);
}

void insert_user(User* user) {
	//Find empty spot to insert
	FILE* user_fl;

	FILE* user_misc = fopen("user.misc", "r+b");
	if (!user_misc) {
		perror("Error while opening user.misc");
		exit(1);
	}

	int c;
	if ((c = fgetc(user_misc)) == EOF) {
		//no GC spots
		fclose(user_misc);

		user_fl = fopen("user.fl", "ab");
		if (!user_fl) {
			fprintf(stderr, "Error appending to user.fl");
			exit(1);
		}
	} else {
		ungetc(c, user_misc);
		//found GC spot
		
		fseek(user_misc, -((long)sizeof(long)), SEEK_END);
		long gc_pos_pos = ftell(user_misc);
		long gc_pos;
		fread(&gc_pos, sizeof(long), 1, user_misc);
		gc_pos--;

		//overwrite user.fl
		char* buffer = malloc(gc_pos_pos);
		fseek(user_misc, 0, SEEK_SET);
		fread(buffer, gc_pos_pos, 1, user_misc);
		user_misc = freopen("user.misc", "wb", user_misc);
		fwrite(buffer, gc_pos_pos, 1, user_misc);
		fclose(user_misc);


		user_fl = fopen("user.fl", "r+b");
		fseek(user_fl, gc_pos, SEEK_SET);
	}

	fputc(1, user_fl);
	long pos = ftell(user_fl);
	fwrite(user, sizeof(User), 1, user_fl);	
	fclose(user_fl);
	insert_user_index(user->username, pos);
	write_user_index();
}


void read_user_index() {
	FILE *input = fopen("user.ind", "r");
	if (!input) {
		fprintf(stderr, "Error opening file!\n");
		exit(1);
	}

	UserIndex** node_ptr = &user_index;
	
	//read first two fields
	void* buffer = malloc(STRING_LENGTH + sizeof(size_t));
	while (fread(buffer, STRING_LENGTH + sizeof(size_t), 1, input)) {

		UserIndex* new_node = malloc(sizeof(UserIndex));
		memcpy(new_node, buffer, STRING_LENGTH + sizeof(size_t));

		new_node->next = NULL;
		*node_ptr = new_node;
		node_ptr = &(*node_ptr)->next;
	}
	free(buffer);
}

long get_user_pos(char* username) {
	UserIndex* node = user_index;
	while (node) {
		int cmp = strncmp(username, node->username, STRING_LENGTH);
		if (cmp < 0)
			return -1;
		if (cmp > 0) {
			node = node->next;
			continue;
		}
		return node->data_pos;
	}
	return -1;
}

User* get_user(char* username) {
	UserIndex* node = user_index;
	long pos = get_user_pos(username);
	if (pos == -1)
		return NULL;

	FILE* input = fopen("user.fl", "rb");
	if (!input) {
		fprintf(stderr, "Error reading user.fl");
		exit(1);
	}
	fseek(input, pos, SEEK_SET);

	User* user = malloc(sizeof(User));
	fread(user, sizeof(User), 1, input);
	fclose(input);
	return user;
}

void insert_image(Image* image) {
	//Find empty spot to insert
	FILE* img_fl;

	FILE* img_misc = fopen("img.misc", "r+b");
	if (!img_misc) {
		perror("Error while opening img.misc");
		exit(1);
	}

	uint32_t last_id;
	fread(&last_id, sizeof(uint32_t), 1, img_misc);
	last_id++;
	image->id = last_id;
	
	int c;
	if ((c = fgetc(img_misc)) == EOF) {
		//no GC spots

		img_fl = fopen("img.fl", "ab");
		if (!img_fl) {
			fprintf(stderr, "Error appending to img.fl");
			exit(1);
		}

		img_misc = freopen("img.misc", "wb", img_misc);
		fwrite(&last_id, sizeof(uint32_t), 1, img_misc);
		fclose(img_misc);
	} else {
		ungetc(c, img_misc);
		//found GC spot
		
		fseek(img_misc, -((long)sizeof(long)), SEEK_END);
		long gc_pos_pos = ftell(img_misc);
		long gc_pos;
		fread(&gc_pos, sizeof(long), 1, img_misc);

		//overwrite img.fl
		char* buffer = malloc(gc_pos_pos);
		fseek(img_misc, 0, SEEK_SET);
		fwrite(&last_id, sizeof(uint32_t), 1, img_misc);
		fseek(img_misc, 0, SEEK_SET);
		fread(buffer, gc_pos_pos, 1, img_misc);
		fclose(img_misc);
		img_misc = fopen("img.misc", "wb");
		fwrite(buffer, gc_pos_pos, 1, img_misc);
		fclose(img_misc);


		img_fl = fopen("img.fl", "r+b");
		fseek(img_fl, gc_pos - 1, SEEK_SET);
	}

	FILE* user_fl = fopen("user.fl", "r+b");
	if (!user_fl) {
		perror("Error while opening user.fl");
		exit(1);
	}


	fputc(1, img_fl);
	long img_pos = ftell(img_fl);
	long user_pos = get_user_pos(image->uploader_username);
	long first_img_pos;
	fseek(user_fl, user_pos + offsetof(User, first_image), SEEK_SET);
	fread(&first_img_pos, sizeof(long), 1, user_fl);

	image->next_image = first_img_pos;
	fwrite(image, sizeof(Image), 1, img_fl);

	fseek(user_fl, -((int)sizeof(long)), SEEK_CUR);
	fwrite(&img_pos, sizeof(long), 1, user_fl);
	

	fclose(img_fl);
	fclose(user_fl);
}

Image* get_image(long id, char* uploader_username) {
	FILE* img_fl = fopen("img.fl", "rb");

	User* uploader = get_user(uploader_username);
	long image_pos = uploader->first_image;
	free(uploader);

	while (image_pos != NO_IMAGE) {
		fseek(img_fl, image_pos, SEEK_SET);
		Image* image = malloc(sizeof(Image));
		fread(image, sizeof(Image), 1, img_fl);

		if (image->id == id) {
			fclose(img_fl);
			return image;
		}
		image_pos = image->next_image;
	}
	fclose(img_fl);
	return NULL;
}

long get_image_pos(long id, char* uploader_username, FILE* img_fl) {
	User* uploader = get_user(uploader_username);
	long image_pos = uploader->first_image;
	free(uploader);


	while (image_pos != NO_IMAGE) {
		fseek(img_fl, image_pos, SEEK_SET);
		Image* image = malloc(sizeof(Image));
		fread(image, sizeof(Image), 1, img_fl);

		if (image->id == id) {
			free(image);
			return image_pos;
		}
		image_pos = image->next_image;
	}
	return NO_IMAGE;
}

void print_image(Image* image) {
	printf("%d\n", image->id);
	printf("\tURL: %s\n", image->url);
	printf("\tUploader: %s\n", image->uploader_username);
	printf("\t(Next image: %d)\n", image->next_image);
}

int update_user_birth_date(char* username, time_t birth_date) {
	long user_pos = get_user_pos(username);
	if (user_pos == -1) {
		fprintf(stderr, "User does not exist");
		return 1;
	}
	FILE* user_fl = fopen("user.fl", "r+b");
	fseek(user_fl, user_pos + offsetof(User, birth_date), SEEK_SET);
	fwrite(&birth_date, sizeof(time_t), 1, user_fl);
	fclose(user_fl);
	return 0;
}

int update_user_registered_time(char* username, time_t registered_time) {
	long user_pos = get_user_pos(username);
	if (user_pos == -1) {
		fprintf(stderr, "User does not exist");
		return 1;
	}
	FILE* user_fl = fopen("user.fl", "r+b");
	fseek(user_fl, user_pos + offsetof(User, registered_time), SEEK_SET);
	fwrite(&registered_time, sizeof(time_t), 1, user_fl);
	fclose(user_fl);
	return 0;
}

int update_user_pass_hash(char* username, char* pass_hash) {
	long user_pos = get_user_pos(username);
	if (user_pos == -1) {
		fprintf(stderr, "User does not exist");
		return 1;
	}
	FILE* user_fl = fopen("user.fl", "r+b");
	fseek(user_fl, user_pos + offsetof(User, pass_hash), SEEK_SET);
	fwrite(pass_hash, strlen(pass_hash), 1, user_fl);
	fputc('\0', user_fl);
	fclose(user_fl);
	return 0;
}

int update_image_url(uint32_t id, char* uploader_username, char* url) {
	FILE* img_fl = fopen("img.fl", "r+b");
	long image_pos = get_image_pos(id, uploader_username, img_fl);
	if (image_pos == NO_IMAGE) {
		fprintf(stderr, "Image does not exist");
		return 1;
	}
	fseek(img_fl, image_pos + offsetof(Image, url), SEEK_SET);
	fwrite(url, strlen(url), 1, img_fl);
	fputc('\0', img_fl);
	fclose(img_fl);
	return 0;
}

void init_file(char* name) {
	if (access(name, F_OK) == -1) {
		FILE* img_misc = fopen(name, "wb");
		fclose(img_misc);
	}
}

void init_files() {
	init_file("user.ind");
	init_file("user.fl");
	init_file("user.misc");
	init_file("img.fl");
	if (access("img.misc", F_OK) == -1) {
		//img.misc does not exist

		FILE* img_misc = fopen("img.misc", "wb");

		uint32_t next_id = 0;
		fwrite(&next_id, sizeof(uint32_t), 1, img_misc);

		fclose(img_misc);
	}
}

int reserve_image_id() {
	FILE* img_misc = fopen("img.misc", "r+b");

	if (!img_misc) {
		perror("Error while opening read/write stream for img.misc");
		exit(1);
	}

	uint32_t next_id;
	fread(&next_id, sizeof(uint32_t), 1, img_misc);

	next_id++;
	rewind(img_misc);
	fwrite(&next_id, sizeof(uint32_t), 1, img_misc);

	fclose(img_misc);
	return next_id;
}

int delete_image(uint32_t id, char* uploader_username) {
	//printf("\n---\nDeleting ID %d, uploader: %s\n---\n", id, uploader_username);
	long user_pos = get_user_pos(uploader_username);
	if (user_pos == -1) {
		fprintf(stderr, "Username %s not found.", uploader_username);
		return 1;
	}
	
	FILE* user_fl = fopen("user.fl", "r+b");	

	fseek(user_fl, user_pos + offsetof(User, first_image), SEEK_SET);
	long image_pos;
	fread(&image_pos, sizeof(long), 1, user_fl);
	
	//printf("First pos: %d\n", image_pos);

	if (image_pos == NO_IMAGE) {
		fclose(user_fl);
		fprintf(stderr, "Image ID &d not found.", id);
		return 1;
	}

	FILE* img_fl = fopen("img.fl", "r+b");
	FILE* img_misc = fopen("img.misc", "ab");

	if (!img_fl) {
		perror("Error while opening read/write stream for img.fl");
		exit(1);
	}

	fseek(img_fl, image_pos, SEEK_SET);
	Image* image = malloc(sizeof(Image));
	fread(image, sizeof(Image), 1, img_fl);

	int failed = 1;
	//printf("Current ID: %d\n", image->id);
	if (image->id == id) {
		//First image in list
		fseek(user_fl, -((int)sizeof(long)), SEEK_CUR);
		fwrite(&image->next_image, sizeof(long), 1, user_fl);

		//mark for GC
		fseek(img_fl, -1l - sizeof(Image), SEEK_CUR);
		fputc(0, img_fl);
		fwrite(&image_pos, sizeof(long), 1, img_misc);

		failed = 0;
	} else {
		while (image->next_image != NO_IMAGE) {
			long next_image_pos = image->next_image;
			fseek(img_fl, next_image_pos, SEEK_SET);
			fread(image, sizeof(Image), 1, img_fl);
			//printf("Current ID: %d\n", image->id);

			if (image->id == id) {
				//mark for GC
				fseek(img_fl, -1l - sizeof(Image), SEEK_CUR);
				fputc(0, img_fl);
				fwrite(&next_image_pos, sizeof(long), 1, img_misc);

				//update list
				fseek(img_fl, image_pos + offsetof(Image, next_image), SEEK_SET);
				fwrite(&image->next_image, sizeof(long), 1, img_fl);
				failed = 0;
				break;
			}
			image_pos = next_image_pos;
		}
	}

	free(image);
	fclose(user_fl);
	fclose(img_fl);
	fclose(img_misc);
	if (failed) {
		fprintf(stderr, "Image ID not found.\n");
	}
	return failed;
}

int delete_user(char* username) {
	//Step 1: delete user from UserIndex
	
	long user_pos = -1;
	UserIndex* node = user_index;
	UserIndex** node_ptr = &user_index;
	while (node) {
		int cmp = strncmp(username, node->username, STRING_LENGTH);
		if (cmp < 0) {
			fprintf(stderr, "Username %s not found.", username);
			return -1;
		}
		if (cmp > 0) {
			node_ptr = &node->next;
			node = node->next;
			continue;
		}
		*node_ptr = node->next;
		user_pos = node->data_pos;
		free(node);
		break;
	}

	if (user_pos == -1) {
		fprintf(stderr, "Username %s not found.", username);
		return -1;
	}

	write_user_index();


	//Step 2: mark for GC in user.fl

	FILE* user_fl = fopen("user.fl", "r+b");
	if (!user_fl) {
		perror("Error while opening read/write stream for user.fl");
		exit(1);
	}
	FILE* user_misc = fopen("user.misc", "ab");
	if (!user_misc) {
		perror("Error while appending to user.misc");
		exit(1);
	}

	//mark for GC
	fseek(user_fl, user_pos - 1, SEEK_SET);
	fputc(0, user_fl);
	fwrite(&user_pos, sizeof(long), 1, user_misc);


	fseek(user_fl, offsetof(User, first_image), SEEK_CUR);
	long image_pos;
	fread(&image_pos, sizeof(long), 1, user_fl);

	fclose(user_fl);
	fclose(user_misc);


	//Step 3: mark for GC in image.fl

	if (image_pos == NO_IMAGE)
		return 0;
		
	FILE* img_fl = fopen("img.fl", "r+b");
	if (!img_fl) {
		perror("Error while opening read/write stream for img.fl");
		exit(1);
	}
	FILE* img_misc = fopen("img.misc", "ab");
	if (!img_misc) {
		perror("Error while appending to img.misc");
		exit(1);
	}

	printf("%d\n", NO_IMAGE == -1);
	while (image_pos != NO_IMAGE) {
		//mark for GC
		fseek(img_fl, image_pos - 1, SEEK_SET);
		fputc(0, img_fl);
		fwrite(&image_pos, sizeof(long), 1, img_misc);

		fseek(img_fl, offsetof(Image, next_image), SEEK_CUR);
		fread(&image_pos, sizeof(long), 1, img_fl);
	}

	fclose(img_fl);
	fclose(img_misc);
}

void gc_users() {
	FILE* user_misc = fopen("user.misc", "rb");
	size_t gc_count = 100;	//TODO: measure actual file size?
	long* gc_positions = malloc(sizeof(long) * gc_count);
	memset(gc_positions, 0, sizeof(long) * gc_count);
	
	fread(gc_positions, sizeof(long), gc_count, user_misc);

	fclose(user_misc);
	user_misc = fopen("user.misc", "wb");
	fclose(user_misc);

	//Step 1: update UserIndex
	
	UserIndex* node = user_index;
	while (node) {
		for (size_t i = 0; i < gc_count; i++) {
			if (gc_positions[i] == 0)
				break;
			if (node->data_pos >= gc_positions[i])
				node->data_pos -= 1 + sizeof(User);
		}
		node = node->next;
	}

	//Step 2: overwrite user.fl
	
	FILE* user_fl_new = fopen("user.fl.new", "wb");
	if (!user_fl_new) {
		perror("Error while writing to user.fl.new");
		exit(1);
	}
	FILE* user_fl = fopen("user.fl", "rb");
	if (!user_fl) {
		perror("Error while reading from user.fl");
		exit(1);
	}

	int is_present;
	User* user = malloc(sizeof(User));
	while ((is_present = fgetc(user_fl)) != EOF) {
		fread(user, sizeof(User), 1, user_fl);
		if (is_present) {
			fputc(1, user_fl_new);
			fwrite(user, sizeof(User), 1, user_fl_new);
		}
	}
	free(user);
	
	fclose(user_fl_new);
	fclose(user_fl);

	remove("user.fl");
	rename("user.fl.new", "user.fl");

	free(gc_positions);
}

void gc_images() {
	FILE* img_misc = fopen("img.misc", "rb");
	size_t gc_count = 100;	//TODO: measure actual file size?
	long* gc_positions = malloc(sizeof(long) * gc_count);
	memset(gc_positions, 0, sizeof(long) * gc_count);
	
	uint32_t last_id;
	fread(&last_id, sizeof(uint32_t), 1, img_misc);
	fread(gc_positions, sizeof(long), gc_count, img_misc);

	fclose(img_misc);
	img_misc = fopen("img.misc", "wb");
	fwrite(&last_id, sizeof(uint32_t), 1, img_misc);
	fclose(img_misc);

	//Step 1: overwrite img.fl
	
	FILE* img_fl_new = fopen("img.fl.new", "wb");
	if (!img_fl_new) {
		perror("Error while writing to img.fl.new");
		exit(1);
	}
	FILE* img_fl = fopen("img.fl", "rb");
	if (!img_fl) {
		perror("Error while reading from img.fl");
		exit(1);
	}

	int is_present;
	Image* image = malloc(sizeof(Image));
	while ((is_present = fgetc(img_fl)) != EOF) {
		fread(image, sizeof(Image), 1, img_fl);
		if (is_present) {
			fputc(1, img_fl_new);
			for (size_t i = 0; i < gc_count; i++) {
				if (gc_positions[i] == 0)
					break;
				if (image->next_image >= gc_positions[i])
					image->next_image -= 1 + sizeof(Image);
			}
			fwrite(image, sizeof(Image), 1, img_fl_new);
		}
	}
	free(image);

	fclose(img_fl_new);
	fclose(img_fl);

	remove("img.fl");
	rename("img.fl.new", "img.fl");

	//Step 2: overwrite user.fl
	
	FILE* user_fl_new = fopen("user.fl.new", "wb");
	if (!user_fl_new) {
		perror("Error while writing to user.fl.new");
		exit(1);
	}
	FILE* user_fl = fopen("user.fl", "rb");
	if (!user_fl) {
		perror("Error while reading from user.fl");
		exit(1);
	}

	User* user = malloc(sizeof(User));
	while ((is_present = fgetc(user_fl)) != EOF) {
		fread(user, sizeof(User), 1, user_fl);

		fputc(1, user_fl_new);
		for (size_t i = 0; i < gc_count; i++) {
			if (gc_positions[i] == 0)
				break;
			if (user->first_image >= gc_positions[i])
				user->first_image -= 1 + sizeof(Image);
		}
		fwrite(user, sizeof(User), 1, user_fl_new);
	}
	free(user);
	
	fclose(user_fl_new);
	fclose(user_fl);

	remove("user.fl");
	rename("user.fl.new", "user.fl");

	free(gc_positions);
}

void print_all_images() {
	FILE* img_fl = fopen("img.fl", "rb");
	if (!img_fl) {
		perror("Error while reading img.fl");
		exit(1);
	}
	Image* image = malloc(sizeof(Image));

	printf("----start----\n");
	int c;
	while ((c = fgetc(img_fl)) != EOF) {
		long pos = ftell(img_fl);
		fread(image, sizeof(Image), 1, img_fl);
		printf("[Position = %d]\n", pos);
		if (c)
			print_image(image);
	}

	FILE* img_misc = fopen("img.misc", "rb");
	if (!img_misc) {
		perror("Error while reading img.misc");
		exit(1);
	}

	printf("[Misc]\n");
	uint32_t last_id;
	fread(&last_id, sizeof(uint32_t), 1, img_misc);
	printf("Last image ID: %d\n", last_id);

	long gc_pos;
	while (fread(&gc_pos, sizeof(long), 1, img_misc)) {
		printf("Garbage Collection pos %d\n", gc_pos);
	}

	printf("----end----\n");
	
	free(image);
	fclose(img_fl);
	fclose(img_misc);
}

void print_all_users() {
	printf("----start----\n");
	print_user_index();
	FILE* user_fl = fopen("user.fl", "rb");
	if (!user_fl) {
		perror("Error while reading user.fl");
		exit(1);
	}
	User* user = malloc(sizeof(User));

	int c;
	while ((c = fgetc(user_fl)) != EOF) {
		long pos = ftell(user_fl);
		fread(user, sizeof(User), 1, user_fl);
		printf("[Position = %d]\n", pos);
		if (c)
			print_user(user);
	}

	FILE* user_misc = fopen("user.misc", "rb");
	if (!user_misc) {
		perror("Error while reading user.misc");
		exit(1);
	}


	printf("[Misc]\n");
	long gc_pos;
	while (fread(&gc_pos, sizeof(long), 1, user_misc)) {
		printf("Garbage Collection pos %d\n", gc_pos);
	}
	printf("----end----\n");
	
	free(user);
	fclose(user_fl);
	fclose(user_misc);
}

char* read_no_newline(char* buffer, size_t len) {
	char* result = fgets(buffer, len, stdin);
	if (result)
		buffer[strcspn(buffer, "\n")] = '\0';
	return result;
}

size_t total_users() {
	size_t count = 0;
	UserIndex* node = user_index;
	while (node) {
		count++;
		node = node->next;
	}
	return count;
}

size_t total_images() {
	size_t count = 0;
	FILE* img_fl = fopen("img.fl", "rb");

	int c;
	while ((c = fgetc(img_fl)) != EOF) {
		if (c == 1)
			count++;
		fseek(img_fl, sizeof(Image), SEEK_CUR);
	}
	fclose(img_fl);
	return count;
}

size_t count_images(char* uploader_username) {
	User* uploader = get_user(uploader_username);
	if (!uploader || uploader->first_image == NO_IMAGE)
		return 0;

	FILE* img_fl = fopen("img.fl", "rb");

	long image_pos = uploader->first_image;
	free(uploader);

	size_t count = 0;
	while (image_pos != NO_IMAGE) {
		fseek(img_fl, image_pos + offsetof(Image, next_image), SEEK_SET);
		fread(&image_pos, sizeof(long), 1, img_fl);
		count++;
	}

	fclose(img_fl);
	return count;
}


int main() {
	init_files();	

	read_user_index();

	printf("Enter command (exit, gc, get-m, get-s, insert-m, insert-s, delete-m, delete-s, update-m, update-s, ul-m, ul-s, total-m, total-s, count-s): \n");
	char str[STRING_LENGTH];
	char str2[STRING_LENGTH];
	while (read_no_newline(str, STRING_LENGTH)) {
		if (strncmp(str, "exit", STRING_LENGTH) == 0) {
			exit(0);
		} else if (strncmp(str, "insert-m", STRING_LENGTH) == 0) {
			User* user = malloc(sizeof(User));

			printf("Username: ");
			read_no_newline(str, STRING_LENGTH);
			strncpy(user->username, str, STRING_LENGTH);

			printf("Password hash: ");
			read_no_newline(str, STRING_LENGTH);
			strncpy(user->pass_hash, str, STRING_LENGTH);

			printf("Registered time (UNIX time): ");
			scanf("%i", &user->registered_time);
			printf(ctime(&user->registered_time));

			printf("Birth date (UNIX time): ");
			scanf("%i", &user->birth_date);
			printf(ctime(&user->birth_date));

			user->first_image = NO_IMAGE;
			insert_user(user);
			free(user);
		} else if (strncmp(str, "insert-s", STRING_LENGTH) == 0) {
			Image* image = malloc(sizeof(Image));

			printf("Uploader username: ");
			read_no_newline(str, STRING_LENGTH);
			strncpy(image->uploader_username, str, STRING_LENGTH);

			printf("URL: ");
			read_no_newline(str, STRING_LENGTH);
			strncpy(image->url, str, STRING_LENGTH);

			image->next_image = NO_IMAGE;
			insert_image(image);
			free(image);
		} else if (strncmp(str, "ul-m", STRING_LENGTH) == 0) {
			printf("\nUsers:\n");
			print_all_users();
		} else if (strncmp(str, "ul-s", STRING_LENGTH) == 0) {
			printf("\nImages:\n");
			print_all_images();
		} else if (strncmp(str, "get-m", STRING_LENGTH) == 0) {
			printf("Search for username: ");
			read_no_newline(str, STRING_LENGTH);

			User* user = get_user(str);
			if (user) {
				printf("\n");
				print_user(user);
				free(user);
			}
		} else if (strncmp(str, "delete-s", STRING_LENGTH) == 0) {
			uint32_t id;
			printf("Delete image ID: ");
			//scanf("%i", &id);
			//printf("Entered %i\n", id);
			read_no_newline(str, STRING_LENGTH);
			id = strtoul(str, NULL, 0);

			printf("Enter uploader's username: ");
			read_no_newline(str, STRING_LENGTH);

			if (!delete_image(id, str))
				printf("Deleted succesfully.");
		} else if (strncmp(str, "delete-m", STRING_LENGTH) == 0) {
			printf("Delete user: ");
			read_no_newline(str, STRING_LENGTH);

			if (!delete_user(str))
				printf("Deleted succesfully.");
		} else if (strncmp(str, "update-s", STRING_LENGTH) == 0) {
			uint32_t id;
			printf("Update image ID: ");
			read_no_newline(str, STRING_LENGTH);
			id = strtoul(str, NULL, 0);

			printf("Enter uploader's username: ");
			read_no_newline(str, STRING_LENGTH);

			printf("Enter new URL: ");
			read_no_newline(str2, STRING_LENGTH);

			//printf("\nID = %i, uploader = %s, URL = %s\n\n", id, str, str2);

			if (!update_image_url(id, str, str2))
				printf("Updated succesfully.");
		} else if (strncmp(str, "update-m", STRING_LENGTH) == 0) {
			printf("Update username: ");
			read_no_newline(str, STRING_LENGTH);

			printf("Enter new password hash: ");
			read_no_newline(str2, STRING_LENGTH);

			//printf("\nusername = %s, hash = %s\n\n", str, str2);

			if (!update_user_pass_hash(str, str2))
				printf("Updated succesfully.");
		} else if (strncmp(str, "gc", STRING_LENGTH) == 0) {
			gc_users();
			gc_images();
		} else if (strncmp(str, "total-m", STRING_LENGTH) == 0) {
			printf("\nTotal users: %d\n", total_users());
		} else if (strncmp(str, "total-s", STRING_LENGTH) == 0) {
			printf("\nTotal images: %d\n", total_images());
		} else if (strncmp(str, "count-s", STRING_LENGTH) == 0) {
			printf("Enter uploader's username: ");
			read_no_newline(str, STRING_LENGTH);

			printf("\nImages: %d\n", count_images(str));
		}
		printf("Enter command (exit, gc, get-m, get-s, insert-m, insert-s, delete-m, delete-s, update-m, update-s, ul-m, ul-s, total-m, total-s, count-s): \n");
	}

	return 0;
}
