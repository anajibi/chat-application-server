#include <stdio.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <time.h>
#include "cJSON.c"

#define SA struct sockaddr
#define PORT 12345
#define NAMEMAX 65
#define PASSMAX 129
#define MESSAGEMAX 150000
#define READMODE "r"
#define WRITEMODE "w"

const char authTokeArr[62] ="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
const char* registerPattern = "register %[^,], %s";
const char* loginPattern = "login %[^,], %s";
const char* createChannelPattern = "create channel %[^,], %s";
const char* joinChannelPattern = "join channel %[^,], %s";
const char* logoutPattern = "logout %s";
const char* sendMessagePattern = "send %[^,], %s";
const char* refreshPattern = "refresh %s";
const char* channelMembersPattern = "channel members %s";
const char* leavePattern = "leave %s";

 typedef struct {
     char username[NAMEMAX];
     char password[PASSMAX];
     char channelName[NAMEMAX];
 }User;
 typedef struct {
     char name[NAMEMAX];
 }Channel;
 typedef struct {
     char Token[33];
     char name[NAMEMAX];
     bool isAMember;
 }Online;
 typedef struct {
     int numberOfOnlineUsers;
     char membersArr[50][NAMEMAX];
     char ChannelName[NAMEMAX];
 }ChannelInfo;
 typedef struct {
     int numberOfUsers;
     int numberOfChannels;
 }DataBase;

int onlineUsersCount = 0;
int onlineChannelsCount = 0;
int userIndex;

Online onlineUsers[100];
ChannelInfo channelsInfo[50];
DataBase mainDataBase;

bool channelExists(const char*name);
bool userExists(const char*name);
const char* makeNewMessage (const char* senderName, const char * contentString);
void getUserMessages(const char* name, char buffer[150000]);
void getChannelMessages(const char* name, char buffer[150000]);
void addMessageToMembers(const char * message,  const char* channelName);
void deleteMemberFromChannel(const char*channelName, const char* memberName );
void registerUser(char* request, char response[150000]);
void login (char* request, char response[150000] );
void createChannel (char* request, char response[150000]);
void logout(char* request, char response[150000]);
void sendMessage (char* request, char response[150000]);
void leave(char * request, char response[150000]);
void channelMembers(char* request, char response[150000]);
void joinChannel (char* request, char response[150000]);
void saveChannelMessages(const char*  channelName, const char* list);
void dataBaseCreator();
void saveUsersMessages(const char*  userName, const char* list);
void createNewUser(User user);
void createNewChannel(Channel channel);
void deleteOnlineUser(int userIndexL);
void authTokenCreator();
void syncDataBase (DataBase dataBase);
void refresh (char* request, char response[150000]);
int usernameFinderByAuthToken(char*authToken);
int socketCreator( int server_socket, struct sockaddr_in client);
int findOnlineUserByAuthToken( const char authToken[33]);
int findOnlineUserByName( const char* name);
int findOnlineChannelByName( const char* name);
Channel findChannelByName(const char* name);
User findUserByName(const char* name);
char response[150000];
int main() {
    dataBaseCreator();
    int server_socket, client_socket;
    struct sockaddr_in server, client;

    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    // Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h
    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        // Tell the user that we could not find a usable Winsock DLL.
        printf("WSAStartup failed with error: %d\n", err);
        return 1;
    }

    // Create and verify socket
    server_socket = socket(AF_INET, SOCK_STREAM, 6);
    if (server_socket == INVALID_SOCKET)
        wprintf(L"socket function failed with error = %d\n", WSAGetLastError() );
    else
        printf("Socket successfully created..\n");

    // Assign IP and port
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);

    // Bind newly created socket to given IP and verify
    if ((bind(server_socket, (SA *)&server, sizeof(server))) != 0)
    {
        printf("Socket binding failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully bound..\n");

    // Now server is ready to listen and verify
    if ((listen(server_socket, 5)) != 0)
    {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");

    // Accept the data packet from client and verify

    while (1){
        findUserByName("test");
        client_socket =socketCreator(server_socket, client);
        char request[150000];

        recv(client_socket, request, 150000, 0);
        printf("%s",request);
        memset(response,0, 150000);
        if(strstr(request,"login") != NULL){
            login(request, response);
        }
        else if (strstr(request,"register") != NULL){
            registerUser(request, response);
        }
        else if (strstr(request,"create channel") != NULL){
            createChannel(request, response);
        }
        else if (strstr(request,"join channel") != NULL){
            joinChannel(request, response);
        }
        else if (strstr(request,"logout") != NULL){
            logout(request, response);
        }
        else if (strstr(request,"send") != NULL){
            sendMessage(request, response);
        }
        else if (strstr(request,"refresh") != NULL){
            refresh(request, response);
        }
        else if (strstr(request,"leave") != NULL){
            leave(request, response);
        }
        else if (strstr(request,"channel members") != NULL){
            channelMembers(request, response);
        }else
            strcpy(response,"{\"type\":\"Error\",\"content\":\"Invalid request pattern!\"}");
        send(client_socket, response, strlen(response), 0);
        printf("%s",response);
        closesocket(client_socket);
    }
}
int socketCreator( int server_socket, struct sockaddr_in client){
    int len = sizeof(client);
    int client_socket;
    client_socket = accept(server_socket, (SA *)&client, &len);
    if (client_socket < 0)
    {
        printf("Server accceptance failed...\n");
        exit(0);
    }
    else
        printf("Server acccepted the client..\n");

    // Function for chatting between client and server
    return client_socket;
    // Close the socket
    closesocket(server_socket);

}
void authTokenCreator(){
    srand(time(NULL));
    for (int i = 0; i < 31; ++i) {
        onlineUsers[onlineUsersCount].Token[i] = authTokeArr[((62 * rand())/RAND_MAX)];
    }
    for(int i = 0;i < onlineUsersCount;++i){
        if(strcmp(onlineUsers[i].Token,onlineUsers[onlineUsersCount].Token) == 0) {
            authTokenCreator();
            break;
        }
    }

}
void dataBaseCreator(){
    FILE *fptr = fopen("./Resources/MainInfo/mainInfo.txt", READMODE);
    if(fptr == NULL){
        mainDataBase.numberOfChannels = 0;
        mainDataBase.numberOfUsers = 0;
    } else{
        fscanf(fptr,"%d %d",&mainDataBase.numberOfChannels, &mainDataBase.numberOfUsers);
    }
    fclose(fptr);
}
const char* makeNewMessage (const char* senderName,const char * contentString){
    cJSON* message = cJSON_CreateObject();
    cJSON* sender = cJSON_CreateString(senderName);
    cJSON*content = cJSON_CreateString(contentString);
    cJSON_AddItemToObject(message,"sender",sender);
    cJSON_AddItemToObject(message,"content",content);

    return cJSON_Print(message);

}
void addMessageToMembers( const char* message,  const char* channelName){
    int channelIndex = findOnlineChannelByName(channelName);
    for (int i = 0; i < channelsInfo[channelIndex].numberOfOnlineUsers; ++i) {
        char buffer[150000];
        getUserMessages(channelsInfo[channelIndex].membersArr[i], buffer);
        cJSON* list =cJSON_Parse (buffer);
        cJSON_AddItemToArray(list ,cJSON_Parse(message));
        saveUsersMessages(channelsInfo[channelIndex].membersArr[i],cJSON_Print(list));
    }
}
cJSON* makeChannelMembersCJSON ( const char*channelName){
    cJSON* message = cJSON_CreateObject();
    cJSON* type = cJSON_CreateString("List");
    cJSON_AddItemToObject(message, "type", type);
    cJSON* list = cJSON_CreateArray();
    int channelIndex = findOnlineChannelByName(channelName);
    for (int i = 0; i < channelsInfo[channelIndex].numberOfOnlineUsers ; ++i) {
        cJSON* member = cJSON_CreateString(findUserByName(channelsInfo[channelIndex].membersArr[i]).username);
        cJSON_AddItemToArray(list, member);
    }
    cJSON_AddItemToObject(message, "content", list);
    return message;
}
void deleteMemberFromChannel(const char* channelName, const char* memberName ){
    int memberPosition = 0;
    int  channelIndex = findOnlineChannelByName(channelName);
    for(;memberPosition < channelsInfo[channelIndex].numberOfOnlineUsers; ++memberPosition){
        if(strcmp(channelsInfo[channelIndex].membersArr[memberPosition], memberName) == 0)
            break;
    }
    for (memberPosition ++ ;memberPosition < channelsInfo[channelIndex].numberOfOnlineUsers; ++memberPosition) {
        strcpy(channelsInfo[channelIndex].membersArr[memberPosition - 1] , channelsInfo[channelIndex].membersArr[memberPosition]);
    }
    memset(channelsInfo[channelIndex].membersArr[memberPosition - 1], 0,NAMEMAX);
}
void login (char* request, char response[150000] ){
    char name[NAMEMAX];
    char password[PASSMAX];
    sscanf(request, loginPattern,name,password);
    if(userExists(name)) {
        if(findOnlineUserByName(name) != -1){
            strcpy(response,"{\"type\":\"Error\",\"content\":\"This user has already logged in.\"}");
        }
        else if(strcmp(findUserByName(name).password,password) == 0) {
            authTokenCreator();
            strcpy(onlineUsers[onlineUsersCount].name, name);
            onlineUsers[onlineUsersCount].isAMember = false;
            ++onlineUsersCount;
            cJSON* list = cJSON_CreateArray();
            saveUsersMessages(onlineUsers[onlineUsersCount - 1].name, cJSON_Print(list));
            free(list);
            snprintf(response, 67,"{\"type\":\"AuthToken\",\"content\":\"%s\"}", onlineUsers[onlineUsersCount - 1].Token);
        } else
            strcpy(response,"{\"type\":\"Error\",\"content\":\"Wrong password.\"}");
    } else
        strcpy(response,"{\"type\":\"Error\",\"content\":\"Username is not valid.\"}");
}
void registerUser(char* request, char response[150000]){
    char name[NAMEMAX];
    char password[PASSMAX];
    sscanf(request, registerPattern,name,password);
    if(!userExists(name)) {
        //making new user
        User newUser;
        strcpy(newUser.username,name);
        strcpy(newUser.password,password);
        strcpy(newUser.channelName,"");
        createNewUser(newUser);
        //updating database
        ++mainDataBase.numberOfUsers;
        syncDataBase(mainDataBase);
        strcpy(response,"{\"type\":\"Successful\",\"content\":\"\"}");
    } else
        strcpy(response,"{\"type\":\"Error\",\"content\":\"this username is not available.\"}");
}
void createChannel (char* request, char response[150000]){
    char name[NAMEMAX];
    char authToken[33];
    sscanf(request, createChannelPattern, name, authToken);
    if((userIndex = findOnlineUserByAuthToken(authToken)) == -1){
        strcpy(response, "{\"type\":\"Error\",\"content\":\"Invalid authentication token.\"}");
    } else if(onlineUsers[userIndex].isAMember){
        strcpy(response, "{\"type\":\"Error\",\"content\":\"This user is a member of another channel.\"}");
    }
    else{
        if(!channelExists(name)){
            Channel newChannel ;
            strcpy(newChannel.name, name);
            strcpy(channelsInfo[onlineChannelsCount++].ChannelName, name);
            channelsInfo[onlineChannelsCount].numberOfOnlineUsers = 0;
            strcpy(channelsInfo[onlineChannelsCount - 1].membersArr[channelsInfo[onlineChannelsCount - 1].numberOfOnlineUsers++], onlineUsers[userIndex].name);
            char buffer[200];
            snprintf(buffer,200, "%s created channel %s", onlineUsers[userIndex].name, name);
            cJSON* list = cJSON_CreateArray();
            cJSON* object = cJSON_Parse(makeNewMessage("server", buffer));
            cJSON_AddItemToArray(list, object);
            saveChannelMessages(name, cJSON_Print(list));
            saveUsersMessages(onlineUsers[userIndex].name, cJSON_Print(list));
            User temp = findUserByName(onlineUsers[userIndex].name);
            strcpy(temp.channelName, name);
            onlineUsers[userIndex].isAMember = true;
            createNewUser(temp);
            createNewChannel(newChannel);
            Channel temp2 = findChannelByName(name);
            ++mainDataBase.numberOfChannels;
            syncDataBase(mainDataBase);
            strcpy(response, "{\"type\":\"Successful\",\"content\":\"\"}");
            free(list);
            free(object);
        }else
            strcpy(response, "{\"type\":\"Error\",\"content\":\"Channel name is not available.\"}");
    }
}
void logout(char* request, char response[150000]){
    char authToken[33];
    sscanf(request, logoutPattern, authToken);
    if((userIndex = findOnlineUserByAuthToken(authToken)) == -1){
        strcpy(response, "{\"type\":\"Error\",\"content\":\"Invalid authentication token.\"}");
    } else{
        deleteOnlineUser(userIndex);
        onlineUsersCount--;
        strcpy(response, "{\"type\":\"Successful\",\"content\":\"\"}");
    }
}
void sendMessage (char* request, char response[150000]){
    char message[MESSAGEMAX];
    char authToken[33];
    sscanf(request, sendMessagePattern, message, authToken);
    if((userIndex = findOnlineUserByAuthToken(authToken)) == -1){
        strcpy(response, "{\"type\":\"Error\",\"content\":\"Invalid authentication token.\"}");
    } else if(onlineUsers[userIndex].isAMember == false){
        strcpy(response, "{\"type\":\"Error\",\"content\":\"This User isn't currently a member of a channel.\"}");
    }
    else{
        char buffer[150000];
        getChannelMessages(findUserByName(onlineUsers[userIndex].name).channelName, buffer);
        cJSON* temp = cJSON_Parse (buffer);
        cJSON* object = cJSON_Parse(makeNewMessage(onlineUsers[userIndex].name, message));
        cJSON_AddItemToArray(temp ,object);
        saveChannelMessages(findUserByName(onlineUsers[userIndex].name).channelName, cJSON_Print(temp));
        addMessageToMembers(cJSON_Print(object) , findUserByName(onlineUsers[userIndex].name).channelName);
        strcpy(response, "{\"type\":\"Successful\",\"content\":\"\"}");
        free(temp);
        free(object);
    }
}
void leave(char * request, char response[150000]){
    char name[NAMEMAX];
    char authToken[33];
    sscanf(request, leavePattern, authToken);
    if((userIndex = findOnlineUserByAuthToken(authToken)) == -1){
        strcpy(response, "{\"type\":\"Error\",\"content\":\"Invalid authentication token.\"}");
    } else{
        strcpy( name,findUserByName(onlineUsers[userIndex].name).channelName);
        int channelIndex;
        if(channelExists(name)){
            if(onlineUsers[userIndex].isAMember == false){
                strcpy(response, "{\"type\":\"Error\",\"content\":\"This user is not a member of a channel.\"}");
            }else {
                channelIndex = findOnlineChannelByName(name);
                deleteMemberFromChannel(name, onlineUsers[userIndex].name);
                char buffer[100];
                snprintf(buffer, 100, "%s left the channel.", onlineUsers[userIndex].name);
                char buffer2[150000];
                getChannelMessages(name, buffer2);
                cJSON* list =cJSON_Parse(buffer2);
                cJSON* object = cJSON_Parse(makeNewMessage("server", buffer));
                cJSON_AddItemToArray(list, object);
                saveChannelMessages(name, cJSON_Print(list));
                channelsInfo[channelIndex].numberOfOnlineUsers--;
                addMessageToMembers(cJSON_Print(object), name);
                onlineUsers[userIndex].isAMember = false;
                strcpy(response, "{\"type\":\"Successful\",\"content\":\"\"}");
                free(list);
                free(object);
            }
        }else
            strcpy(response, "{\"type\":\"Error\",\"content\":\"Channel doesn't exist.\"}");
    }
}
void channelMembers(char* request, char response[150000]){
    char authToken[33];
    sscanf(request, channelMembersPattern, authToken);
    userIndex = usernameFinderByAuthToken(authToken);
   if(onlineUsers[userIndex].isAMember == false)
        strcpy(response, "{\"type\":\"Error\",\"content\":\"This User isn't currently a member of a channel.\"}");
   else if(userIndex  == -1){
       strcpy(response, "{\"type\":\"Error\",\"content\":\"Invalid authentication token.\"}");
   }
    else{
        strcpy(response, cJSON_Print(makeChannelMembersCJSON(findUserByName(onlineUsers[userIndex].name).channelName)));
    }
}
void joinChannel (char* request, char response[150000]){
    char name[NAMEMAX];
    char authToken[33];
    sscanf(request, joinChannelPattern, name, authToken);
    if((userIndex = findOnlineUserByAuthToken(authToken)) == -1){
        strcpy(response, "{\"type\":\"Error\",\"content\":\"Invalid authentication token.\"}");
    } else{
        int channelIndex;
        if(channelExists(name)){
            if(onlineUsers[userIndex].isAMember){
                strcpy(response, "{\"type\":\"Error\",\"content\":\"This user is already a member of another channel.\"}");
            }else {
                if ((channelIndex = findOnlineChannelByName(name)) == -1){
                    channelIndex = onlineChannelsCount++;
                    channelsInfo[channelIndex].numberOfOnlineUsers = 0;
                }
                strcpy(channelsInfo[channelIndex].ChannelName, name);
            strcpy(channelsInfo[channelIndex].membersArr[channelsInfo[channelIndex].numberOfOnlineUsers++], onlineUsers[userIndex].name);
            char buffer[100];
            snprintf(buffer, 100, "%s joined the channel", onlineUsers[userIndex].name);
            char test[150000];
            char temp[NAMEMAX];
            strcpy(temp, name);
            getChannelMessages(temp, test);
            cJSON* list =cJSON_Parse(test);
                cJSON* object = cJSON_Parse(makeNewMessage("server", buffer));
                cJSON_AddItemToArray(list, object);
            saveChannelMessages(temp, cJSON_Print(list));
            addMessageToMembers(cJSON_Print(object), temp);
                saveUsersMessages(onlineUsers[userIndex].name, cJSON_Print(list));
                User tempUser = findUserByName(onlineUsers[userIndex].name);
                onlineUsers[userIndex].isAMember = true;
                strcpy(tempUser.channelName, temp);
                createNewUser(tempUser);\
            strcpy(response, "{\"type\":\"Successful\",\"content\":\"\"}");
                free(list);
                free(object);
            }
        }else
            strcpy(response, "{\"type\":\"Error\",\"content\":\"Channel doesn't exist.\"}");
    }
}
void refresh (char* request, char response[150000]){
    char authToken[33];
    sscanf(request, refreshPattern,  authToken);
    if((userIndex = usernameFinderByAuthToken(authToken)) == -1){
        strcpy(response, "{\"type\":\"Error\",\"content\":\"Invalid authentication token.\"}");
    } else if(!onlineUsers[userIndex].isAMember){
        strcpy(response, "{\"type\":\"Error\",\"content\":\"This user isn't a member of a channel.\"}");}
    else {
        int channelIndex;
        if((channelIndex = findOnlineChannelByName(findUserByName(onlineUsers[userIndex].name).channelName)) != -1){
            char buffer[150000];
            cJSON* message = cJSON_CreateObject();
            cJSON* type = cJSON_CreateString("List");
            cJSON_AddItemToObject(message, "type", type);
            getUserMessages(onlineUsers[userIndex].name, buffer);
            cJSON_AddItemToObject(message, "content", cJSON_Parse(buffer));
            strcpy(response,cJSON_Print(message));
            saveUsersMessages(onlineUsers[userIndex].name, "[]");
            free(message);
            free(type);
        }else
            strcpy(response, "{\"type\":\"Error\",\"content\":\"Channel doesn't exist.\"}");
    }
}
User findUserByName(const char* name){
    char path[150];
    User user;
    snprintf(path, 150, "./Resources/Users/%s.txt", name);
    FILE* fptr = fopen(path,READMODE);
    fread(&user, sizeof(user), 1, fptr);
    return user;
}
Channel findChannelByName(const char* name){
    char path[150];
    Channel channel;
    snprintf(path, 150, "./Resources/Channels/%s.txt", name);
    FILE* fptr = fopen(path,READMODE);
    fread(&channel, sizeof(channel), 1, fptr);
    return channel;
}
bool userExists(const char*name){
    char buffer[150];
    snprintf(buffer, 150, "./Resources/users/%s.txt", name);
    FILE* fptr = fopen(buffer, READMODE);
    if(fptr == NULL)
        return false;
    else
        return true;
}
bool channelExists(const char*name){
    char buffer[150];
    snprintf(buffer, 150, "./Resources/Channels/%s.txt", name);
    FILE* fptr = fopen(buffer, READMODE);
    if(fptr == NULL)
        return false;
    else
        return true;
}
void saveChannelMessages(const char*  channelName, const char* list){
    char path[150];
    snprintf(path, 150, "./Resources/Channels/%s_messages.txt", channelName);
    FILE* fptr = fopen(path,WRITEMODE);
    fwrite(list, strlen(list), 1, fptr);
    fclose(fptr);
}
void saveUsersMessages(const char*  userName, const char * list){
    char path[150];
    snprintf(path, 150, "./Resources/Users/%s_messages.txt", userName);
    FILE* fptr = fopen(path,WRITEMODE);
    fwrite(list, strlen(list), 1, fptr);
    fclose(fptr);
}
int usernameFinderByAuthToken(char*authToken){
    for(int i = 0; i < onlineUsersCount; ++i)
        if(strcmp(onlineUsers[i].Token,authToken) == 0)
            return i;
    return -1;
}
void getUserMessages(const char* name, char buffer[150000]){
    char path[150];
    snprintf(path, 150, "./Resources/Users/%s_messages.txt", name);
    FILE* fptr = fopen(path,READMODE);

    char temp;
    int i = 0;
    while ((temp = fgetc(fptr)) != EOF){
        buffer[i++] = temp;
    }
    fclose(fptr);
}
void getChannelMessages(const char* name , char buffer[150000]){
    char path[150];
    snprintf(path, 150, "./Resources/Channels/%s_messages.txt", name);
    FILE* fptr = fopen(path, READMODE);
    char temp;
    int i = 0;
    while ((temp = fgetc(fptr)) != EOF){
        buffer[i++] = temp;
    }
    fclose(fptr);

}
void createNewUser(User user){
    char buffer[150];
    snprintf(buffer, 150, "./Resources/Users/%s.txt", user.username);
    FILE* fptr = fopen(buffer, WRITEMODE);
    fwrite(&user, sizeof(user), 1, fptr);
    fclose(fptr);
}
void createNewChannel(Channel channel){
    char buffer[150];
    snprintf(buffer, 150, "./Resources/Channels/%s.txt", channel.name);
    FILE* fptr = fopen(buffer, WRITEMODE);
    fwrite(&channel, sizeof(channel), 1, fptr);
    fclose(fptr);
}
int findOnlineUserByAuthToken( const char authToken[33]){
    for (int i = 0; i < onlineUsersCount ; ++i) {
        if(strcmp(authToken, onlineUsers[i].Token) == 0)
            return i;
    }
    return -1;
}
int findOnlineUserByName( const char* name){
    for (int i = 0; i < onlineUsersCount ; ++i) {
        if(strcmp(name, onlineUsers[i].name) == 0)
            return i;
    }
    return -1;
}
int findOnlineChannelByName( const char* name){
    for (int i = 0; i < onlineChannelsCount ; ++i) {
        if(strcmp(name, channelsInfo[i].ChannelName) == 0)
            return i;
    }
    return -1;
}
void deleteOnlineUser(int userIndexL){
    for (userIndexL++ ; userIndexL < onlineUsersCount; ++userIndexL) {
        strcpy(onlineUsers[userIndexL - 1].name , onlineUsers[userIndexL ].name);
    }
    memset(onlineUsers[onlineUsersCount - 1].name, 0, NAMEMAX);
    memset(onlineUsers[onlineUsersCount - 1].Token, 0, 33);
}
void syncDataBase (DataBase dataBase){
    FILE *fptr = fopen("./Resources/MainInfo/mainInfo.txt", WRITEMODE);
    fprintf(fptr, "%d %d",dataBase.numberOfChannels, dataBase.numberOfUsers);
    fclose(fptr);
}