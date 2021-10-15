typedef struct Hash_Data
{
    char *file_path;
    char *hash_value;
}HASH_DATA;
void step1_2(char *msg_str, char *msg, int len);
void step3(unsigned int *buffer);
void step4(unsigned int *s);
int leftrotate(int x, int c);
int power_fn(int x, int n);
void md5Algo(char *msg, int len, char *hash_value);
void storeHash();
void validateFile();