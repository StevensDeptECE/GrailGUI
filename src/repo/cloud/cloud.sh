#!/bin/sh
# https://towardsdatascience.com/uploading-files-to-google-drive-directly-from-the-terminal-using-curl-2b89db28bb06

setup()
{
    # Step 1. Visit https://console.developers.google.com/apis/credentials?pli=1
    # Creating account will provide the client ID and client secret.

    # Step 2. Verify device
    codes=$(curl -d \
        "client_id=$GOOGLE_CLIENT_ID&scope=https://www.googleapis.com/auth/drive.file" \
        https://oauth2.googleapis.com/device/code | head -3 | cut -d'"' -f4)
    DEVICE_CODE=$(echo "$codes" | head -2 | tail -1)
    USER_CODE=$(echo "$codes" | tail -1)

    xdg-open "https://www.google.com/device"
    echo "Provide this code to the Google Device page that just was opened:"
    echo "$USER_CODE"
    echo ""
    echo "Once you're done, enter 'y' to continue."
    while [ "$continue_on" != 'y' ]; do
        read -r continue_on
    done

    # Step 4: Bearer code: Used to fingerprint account.
    curl -d client_id="$GOOGLE_CLIENT_ID" -d client_secret="$GOOGLE_CLIENT_SECRET" \
        -d device_code="$DEVICE_CODE" \
        -d grant_type=urn%3Aietf%3Aparams%3Aoauth%3Agrant-type%3Adevice_code \
        https://accounts.google.com/o/oauth2/token
}

upload()
{
    # File we want to upload (In our case, the website key/sitename pair file)
    FILENAME="$1"
    # Refresh the access token each upload so you never get locked out.
    ACCESS_TOKEN=$(curl -d client_id="$GOOGLE_CLIENT_ID" \
        -d client_secret="$GOOGLE_CLIENT_SECRET" \
        -d refresh_token="$GOOGLE_REFRESH_TOKEN" \
        -d grant_type=refresh_token \
        https://accounts.google.com/o/oauth2/token | head -2 | cut -d'"' -f4 | tail -1)

    # TODO: If file already exists, use a PUT to overwrite it instead of
    # creating 2 copies.
    UPLOAD_ID=$(curl -X POST -L \
        -H "Authorization: Bearer $ACCESS_TOKEN" \
        -F "metadata={name :'$FILENAME'};type=application/json;charset=UTF-8" \
        -F "file=@$FILENAME;type=$(file --mime-type -b "$FILENAME")" \
        "https://www.googleapis.com/upload/drive/v3/files?uploadType=multipart" | head -3 | cut -d'"' -f4 | tail -1)

    echo "File uploaded to:"
    echo "https://drive.google.com/file/d/$UPLOAD_ID"
}

download()
{
    # Name of file we want to download (In our case, the website key/sitename pair file)
    FILENAME="$1"
    # Refresh the access token each upload so you never get locked out.
    ACCESS_TOKEN=$(curl -d client_id="$GOOGLE_CLIENT_ID" \
        -d client_secret="$GOOGLE_CLIENT_SECRET" \
        -d refresh_token="$GOOGLE_REFRESH_TOKEN" \
        -d grant_type=refresh_token \
        https://accounts.google.com/o/oauth2/token | head -2 | cut -d'"' -f4 | tail -1)

    return 1
    # XXX: Does Google even support downloading files from drive over HTTP?
    curl -X POST -L \
        -H "Authorization: Bearer $ACCESS_TOKEN" \
        -F "metadata={name :'$FILENAME'};type=application/json;charset=UTF-8" \
        -F "file=@$FILENAME;type=$(file --mime-type -b "$FILENAME")" \
        "https://www.googleapis.com/upload/drive/v3/files?"
}

if [ ! "$1" ]; then
    setup
elif [ "$1" = "upload" ]; then
    if [ -f "$2" ]; then
        upload "$2"
    else
        echo "$2 is not a valid file to upload"
    fi
elif [ "$1" = "download" ]; then
    download "$2"
fi
