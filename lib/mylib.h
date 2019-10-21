#include <stdio.h>
#include <windows.h>
#include <string.h>

#include <stdlib.h>
#include <ctype.h>


//移除註解
char *RemoveComment (char *str) {
    char *p = strstr(str, "//");
    if (p != NULL) {
        *p = '\0';
    }
    return str;
}
//刪除頭尾空白跟\t
char *strtrim (char *str) {
    char *start = str;
    char *end = str;

    while (*start == ' ' || *start == '\t') start++;
    while (start != NULL && end != NULL && *start != '\0') {
        *end++ = *start++;
    }
    //跳過'\0'
    *end--;
    
    while (*end == ' ' || *end == '\t') end--;
    //補上結束字元
    *(end + 1) = '\0';
    return str;
}

//wchar_t 型態轉成URL encode
char *wstring_to_utf8_hex(const wchar_t *input) {
    char *output;
    int *t = 0;

    int utf8Size = WideCharToMultiByte(CP_UTF8, 0, input, -1, NULL, 0, NULL, t);
    if (utf8Size > 0) {
        char *utf8 = malloc(utf8Size * sizeof(char));
        output = malloc(utf8Size * 5 * sizeof(char) + 1);
        char *temp = output;
        if (WideCharToMultiByte(CP_UTF8, 0, input, -1, utf8, utf8Size + 1, NULL, t) != 0) {
            for (char *p = utf8; *p; *p++) {
                char onehex[5];
                _snprintf(onehex, sizeof(onehex), "%%%02.2X", (unsigned char)*p);
                for (int i = 0; i < strlen(onehex); i++) {
                    *(temp++) = onehex[i];
                }
                // printf("%s:%s\n", output, onehex);
            }
            *temp = '\0';
        }
        free(utf8);
    }
    return output;
}

//取得檔案的設定
int User_Connect_Info_Init (char *fname) {
    FILE *fp = fopen(fname, "r");
    const int DataMaxLength = 1024;

    char *fStr;
    char buffer[1024];

    char *name = NULL;
    char *ipaddress = NULL;
    char *port = NULL;
    char *dbug = NULL;

    fread(buffer, DataMaxLength, 1, fp);
    fStr = strtok(buffer, "\n");
    while (fStr) {
        
        char *temp = strstr(fStr, "name:");
        if (temp != NULL) {
            name = malloc(strlen(temp)-5+1);
            strcpy(name, temp + 5);
            fStr = strtok(NULL, "\n");
            continue;
        }
        
        temp = strstr(fStr, "ipaddress:");
        if (temp != NULL) {
            ipaddress = malloc(strlen(temp)-10+1);
            strcpy(ipaddress, temp + 10);
            fStr = strtok(NULL, "\n");
            continue;
        }

        temp = strstr(fStr, "port:");
        if (temp != NULL) {
            port = malloc(strlen(temp)-5+1);
            strcpy(port, temp + 5);
            fStr = strtok(NULL, "\n");
            continue;
        }
        
        temp = strstr(fStr, "debug:");
        if (temp != NULL) {
            dbug = malloc(strlen(temp)-6+1);
            strcpy(dbug, temp + 6);
            fStr = strtok(NULL, "\n");
            continue;
        }

        fStr = strtok(NULL, "\n");
    }

    if (name) {
        user_info.name = strtrim(RemoveComment(name));
    } else {
        return 1;
    }
    if (ipaddress) {
        user_info.ip = strtrim(RemoveComment(ipaddress));
    } else {
        return 2;
    }
    if (port) {
        user_info.port = atoi(strtrim(RemoveComment(port)));
    } else {
        return 3;
    }
    if (dbug) {
        user_info.debug = atoi(strtrim(RemoveComment(dbug)));
    }
    
    fclose(fp);

    return 0;
}

//取得YT檔案的設定
int User_YT_Connect_Info_Init (char *fname) {
    FILE *fp = fopen(fname, "r");
    const int DataMaxLength = 1024;

    char *fStr;
    char buffer[1024];

    char *ipaddress = NULL;
    char *port = NULL;

    fread(buffer, DataMaxLength, 1, fp);
    fStr = strtok(buffer, "\n");
    while (fStr) {
        
        char *temp = strstr(fStr, "ipaddress:");
        if (temp != NULL) {
            ipaddress = malloc(strlen(temp)-10+1);
            strcpy(ipaddress, temp + 10);
            fStr = strtok(NULL, "\n");
            continue;
        }

        temp = strstr(fStr, "port:");
        if (temp != NULL) {
            port = malloc(strlen(temp)-5+1);
            strcpy(port, temp + 5);
            fStr = strtok(NULL, "\n");
            continue;
        }

        fStr = strtok(NULL, "\n");
    }

    if (ipaddress) {
        user_info.ytip = strtrim(RemoveComment(ipaddress));
    } else {
        return 2;
    }
    if (port) {
        user_info.ytport = atoi(strtrim(RemoveComment(port)));
    } else {
        return 3;
    }
    
    fclose(fp);

    return 0;
}