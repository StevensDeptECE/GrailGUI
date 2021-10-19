#!/bin/sh
# https://towardsdatascience.com/uploading-files-to-google-drive-directly-from-the-terminal-using-curl-2b89db28bb06

# File we want to upload (In our case, the website key/sitename pair file)
FILENAME=

# Step 1. Visit https://console.developers.google.com/apis/credentials?pli=1
# Creating account will provide these tokens.
CLIENT_ID=
CLIENT_SECRET=

# Step 2. Verify device
codes=$(curl -d \
    "client_id=$CLIENT_ID&scope=https://www.googleapis.com/auth/drive.file" \
    https://oauth2.googleapis.com/device/code | head -2 | cut -d'"' -f4)
DEVICE_CODE=$(echo $codes | head -1)
USER_CODE=$(echo $codes | tail -1)

# Step 3. Visit https://www.google.com/device and provide $USER_CODE

# Step 4: Bearer code: Used to fingerprint account.
curl -d client_id="$CLIENT_ID" -d client_secret="$CLIENT_SECRET" \
    -d device_code="$DEVICE_CODE" \
    -d grant_type=urn%3Aietf%3Aparams%3Aoauth%3Agrant-type%3Adevice_code \
    https://accounts.google.com/o/oauth2/token
ACCESS_TOKEN=

# Step 5: Upload file
curl -X POST -L \
    -H "Authorization: Bearer $ACCESS_TOKEN" \
    -F "metadata={name :'$FILENAME'};type=application/json;charset=UTF-8" \
    -F "file=@$FILENAME;type=$(file --mime-type -b "$FILENAME")" \
    "https://www.googleapis.com/upload/drive/v3/files?uploadType=multipart"
