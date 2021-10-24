#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>
// https://curl.se/libcurl/c/libcurl-tutorial.html
#include <curl/curl.h>

int main(int argc, char** argv)
{
    if (argc != 2) {
        fprintf(stderr, "File to upload required\n");
        return EXIT_FAILURE;
    }

    // Read in the input file which will be uploaded.
    FILE *file = fopen(argv[1], "rb");
    if (!file) {
        fprintf(stderr, "Can't open %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    // Get the input file's metadata.
    struct stat file_info;
    if (fstat(fileno(file), &file_info))
        return EXIT_FAILURE;

    // Verifying tokens in environment.
    char *token_strs[3] = { (char *)"GOOGLE_CLIENT_ID",
        (char *)"GOOGLE_CLIENT_SECRET", (char *)"GOOGLE_REFRESH_TOKEN" };
    char *tokens[3] = {};
    for (int i = 0; i < 3; ++i) {
        if (!(tokens[i] = getenv(token_strs[i]))) {
            fprintf(stderr, "%s token not in environment.\n", token_strs[i]);
            return EXIT_FAILURE;
        }
    }

    // Get temp access token.
    CURL *curl = curl_easy_init();
    if (curl) {
        // ACCESS_TOKEN=$(curl -d client_id="$GOOGLE_CLIENT_ID" \
        //     -d client_secret="$GOOGLE_CLIENT_SECRET" \
        //     -d refresh_token="$GOOGLE_REFRESH_TOKEN" \
        //     -d grant_type=refresh_token \
        //     https://accounts.google.com/o/oauth2/token | head -2 | cut -d'"' -f4 | tail -1)
        curl_easy_setopt(curl, CURLOPT_URL,
                "https://accounts.google.com/o/oauth2/token");

        char fields[1024] = "";
        strcpy(fields, strcat(fields, "client_id="));
        strcpy(fields, strcat(fields, tokens[0]));
        strcpy(fields, strcat(fields, "&client_secret="));
        strcpy(fields, strcat(fields, tokens[1]));
        strcpy(fields, strcat(fields, "&refresh_token="));
        strcpy(fields, strcat(fields, tokens[2]));
        strcpy(fields, strcat(fields, "&grant_type=refresh_token"));
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, fields);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "Failed to get temp access token: %s\n",
                    curl_easy_strerror(res));
            return EXIT_FAILURE;
        }

        // UPLOAD_ID=$(curl -X POST -L \
        //     -H "Authorization: Bearer $ACCESS_TOKEN" \
        //     -F "metadata={name :'$FILENAME'};type=application/json;charset=UTF-8" \
        //     -F "file=@$FILENAME;type=$(file --mime-type -b "$FILENAME")" \
        //     "https://www.googleapis.com/upload/drive/v3/files?uploadType=multipart" | head -3 | cut -d'"' -f4 | tail -1)
        curl_easy_setopt(curl, CURLOPT_URL,
                "https://www.googleapis.com/upload/drive/v3/files?uploadType=media");

        // TODO: Need to add metadata for filename based on argv[1].
        char auth[] = "Authorization: Bearer ";
        char access_code[] = "";

        struct curl_slist *header = NULL;
        header = curl_slist_append(header, strcat(auth, access_code));
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header);
        /* tell it to "upload" to the URL */
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

        /* set where to read from (on Windows you need to use READFUNCTION too) */
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, file);
        /* give the size of the upload */
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (curl_off_t)file_info.st_size);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
        curl_easy_setopt(curl, CURLOPT_READDATA, file);

        /* enable verbose for easier tracing */
        /* curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); */

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "Failed to upload file: %s\n",
                    curl_easy_strerror(res));
            return EXIT_FAILURE;
        }
        curl_easy_cleanup(curl);
    }
    fclose(file);
    return EXIT_SUCCESS;
}
