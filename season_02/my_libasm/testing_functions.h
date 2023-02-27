void test_strlen();
void test_strchr();
void test_memset();
void test_memcpy();
void test_strcmp();
void test_memmove();
void test_strncmp();
void test_strcasecmp();
void test_index();
void test_read();
void test_write();

// You can use thise testing functions or you can provide your own
#ifdef USE_TESTING_FUNCTIONS
static char msg1[] = "If you don't know memset idk what are you doing with your life";
static char msg2[] = "If you don't know memset idk what are you doing with your life";

void test_strlen() {
    printf("\nTesting my_strlen: ");
    assert(my_strlen("") == strlen(""));
    assert(my_strlen("my_strlen test") == strlen("my_strlen test"));
    assert(my_strlen("The quick brown fox jumps over the lazy dog") == strlen("The quick brown fox jumps over the lazy dog"));
    printf("\"my_strlen\" works as expected!\n");
}

void test_strchr() {
    printf("\nTesting my_strchr: ");
    assert(my_strchr("", 'a') == strchr("", 'a')); // both should be NULL (or 0)
    assert(my_strchr("The quick brown fox", 'l') == strchr("The quick brown fox", 'l'));
    assert(my_strchr("The quick brown fox jumps over the lazy dog", 'l') == strchr("The quick brown fox jumps over the lazy dog", 'l'));
    printf("\"my_strchr\" works as expected!\n");
}

void test_memset(){
    printf("\nTesting my_memset: ");

    // replacing the first character with a -
    assert(!strcmp((char*)my_memset(msg1, '-', 1), (char*)memset(msg2, '-', 1)));
    // replacing all characters with -
    assert(!strcmp((char*)my_memset(msg1, '-', strlen(msg1)), (char*)memset(msg2, '-', strlen(msg2))));
    printf("\"my_memset\" works as expected!\n");
}

void test_memcpy(){
    printf("\nTesting my_memcpy: ");
    assert(!strcmp((char*)my_memcpy(msg1, "EXPECT THAT SHIT", 8), (char*)memcpy(msg2, "EXPECT THAT SHIT", 8)));
    printf("\"my_memcpy\" works as expected!\n");
}

void test_strcmp(){
    printf("\nTesting my_strcmp: ");
    assert(my_strcmp("", "") == strcmp("", ""));
    assert(my_strcmp("a", "a") == strcmp("a", "a"));
    assert(my_strcmp("a", "") > 0);
    assert(my_strcmp("", "a") < 0);
    assert(my_strcmp("The quick brown fox jumps", "The quick brown dog") > 0);
    assert(my_strcmp("The quick brown dog", "The quick brown fox jumps") < 0);
    printf("\"my_strcmp\" works as expected!\n");
}

void test_memmove(){
    printf("\nTesting my_memmove: ");
    strcpy(msg1, "memmove is very useful...;)");
    my_memmove(msg1 + 20, msg1 + 15, 11);
    strcpy(msg2, "memmove is very useful...;)");
    memmove(msg2 + 20, msg2 + 15, 11);

    assert(!my_strcmp(msg1, msg2));
    printf("\"my_memmove\" works as expected!\n");
}

void test_strncmp(){
    printf("\nTesting my_strncmp: ");
    assert(my_strncmp("The quick brown dog", "A quick brown fox jumps", 0) == strncmp("The quick brown dog", "A quick brown fox jumps", 0));
    assert(my_strncmp("The quick brown dog", "The quick brown fox jumps", 15) == 0);
    assert(my_strncmp("The quick brown dog", "The quick brown fox jumps", 19) < 0);
    assert(my_strncmp("The quick brown fox jumps", "The quick brown dog", 19) > 0);
    printf("\"my_strncmp\" works as expected!\n");
}

void test_strcasecmp(){
    printf("\nTesting my_strcasecmp: ");
    assert(my_strcasecmp("", "") == strcasecmp("", ""));
    assert(my_strcasecmp("A", "a") == strcasecmp("A", "a"));
    assert(
      my_strcasecmp("The quick brown fox jumps over the lazy dog", "thE quicK browN foX jumpS oveR thE lazY doG") ==
      strcasecmp("The quick brown fox jumps over the lazy dog", "thE quicK browN foX jumpS oveR thE lazY doG")
    );
    assert(my_strcasecmp("a", "") > 0);
    assert(my_strcasecmp("", "a") < 0);
    assert(my_strcasecmp("THE QUICK BROWN FOX JUMPS", "The quick brown dog") > 0);
    assert(my_strcasecmp("The quick brown dog", "THE QUICK BROWN FOX JUMPS") < 0);
    printf("\"my_strcasecmp\" works as expected!\n");
}

void test_index(){
    printf("\nTesting my_index: ");
    assert(my_index("", 'a') == index("", 'a'));
    assert(my_index("The quick brown fox", 'l') == index("The quick brown fox", 'l'));
    assert(my_index("The quick brown fox jumps over the lazy dog", 'l') == index("The quick brown fox jumps over the lazy dog", 'l'));
    printf("\"my_index\" works as expected!\n");
}

void test_read(){
    printf("\nTesting my_read: ");
    char my_read_file_text[13], read_file_text[13];
    int my_fd = open("./read_file_test.txt", O_RDONLY), fd = open("./read_file_test.txt", O_RDONLY);

    assert(my_read(my_fd, my_read_file_text, 13) == read(fd, read_file_text, 13));
    close(my_fd);
    close(fd);
    printf("\"my_read\" works as expected!\n");
}

void test_write(){
    printf("\nTesting my_write: ");
    char string[10];
    assert(my_write(1, string, 7) == write(1, string, 7));
    printf("\"my_write\" works as expected!\n");
}


#endif
